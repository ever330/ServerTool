#pragma once
#include "PacketDefine.h"
#include "TcpSocket.h"


BLANCNET_BEGIN

struct LoginData
{
	int modelGender;
	int modelH;
	int modelU;
	int modelL;
};

class PacketAnalyzer
{
public:
	PacketAnalyzer();
	~PacketAnalyzer() {}

public:
	std::string PacAnalyze(std::shared_ptr<TcpSocket> soc, void* packet, int len);

private:
	void PacHeartBeat(int netId);
	void PacLogin(int netId, void* packet, int len);
	void PacDuplicateCheck(int netId, void* packet, int len);
	void PacSignUp(int netId, void* packet, int len);
	void PacCreateModel(int netId, void* packet, int len);
	void PacCreateRoom(int netId, void* packet, int len);
	void PacEnterRoom(int netId, void* packet, int len);
	void PacChat(int netId, void* packet, int len);
	void PacLeaveRoom(int netId, void* packet, int len);
};

BLANCNET_END