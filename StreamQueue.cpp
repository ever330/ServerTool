#include "pch.h"
#include "StreamQueue.h"

BLANCNET_BEGIN

bool StreamQueue::Peek(char* peekBuf, int peekLen)
{
	if (peekLen > m_nData) return false;

	if (m_readIndex < m_writeIndex)
	{
		memcpy(peekBuf, &m_buffer[m_readIndex], peekLen);
	}
	else
	{
		int nBackPart = m_size - m_readIndex; // 큐의 뒤쪽에 남은 데이터의 수

		// 확인하려는 데이터가 큐 뒤쪽 데이터보다 적은경우
		if (peekLen <= nBackPart)
		{
			memcpy(peekBuf, &m_buffer[m_readIndex], peekLen);
		}
		// 확인하려는 데이터가 큐 뒤쪽보다 적어서 큐 앞에서도 데이터를 가져올 경우
		else
		{
			int nFrontPart = peekLen - nBackPart;
			memcpy(&peekBuf[0], &m_buffer[m_readIndex], nBackPart);
			memcpy(&peekBuf[nBackPart], &m_buffer[0], nFrontPart);
		}
	}

	return true;
}

int StreamQueue::Read(char* desBuf, int bufLen)
{
	if (IsEmpty()) return 0;

	int nRead = std::min<short>(m_nData, bufLen);

	if (m_readIndex < m_writeIndex)
	{
		memcpy(desBuf, &m_buffer[m_readIndex], nRead);
	}
	else
	{
		int nBackPart = m_size - m_readIndex;

		if (nRead <= nBackPart)
		{
			memcpy(desBuf, &m_buffer[m_readIndex], nRead);
		}
		else
		{
			int nFrontPart = nRead - nBackPart;
			memcpy(&desBuf[0], &m_buffer[m_readIndex], nBackPart);
			memcpy(&desBuf[nBackPart], &m_buffer[0], nFrontPart);
		}
	}

	m_nData -= nRead;
	m_readIndex += nRead;
	if (m_readIndex >= m_size) m_readIndex -= m_size;

	return nRead;
}

int StreamQueue::Write(const char* srcData, int dataLen)
{
	if (IsFull()) return 0;

	int nEmptySpace = EmptySpace();
	int nWrite = std::min<short>(nEmptySpace, dataLen);

	if (m_readIndex > m_writeIndex)
	{
		memcpy(&m_buffer[m_writeIndex], srcData, nWrite);
	}
	else
	{
		int nBackSpace = m_size - m_writeIndex;

		if (nBackSpace >= nWrite)
		{
			memcpy(&m_buffer[m_writeIndex], srcData, nWrite);
		}
		else
		{
			int nFrontSpace = nWrite - nBackSpace;
			memcpy(&m_buffer[m_writeIndex], &srcData[0], nBackSpace);
			memcpy(&m_buffer[0], &srcData[nBackSpace], nFrontSpace);
		}
	}

	m_nData += nWrite;
	m_writeIndex += nWrite;
	if (m_writeIndex >= m_size) m_writeIndex -= m_size;

	return nWrite;
}

BLANCNET_END