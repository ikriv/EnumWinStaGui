#ifndef KHANDLE_H_INCLUDED
#define KHANDLE_H_INCLUDED

#pragma once

class KHandle
{
    public:
        KHandle( HANDLE h = NULL ) : m_Handle(h) {};
        ~KHandle() 
            { 
                Close();
            };

        operator HANDLE() { return m_Handle; };

        void operator= (HANDLE h) 
            { 
                if (m_Handle == h) return; 
                Close(); 
                m_Handle = h; 
            };

        HANDLE* operator&() { ASSERT(!m_Handle); return &m_Handle; };

        void Close()
        {
            if (m_Handle && (m_Handle != INVALID_HANDLE_VALUE))
            {
                CloseHandle(m_Handle);
            }
        };

    private:
        KHandle( KHandle const& ) {}; // not implemented
        void operator=( KHandle const& ); // not implemented

        HANDLE m_Handle;
};

class KWinsta
{
    public:
        KWinsta( HWINSTA h = NULL ) : m_Handle(h) {};
        ~KWinsta() 
            { 
                Close();
            };

        operator HWINSTA() { return m_Handle; };

        void operator= (HWINSTA h) 
            { 
                if (m_Handle == h) return; 
                Close(); 
                m_Handle = h; 
            };

        HWINSTA* operator&() { ASSERT(!m_Handle); return &m_Handle; };

        void Close()
        {
            if (m_Handle && (m_Handle != INVALID_HANDLE_VALUE))
            {
                CloseWindowStation(m_Handle);
            }
        };

    private:
        KWinsta( KWinsta const& ) {}; // not implemented
        void operator=( KWinsta const& ); // not implemented

        HWINSTA m_Handle;
};

class KDesktop
{
    public:
        KDesktop( HDESK h = NULL ) : m_Handle(h) {};
        ~KDesktop() 
            { 
                Close();
            };

        operator HDESK() { return m_Handle; };

        void operator= (HDESK h) 
            { 
                if (m_Handle == h) return; 
                Close(); 
                m_Handle = h; 
            };

        HDESK* operator&() { ASSERT(!m_Handle); return &m_Handle; };

        void Close()
        {
            if (m_Handle && (m_Handle != INVALID_HANDLE_VALUE))
            {
                CloseDesktop(m_Handle);
            }
        };

    private:
        KDesktop( KDesktop const& ) {}; // not implemented
        void operator=( KDesktop const& ); // not implemented

        HDESK m_Handle;
};

#endif //KHANDLE_H_INCLUDED
