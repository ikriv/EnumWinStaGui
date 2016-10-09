#ifndef XERROR_H_INCLUDED
#define XERROR_H_INCLUDED

#pragma once

class XError
{
    public:
        XError( DWORD ErrorCode, LPCTSTR Message, ... );
        void Show( CWnd* Parent );
        DWORD GetErrorCode() { return m_ErrorCode; };

    private:
        CString m_Msg;
        DWORD m_ErrorCode;
};

#endif //XERROR_H_INCLUDED
