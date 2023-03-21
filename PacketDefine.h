#pragma once

#include "pch.h"

///
///패킷 ID용 enum class 정의
///
enum class COMMON_PACKET_ID : INT16
{
	S2C_ERROR = 1,
	S2C_ACCEPT,
	S2C_HEART_BEAT,
	C2S_HEART_BEAT,
};

enum class TITLE_PACKET_ID : INT16
{
	C2S_LOGIN = 1,
	S2C_LOGIN_RES,
	C2S_DUPLICATE_CHECK,
	S2C_DUPLICATE_CHECK_RES,
	C2S_SIGNUP,
	S2C_SIGNUP_RES,
};

enum class LOBBY_PACKET_ID : INT16
{
	S2C_LOBBY_USER = 1,
	C2S_CREATE_ROOM,
	S2C_CREATE_ROOM_RES,
	C2S_ENTER_ROOM,
	S2C_ENTER_ROOM_RES,
};

enum class ROOM_PACKET_ID : INT16
{
	S2C_ROOM_USER = 1,
	C2S_CHAT,
	S2C_CHAT,
	C2S_LEAVE_ROOM,
	S2C_LEAVE_ROOM_RES,
};

///
/// 패킷 헤더와 패킷 정의
///

struct PACKET_BASE
{
	// 모든 패킷에 쓰일 헤더
	INT16 packetSize;
	INT16 packetId;

	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
	}
};

#pragma region COMMON PACKET

struct MSG_S2C_ERROR : PACKET_BASE
{
	INT16 errorCode;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& errorCode;
	}
};

struct MSG_S2C_ACCEPT : PACKET_BASE
{
	int netId;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& netId;
	}
};

struct MSG_S2C_HEART_BEAT : PACKET_BASE
{
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
	}
};

struct MSG_C2S_HEART_BEAT : PACKET_BASE
{
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
	}
};

#pragma endregion

#pragma region TITLE PACKET

struct MSG_C2S_LOGIN : PACKET_BASE
{
	std::string userId;
	std::string userPw;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& userId;
		ar& userPw;
	}
};

struct MSG_S2C_LOGIN_RES : PACKET_BASE
{
	bool loginResult;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& loginResult;
	}
};

struct MSG_C2S_DUPLICATE_CHECK : PACKET_BASE
{
	std::string userId;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& userId;
	}
};

struct MSG_S2C_DUPLICATE_CHECK_RES : PACKET_BASE
{
	bool checkResult;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& checkResult;
	}
};

struct MSG_C2S_SIGNUP : PACKET_BASE
{
	std::string userId;
	std::string userPw;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& userId;
		ar& userPw;
	}
};

struct MSG_S2C_SIGNUP_RES : PACKET_BASE
{
	bool signUpResult;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& signUpResult;
	}
};

#pragma endregion

#pragma region LOBBY PACKET

struct MSG_S2C_LOBBY_USER : PACKET_BASE
{
	std::list<std::string> userList;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& userList;
	}
};

struct MSG_C2S_CREATE_ROOM : PACKET_BASE
{
	std::string roomName;
	std::string roomPW;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& roomName;
		ar& roomPW;
	}
};

struct MSG_S2C_CREATE_ROOM_RES : PACKET_BASE
{
	bool createResult;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& createResult;
	}
};

struct MSG_C2S_ENTER_ROOM : PACKET_BASE
{
	std::string roomName;
	std::string roomPW;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& roomName;
		ar& roomPW;
	}
};

struct MSG_C2S_ENTER_ROOM_RES : PACKET_BASE
{
	bool enterResult;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& enterResult;
	}
};

#pragma endregion

#pragma region ROOM PACKET

//
//S2C_ROME_USER = 1,
//C2S_CHAT,
//S2C_CHAT,
//C2S_LEAVE_ROOM,
//S2C_LEAVE_ROOM_RES,

struct MSG_S2C_ROOM_USER : PACKET_BASE
{
	std::list<std::string> userList;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& userList;
	}
};

struct MSG_C2S_CHAT : PACKET_BASE
{
	std::string userChat;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& userChat;
	}
};

struct MSG_S2C_CHAT : PACKET_BASE
{
	std::string echoChat;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& echoChat;
	}
};

struct MSG_C2S_LEAVE_ROOM : PACKET_BASE
{
	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
	}
};

struct MSG_S2C_LEAVE_ROOM_RES : PACKET_BASE
{
	std::string leaveAnnounce;

	template<typename Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& packetSize;
		ar& packetId;
		ar& leaveAnnounce;
	}
};

#pragma endregion