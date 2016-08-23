
// DupFileFinderDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"

#include <map>
#include <vector>


// CDupFileFinderDlg 대화 상자
class CDupFileFinderDlg : public CDialogEx
{
// 생성입니다.
public:
	CDupFileFinderDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUPFILEFINDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	std::map<CString, CString> DupFilesMap;

	bool GetExts(std::vector<CString>& OutExts);

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSrcPathBrowserButton();
	afx_msg void OnBnClickedDestPathBrowserButton();
	CEdit SrcPathEdit;
	CEdit DestPathEdit;
	CEdit ExtsEdit;
	CListBox ResultListCtrl;
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnBnClickedRemvoeButton();
};
