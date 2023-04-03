#pragma once

#include "pch.h"
#include "PacketAnalyzer.h"

class PlayerInfo
{
public:
	PlayerInfo() = delete;
	~PlayerInfo() {}
	PlayerInfo(int netId, std::wstring id, myNet::LoginData modelData) : m_netId(netId), m_nickName(id), m_modelData(modelData) {}

public:
	void SetNetId(int netId) { m_netId = netId; }
	int GetNetId() { return m_netId; }

	void SetNickName(std::wstring nickName) { m_nickName = nickName; }
	std::wstring GetNickName() { return m_nickName; }

	void SetLifeTimer(std::tm lifeTimer) { m_lifeTimer = lifeTimer; }
	std::tm GetLifeTimer() { return m_lifeTimer; }

	myNet::LoginData GetModelData() { return m_modelData; }
	void SetModelData(myNet::LoginData modelData) { m_modelData = modelData; }

private:
	int m_netId;
	std::wstring m_nickName;
	std::tm m_lifeTimer;
	myNet::LoginData m_modelData;
};