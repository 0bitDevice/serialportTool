// serialToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "serialTool.h"
#include "serialToolDlg.h"
#include "SerialToolDlgFunc.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EPSILON					0.00000001
#define FSPROGRESSMAXRANGE		100

#define COMMSENDDATACYCLE		10//ms

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialToolDlg dialog

CSerialToolDlg::CSerialToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSerialToolDlg)
	m_SendStr = _T("");
	m_RecvStr = _T("");
	m_HexSendChk = FALSE;
	m_HexRecvChk = FALSE;
	m_bOpenComm = FALSE;
	m_bTimerStart = FALSE;
	m_TimerThread = NULL;
	m_bRecordRecv = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSerialToolDlg)
	DDX_Control(pDX, IDC_PROGRESS_SENDFILE, m_ProgressSendFile);
	DDX_Control(pDX, IDC_BUTTON_RECVSAVE, m_RecvSaveBut);
	DDX_Control(pDX, IDC_BUTTON_SETTIMER, m_SetTimerBut);
	DDX_Control(pDX, IDC_EDIT_RECV, m_RecvEdit);
	DDX_Control(pDX, IDC_BUTTON_OPENCOM, m_OpenCommBut);
	DDX_Control(pDX, IDC_BUTTON_OPENSENDFILE, m_OpensendfileBut);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_SendBut);
	DDX_Control(pDX, IDC_CHECK_HEXRECV, m_HexRecvChkBut);
	DDX_Control(pDX, IDC_CHECK_HEXSEND, m_HexSendChkBut);
	DDX_Control(pDX, IDC_EDIT_SEND, m_SendEdit);
	DDX_Control(pDX, IDC_COMBO_PORT, m_PortNumCom);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_BaudRateCom);
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_StopBitCom);
	DDX_Control(pDX, IDC_COMBO_PARITY, m_ParityCom);
	DDX_Control(pDX, IDC_COMBO_FLOW, m_FLowCtrlCom);
	DDX_Control(pDX, IDC_COMBO_BIT, m_BitWidthCom);
	DDX_Control(pDX, IDC_MSCOMM1, m_mscomm);
	DDX_Text(pDX, IDC_EDIT_SEND, m_SendStr);
	DDX_Text(pDX, IDC_EDIT_RECV, m_RecvStr);
	DDX_Check(pDX, IDC_CHECK_HEXSEND, m_HexSendChk);
	DDX_Check(pDX, IDC_CHECK_HEXRECV, m_HexRecvChk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSerialToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSerialToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENCOM, OnButtonOpencom)
	ON_CBN_SELCHANGE(IDC_COMBO_PORT, OnSelchangeComboPort)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUD, OnSelchangeComboBaud)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITY, OnSelchangeComboParity)
	ON_CBN_SELCHANGE(IDC_COMBO_STOPBIT, OnSelchangeComboStopbit)
	ON_CBN_SELCHANGE(IDC_COMBO_FLOW, OnSelchangeComboFlow)
	ON_CBN_SELCHANGE(IDC_COMBO_BIT, OnSelchangeComboBit)
	ON_CBN_DROPDOWN(IDC_COMBO_PORT, OnDropdownComboPort)
	ON_BN_CLICKED(IDC_BUTTON_OPENSENDFILE, OnButtonOpensendfile)
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_CHECK_HEXSEND, OnCheckHexsend)
	ON_BN_CLICKED(IDC_CHECK_HEXRECV, OnCheckHexrecv)
	ON_BN_CLICKED(IDC_BUTTON_CLEARRECV, OnButtonClearrecv)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SETTIMER, OnButtonSettimer)
	ON_BN_CLICKED(IDC_BUTTON_RECVSAVE, OnButtonRecvsave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSerialToolDlg message handlers

BOOL CSerialToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	CString title;						//���ֲ�ͬ����
	title.Format("serialTool-%d", this->m_hWnd);
	SetWindowText(title);

	m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERSTART));
	m_ProgressSendFile.SetRange(0, FSPROGRESSMAXRANGE);

	m_RecvEdit.SetLimitText(UINT_MAX);

	CStringArray strArrCom;
	CSerialToolDlgFunc::QueryComm(strArrCom);
	for (int i = 0; i < strArrCom.GetSize(); ++i)
	{
		m_PortNumCom.AddString(strArrCom.GetAt(i));
	}
	m_PortNumCom.SetCurSel(2);

	m_BaudRateCom.AddString(_T("115200"));
	m_BaudRateCom.AddString(_T("9600"));
	m_BaudRateCom.AddString(_T("38400"));
	m_BaudRateCom.AddString(_T("57600"));
	m_BaudRateCom.SetCurSel(0);


	m_ParityCom.AddString(_T("N"));
	m_ParityCom.AddString(_T("O"));
	m_ParityCom.AddString(_T("E"));
	m_ParityCom.SetCurSel(0);
	

	m_BitWidthCom.AddString(_T("8"));
	m_BitWidthCom.AddString(_T("7"));
	m_BitWidthCom.SetCurSel(0);

	m_StopBitCom.AddString(_T("1"));
	m_StopBitCom.AddString(_T("2"));
	m_StopBitCom.SetCurSel(0);

	OnSelchangeComboPort();
	OnSelchangeComboBaud(); 
	OnSelchangeComboParity();
	OnSelchangeComboBit(); 
	OnSelchangeComboStopbit();

	m_SendBut.EnableWindow(FALSE);
	m_OpensendfileBut.EnableWindow(FALSE);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CSerialToolDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CSerialToolDlg)
	ON_EVENT(CSerialToolDlg, IDC_MSCOMM1, 1 /* OnComm */, OnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CSerialToolDlg::OnCommMscomm() 
{
	// TODO: Add your control notification handler code here
	VARIANT variant_inp;
	COleSafeArray safearray_inp;
	LONG len;
	BYTE rxdata[2048];								//����BYTE���� An 8-bit integerthat is not signed.
	CString strtemp, strPack;

	if(m_mscomm.GetCommEvent()==2)					//�¼�ֵΪ2��ʾ���ջ����������ַ�
	{
		variant_inp=m_mscomm.GetInput();			//��������
		safearray_inp=variant_inp;					//VARIANT�ͱ���ת��ΪColeSafeArray�ͱ���
		len=safearray_inp.GetOneDimSize();			//�õ���Ч���ݳ���

		for(long i=0; i<len; i++)
		{
			safearray_inp.GetElement(&i,rxdata+i);	//ת��ΪBYTE������
		}

		strPack.Empty();

		for(long k=0; k<len; k++)
		{
			BYTE bt=*(char*)(rxdata+k);
			if (m_HexRecvChk)
			{
				strtemp.Format("%02x ",bt);
			}
			else
			{
				strtemp.Format("%c",bt);
			}

			strPack += strtemp;

			m_RecvStrBuff += strtemp;
		}

		if (m_bRecordRecv)
		{
			CSerialToolDlgFunc::RecordData(m_fileRecv, strPack);
		}
		
	}
}

void CSerialToolDlg::OnButtonOpencom()
{
	// TODO: Add your control notification handler code here
	if(m_mscomm.GetPortOpen())					//��������Ǵ򿪵ģ����йرմ���
    {
		KillTimer(2);				//�رս�������ʾˢ�µĶ�ʱ��
		((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(TRUE);			//�����ò���combox
		((CComboBox *)GetDlgItem(IDC_COMBO_BAUD))->EnableWindow(TRUE);
		((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->EnableWindow(TRUE);
		((CComboBox *)GetDlgItem(IDC_COMBO_BIT))->EnableWindow(TRUE);
		((CComboBox *)GetDlgItem(IDC_COMBO_STOPBIT))->EnableWindow(TRUE);
		
		m_OpensendfileBut.EnableWindow(FALSE);
		m_SendBut.EnableWindow(FALSE);

        m_mscomm.SetPortOpen(FALSE);
		m_OpenCommBut.SetWindowText( _T("�򿪴���") );
		m_bOpenComm = FALSE;
		m_bTimerStart = FALSE;

		if (m_TimerThread != NULL)
		{
			WaitForSingleObject(m_ThreadStopEvnt, INFINITE);
			m_TimerThread = NULL;

			m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERSTART));
			KillTimer(1);
		}

		m_ProgressSendFile.SetPos(0);
		if (m_fileSend.m_pStream)
		{	
			m_fileSend.Close();
			m_OpensendfileBut.SetWindowText("�򿪷����ļ�");
			m_SendEdit.EnableWindow(TRUE);
			m_SendEdit.SetWindowText(NULL);
		}

		if (m_fileRecv.m_pStream)
		{
			m_fileRecv.Close();
			m_bRecordRecv = FALSE;
			m_RecvSaveBut.SetWindowText("��¼������");
		}
    }
	else
	{
		SetTimer(2, 200, NULL);		//�򿪽�������ʾˢ�µĶ�ʱ��
		m_PortNum.Delete(0, 3);
		m_mscomm.SetCommPort(_ttoi(m_PortNum));
		CString strPara;
		strPara=m_BaudRate;
		strPara+=",";
		strPara+=m_Parity;
		strPara+=",";
		strPara+=m_BitWidth;
		strPara+=",";
		strPara+=m_StopBit;
		m_mscomm.SetSettings(strPara);
		m_mscomm.SetInBufferSize(1024);			//���ջ�����
		m_mscomm.SetInBufferCount(0);
		m_mscomm.SetOutBufferSize(1024);		//���ͻ�����
		m_mscomm.SetInputLen(0);				//���õ�ǰ���������ݳ���Ϊ0,��ʾȫ����ȡ
		m_mscomm.SetInputMode(1);				//�Զ����Ʒ�ʽ��д����   0���ַ�, 1�Ƕ�����
		m_mscomm.SetRThreshold(1);				//���ջ�������1����1�������ַ�ʱ���������������ݵ�  OnComm
		m_mscomm.SetPortOpen(TRUE);

		if (m_mscomm.GetPortOpen())
		{
			((CComboBox *)GetDlgItem(IDC_COMBO_PORT))->EnableWindow(FALSE);			//�ر����ò���combox
			((CComboBox *)GetDlgItem(IDC_COMBO_BAUD))->EnableWindow(FALSE);
			((CComboBox *)GetDlgItem(IDC_COMBO_PARITY))->EnableWindow(FALSE);
			((CComboBox *)GetDlgItem(IDC_COMBO_BIT))->EnableWindow(FALSE);
			((CComboBox *)GetDlgItem(IDC_COMBO_STOPBIT))->EnableWindow(FALSE);

			m_OpensendfileBut.EnableWindow(TRUE);
			m_SendBut.EnableWindow(TRUE);

			m_OpenCommBut.SetWindowText( _T("�رմ���") );
			m_bOpenComm = TRUE;
		}
		else
		{
			MessageBox("���ڴ�ʧ��!!!");
			return;
		}
	}
}

void CSerialToolDlg::OnSelchangeComboPort() 
{
	// TODO: Add your control notification handler code here
	m_PortNumCom.GetLBText( m_PortNumCom.GetCurSel(), m_PortNum);
}

void CSerialToolDlg::OnDropdownComboPort() 
{
	// TODO: Add your control notification handler code here
	m_PortNumCom.SetCurSel(-1);
	m_PortNumCom.ResetContent();

	CStringArray strArrCom;
	CSerialToolDlgFunc::QueryComm(strArrCom);
	for (int i = 0; i < strArrCom.GetSize(); ++i)
	{
		m_PortNumCom.AddString(strArrCom.GetAt(i));
	}
	m_PortNumCom.SetCurSel(1);
}

void CSerialToolDlg::OnSelchangeComboBaud() 
{
	// TODO: Add your control notification handler code here
	m_BaudRateCom.GetLBText( m_BaudRateCom.GetCurSel(), m_BaudRate);
}

void CSerialToolDlg::OnSelchangeComboParity() 
{
	// TODO: Add your control notification handler code here
	m_ParityCom.GetLBText( m_ParityCom.GetCurSel(), m_Parity);
}

void CSerialToolDlg::OnSelchangeComboBit() 
{
	// TODO: Add your control notification handler code here
	m_BitWidthCom.GetLBText( m_BitWidthCom.GetCurSel(), m_BitWidth);
}

void CSerialToolDlg::OnSelchangeComboStopbit() 
{
	// TODO: Add your control notification handler code here
	m_StopBitCom.GetLBText( m_StopBitCom.GetCurSel(), m_StopBit);
}

void CSerialToolDlg::OnSelchangeComboFlow() 
{
	// TODO: Add your control notification handler code here
}


void CSerialToolDlg::OnButtonOpensendfile() 
{
	// TODO: Add your control notification handler code here
    LPCTSTR szFilter = "�ļ� (*.txt)|*.txt|�����ļ� (*.*)|*.*||";
	CString strPathName;
	
	if (m_fileSend.m_pStream)
	{	
		m_fileSend.Close();
		m_OpensendfileBut.SetWindowText("�򿪷����ļ�");
		m_SendEdit.EnableWindow(TRUE);
		m_SendEdit.SetWindowText(NULL);
	}
	else
	{
		CFileDialog fd(TRUE,"*.txt", NULL, OFN_HIDEREADONLY, szFilter);	
		if(IDCANCEL == fd.DoModal())
			return ;
		strPathName = fd.GetPathName();
		
		if(!m_fileSend.Open(strPathName, CFile::shareExclusive | CFile::modeRead))
		{
			AfxMessageBox("��ʧ�ܣ�");
			return ;
		}

		m_SendEdit.SetWindowText(strPathName);
		m_SendEdit.EnableWindow(FALSE);
		m_fileSendcentiLength = m_fileSend.GetLength() / FSPROGRESSMAXRANGE;
		m_ProgressSendFile.SetPos(0);
		m_OpensendfileBut.SetWindowText("�رշ����ļ�");
	}
}

BOOL CSerialToolDlg::PreTranslateMessage(MSG* pMsg)
{
    if(pMsg->message==WM_KEYDOWN&&pMsg->wParam==VK_RETURN)    
        return TRUE; 

    return CDialog::PreTranslateMessage(pMsg);
}

void CSerialToolDlg::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	BOOL bFLag = m_HexSendChkBut.GetCheck();
	if(bFLag)
	{
		CByteArray hexdata;  
		CSerialToolDlgFunc::ConvertCString2Hex(m_SendStr, hexdata);
		m_mscomm.SetOutput(COleVariant(hexdata));
	}
	else
	{
		m_mscomm.SetOutput(COleVariant(m_SendStr));
	}
}

void CSerialToolDlg::OnCheckHexsend() 
{
	// TODO: Add your control notification handler code here
	BOOL bFLag = m_HexSendChkBut.GetCheck();
	if(bFLag)
	{
		if(IDCANCEL == MessageBox(_T("16���Ʒ��������Կո�Ϊ�ָ���\neg��7F 89 2A���Ƿ����뽫�����ԣ�"), NULL, MB_OKCANCEL))
		{
			m_HexSendChkBut.SetCheck(BST_UNCHECKED);
			return;
		}
	}

	m_HexSendChkBut.SetCheck(bFLag);
	m_HexSendChk = bFLag;
}

void CSerialToolDlg::OnCheckHexrecv() 
{
	// TODO: Add your control notification handler code here
	BOOL bFLag = m_HexRecvChkBut.GetCheck();
	if(bFLag)
	{}

	m_HexRecvChkBut.SetCheck(bFLag);
	m_HexRecvChk = bFLag;
}

void CSerialToolDlg::OnButtonClearrecv() 
{
	// TODO: Add your control notification handler code here
	m_RecvEdit.Clear();
	m_RecvStr.Empty();
	UpdateData(FALSE);
}

void CSerialToolDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bTimerStart)
	{
		m_bTimerStart = FALSE;
		WaitForSingleObject(m_ThreadStopEvnt, INFINITE);
		m_TimerThread = NULL;
	}
	if(m_mscomm.GetPortOpen())		//��������Ǵ򿪵ģ����йرմ���
    {
        m_mscomm.SetPortOpen(FALSE);
    }
	if (m_fileSend.m_pStream)				//�������ļ��򿪣���ر�
	{
		m_fileSend.Close();
	}
	if (m_fileRecv.m_pStream)
	{
		m_fileRecv.Close();
	}

	CDialog::OnClose();
}

void CSerialToolDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
        case 1:
			if (m_fileSend.m_pStream)
			{
				int posProgress = m_fileSend.GetPosition() / m_fileSendcentiLength;
				m_ProgressSendFile.SetPos( posProgress );
				if (m_fileSend.GetLength() == m_fileSend.GetPosition())
				{
					OnButtonSettimer();
					KillTimer(1);
					m_fileSend.Close();
					m_OpensendfileBut.SetWindowText("�򿪷����ļ�");
					m_SendEdit.EnableWindow(TRUE);
					m_SendEdit.SetWindowText(NULL);
				}
			}
            break;
        case 2:
			{
				int RecvStrLen = m_RecvEdit.GetWindowTextLength();		
				m_RecvEdit.SetSel(RecvStrLen, RecvStrLen, TRUE);
				m_RecvEdit.ReplaceSel(m_RecvStrBuff);
				m_RecvStrBuff.Empty();
			}
            break;
        case 3:
			{
				CString strData;
				int ret = 0, retCount = 0;
				while (0 == ret && retCount < 3)		//����3�зǷ��ַ�����!!!������������!!!
				{
					ret = CSerialToolDlgFunc::ProcessingData(m_fileSend, strData);
					++retCount;
				}
				m_mscomm.SetOutput(COleVariant(strData));
			}
            break;
    }
	CDialog::OnTimer(nIDEvent);
}

void CSerialToolDlg::OnButtonSettimer() 
{
	// TODO: Add your control notification handler code here
	m_bTimerStart = !m_bTimerStart;
	if (m_bTimerStart)
	{
		if (m_fileSend.m_pStream && m_bOpenComm)
		{
			m_TimerThread = AfxBeginThread((AFX_THREADPROC)timerThreadProc, this, THREAD_PRIORITY_HIGHEST);

			m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERFIN));
			SetTimer(1, 1000, NULL);

			SetTimer(3, COMMSENDDATACYCLE, NULL);
		}
	}
	else
	{
		if (m_TimerThread != NULL)
		{
			WaitForSingleObject(m_ThreadStopEvnt, INFINITE);
			m_TimerThread = NULL;

			m_SetTimerBut.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_TIMERSTART));
			KillTimer(1);

			KillTimer(3);
		}
	}
}

DWORD WINAPI CSerialToolDlg::timerThreadProc(LPVOID pParam)
{
	CSerialToolDlg* pSerialToolDlg = (CSerialToolDlg*)pParam;
	SetThreadAffinityMask(GetCurrentThread(), 1);  
		
	while (pSerialToolDlg->m_bTimerStart)
	{
//		CString strData;
//		int ret = 0, retCount = 0;
//		while (0 == ret && retCount < 3)		//����3�зǷ��ַ�����!!!������������!!!
//		{
//			ret = CSerialToolDlgFunc::ProcessingData(pSerialToolDlg->m_fileSend, strData);
//			++retCount;
//		}
//		pSerialToolDlg->m_mscomm.SetOutput(COleVariant(strData));

		LARGE_INTEGER start, end;
		LARGE_INTEGER freq;
		double passedTime = 0.0;
		QueryPerformanceFrequency(&freq);  
		QueryPerformanceCounter(&start);  

		while ((pSerialToolDlg->m_bTimerStart) && ((passedTime < COMMSENDDATACYCLE) && (fabs(passedTime - COMMSENDDATACYCLE) >= EPSILON)))
		{
			QueryPerformanceCounter(&end);

			passedTime = 1000 * (end.QuadPart - start.QuadPart) / (double)freq.QuadPart;
		}
	}

	pSerialToolDlg->m_ThreadStopEvnt.SetEvent();

	return 0;
}

void CSerialToolDlg::OnButtonRecvsave() 
{
	// TODO: Add your control notification handler code here
	LPCTSTR szFilter = "�ļ� (*.txt)|*.txt|�����ļ� (*.*)|*.*||";
	CString strPathName;
	CFileDialog fd(FALSE,"*.txt", NULL, OFN_HIDEREADONLY, szFilter);	

	if (m_fileRecv.m_pStream)
	{
		m_fileRecv.Close();
		m_bRecordRecv = FALSE;
		AfxMessageBox("����ɹ���");
		m_RecvSaveBut.SetWindowText("��¼������");
	}
	else
	{
		if(IDCANCEL == fd.DoModal())
			return;
		strPathName = fd.GetPathName();
		
		if(!m_fileRecv.Open(strPathName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			AfxMessageBox("����ʧ�ܣ�");
			m_bRecordRecv = FALSE;
			m_RecvSaveBut.SetWindowText("��¼������");
		}
		else
		{
			m_bRecordRecv = TRUE;
			m_RecvSaveBut.SetWindowText("��¼�С���");
		}
	}
}
