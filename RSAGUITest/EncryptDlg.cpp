// EncryptDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSAGUITest.h"
#include "EncryptDlg.h"
#include "afxdialogex.h"


// CEncryptDlg 对话框

IMPLEMENT_DYNAMIC(CEncryptDlg, CDialogEx)

CEncryptDlg::CEncryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ENCRYPT, pParent)
	, m_lblKeyInfo("请选择公钥")
{

}

CEncryptDlg::~CEncryptDlg()
{
}

void CEncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDE_CRYPTED, m_edtEncrypted);
	DDX_Control(pDX, IDE_MESSAGE, m_edtMessage);
	DDX_Text(pDX, IDL_KEY_INFO, m_lblKeyInfo);
	DDX_Control(pDX, IDB_ENCRYPT, m_btnEncrypt);
}


BEGIN_MESSAGE_MAP(CEncryptDlg, CDialogEx)
	ON_BN_CLICKED(IDB_READ_PUB_KEY, &CEncryptDlg::OnBnClickedReadPubKey)
	ON_BN_CLICKED(IDB_ENCRYPT, &CEncryptDlg::OnBnClickedEncrypt)
END_MESSAGE_MAP()


// CEncryptDlg 消息处理程序


void CEncryptDlg::OnBnClickedReadPubKey()
{
	CFileDialog dlgOpenPubKey(TRUE);
	dlgOpenPubKey.m_ofn.lpstrFilter = "Public Key Files(*.pub)\0*.pub\0\0";
	dlgOpenPubKey.m_ofn.lpstrDefExt = "pub";
	char defaultFileName[MAX_PATH] = "PubKey.pub";
	dlgOpenPubKey.m_ofn.lpstrFile = defaultFileName;
	if (IDOK == dlgOpenPubKey.DoModal()) {
		CFile file(dlgOpenPubKey.GetPathName(), CFile::modeRead);
		CArchive arch(&file, CArchive::load);
		arch >> m_stPublicKey.nBit;
		CString n, e;
		arch >> n >> e;
		m_stPublicKey.n = std::string(n.GetBuffer());
		m_stPublicKey.e = std::string(e.GetBuffer());
		m_lblKeyInfo.Format("已加载公钥，密钥长度%d位。", m_stPublicKey.nBit);
		UpdateData(FALSE);
		m_edtMessage.EnableWindow(TRUE);
		m_edtEncrypted.EnableWindow(TRUE);
		m_btnEncrypt.EnableWindow(TRUE);
	}
}


void CEncryptDlg::OnBnClickedEncrypt()
{
	CString message;
	m_edtMessage.GetWindowTextA(message);
	std::string sEncryptedMsg = RSAUtil::GetInstance()->Encrypt(message.GetBuffer(), m_stPublicKey);
	CString encryptedMessage(sEncryptedMsg.c_str());
	m_edtEncrypted.SetWindowTextA(encryptedMessage);
}
