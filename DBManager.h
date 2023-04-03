#pragma once
#include "PacketAnalyzer.h"

BLANCNET_BEGIN

class DBManager
{
private:
	DBManager() {}
	DBManager(const DBManager&) {}
	DBManager& operator=(const DBManager&) {}
	~DBManager() {}

public:
	static DBManager& GetInstance()
	{
		static DBManager* instance = new DBManager();
		return *instance;
	}

public:
	bool Init();
	bool LoginCheck(const wchar_t* userId, const wchar_t* userPw, LoginData& loginData);
	bool IdDuplicateCheck(const wchar_t* userId);
	bool SignUp(const wchar_t* userId, const wchar_t* userPw);
	bool UserModelUpdate(const wchar_t* userId, LoginData& loginData);
	void Finalize();

private:
	SQLHENV m_henv;
	SQLHDBC m_hdbc;
	SQLHSTMT m_hstmt;
	SQLRETURN m_retcode;
	SQLHDESC m_hIpd;
};

BLANCNET_END