#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <string>
#include <memory>
#include <iostream>
#include <WS2tcpip.h>
#include <thread>
#include <atomic>
#include <map>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>

#include "Packet.h"

struct ssl_st;

class TlsClient
{
public:
	TlsClient();
	int Connect(const char* host, const int& port);

	std::map<const unsigned int, std::shared_ptr<Packet>> receivedPackets;
	int Send(const Packet& packet);
	int SendRequest(const char* content) const;
	std::shared_ptr<Packet> WaitHeader(const char* header) const;
	std::shared_ptr<Packet> GetByHeader(const char* header) const;
	void DeletePacket(const std::shared_ptr<Packet>& packet);

private:
	SSL_CTX* m_ctx;
	SOCKET m_socket;
	SSL* m_ssl;

	std::atomic<bool> _listening;
	std::unique_ptr<std::thread> _listeningThread;
	void Listen();

	SSL_CTX* InitCTX(void);
	void ShowCerts(SSL* ssl);
	SOCKET OpenConnection(const char *host, int port);
};