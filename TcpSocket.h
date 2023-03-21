#pragma once
#include "WinSocket.h"
#include "StreamQueue.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

BLANCNET_BEGIN

class TcpSocket : public WinSocket
{
	struct TCPBuffer
	{
		short count;
		short done;
		short bufSize;
		char* data;
	};

public:
	TcpSocket(const TcpSocket&) = delete;
	TcpSocket& operator=(const TcpSocket&) = delete;

	TcpSocket();
	~TcpSocket();

	// 클라이언트용(접속요청)
	bool Connect(SOCKADDR_IN& remoteAddr);

	// 서버용(접속대기 / 접속수락)
	bool Listen();
	SOCKET Accept(SOCKADDR_IN& remoteAddr);

	bool SendUpdate();
	int	Send(char* outBuf, int nLen, int& error, LPWSAOVERLAPPED pOv = NULL, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine = NULL);

	bool RecvUpdate();
	int	Recv(char* inBuf, int nLen, int& error, LPWSAOVERLAPPED pOv = NULL, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine = NULL);

	bool PostPacket(char* buffer, int nSize);
	bool ReadPacket(char* buffer, int nSize);
	bool PeekPacket(char* buffer, int nSize);

	void Reset();

	int GetNetId() { return m_netId; }
	void SetNetId(int netId) { m_netId = netId; }

private:
	TCPBuffer m_sendBuf;
	TCPBuffer m_recvBuf;

	StreamQueue* m_sendQ;
	StreamQueue* m_recvQ;

	int m_netId;
};

BLANCNET_END