// serialToolDlg.h : header file
//
//{{AFX_INCLUDES()
#include "mscomm.h"

//}}AFX_INCLUDES

#if !defined(AFX_SERIALTOOLDLG_H__DF70D52F_48A3_417D_BBCA_BDFD6858B126__INCLUDED_)
#define AFX_SERIALTOOLDLG_H__DF70D52F_48A3_417D_BBCA_BDFD6858B126__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSerialToolDlg dialog

class CSerialToolDlg : public CDialog
{
// Construction
public:
	CSerialToolDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL PreTranslateMessage(MSG* pMsg);
// Dialog Data
	//{{AFX_DATA(CSerialToolDlg)
	enum { IDD = IDD_SERIALTOOL_DIALOG };
	CEdit	m_RecvEdit;
	CButton	m_OpenCommBut;
	CButton	m_OpensendfileBut;
	CButton	m_SendBut;
	CButton	m_HexRecvChkBut;
	CButton	m_HexSendChkBut;
	CEdit	m_SendEdit;
	CComboBox	m_PortNumCom;
	CComboBox	m_BaudRateCom;
	CComboBox	m_StopBitCom;
	CComboBox	m_ParityCom;
	CComboBox	m_FLowCtrlCom;
	CComboBox	m_BitWidthCom;
	CString		m_PortNum;
	CString		m_BaudRate;
	CString		m_StopBit;
	CString		m_Parity;
	CString		m_FLowCtrl;
	CString		m_BitWidth;
	CMSComm		m_mscomm;
	CString		m_SendStr;
	CString		m_RecvStr;
	BOOL	m_HexSendChk;
	BOOL	m_HexRecvChk;
	//}}AFX_DATA

	CStdioFile	m_fileSend;
	BOOL		m_bFileExist;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSerialToolDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSerialToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCommMscomm();
	afx_msg void OnButtonOpencom();
	afx_msg void OnSelchangeComboPort();
	afx_msg void OnSelchangeComboBaud();
	afx_msg void OnSelchangeComboParity();
	afx_msg void OnSelchangeComboStopbit();
	afx_msg void OnSelchangeComboFlow();
	afx_msg void OnSelchangeComboBit();
	afx_msg void OnDropdownComboPort();
	afx_msg void OnButtonOpensendfile();
	afx_msg void OnButtonSend();
	afx_msg void OnCheckHexsend();
	afx_msg void OnCheckHexrecv();
	afx_msg void OnButtonClearrecv();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERIALTOOLDLG_H__DF70D52F_48A3_417D_BBCA_BDFD6858B126__INCLUDED_)
