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
	C2S_LOGIN = 100,
	S2C_LOGIN_RES,
	C2S_DUPLICATE_CHECK,
	S2C_DUPLICATE_CHECK_RES,
	C2S_SIGNUP,
	S2C_SIGNUP_RES,
};

enum class LOBBY_PACKET_ID : INT16
{
	S2C_LOBBY_USER = 200,
	C2S_CREATE_ROOM,
	S2C_CREATE_ROOM_RES,
	C2S_ENTER_ROOM,
	S2C_ENTER_ROOM_RES,
};

enum class ROOM_PACKET_ID : INT16
{
	S2C_ROOM_USER = 300,
	C2S_CHAT,
	S2C_CHAT,
	C2S_LEAVE_ROOM,
	S2C_LEAVE_ROOM_RES,
};

///
/// 패킷 헤더와 패킷 정의
///
#pragma pack(push)
#pragma pack(1)
struct PACKET_BASE
{
	// 모든 패킷에 쓰일 헤더
	INT16 packetSize;
	INT16 packetId;
};

#pragma region COMMON PACKET

struct MSG_S2C_ERROR : PACKET_BASE
{
	INT16 errorCode;
};

struct MSG_S2C_ACCEPT : PACKET_BASE
{
	int netId;
};

struct MSG_S2C_HEART_BEAT : PACKET_BASE
{

};

struct MSG_C2S_HEART_BEAT : PACKET_BASE
{

};

#pragma endregion

#pragma region TITLE PACKET

struct MSG_C2S_LOGIN : PACKET_BASE
{
	char userId[40];
	char userPw[100];
};

struct MSG_S2C_LOGIN_RES : PACKET_BASE
{
	bool loginResult;
};

struct MSG_C2S_DUPLICATE_CHECK : PACKET_BASE
{
	char userId[40];
};

struct MSG_S2C_DUPLICATE_CHECK_RES : PACKET_BASE
{
	bool checkResult;
};

struct MSG_C2S_SIGNUP : PACKET_BASE
{
	char userId[40];
	char userPw[100];
};

struct MSG_S2C_SIGNUP_RES : PACKET_BASE
{
	bool signUpResult;
};

#pragma endregion

#pragma region LOBBY PACKET

struct MSG_S2C_LOBBY_USER : PACKET_BASE
{
	std::list<char[40]> userList;
};

struct MSG_C2S_CREATE_ROOM : PACKET_BASE
{
	char roomName[40];
	char roomPW[100];
};

struct MSG_S2C_CREATE_ROOM_RES : PACKET_BASE
{
	bool createResult;
};

struct MSG_C2S_ENTER_ROOM : PACKET_BASE
{
	char roomName[40];
	char roomPW[100];
};

struct MSG_C2S_ENTER_ROOM_RES : PACKET_BASE
{
	bool enterResult;
};

#pragma endregion

#pragma region ROOM PACKET

struct MSG_S2C_ROOM_USER : PACKET_BASE
{
	std::list<char[40]> userList;
};

struct MSG_C2S_CHAT : PACKET_BASE
{
	char userChat[100];
};

struct MSG_S2C_CHAT : PACKET_BASE
{
	char echoChat[100];
};

struct MSG_C2S_LEAVE_ROOM : PACKET_BASE
{

};

struct MSG_S2C_LEAVE_ROOM_RES : PACKET_BASE
{
	char leaveAnnounce[100];
};

#pragma endregion

#pragma pack(pop)