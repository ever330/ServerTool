#include "pch.h"
#include "MainSystem.h"

BLANCNET_BEGIN

MainSystem::MainSystem()
{

}

void MainSystem::Init(std::string ip /*= "0.0.0.0"*/, int port /*= 0*/)
{
	m_winManager = std::make_unique<WinSocManager>();
	m_winManager->Init();
	if (port != 0)
	{
		//m_listenSocket = std::make_unique<TcpSocket>();
		m_listenSocket = std::make_shared<TcpSocket>();
		m_listenIp = ip;
		m_listenSocket->SetNetId(0);
		m_socketList.push_back(m_listenSocket);
	}
	else
	{
		//m_connectSocket = std::make_unique<TcpSocket>();
		m_connectIp = ip;
	}
	m_port = port;
	//m_netId = boost::uuids::random_generator()();
}

void MainSystem::Close()
{
	if (m_socketList.empty() == false)
	{
		for (auto soc : m_socketList)
		{
			soc->Close();
		}
		m_socketList.clear();
	}

	m_isRunning = false;
	m_winManager->CleanUp();
}

bool MainSystem::Run()
{
	if (nullptr != m_listenSocket)
	{
		if (false == Listen()) return false;
	}

	/*if (m_isClient)
	{
		if (false == Connect()) return false;
	}*/

	m_isRunning = true;

	//while (m_isRunning)
	//{
	//	Update();
	//}

	return true;
}

bool MainSystem::Send(int netId, PACKET_BASE packet)
{
	auto soc = m_socketList[netId];

	if (nullptr != soc)
	{
		std::ostringstream oss;
		boost::archive::text_oarchive oa(oss);
		oa << packet;
		std::string str = oss.str();
		char* cstr = (char*)str.c_str();
		soc->PostPacket(cstr, strlen(cstr));

		return true;
	}

	m_systemMessage.push("패킷을 보낼 유저가 없습니다.");

	return false;
}

bool MainSystem::Broadcast(PACKET_BASE packet)
{
	for (auto soc : m_socketList)
	{
		std::ostringstream oss;
		boost::archive::text_oarchive oa(oss);
		oa << packet;
		std::string str = oss.str();
		char* cstr = (char*)str.c_str();
		soc->PostPacket(cstr, strlen(cstr));
	}

	return true;
}

bool MainSystem::Listen()
{
	if (nullptr == m_listenSocket)
	{
		m_systemMessage.push("리슨 소켓이 생성되지 않았습니다. Init 필요. 에러코드 :" + WSAGetLastError());
		return false;
	}

	if (false == m_listenSocket->Open(IPPROTO_TCP))
	{
		m_systemMessage.push("리슨 소켓을 열지 못했습니다. 에러코드 :" + WSAGetLastError());
		return false;
	}

	if (false == m_listenSocket->EventSelect(FD_ACCEPT | FD_CLOSE))
	{
		m_systemMessage.push("리슨 소켓에 이벤트 연결을 실패했습니다. 에러코드 :" + WSAGetLastError());
		return false;
	}

	SOCKADDR_IN listenAddr;
	listenAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, m_listenIp.c_str(), &listenAddr.sin_addr);
	listenAddr.sin_port = htons(m_port);

	if (false == m_listenSocket->Bind(listenAddr))
	{
		m_systemMessage.push("리슨 소켓 바인드가 실패했습니다. 에러코드 :" + WSAGetLastError());
		return false;
	}

	if (false == m_listenSocket->Listen())
	{
		m_systemMessage.push("리슨 실패. 에러코드 :" + WSAGetLastError());
		return false;
	}

	m_eventTable[m_socketList.size()] = m_listenSocket->GetEventHandle();

	return true;
}

bool MainSystem::Connect()
{
	if (nullptr == m_connectSocket)
	{
		m_systemMessage.push("커넥트 소켓이 생성되지 않았습니다. Init 필요. 에러코드 :" + WSAGetLastError());
		return false;
	}

	if (false == m_connectSocket->Open(IPPROTO_TCP))
	{
		m_systemMessage.push("커넥트 소켓을 열지 못했습니다. Init 필요. 에러코드 :" + WSAGetLastError());
		return false;
	}

	if (false == m_connectSocket->EventSelect(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE))
	{
		m_systemMessage.push("커넥트 소켓에 이벤트 연결을 실패했습니다. 에러코드 :" + WSAGetLastError());
		return false;
	}

	SOCKADDR_IN connectAddr;
	connectAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, m_connectIp.c_str(), &connectAddr.sin_addr);
	connectAddr.sin_port = htons(m_port);

	if (false == m_connectSocket->Connect(connectAddr))
	{
		m_systemMessage.push("커넥트 실패. 에러코드 :" + WSAGetLastError());
		return false;
	}

	return true;
}

void MainSystem::EventUpdate()
{
	int socIndex = 0;

	if ((socIndex = WSAWaitForMultipleEvents(m_socketList.size(), m_eventTable, FALSE, 100, FALSE)) == WSA_WAIT_FAILED)
	{
		m_systemMessage.push("WSAWaitForMultipleEvents() failed. 에러코드 :" + WSAGetLastError());
		return;
	}

	std::shared_ptr<TcpSocket> nowSoc = m_socketList.at(socIndex);

	if (nullptr != nowSoc)
	{
		m_systemMessage.push(nowSoc->GetNetId()+ "번 소켓 이벤트 실행.");

		WSANETWORKEVENTS NetworkEvents;

		if (SOCKET_ERROR == WSAEnumNetworkEvents(nowSoc->GetSocHandle(), nowSoc->GetEventHandle(), &NetworkEvents))
		{
			m_systemMessage.push("WSAEnumNetworkEvents() failed. 에러코드 :" + WSAGetLastError());
			return;
		}

		if (NetworkEvents.lNetworkEvents == FD_ACCEPT)
		{
			if (0 != NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
			{
				m_systemMessage.push("FD_ACCEPT failed. 에러코드 :" + NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
				return;
			}
			OnNetAccept(nowSoc);
		}

		if (NetworkEvents.lNetworkEvents == FD_CLOSE)
		{
			if (0 !=NetworkEvents.iErrorCode[FD_CLOSE_BIT])
			{
				m_systemMessage.push("FD_CLOSE failed. 에러코드 :" + NetworkEvents.iErrorCode[FD_ACCEPT_BIT]);
				return;
			}
			OnNetClose(nowSoc, nowSoc->GetNetId());
			return;
		}

		if (NetworkEvents.lNetworkEvents == FD_READ)
		{
			if (0 != NetworkEvents.iErrorCode[FD_READ_BIT])
			{
				m_systemMessage.push("FD_READ failed. 에러코드 :" + NetworkEvents.iErrorCode[FD_READ_BIT]);
				return;
			}
			OnNetRecv(nowSoc);
		}

		if (NetworkEvents.lNetworkEvents == FD_WRITE)
		{
			if (0 != NetworkEvents.iErrorCode[FD_WRITE_BIT])
			{
				m_systemMessage.push("FD_WRITE failed. 에러코드 :" + NetworkEvents.iErrorCode[FD_WRITE_BIT]);
				return;
			}
			OnNetSend(nowSoc);
		}
	}
}

void MainSystem::GetClientIPPort(SOCKET clientSoc, char* ipPort)
{
	char addr[IpPort_Len];
	addr[0] = 0;

	sockaddr_in endpoint;
	socklen_t epLength = sizeof(endpoint);

	if (getpeername(clientSoc, (sockaddr*)&endpoint, &epLength) < 0)
	{
		m_systemMessage.push("getpeername 실패. 에러코드 : " + WSAGetLastError());
		return;
	}
	if (epLength > sizeof(endpoint))
	{
		m_systemMessage.push("getpeername buffer overrun. 에러코드 : " + WSAGetLastError());
		return;
	}

	inet_ntop(AF_INET, &endpoint.sin_addr, addr, sizeof(addr) - 1);
	char div[2] = ":";
	strcat_s(addr, sizeof(addr), div);

	char portNum[10];
	portNum[0] = 0;
	sprintf_s(portNum, "%d", htons(endpoint.sin_port));
	strcat_s(addr, sizeof(addr), portNum);

	memcpy(ipPort, addr, IpPort_Len);
}

std::string MainSystem::GetSystemMessage()
{
	if (m_systemMessage.empty())
		return "시스템 큐가 비었습니다.";

	std::string message = m_systemMessage.front();
	m_systemMessage.pop();

	return message;
}

void MainSystem::OnNetAccept(std::shared_ptr<TcpSocket> soc)
{
	if (Max_Client_Connect <= m_socketList.size())
	{
		m_systemMessage.push("최대 접속 초과!!");
		return;
	}

	int newSocId = 0;
	if (0 == m_emptySocket.size())
	{
		newSocId = m_socketList.size();
	}
	else
	{
		newSocId = m_emptySocket.front();
		m_emptySocket.pop();
	}

	std::shared_ptr<TcpSocket> newSocket = std::make_shared<TcpSocket>();
	newSocket->SetSocHandle(accept(soc->GetSocHandle(), NULL, NULL));
	if (newSocket->GetSocHandle() == INVALID_SOCKET)
	{
		m_systemMessage.push("accept() filad. 에러코드 : " + WSAGetLastError());
		return;
	}

	char ipPort[IpPort_Len];
	GetClientIPPort(newSocket->GetSocHandle(), ipPort);
	m_systemMessage.push("접속한 클라이언트 : " + static_cast<std::string>(ipPort));
	newSocket->SetNetId(newSocId);

	if (false == newSocket->EventSelect(FD_READ | FD_WRITE | FD_CLOSE))
	{
		m_systemMessage.push("이벤트 설정 실패!");
		closesocket(newSocket->GetSocHandle());
		return;
	}

	m_eventTable[newSocId] = newSocket->GetEventHandle();
	m_socketList[newSocId] = newSocket;

	MSG_S2C_ACCEPT s2cPacket;
	s2cPacket.packetId = static_cast<INT16>(COMMON_PACKET_ID::S2C_ACCEPT);
	s2cPacket.packetSize = sizeof(MSG_S2C_ACCEPT);

	if (false == Send(newSocId, s2cPacket))
	{
		m_systemMessage.push(newSocId + "로 패킷 전송 실패!!");
	}
}

void MainSystem::OnNetClose(std::shared_ptr<TcpSocket> soc, int socketIndex)
{

}

void MainSystem::OnNetRecv(std::shared_ptr<TcpSocket> soc)
{

}

void MainSystem::OnNetSend(std::shared_ptr<TcpSocket> soc)
{

}

void MainSystem::SendUpdate()
{
	// Listen 소켓 제외 모든 소켓의 큐에있는 데이터 전송
	for (int i = 1; i < m_socketList.size(); i++)
	{
		if (nullptr != m_socketList[i] && false == m_socketList[i]->SendUpdate())
		{
			m_systemMessage.push(i + "번째 Socket->SendUpdate 실패");
		}
	}
}

BLANCNET_END