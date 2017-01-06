#pragma once

#include "RSAUtil.h"
#include "afxwin.h"

// CEncryptDlg 对话框

class CEncryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CEncryptDlg)

public:
	CEncryptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEncryptDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ENCRYPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadPubKey();
private:
	RSAKey m_stPublicKey;
	CEdit m_edtEncrypted;
	CEdit m_edtMessage;
	CString m_lblKeyInfo;
	CButton m_btnEncrypt;
public:
	afx_msg void OnBnClickedEncrypt();
};
