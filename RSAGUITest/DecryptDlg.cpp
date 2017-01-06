// DecryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSAGUITest.h"
#include "DecryptDlg.h"
#include "afxdialogex.h"


// CDecryptDlg 对话框

IMPLEMENT_DYNAMIC(CDecryptDlg, CDialogEx)

CDecryptDlg::CDecryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DECRYPT, pParent)
	, m_lblKeyInfo(_T(""))
{

}

CDecryptDlg::~CDecryptDlg()
{
}

void CDecryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDB_DECRYPT, m_btnDecrypt);
	DDX_Control(pDX, IDE_ENCRYPTED, m_edtEncrypted);
	DDX_Control(pDX, IDE_MESSAGE, m_edtMessage);
	//  DDX_Control(pDX, IDL_KEY_INFO, m_lblKeyInfo);
	DDX_Text(pDX, IDL_KEY_INFO, m_lblKeyInfo);
}


BEGIN_MESSAGE_MAP(CDecryptDlg, CDialogEx)
	ON_BN_CLICKED(IDB_READ_PRIV_KEY, &CDecryptDlg::OnBnClickedReadPrivKey)
	ON_BN_CLICKED(IDB_DECRYPT, &CDecryptDlg::OnBnClickedDecrypt)
END_MESSAGE_MAP()


// CDecryptDlg 消息处理程序


void CDecryptDlg::OnBnClickedReadPrivKey()
{
	CFileDialog dlgOpenPrivateKey(TRUE);
	dlgOpenPrivateKey.m_ofn.lpstrFilter = "Private Key Files(*.pri)\0*.pri\0\0";
	dlgOpenPrivateKey.m_ofn.lpstrDefExt = "pri";
	char defaultFileName[MAX_PATH] = "PrivateKey.pri";
	dlgOpenPrivateKey.m_ofn.lpstrFile = defaultFileName;
	if (IDOK == dlgOpenPrivateKey.DoModal()) {
		CFile file(dlgOpenPrivateKey.GetPathName(), CFile::modeRead);
		CArchive arch(&file, CArchive::load);
		arch >> m_stPrivateKey.nBit;
		CString n, e;
		arch >> n >> e;
		m_stPrivateKey.n = std::string(n.GetBuffer());
		m_stPrivateKey.e = std::string(e.GetBuffer());
		m_lblKeyInfo.Format("已加载私钥，密钥长度%d位。", m_stPrivateKey.nBit);
		UpdateData(FALSE);
		m_edtMessage.EnableWindow(TRUE);
		m_edtEncrypted.EnableWindow(TRUE);
		m_btnDecrypt.EnableWindow(TRUE);
	}
}


void CDecryptDlg::OnBnClickedDecrypt()
{
	CString sEncryptedMsg;
	m_edtEncrypted.GetWindowTextA(sEncryptedMsg);
	std::string sMessage = RSAUtil::GetInstance()->Decrypt(sEncryptedMsg.GetBuffer(), m_stPrivateKey);
	CString message(sMessage.c_str());
	m_edtMessage.SetWindowTextA(message);
}
