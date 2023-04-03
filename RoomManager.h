#pragma once

#include "RoomInfo.h"

class RoomManager
{
private:
	RoomManager() {}
	RoomManager(const RoomManager&) {}
	RoomManager& operator=(const RoomManager&) {}
	~RoomManager() {}

public:
	static RoomManager& GetInstance()
	{
		static RoomManager* instance = new RoomManager();
		return *instance;
	}

public:
	void Init();
	void Update();
	int CreateRoom(std::shared_ptr<PlayerInfo> playerInfo, std::wstring roomName, std::wstring roomPw);
	std::shared_ptr<PlayerInfo> LeaveRoom(int roomNum, int netId);
	bool EnterRoom(int roomNum, std::shared_ptr<PlayerInfo> playerInfo, std::wstring roomPw);
	std::shared_ptr<PlayerInfo> GetRoomPlayer(int roomNum, int netId);
	void Finalize();
	std::map<int, std::shared_ptr<RoomInfo>> m_roomList;

	void ChatToPlayer(int netId, int roomNum, std::wstring chat);

private:
	int m_roomNum;
	std::queue<int> m_emptyRoomNum;
};

