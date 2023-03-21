#pragma once

#include "PacketDefine.h"
#include "TcpSocket.h"
#include "WinSocManager.h"

BLANCNET_BEGIN

class MainSystem
{
public:
	MainSystem();
	~MainSystem() {};

	void Init(std::string ip = "0.0.0.0", int port = 0);
	void Close();

	bool Run();
	bool Send(int netId, PACKET_BASE packet);
	bool Broadcast(PACKET_BASE packet);

	void EventUpdate();
	void SendUpdate();

	std::string GetSystemMessage();

private:
	bool Listen();
	bool Connect();
	void Accepting();

	void OnNetAccept(std::shared_ptr<TcpSocket> soc);
	void OnNetClose(std::shared_ptr<TcpSocket> soc, int socketIndex);
	void OnNetRecv(std::shared_ptr<TcpSocket> soc);
	void OnNetSend(std::shared_ptr<TcpSocket> soc);

	void GetClientIPPort(SOCKET clientSoc, char* ipPort);

private:
	std::shared_ptr<TcpSocket> m_listenSocket;
	std::shared_ptr<TcpSocket> m_connectSocket;

	static const int Max_Client_Connect = 100;
	static const int IpPort_Len = 50;

	WSAEVENT m_eventTable[WSA_MAXIMUM_WAIT_EVENTS] = { 0, };
	std::mutex clientMutex;
	std::vector<std::shared_ptr<TcpSocket>> m_socketList;
	std::queue<int> m_emptySocket;
	//std::list<std::shared_ptr<TcpSocket>> m_socketList;
	//std::map<boost::uuids::uuid, std::shared_ptr<TcpSocket>> m_clientList;
	
	std::thread m_recvThread;
	std::thread m_sendThread;
	
	bool m_isRunning = false;

	std::queue<std::string> m_systemMessage;

	std::string m_listenIp;
	std::string m_connectIp;
	int m_port;

	std::unique_ptr<WinSocManager> m_winManager;
};

BLANCNET_END