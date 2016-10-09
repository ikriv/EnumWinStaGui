#include "stdafx.h"
#include "WinstaSwitcher.h"

KWinstaSwitcher::KWinstaSwitcher()
{
    m_hCurrentWinsta = GetProcessWindowStation();
};

KWinstaSwitcher::~KWinstaSwitcher()
{
    SetProcessWindowStation(m_hCurrentWinsta);
};

BOOL KWinstaSwitcher::SwitchTo( HWINSTA hWinsta )
{
    return SetProcessWindowStation(hWinsta);
};
