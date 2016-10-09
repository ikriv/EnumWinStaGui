#ifndef WINSTASWITCHER_H_INCLUDED
#define WINSTASWITCHER_H_INCLUDED

#pragma once

class KWinstaSwitcher
{
    public:
        KWinstaSwitcher();
        ~KWinstaSwitcher();

        BOOL SwitchTo( HWINSTA hWinsta );

    private:
        HWINSTA m_hCurrentWinsta;
};

#endif //WINSTASWITCHER_H_INCLUDED
