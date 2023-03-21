#pragma once

BLANCNET_BEGIN

class StreamQueue
{
public:
	StreamQueue() = delete;
	StreamQueue(const StreamQueue&) = delete;
	StreamQueue& operator=(const StreamQueue&) = delete;

	StreamQueue(const int size)
	{
		m_size = size;
		m_buffer = (char*)malloc(size);
		Clear();
	}
	~StreamQueue()
	{
		free(m_buffer);
	}

	bool IsEmpty() { return (m_nData == 0) ? true : false; }
	bool IsFull() { return (m_size == m_nData) ? true : false; }

	void Clear()
	{
		m_nData = m_readIndex = m_writeIndex = 0;
		memset(m_buffer, 0, m_size);
	}

	int EmptySpace() { return m_size - m_nData; }
	int Size() { return m_size; }
	int Count() { return m_nData; }
	
	bool Peek(char* peekBuf, int peekLen);
	int Read(char* desBuf, int bufLen);
	int Write(const char* srcData, int bytesData);


private:
	int m_size;			// ť�� ũ��
	int m_nData;		// ť�� ����ִ� �������� ����
	int m_readIndex;	// ����ť�� front
	int m_writeIndex;	// ����ť�� rear
	char* m_buffer;
};

BLANCNET_END