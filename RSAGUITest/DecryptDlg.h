#pragma once
#include "afxwin.h"
#include "RSAUtil.h"


// CDecryptDlg �Ի���

class CDecryptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CDecryptDlg)

public:
	CDecryptDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDecryptDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DECRYPT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReadPrivKey();
private:
	CButton m_btnDecrypt;
	CEdit m_edtEncrypted;
	CEdit m_edtMessage;
//	CStatic m_lblKeyInfo;
	RSAKey m_stPrivateKey;
	CString m_lblKeyInfo;
public:
	afx_msg void OnBnClickedDecrypt();
};
