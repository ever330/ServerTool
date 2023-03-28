#include "pch.h"
#include "DBManager.h"

BLANCNET_BEGIN

void DBManager::Init()
{
	m_henv = NULL;
	m_hdbc = NULL;
	m_hstmt = NULL;
	m_hIpd = NULL;
}

bool DBManager::LoginCheck(const char* userId, const char* userPw)
{
	SQLCHAR* id = (SQLCHAR*)userId;
	SQLCHAR* pw = (SQLCHAR*)userPw;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;
	SQLLEN cbValue3 = SQL_NTS;
	bool result = false;
	
	m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLConnect(m_hdbc, (SQLWCHAR*)L"MyDatabase", SQL_NTS, (SQLWCHAR*)L"Blanc", SQL_NTS, (SQLWCHAR*)L"kocca1234@", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call LogIn(?, ?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_WVARCHAR, 20, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_WVARCHAR, 100, 0, pw, 0, &cbValue2);

	m_retcode = SQLBindParameter(m_hstmt, 3, SQL_PARAM_OUTPUT, SQL_C_BIT, SQL_BIT, 2, 0, &result, 0, &cbValue3);

	m_retcode = SQLGetStmtAttr(m_hstmt, SQL_ATTR_IMP_PARAM_DESC, &m_hIpd, 0, 0);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}
	m_retcode = SQLExecute(m_hstmt);

	if (result)
	{
		return true;
	}
	return false;
}

bool DBManager::IdDuplicateCheck(const char* userId)
{
	SQLCHAR* id = (SQLCHAR*)userId;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;
	bool result = false;

	m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLConnect(m_hdbc, (SQLWCHAR*)L"MyDatabase", SQL_NTS, (SQLWCHAR*)L"Blanc", SQL_NTS, (SQLWCHAR*)L"kocca1234@", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call IdDuplicateCheck(?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_WVARCHAR, 10, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_BIT, SQL_BIT, 2, 0, &result, 0, &cbValue2);

	m_retcode = SQLGetStmtAttr(m_hstmt, SQL_ATTR_IMP_PARAM_DESC, &m_hIpd, 0, 0);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}
	m_retcode = SQLExecute(m_hstmt);

	if (result)
	{
		return true;
	}
	return false;
}

bool DBManager::SignUp(const char* userId, const char* userPw)
{

	SQLCHAR* id = (SQLCHAR*)userId;
	SQLCHAR* pw = (SQLCHAR*)userPw;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;

	m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLSetEnvAttr(m_henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_DBC, m_henv, &m_hdbc);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLConnect(m_hdbc, (SQLWCHAR*)L"MyDatabase", SQL_NTS, (SQLWCHAR*)L"Blanc", SQL_NTS, (SQLWCHAR*)L"kocca1234@", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLAllocHandle(SQL_HANDLE_STMT, m_hdbc, &m_hstmt);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call SignUp(?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_WVARCHAR, 10, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR, SQL_WVARCHAR, 10, 0, pw, 0, &cbValue2);

	m_retcode = SQLGetStmtAttr(m_hstmt, SQL_ATTR_IMP_PARAM_DESC, &m_hIpd, 0, 0);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}
	m_retcode = SQLExecute(m_hstmt);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}
	return true;
}

BLANCNET_END