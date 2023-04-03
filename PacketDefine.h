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
	C2S_CREATE_MODEL = 200,
	S2C_NEW_LOBBY_USER,
	S2C_DELETE_LOBBY_USER,
	C2S_CREATE_ROOM,
	S2C_CREATE_ROOM_RES,
	C2S_ENTER_ROOM,
	S2C_ENTER_ROOM_RES,
	S2C_NEW_ROOM,
	S2C_DELETE_ROOM,
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
	INT16 idSize;
	INT16 pwSize;
	wchar_t userId[128];
	wchar_t userPw[128];
};

struct MSG_S2C_LOGIN_RES : PACKET_BASE
{
	bool loginResult;
	INT16 modelExist;
	INT16 modelH;
	INT16 modelU;
	INT16 modelL;
};

struct MSG_C2S_DUPLICATE_CHECK : PACKET_BASE
{
	INT16 idSize;
	wchar_t userId[128];
};

struct MSG_S2C_DUPLICATE_CHECK_RES : PACKET_BASE
{
	bool checkResult;
};

struct MSG_C2S_SIGNUP : PACKET_BASE
{
	INT16 idSize;
	INT16 pwSize;
	wchar_t userId[128];
	wchar_t userPw[128];
};

struct MSG_S2C_SIGNUP_RES : PACKET_BASE
{
	bool signUpResult;
};

#pragma endregion

#pragma region LOBBY PACKET

struct MSG_C2S_CREATE_MODEL : PACKET_BASE
{
	INT16 idSize;
	wchar_t userId[128];
	INT16 modelGender;
	INT16 modelH;
	INT16 modelU;
	INT16 modelL;
};

struct MSG_S2C_NEW_LOBBY_USER : PACKET_BASE
{
	INT16 idSize;
	wchar_t userId[128];
};

struct MSG_S2C_DELETE_LOBBY_USER : PACKET_BASE
{
	INT16 idSize;
	wchar_t userId[128];
};

struct MSG_C2S_CREATE_ROOM : PACKET_BASE
{
	INT16 nameSize;
	INT16 pwSize;
	wchar_t roomName[128];
	wchar_t roomPw[128];
};

struct MSG_S2C_CREATE_ROOM_RES : PACKET_BASE
{
	INT16 roomNum;
};

struct MSG_C2S_ENTER_ROOM : PACKET_BASE
{
	INT16 roomNum;
	INT16 pwSize;
	wchar_t roomPw[128];
};

struct MSG_S2C_ENTER_ROOM_RES : PACKET_BASE
{
	bool enterResult;
};

struct MSG_S2C_NEW_ROOM : PACKET_BASE
{
	bool isPrivate;
	INT16 roomNum;
	INT16 nameSize;
	wchar_t roomName[128];
};

struct MSG_S2C_DELETE_ROOM : PACKET_BASE
{
	INT16 roomNum;
};

#pragma endregion

#pragma region ROOM PACKET

struct MSG_S2C_ROOM_USER : PACKET_BASE
{
	INT16 modelGender;
	INT16 modelH;
	INT16 modelU;
	INT16 modelL;
	INT16 nameSize;
	wchar_t userName[128];
};

struct MSG_C2S_CHAT : PACKET_BASE
{
	INT16 roomNum;
	INT16 chatSize;
	wchar_t userChat[128];
};

struct MSG_S2C_CHAT : PACKET_BASE
{
	INT16 idSize;
	INT16 chatSize;
	wchar_t chatUser[128];
	wchar_t echoChat[128];
};

struct MSG_C2S_LEAVE_ROOM : PACKET_BASE
{
	INT16 roomNum;
};

struct MSG_S2C_LEAVE_ROOM_RES : PACKET_BASE
{
	INT16 nameSize;
	wchar_t leavePlayer[128];
};

#pragma endregion

#pragma pack(pop)