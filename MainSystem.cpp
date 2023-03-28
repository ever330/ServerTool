#include "pch.h"
#include "MainSystem.h"

#include "LobbyManager.h"


BLANCNET_BEGIN
	void MainSystem::Init(std::string ip /*= "0.0.0.0"*/, int port /*= 0*/)
{
	m_socCount = 0;
	m_winManager = std::make_unique<WinSocManager>();
	m_winManager->Init();
	DBManager::GetInstance().Init();

	m_pacAnalyzer = std::make_unique<PacketAnalyzer>();

	/*DBManager::GetInstance().LoginCheck("UnityTest", "1234");
	DBManager::GetInstance().IdDuplicateCheck("UnityTest");
	DBManager::GetInstance().SignUp("CppTest", "1234");*/

	if (port != 0)
	{
		//m_listenSocket = std::make_unique<TcpSocket>();
		m_listenSocket = std::make_shared<TcpSocket>();
		m_listenIp = ip;
		m_listenSocket->SetNetId(0);
		m_socketList.push_back(m_listenSocket);
		m_systemMessage.push("리슨소켓 생성");
		/*m_socCount++;

		for (int i = 0; i < WSA_MAXIMUM_WAIT_EVENTS - 1; i++)
		{
			std::shared_ptr<TcpSocket> newSoc = std::make_shared<TcpSocket>();
			m_socketList.push_back(newSoc);
		}*/
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

	m_sendThread.join();

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

	if (!m_isSThreadRunning)
	{
		m_isSThreadRunning = true;
		m_sendThread = std::thread([] {MainSystem::GetInstance().SendUpdate(); });
	}

	while (m_isRunning)
	{
		EventUpdate();
	}


	return true;
}

bool MainSystem::Send(int netId, void* packet,int nLen)
{
	auto soc = m_socketList[netId];

	if (nullptr != soc)
	{
		soc->PostPacket((char*)packet, nLen);
		m_systemMessage.push(std::to_string(netId) + "번 클라이언트에 패킷 전송.");

		return true;
	}

	m_systemMessage.push("패킷을 보낼 유저가 없습니다.");

	return false;
}

bool MainSystem::Broadcast(PACKET_BASE packet)
{
	for (auto soc : m_socketList)
	{

	}

	return true;
}

bool MainSystem::Listen()
{
	if (nullptr == m_listenSocket)
	{
		m_systemMessage.push("리슨 소켓이 생성되지 않았습니다. Init 필요. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	if (false == m_listenSocket->Open(IPPROTO_TCP))
	{
		m_systemMessage.push("리슨 소켓을 열지 못했습니다. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	if (false == m_listenSocket->EventSelect(FD_ACCEPT | FD_CLOSE))
	{
		m_systemMessage.push("리슨 소켓에 이벤트 연결을 실패했습니다. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	SOCKADDR_IN listenAddr;
	listenAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, m_listenIp.c_str(), &listenAddr.sin_addr);
	listenAddr.sin_port = htons(m_port);

	if (false == m_listenSocket->Bind(listenAddr))
	{
		m_systemMessage.push("리슨 소켓 바인드가 실패했습니다. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	if (false == m_listenSocket->Listen())
	{
		m_systemMessage.push("리슨 실패. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	m_eventTable[m_socketList.size() - 1] = m_listenSocket->GetEventHandle();
	m_systemMessage.push("리슨 시작");

	return true;
}

bool MainSystem::Connect()
{
	if (nullptr == m_connectSocket)
	{
		m_systemMessage.push("커넥트 소켓이 생성되지 않았습니다. Init 필요. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	if (false == m_connectSocket->Open(IPPROTO_TCP))
	{
		m_systemMessage.push("커넥트 소켓을 열지 못했습니다. Init 필요. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	if (false == m_connectSocket->EventSelect(FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE))
	{
		m_systemMessage.push("커넥트 소켓에 이벤트 연결을 실패했습니다. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	SOCKADDR_IN connectAddr;
	connectAddr.sin_family = AF_INET;
	InetPtonA(AF_INET, m_connectIp.c_str(), &connectAddr.sin_addr);
	connectAddr.sin_port = htons(m_port);

	if (false == m_connectSocket->Connect(connectAddr))
	{
		m_systemMessage.push("커넥트 실패. 에러코드 :" + std::to_string(WSAGetLastError()));
		return false;
	}

	return true;
}

void MainSystem::EventUpdate()
{
	int socIndex = 0;

	if ((socIndex = WSAWaitForMultipleEvents(m_socketList.size(), m_eventTable, FALSE, 100, FALSE)) == WSA_WAIT_FAILED)
	{
		m_systemMessage.push("WSAWaitForMultipleEvents() failed. 에러코드 :" + std::to_string(WSAGetLastError()));
		return;
	}

	if (socIndex > m_socketList.size())
		return;

	std::shared_ptr<TcpSocket> nowSoc = m_socketList.at(socIndex);

	if (nullptr != nowSoc)
	{
		m_systemMessage.push(std::to_string(nowSoc->GetNetId()) + "번 소켓 이벤트 실행.");

		WSANETWORKEVENTS NetworkEvents;

		if (SOCKET_ERROR == WSAEnumNetworkEvents(nowSoc->GetSocHandle(), nowSoc->GetEventHandle(), &NetworkEvents))
		{
			m_systemMessage.push("WSAEnumNetworkEvents() failed. 에러코드 :" + std::to_string(WSAGetLastError()));
			return;
		}

		if (NetworkEvents.lNetworkEvents == FD_ACCEPT)
		{
			if (0 != NetworkEvents.iErrorCode[FD_ACCEPT_BIT])
			{
				m_systemMessage.push("FD_ACCEPT failed.");
				return;
			}
			OnNetAccept(nowSoc);
		}

		if (NetworkEvents.lNetworkEvents == FD_CLOSE)
		{
			if (0 != NetworkEvents.iErrorCode[FD_CLOSE_BIT])
			{
				m_systemMessage.push("FD_CLOSE failed.");
				return;
			}
			OnNetClose(nowSoc, nowSoc->GetNetId());
			return;
		}

		if (NetworkEvents.lNetworkEvents == FD_READ)
		{
			if (0 != NetworkEvents.iErrorCode[FD_READ_BIT])
			{
				m_systemMessage.push("FD_READ failed.");
				return;
			}
			OnNetRecv(nowSoc);
		}

		if (NetworkEvents.lNetworkEvents == FD_WRITE)
		{
			if (0 != NetworkEvents.iErrorCode[FD_WRITE_BIT])
			{
				m_systemMessage.push("FD_WRITE failed.");
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
		m_systemMessage.push("getpeername 실패. 에러코드 : " + std::to_string(WSAGetLastError()));
		return;
	}
	if (epLength > sizeof(endpoint))
	{
		m_systemMessage.push("getpeername buffer overrun. 에러코드 : " + std::to_string(WSAGetLastError()));
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
		return " ";

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

	std::lock_guard<std::mutex> lock_guard(socListMutex);

	std::shared_ptr<TcpSocket> newSocket = std::make_shared<TcpSocket>();
	newSocket->SetSocHandle(accept(soc->GetSocHandle(), NULL, NULL));
	if (newSocket->GetSocHandle() == INVALID_SOCKET)
	{
		m_systemMessage.push("accept() filad. 에러코드 : " + std::to_string(WSAGetLastError()));
		return;
	}

	char ipPort[IpPort_Len];
	GetClientIPPort(newSocket->GetSocHandle(), ipPort);
	m_systemMessage.push("접속한 클라이언트 : " + static_cast<std::string>(ipPort));

	int newSocId = 0;
	if (m_emptySocket.empty())
	{
		newSocId = m_socketList.size();
		newSocket->SetNetId(newSocId);
		m_socketList.push_back(newSocket);
	}
	else
	{
		newSocId = m_emptySocket.front();
		m_emptySocket.pop();
		newSocket->SetNetId(newSocId);
		m_socketList[newSocId] = newSocket;
	}


	if (false == newSocket->EventSelect(FD_READ | FD_WRITE | FD_CLOSE))
	{
		m_systemMessage.push("이벤트 설정 실패!");
		closesocket(newSocket->GetSocHandle());
		return;
	}

	m_eventTable[newSocId] = newSocket->GetEventHandle();

	MSG_S2C_ACCEPT s2cPacket;
	s2cPacket.packetId = static_cast<INT16>(COMMON_PACKET_ID::S2C_ACCEPT);
	s2cPacket.packetSize = sizeof(MSG_S2C_ACCEPT);

	if (false == Send(newSocId, reinterpret_cast<char*>(&s2cPacket), s2cPacket.packetSize))
	{
		m_systemMessage.push(std::to_string(newSocId) + "로 패킷 전송 실패!!");
	}
}

void MainSystem::OnNetClose(std::shared_ptr<TcpSocket> soc, int netId)
{
	std::lock_guard<std::mutex> lock_guard(socListMutex);
	// 소켓을 닫고 다음 연결될 소켓이 사용할 수 있도록 인덱스를 보관해둔다.
	soc->Close();
	soc->Reset();
	m_emptySocket.push(netId);
	LobbyManager::GetInstance().PlayerLogOut(netId);

	m_systemMessage.push(std::to_string(netId) + "번 소켓 연결 종료");
}

void MainSystem::OnNetRecv(std::shared_ptr<TcpSocket> soc)
{
	soc->RecvUpdate();

	PACKET_BASE pacHeader;
	while (soc->PeekPacket(reinterpret_cast<char*>(&pacHeader), sizeof(PACKET_BASE)))
	{
		memset(&m_packetRecvBuf, 0, 1024);

		if (soc->ReadPacket(m_packetRecvBuf, pacHeader.packetSize))
		{
			char ipPort[IpPort_Len];
			GetClientIPPort(soc->GetSocHandle(), ipPort);
			//m_systemMessage.push(std::to_string(soc->GetNetId()) + "번 클라이언트의 패킷 도착");

			m_systemMessage.push(m_pacAnalyzer->PacAnalyze(soc, m_packetRecvBuf, pacHeader.packetSize));
		}
		else
			break;
	}
}

void MainSystem::OnNetSend(std::shared_ptr<TcpSocket> soc)
{

}

void MainSystem::SendUpdate()
{
	while (m_isSThreadRunning)
	{
		// Listen 소켓 제외 모든 소켓의 큐에있는 데이터 전송
		for (int i = 1; i < m_socketList.size(); i++)
		{
			if (INVALID_SOCKET != m_socketList[i].get()->GetSocHandle() && false == m_socketList[i]->SendUpdate())
			{
				m_systemMessage.push((std::to_string(i)) + "번째 Socket->SendUpdate 실패");
				OnNetClose(m_socketList[i], i);
			}
		}
	}
}

BLANCNET_END