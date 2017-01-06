#pragma once
#include "afxwin.h"


// CKeyGenDlg 对话框

class CKeyGenDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CKeyGenDlg)

public:
	CKeyGenDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeyGenDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEYGEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CString m_szKeyLevel;
	CComboBox m_cbKeyLevel;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGenerateKey();
	CEdit m_edtValueN;
private:
	CEdit m_edtValueE1;
	CEdit m_edtValueE2;
	CButton m_btnSavePubKey;
	CButton m_btnSavePrivKey;
public:
	afx_msg void OnBnClickedSavePubkey();
	afx_msg void OnBnClickedSavePrivkey();
};
