// NewWinstaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EnumWinstaGui.h"
#include "NewWinstaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewWinstaDlg dialog


CNewWinstaDlg::CNewWinstaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewWinstaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewWinstaDlg)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void CNewWinstaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewWinstaDlg)
	DDX_Text(pDX, IDC_DESKTOP_NAME, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewWinstaDlg, CDialog)
	//{{AFX_MSG_MAP(CNewWinstaDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewWinstaDlg message handlers
