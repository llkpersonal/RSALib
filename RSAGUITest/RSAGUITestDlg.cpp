
// RSAGUITestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RSAGUITest.h"
#include "RSAGUITestDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRSAGUITestDlg �Ի���



CRSAGUITestDlg::CRSAGUITestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RSAGUITEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRSAGUITestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDT_CONTROL_PANEL, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CRSAGUITestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDT_CONTROL_PANEL, &CRSAGUITestDlg::OnTcnSelchangeControlPanel)
END_MESSAGE_MAP()


// CRSAGUITestDlg ��Ϣ�������

BOOL CRSAGUITestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	m_tabCtrl.InsertItem(0,"��Կ����");
	m_tabCtrl.InsertItem(1, "RSA����");
	m_tabCtrl.InsertItem(2, "RSA����");

	m_dlgKeyGen.Create(IDD_KEYGEN, &m_tabCtrl);
	m_dlgEncrypt.Create(IDD_ENCRYPT, &m_tabCtrl);
	m_dlgDecrypt.Create(IDD_DECRYPT, &m_tabCtrl);

	CRect rs;
	m_tabCtrl.GetClientRect(&rs);
	rs.top += 21;
	rs.bottom -= 1;
	m_dlgKeyGen.MoveWindow(rs);
	m_dlgEncrypt.MoveWindow(rs);
	m_dlgDecrypt.MoveWindow(rs);
	m_dlgKeyGen.ShowWindow(true);
	m_dlgEncrypt.ShowWindow(false);
	m_dlgDecrypt.ShowWindow(false);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CRSAGUITestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CRSAGUITestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CRSAGUITestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRSAGUITestDlg::OnTcnSelchangeControlPanel(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nCurSel = m_tabCtrl.GetCurSel();

	switch (nCurSel) {
	case 0:
		m_dlgKeyGen.ShowWindow(true);
		m_dlgEncrypt.ShowWindow(false);
		m_dlgDecrypt.ShowWindow(false);
		break;
	case 1:
		m_dlgKeyGen.ShowWindow(false);
		m_dlgEncrypt.ShowWindow(true);
		m_dlgDecrypt.ShowWindow(false);
		break;
	case 2:
		m_dlgKeyGen.ShowWindow(false);
		m_dlgEncrypt.ShowWindow(false);
		m_dlgDecrypt.ShowWindow(true);
		break;
	}

	*pResult = 0;
}
