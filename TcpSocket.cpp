#include "pch.h"
#include "TcpSocket.h"

BLANCNET_BEGIN

TcpSocket::TcpSocket()
{
	const int sendBufSize = 4096;
	const int recvBufSize = 4096;

	m_sendBuf.count = 0;
	m_sendBuf.done = 0;
	m_sendBuf.bufSize = sendBufSize;
	m_sendBuf.data = (char*)malloc(sendBufSize);

	m_recvBuf.count = 0;
	m_recvBuf.done = 0;
	m_recvBuf.bufSize = recvBufSize;
	m_recvBuf.data = (char*)malloc(sendBufSize);

	m_sendQ = new StreamQueue(sendBufSize * 4);
	m_recvQ = new StreamQueue(recvBufSize * 4);
}

TcpSocket::~TcpSocket()
{
	free(m_sendBuf.data);
	free(m_recvBuf.data);

	delete m_sendQ;
	delete m_recvQ;
}

bool TcpSocket::Connect(SOCKADDR_IN& remoteAddr)
{
	if (SOCKET_ERROR == WSAConnect(m_soc, (SOCKADDR*)(&remoteAddr), sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL))
	{
		if (WSAEWOULDBLOCK != WSAGetLastError()) return false;
	}

	return true;
}

bool TcpSocket::Listen()
{
	if (SOCKET_ERROR == listen(m_soc, SOMAXCONN)) return false;

	return true;
}

SOCKET TcpSocket::Accept(SOCKADDR_IN& remoteAddr)
{
	int iAddrLen = sizeof(remoteAddr);

	return WSAAccept(m_soc, (SOCKADDR*)&remoteAddr, &iAddrLen, NULL, 0);
}

bool TcpSocket::SendUpdate()
{
	int error = 0;
	int nSent = 0;
	char* dataFront = nullptr;

	int remain = m_sendBuf.count - m_sendBuf.done;

	if (remain > 0)
	{
		dataFront = &m_sendBuf.data[m_sendBuf.done];
	}
	else if (remain == 0)
	{
		m_sendBuf.count = m_sendQ->Read(m_sendBuf.data, m_sendBuf.bufSize);
		m_sendBuf.done = 0;
		remain = m_sendBuf.count;
		dataFront = m_sendBuf.data;
	}

	assert(remain >= 0);

	nSent = Send(dataFront, remain, error);
	if (nSent < 0) return false;

	m_sendBuf.done += nSent;

	return true;
}

int TcpSocket::Send(char* outBuf, int nLen, int& error, LPWSAOVERLAPPED pOv /*= NULL*/, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine /*= NULL*/)
{
	WSABUF wsaBuf;
	wsaBuf.buf = outBuf;
	wsaBuf.len = nLen;

	DWORD nSent = 0, flag = 0;

	if (SOCKET_ERROR != WSASend(m_soc, &wsaBuf, 1, &nSent, flag, pOv, lpCompletionRoutine))
	{
		return nSent;
	}

	int err = WSAGetLastError();
	error = err;

	if (WSAEWOULDBLOCK == err || WSA_IO_PENDING == err) return 0;

	return -1;
}

bool TcpSocket::RecvUpdate()
{
	int error = 0;

	int emptySpace = m_recvBuf.bufSize - m_recvBuf.count;
	if (emptySpace > 0)
	{
		int nRecv = Recv(&m_recvBuf.data[m_recvBuf.count], emptySpace, error);
		if (nRecv > 0) m_recvBuf.count += nRecv;
	}

	int remain = m_recvBuf.count - m_recvBuf.done;

	assert(remain >= 0);

	if (remain > 0)
	{
		m_recvBuf.done += m_recvQ->Write(&m_recvBuf.data[m_recvBuf.done], remain);
	}

	if (m_recvBuf.done == m_recvBuf.count)
	{
		m_recvBuf.count = 0;
		m_recvBuf.done = 0;
	}

	return true;
}

int TcpSocket::Recv(char* inBuf, int nLen, int& error, LPWSAOVERLAPPED pOv /*= NULL*/, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine /*= NULL*/)
{
	WSABUF wsaBuf;
	wsaBuf.buf = inBuf;
	wsaBuf.len = nLen;

	DWORD nRecv = 0, flag = 0;

	if (SOCKET_ERROR != WSARecv(m_soc, &wsaBuf, 1, &nRecv, &flag, (LPWSAOVERLAPPED)pOv, lpCompletionRoutine))
	{
		return nRecv;
	}

	int err = WSAGetLastError();
	error = err;

	if (WSAEWOULDBLOCK == err || WSA_IO_PENDING == err) return 0;

	return -1;
}

bool TcpSocket::PostPacket(char* buffer, int nSize)
{
	if (m_sendQ->EmptySpace() < nSize) return false;

	m_sendQ->Write(buffer, nSize);

	return true;
}

bool TcpSocket::ReadPacket(char* buffer, int nSize)
{
	if (m_recvQ->Count() < nSize) return false;

	m_recvQ->Read(buffer, nSize);

	return true;
}

bool TcpSocket::PeekPacket(char* buffer, int nSize)
{
	return m_recvQ->Peek(buffer, nSize);
}

void TcpSocket::Reset()
{
	m_sendBuf.count = 0;
	m_sendBuf.done = 0;

	m_recvBuf.count = 0;
	m_recvBuf.done = 0;

	m_sendQ->Clear();
	m_recvQ->Clear();
}

BLANCNET_END