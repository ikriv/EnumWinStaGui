// EnumWinstaGuiDlg.h : header file
//

#if !defined(AFX_ENUMWINSTAGUIDLG_H__664A7F70_076F_11D5_9100_00E0293CAA2C__INCLUDED_)
#define AFX_ENUMWINSTAGUIDLG_H__664A7F70_076F_11D5_9100_00E0293CAA2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEnumWinstaGuiDlg dialog

class CEnumWinstaGuiDlg : public CDialog
{
// Construction
public:
	CEnumWinstaGuiDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEnumWinstaGuiDlg)
	enum { IDD = IDD_ENUMWINSTAGUI_DIALOG };
	CTreeCtrl	m_WinstaTree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEnumWinstaGuiDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

    CImageList m_ImageList;
    void PopulateTree();
    void ShowContextMenu( UINT MenuId, CPoint ScreenPos );

    void OnEditWinstaSecurity(HTREEITEM hItem);
    void OnEditDesktopSecurity(HTREEITEM hItem);

    HANDLE OpenUserObject( HTREEITEM hItem, ACCESS_MASK Mask );
    HANDLE OpenUserObject( HTREEITEM hItem, int nAttempts, ACCESS_MASK const* pMasks );

    HDESK   OpenDesktop( HTREEITEM hItem, ACCESS_MASK Mask );
    HDESK   OpenDesktop( HTREEITEM hItem, int nAttempts, ACCESS_MASK const* pMasks );

    HWINSTA OpenWinsta( HTREEITEM hItem, ACCESS_MASK Mask );
    HWINSTA OpenWinsta( HTREEITEM hItem, int nAttempts, ACCESS_MASK const* pMasks ); 

	// Generated message map functions
	//{{AFX_MSG(CEnumWinstaGuiDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRclickWinstaTree(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRefreshTree();
    afx_msg void OnNewWinsta();
    afx_msg void OnEditSecurity();
    afx_msg void OnCreateDesktop();
    afx_msg void OnRunCmdExe();
    afx_msg void OnTakeOwnership();    
    afx_msg void OnSwitchTo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENUMWINSTAGUIDLG_H__664A7F70_076F_11D5_9100_00E0293CAA2C__INCLUDED_)
