// NewDesktopDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnumWinstaGui.h"
#include "NewDesktopDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewDesktopDlg dialog


CNewDesktopDlg::CNewDesktopDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewDesktopDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewDesktopDlg)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void CNewDesktopDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewDesktopDlg)
	DDX_Text(pDX, IDC_DESKTOP_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewDesktopDlg, CDialog)
	//{{AFX_MSG_MAP(CNewDesktopDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewDesktopDlg message handlers
