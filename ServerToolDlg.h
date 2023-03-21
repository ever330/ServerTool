
// ServerToolDlg.h: 헤더 파일
//

#pragma once
#include "MainSystem.h"

using namespace myNet;

// CServerToolDlg 대화 상자
class CServerToolDlg : public CDialogEx
{
// 생성입니다.
public:
	CServerToolDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVERTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();

	void WriteServerMessage(CString message);

private:
	std::unique_ptr<MainSystem> mainSys = nullptr;
	std::thread NetThread;

public:
	CEdit m_messageOut;
};
