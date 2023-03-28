#pragma once

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
	void Init();
	bool LoginCheck(const char* userId, const char* userPw);
	bool IdDuplicateCheck(const char* userId);
	bool SignUp(const char* userId, const char* userPw);

private:
	SQLHENV m_henv;
	SQLHDBC m_hdbc;
	SQLHSTMT m_hstmt;
	SQLRETURN m_retcode;
	SQLHDESC m_hIpd;
};

BLANCNET_END