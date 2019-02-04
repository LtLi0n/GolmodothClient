#include "TlsClient.h"
#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include <string>
#include <string.h>
#include <cstdlib>
#include <memory>

TlsClient::TlsClient() : _listening(false) { }

SOCKET TlsClient::OpenConnection(const char* host, int port)
{
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0)
	{
		printf("Can't start Winsock, Err #%s\n", wsResult);
		return -1;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, NULL);
	if (sock == INVALID_SOCKET)
	{
		printf("Can't create socket, Err #%s\n", WSAGetLastError());
		return -1;
	}

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, host, &hint.sin_addr);

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		printf("Can't connect to host, Err #%s\n", WSAGetLastError());
		closesocket(sock);
		WSACleanup();
		return -1;
	}

	return sock;
}

SSL_CTX* TlsClient::InitCTX(void)
{
	const SSL_METHOD *method;
	SSL_CTX *ctx;

	OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
	SSL_load_error_strings();   /* Bring in and register error messages */
	method = TLSv1_2_client_method();  /* Create new client-method instance */
	ctx = SSL_CTX_new(method);   /* Create new context */
	if (ctx == NULL)
	{
		ERR_print_errors_fp(stderr);
		abort();
	}
	return ctx;
}

void TlsClient::ShowCerts(SSL* ssl)
{
	X509 *cert;
	char *line;

	cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
	if (cert != NULL)
	{
		printf("Server certificates:\n");
		line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
		printf("Subject: %s\n", line);
		line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
		printf("Issuer: %s\n", line);
		X509_free(cert);     /* free the malloc'ed certificate copy */
	}
	else
		printf("Info: No client certificates configured.\n");
}

int TlsClient::Connect(const char* host, const int& port)
{
	SSL_library_init();
	m_ctx = InitCTX();
	SSL_CTX_load_verify_locations(m_ctx, "public_cert.pem", NULL);
	m_socket = OpenConnection(host, port);
	m_ssl = SSL_new(m_ctx);
	SSL_set_fd(m_ssl, m_socket);
	if (SSL_connect(m_ssl) == -1)
	{
		printf("SSL Connection Error.");
		return -1;
	}
	else
	{
		SSL_get_cipher(m_ssl);
		//ShowCerts(m_ssl);
	}

	_listening = true;
	_listeningThread = std::make_unique<std::thread>(&TlsClient::Listen, this);
}

int TlsClient::Send(const Packet& packet)
{
	return SSL_write(m_ssl, packet.GenerateBuffer(), packet.ContentLength() + 5);
}

int TlsClient::SendRequest(const char* content) const
{
	Packet p(PACKET_REQUEST);
	p.AddContent(content);
	return SSL_write(m_ssl, p.GenerateBuffer(), p.ContentLength() + 5);
}

int TlsClient::SendRequest(const std::wstring& content) const
{
	Packet p(PACKET_REQUEST);
	std::string str = WidestringToString(content);
	p.AddContent(str.c_str());
	return SSL_write(m_ssl, p.GenerateBuffer(), p.ContentLength() + 5);
}

std::shared_ptr<Packet> TlsClient::WaitHeader(const char* header) const
{
	while (true)
	{
		const std::shared_ptr<Packet> p = GetByHeader(header);

		if (p != nullptr)
		{
			return p;
		}
	}
}

std::shared_ptr<Packet> TlsClient::GetByHeader(const char* header) const
{
	for (auto const& x : receivedPackets)
	{
		if (x.second == nullptr) continue;

		const char* content = x.second->Content();
		std::string headerStr = "";
		bool header_found = false;

		for (int i = 0; i < x.second->ContentLength(); i++)
		{
			headerStr += content[i];
			if (strcmp(headerStr.c_str(), header) == 0) return x.second;
		}
	}

	return nullptr;
}

void TlsClient::DeletePacket(const std::shared_ptr<Packet>& packet)
{
	for (auto const& x : receivedPackets)
	{
		if (x.second == packet)
		{
			receivedPackets.erase(x.first);
			break;
		}
	}
}

void TlsClient::Listen()
{
	int heldBytes = 0;
	char* data = nullptr;
	char size_bytes[4];

	while (_listening)
	{
		if (heldBytes == 0)
		{
			int bytes = SSL_peek(m_ssl, size_bytes, 4);
			if (bytes < 4) continue;

			int iResult = SSL_read(m_ssl, size_bytes, 4);

			if (iResult == SOCKET_ERROR) continue;
			heldBytes = *((unsigned int*)size_bytes);
			data = new char[heldBytes];
			ZeroMemory(data, heldBytes);
		}
		if (heldBytes > 0)
		{
			int result = SSL_read(m_ssl, data, heldBytes);

			int content_size = heldBytes - 5;

			//NULL terminator
			char* content = new char[content_size + 1];
			ZeroMemory(content, content_size + 1);

			unsigned int packet_id = *((unsigned int*)data);
			PacketType packet_type = (PacketType)data[4];
			for (int i = 0; i < content_size; i++) content[i] = data[i + 5];

			if (packet_type != PACKET_NULL)
			{
				std::shared_ptr<Packet> packet = std::make_shared<Packet>(packet_type, packet_id);
				packet->AddContent(content);
				receivedPackets[packet_id] = packet;
			}

			heldBytes = 0;
			delete[] data;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

std::string TlsClient::WidestringToString(std::wstring wstr) const
{
	if (wstr.empty()) return std::string();

#if defined WIN32
	int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &wstr[0], wstr.size(), NULL, 0, NULL, NULL);
	std::string ret = std::string(size, 0);
	WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, &wstr[0], wstr.size(), &ret[0], size, NULL, NULL);
#else
	size_t size = 0;
	_locale_t lc = _create_locale(LC_ALL, "en_US.UTF-8");
	errno_t err = _wcstombs_s_l(&size, NULL, 0, &wstr[0], _TRUNCATE, lc);
	std::string ret = std::string(size, 0);
	err = _wcstombs_s_l(&size, &ret[0], size, &wstr[0], _TRUNCATE, lc);
	_free_locale(lc);
	ret.resize(size - 1);
#endif
	return ret;
}

