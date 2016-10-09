#ifndef AUTOPTR_H_INCLUDED
#define AUTOPTR_H_INCLUDED

#pragma once

template <class T>
class KAutoArrayPtr
{
    public:
        KAutoArrayPtr( T* Ptr = NULL) : m_Ptr(Ptr) {};
        ~KAutoArrayPtr() { delete[] m_Ptr; };

        operator T*() { return m_Ptr; };

        void operator=( T* Ptr ) 
            { 
                if (Ptr == m_Ptr) return;
                delete[] m_Ptr;
                m_Ptr = Ptr;
            };
    private:
        KAutoArrayPtr( KAutoArrayPtr const& ); // not implemented
        void operator=( KAutoArrayPtr const& ); // not implemented
        T* m_Ptr;
};
                
#endif //AUTOPTR_H_INCLUDED
