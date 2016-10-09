#if !defined(AFX_NEWWINSTADLG_H__8D2E9B12_08E0_11D5_9104_00E0293CAA2C__INCLUDED_)
#define AFX_NEWWINSTADLG_H__8D2E9B12_08E0_11D5_9104_00E0293CAA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewWinstaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewWinstaDlg dialog

class CNewWinstaDlg : public CDialog
{
// Construction
public:
	CNewWinstaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewWinstaDlg)
	enum { IDD = IDD_NEW_WINSTA };
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewWinstaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewWinstaDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWWINSTADLG_H__8D2E9B12_08E0_11D5_9104_00E0293CAA2C__INCLUDED_)
