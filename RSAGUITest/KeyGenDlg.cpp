// KeyGenDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RSAGUITest.h"
#include "KeyGenDlg.h"
#include "afxdialogex.h"
#include "PrimeUtil.h"
#include "RSAUtil.h"

// CKeyGenDlg 对话框

IMPLEMENT_DYNAMIC(CKeyGenDlg, CDialogEx)

CKeyGenDlg::CKeyGenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KEYGEN, pParent)
{

}

CKeyGenDlg::~CKeyGenDlg()
{
}

void CKeyGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_CBString(pDX, IDC_KEYLEVEL, m_szKeyLevel);
	DDX_Control(pDX, IDC_KEYLEVEL, m_cbKeyLevel);
	DDX_Control(pDX, IDE_VALUE_N, m_edtValueN);
	DDX_Control(pDX, IDE_VALUE_E1, m_edtValueE1);
	DDX_Control(pDX, IDE_VALUE_E2, m_edtValueE2);
	DDX_Control(pDX, IDB_SAVE_PUBKEY, m_btnSavePubKey);
	DDX_Control(pDX, IDB_SAVE_PRIVKEY, m_btnSavePrivKey);
}


BEGIN_MESSAGE_MAP(CKeyGenDlg, CDialogEx)
	ON_BN_CLICKED(IDB_GENERATE_KEY, &CKeyGenDlg::OnBnClickedGenerateKey)
	ON_BN_CLICKED(IDB_SAVE_PUBKEY, &CKeyGenDlg::OnBnClickedSavePubkey)
	ON_BN_CLICKED(IDB_SAVE_PRIVKEY, &CKeyGenDlg::OnBnClickedSavePrivkey)
END_MESSAGE_MAP()


// CKeyGenDlg 消息处理程序


BOOL CKeyGenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	srand(time(NULL));
	m_cbKeyLevel.AddString("RSA-512");
	m_cbKeyLevel.AddString("RSA-768");
	m_cbKeyLevel.AddString("RSA-1024");
	m_cbKeyLevel.AddString("RSA-2048");
	m_cbKeyLevel.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CKeyGenDlg::OnBnClickedGenerateKey()
{
	int nSelIdx = m_cbKeyLevel.GetCurSel();
	int nKeyBit[] = {512,768,1024,2048};
	std::vector<BigInteger> vKeys;
	RSAUtil::GetInstance()->GenerateKey(nKeyBit[nSelIdx],vKeys);
	std::ostringstream soutn,soute1,soute2;
	soutn << vKeys[0];
	CString s(soutn.str().c_str());
	m_edtValueN.SetWindowTextA(s);
	soute1 << vKeys[1];
	soute2 << vKeys[2];
	CString se1(soute1.str().c_str());
	CString se2(soute2.str().c_str());
	m_edtValueE1.SetWindowTextA(se1);
	m_edtValueE2.SetWindowTextA(se2);
	m_btnSavePubKey.EnableWindow(TRUE);
	m_btnSavePrivKey.EnableWindow(TRUE);
}


void CKeyGenDlg::OnBnClickedSavePubkey()
{
	CFileDialog dlgSavePubKey(FALSE);
	dlgSavePubKey.m_ofn.lpstrFilter = "Public Key Files(*.pub)\0*.pub\0\0";
	char fileName[MAX_PATH] = "PubKey.pub";
	dlgSavePubKey.m_ofn.lpstrFile = fileName;
	dlgSavePubKey.m_ofn.lpstrDefExt = "pub";
	dlgSavePubKey.m_ofn.lStructSize = sizeof(dlgSavePubKey.m_ofn);
	if (IDOK == dlgSavePubKey.DoModal()) {
		CString sFileName = dlgSavePubKey.GetPathName();
		CFile file(sFileName, CFile::modeCreate | CFile::modeWrite);
		CArchive arch(&file, CArchive::store);
		CString sValueN, sValueE1;
		m_edtValueN.GetWindowTextA(sValueN);
		m_edtValueE1.GetWindowTextA(sValueE1);
		int nSelIdx = m_cbKeyLevel.GetCurSel();
		int nKeyBit[] = { 512,768,1024,2048 };
		arch << nKeyBit[nSelIdx] << sValueN << sValueE1;
	}
}


void CKeyGenDlg::OnBnClickedSavePrivkey()
{
	CFileDialog dlgSavePubKey(FALSE);
	dlgSavePubKey.m_ofn.lpstrFilter = "Private Key Files(*.pri)\0*.pri\0\0";
	char fileName[MAX_PATH] = "PrivateKey.pri";
	dlgSavePubKey.m_ofn.lpstrFile = fileName;
	dlgSavePubKey.m_ofn.lpstrDefExt = "pri";
	dlgSavePubKey.m_ofn.lStructSize = sizeof(dlgSavePubKey.m_ofn);
	if (IDOK == dlgSavePubKey.DoModal()) {
		CString sFileName = dlgSavePubKey.GetPathName();
		CFile file(sFileName, CFile::modeCreate | CFile::modeWrite);
		CArchive arch(&file, CArchive::store);
		CString sValueN, sValueE2;
		m_edtValueN.GetWindowTextA(sValueN);
		m_edtValueE2.GetWindowTextA(sValueE2);
		int nSelIdx = m_cbKeyLevel.GetCurSel();
		int nKeyBit[] = { 512,768,1024,2048 };
		arch << nKeyBit[nSelIdx] << sValueN << sValueE2;
	}
}
