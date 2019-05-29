// SerialToolDlgFunc.h: interface for the CSerialToolDlgFunc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALTOOLDLGFUNC_H__CB10B155_F5E3_434B_92AF_5D05DEE57507__INCLUDED_)
#define AFX_SERIALTOOLDLGFUNC_H__CB10B155_F5E3_434B_92AF_5D05DEE57507__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSerialToolDlgFunc  
{
public:
	CSerialToolDlgFunc();
	virtual ~CSerialToolDlgFunc();

	static void CSerialToolDlgFuncQueryComm(CStringArray&);  
	static int CSerialToolDlgFuncConvertHex2CString(CString hexStr, CString dstStr);
	static int CSerialToolDlgFuncConvertCString2Hex(CString srcStr, CString hexStr);
};

#endif // !defined(AFX_SERIALTOOLDLGFUNC_H__CB10B155_F5E3_434B_92AF_5D05DEE57507__INCLUDED_)
