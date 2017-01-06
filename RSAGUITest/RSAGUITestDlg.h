
// RSAGUITestDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "KeyGenDlg.h"
#include "EncryptDlg.h"
#include "DecryptDlg.h"

// CRSAGUITestDlg 对话框
class CRSAGUITestDlg : public CDialogEx
{
// 构造
public:
	CRSAGUITestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RSAGUITEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnTcnSelchangeControlPanel(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CTabCtrl m_tabCtrl;
	CKeyGenDlg m_dlgKeyGen;
	CEncryptDlg m_dlgEncrypt;
public:
	afx_msg void OnTcnSelchangeControlPanel(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CDecryptDlg m_dlgDecrypt;
};
