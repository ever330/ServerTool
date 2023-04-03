#pragma once

#include "pch.h"
#include "PlayerInfo.h"

class RoomInfo
{
public:
	RoomInfo() = delete;
	~RoomInfo() {}
	RoomInfo(int roomNum, std::shared_ptr<PlayerInfo> playerInfo, std::wstring roomName, std::wstring roomPw)
	{
		m_roomPlayerList.push_back(playerInfo);
		m_roomNum = roomNum;
		m_roomName = roomName;
		m_roomPw = roomPw;
	}

public:
	void SetRoomPlayer(std::shared_ptr<PlayerInfo> playerInfo) { m_roomPlayerList.push_back(playerInfo); }

	std::shared_ptr<PlayerInfo> LeaveRoomPlayer(int netId)
	{
		std::shared_ptr<PlayerInfo> leavePlayer;
		for (auto player : m_roomPlayerList)
		{
			if (player->GetNetId() == netId)
			{
				leavePlayer = player;
				m_roomPlayerList.remove(player);
				return leavePlayer;
			}
		}
	}

	std::list<std::shared_ptr<PlayerInfo>> GetRoomPlayers()
	{ return m_roomPlayerList; }

	std::wstring GetRoomName() { return m_roomName; }
	std::wstring GetRoomPw() { return m_roomPw; }

	int RoomPlayers() { return m_roomPlayerList.size(); }

private:
	std::wstring m_roomName;
	std::wstring m_roomPw;
	std::list<std::shared_ptr<PlayerInfo>> m_roomPlayerList;
	int m_roomNum;
};
