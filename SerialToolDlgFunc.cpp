// SerialToolDlgFunc.cpp: implementation of the CSerialToolDlgFunc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "serialTool.h"
#include "SerialToolDlgFunc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define COMCOUNT	16

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSerialToolDlgFunc::CSerialToolDlgFunc()
{

}

CSerialToolDlgFunc::~CSerialToolDlgFunc()
{

}
void CSerialToolDlgFunc::CSerialToolDlgFuncQueryComm(CStringArray& strArrCom)
{
	HANDLE hCom;
	CString strComName,strCom;

	for (int i = 1; i <= COMCOUNT; ++i)
	{
		strComName.Format("\\\\.\\COM%d", i);
		hCom = CreateFile(strComName, GENERIC_READ | GENERIC_WRITE, 0, 0,  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
		if(INVALID_HANDLE_VALUE != hCom )
		{
			//能打开该串口，则添加该串口
			strCom.Format("COM%d", i);
			strArrCom.Add(strCom);

			CloseHandle(hCom);
		}
	}
}

int CSerialToolDlgFunc::CSerialToolDlgFuncConvertHex2CString(CString hexStr, CString dstStr)
{
	return -1;
}

static char ConvertHexChar(char ch)  
{  
	if((ch>='0')&&(ch<='9')) 
		return ch-0x30;  
	else if((ch>='A')&&(ch<='F')) 
		return ch-'A'+10;  
	else if((ch>='a')&&(ch<='f')) 
		return ch-'a'+10; 
	else return (-1); 
}

int CSerialToolDlgFunc::CSerialToolDlgFuncConvertCString2Hex(CString srcStr, CString hexStr)
{
	int hexdata,lowhexdata;
	int hexdatalen = 0;
//	int len = srcStr.GetLength();
////	senddata.SetSize(len/2);
//	for(int i = 0;i<len;)
//	{
//		char lstr;
//		char hstr = str[i];
//		if(hstr == ' ')
//		{
//			i++;
//			continue;
//		}
//		i++;
//		if(i>len)
//			break;
//		lstr = str[i];
//		hexdata = ConvertHexChar(hstr);
//		lowhexdata = ConvertHexChar(lstr);
//		if((hexdata == 16)||(lowhexdata == 16))
//			break;
//		else
//			hexdata = hexdata*16 + lowhexdata;
//		i++;
//		senddata[hexdatalen] = (char)hexdata;
//		hexdatalen++;
//	}
//	senddata.SetSize(hexdatalen);
	return hexdatalen;
}