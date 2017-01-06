
// RSAGUITestDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "KeyGenDlg.h"
#include "EncryptDlg.h"
#include "DecryptDlg.h"

// CRSAGUITestDlg �Ի���
class CRSAGUITestDlg : public CDialogEx
{
// ����
public:
	CRSAGUITestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RSAGUITEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
