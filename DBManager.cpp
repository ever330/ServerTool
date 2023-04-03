#include "pch.h"
#include "DBManager.h"

BLANCNET_BEGIN

bool DBManager::Init()
{
	m_henv = NULL;
	m_hdbc = NULL;
	m_hstmt = NULL;
	m_hIpd = NULL;

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

	return true;
}

bool DBManager::LoginCheck(const wchar_t* userId, const wchar_t* userPw, LoginData& loginData)
{
	SQLWCHAR* id = (SQLWCHAR*)userId;
	SQLWCHAR* pw = (SQLWCHAR*)userPw;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;
	SQLLEN cbValue3 = SQL_NTS;
	SQLLEN cbValue4 = SQL_NTS;
	SQLLEN cbValue5 = SQL_NTS;
	SQLLEN cbValue6 = SQL_NTS;
	SQLLEN cbValue7 = SQL_NTS;

	bool result = false;

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call LogIn(?, ?, ?, ?, ?, ?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, pw, 0, &cbValue2);

	m_retcode = SQLBindParameter(m_hstmt, 3, SQL_PARAM_OUTPUT, SQL_C_BIT, SQL_BIT, 0, 0, &result, sizeof(bool), &cbValue3);

	m_retcode = SQLBindParameter(m_hstmt, 4, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &loginData.modelGender, sizeof(int), &cbValue4);

	m_retcode = SQLBindParameter(m_hstmt, 5, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &loginData.modelH, sizeof(int), &cbValue5);

	m_retcode = SQLBindParameter(m_hstmt, 6, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &loginData.modelU, sizeof(int), &cbValue6);

	m_retcode = SQLBindParameter(m_hstmt, 7, SQL_PARAM_OUTPUT, SQL_C_SLONG, SQL_INTEGER, 0, 0, &loginData.modelL, sizeof(int), &cbValue7);

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

bool DBManager::IdDuplicateCheck(const wchar_t* userId)
{
	SQLWCHAR* id = (SQLWCHAR*)userId;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;
	bool result = false;

	/*m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
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
	}*/

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call IdDuplicateCheck(?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_OUTPUT, SQL_C_BIT, SQL_BIT, 0, 0, &result, 0, &cbValue2);

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

bool DBManager::SignUp(const wchar_t* userId, const wchar_t* userPw)
{
	SQLWCHAR* id = (SQLWCHAR*)userId;
	SQLWCHAR* pw = (SQLWCHAR*)userPw;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;

	/*m_retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_henv);
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
	}*/

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call SignUp(?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 100, 0, pw, 0, &cbValue2);

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

bool DBManager::UserModelUpdate(const wchar_t* userId, LoginData& loginData)
{
	LPCWCH* id = (LPCWCH*)userId;
	SQLLEN cbValue1 = SQL_NTS;
	SQLLEN cbValue2 = SQL_NTS;
	SQLLEN cbValue3 = SQL_NTS;
	SQLLEN cbValue4 = SQL_NTS;
	SQLLEN cbValue5 = SQL_NTS;

	m_retcode = SQLPrepare(m_hstmt, (SQLWCHAR*)L"{call ModelUpdate(?, ?, ?, ?, ?)}", SQL_NTS);
	if ((m_retcode != SQL_SUCCESS_WITH_INFO) && (m_retcode != SQL_SUCCESS))
	{
		return false;
	}

	m_retcode = SQLBindParameter(m_hstmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WVARCHAR, 20, 0, id, 0, &cbValue1);

	m_retcode = SQLBindParameter(m_hstmt, 2, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 4, 0, &loginData.modelGender, 0, &cbValue2);

	m_retcode = SQLBindParameter(m_hstmt, 3, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 4, 0, &loginData.modelH, 0, &cbValue3);

	m_retcode = SQLBindParameter(m_hstmt, 4, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 4, 0, &loginData.modelU, 0, &cbValue4);

	m_retcode = SQLBindParameter(m_hstmt, 5, SQL_PARAM_INPUT, SQL_C_SLONG, SQL_INTEGER, 4, 0, &loginData.modelL, 0, &cbValue5);

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

void DBManager::Finalize()
{
	SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
	SQLDisconnect(m_hdbc);
	SQLFreeHandle(SQL_HANDLE_DBC, m_hdbc);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hstmt);
	SQLFreeHandle(SQL_HANDLE_DESC, m_hIpd);
}


BLANCNET_END