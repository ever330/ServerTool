#pragma once

#include "pch.h"

class PlayerInfo
{
public:
	PlayerInfo() = delete;
	~PlayerInfo() {}
	PlayerInfo(int netId, std::string id) : m_netId(netId), m_nickName(id) {}

public:
	void SetNetId(int netId) { m_netId = netId; }
	int GetNetId() { return m_netId; }
	void SetNickName(std::string nickName) { m_nickName = nickName; }
	std::string GetNickName() { return m_nickName; }
	void SetLifeTimer(std::tm lifeTimer) { m_lifeTimer = lifeTimer; }
	std::tm GetLifeTimer() { return m_lifeTimer; }

private:
	int m_netId;
	std::string m_nickName;
	std::tm m_lifeTimer;
};