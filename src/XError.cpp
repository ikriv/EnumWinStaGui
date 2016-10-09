#include "stdafx.h"
#include "Xerror.h"

XError::XError( DWORD ErrorCode, LPCTSTR Format, ... )
:
m_ErrorCode(ErrorCode)
{
    CString Msg;

    va_list Args;
    va_start( Args, Format );

    int const MAX_SIZE = 1024;

    LPTSTR Buffer = Msg.GetBuffer(MAX_SIZE);
    _vsntprintf( Buffer, MAX_SIZE-1, Format, Args );
    Buffer[MAX_SIZE-1] = 0;
    Msg.ReleaseBuffer();

    va_end( Args );

    CString SystemMsg;
    DWORD FormatResult = FormatMessage(
                                          FORMAT_MESSAGE_FROM_SYSTEM, // flags              
                                          NULL,                       // source             
                                          ErrorCode,                  // message id         
                                          0,                          // language           
                                          SystemMsg.GetBuffer(1024),  // buffer             
                                          1024,                       // buffer size        
                                          NULL                        // arguments          
                                         );
    SystemMsg.ReleaseBuffer();
    
    if (FormatResult==0)
        SystemMsg.Empty();

    m_Msg.Format("%s. Error %d. %s", Msg, ErrorCode, SystemMsg );
};

void XError::Show( CWnd* Parent )
{
    Parent->MessageBox( m_Msg, NULL, MB_ICONSTOP | MB_OK );
};
