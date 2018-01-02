/*
 * Minimal replacement for class CComPtr and CComBSTR
 * Based on common public IUnknown interface only
 */

template <class T>
class _NoAddRefReleaseOnCComPtr : public T
{
private:
    STDMETHOD_(ULONG, AddRef)()=0;
    STDMETHOD_(ULONG, Release)()=0;
};

template <class T>
class CComPtrBase
{
protected:
    CComPtrBase() {
        p = NULL;
    }
    CComPtrBase(T* t) {
        p = t;
        if (p) p->AddRef();
    }
    void Swap(CComPtrBase& other)
    {
        T* pTemp = p;
        p = other.p;
        other.p = pTemp;
    }
public:
    typedef T _PtrClass;
    ~CComPtrBase()
    {
        if (p)
            p->Release();
    }
    operator T*() const
    {
        return p;
    }
    operator T**() const
    {
        return &p;
    }
    T& operator*() const
    {
        return *p;
    }
    operator T*()
    {
        return p;
    }
    operator T**()
    {
        return &p;
    }
    _NoAddRefReleaseOnCComPtr<T>* operator->() const
    {
        return (_NoAddRefReleaseOnCComPtr<T>*)p;
    }

    // Boolean test and operator
    bool operator!() const throw()
    {
        return (p == NULL);
    }
    bool operator<(T* pT) const throw()
    {
        return p < pT;
    }
    bool operator!=(T* pT) const
    {
        return !operator==(pT);
    }
    bool operator==(T* pT) const throw()
    {
        return p == pT;
    }

    // Release the interface and set to NULL
    void Release()
    {
        T* pTemp = p;
        if (pTemp)
        {
            p = NULL;
            pTemp->Release();
        }
    }
    // Compare two objects for equivalence
    inline bool IsEqualObject(IUnknown* pOther);

    // Attach to an existing interface (does not AddRef)
    void Attach(T* p2)
    {
        if (p)
        {
            ULONG ref = p->Release();
            (ref);
            // Attaching to the same object only works if duplicate references are being coalesced.  Otherwise
            // re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
        }
        p = p2;
    }
    // Detach the interface (does not Release)
    T* Detach()
    {
        T* pt = p;
        p = NULL;
        return pt;
    }
    HRESULT CoCreateInstance(
        REFCLSID rclsid,
        LPUNKNOWN pUnkOuter = NULL,
        DWORD dwClsContext = CLSCTX_ALL)
    {
        return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
    }
    template <class Q>
    HRESULT QueryInterface(Q** pp) const
    {
        return p->QueryInterface(__uuidof(Q), (void**)pp);
    }

public:
    T* p;
};

template <class T>
class CComPtr : public CComPtrBase<T>
{
public:
    CComPtr()
    {
    }

    CComPtr(T* lp) : CComPtrBase<T>(lp)
    {
    }

    CComPtr(const CComPtr<T>& lp) : CComPtrBase<T>(lp.p)
    {
    }

    T* operator=(T* lp)
    {
        if(*this!=lp)
        {
            CComPtr(lp).Swap(*this);
        }
        return *this;
    }
    
    T* operator=(const CComPtr<T>& lp)
    {
        if(*this!=lp)
        {
            CComPtr(lp).Swap(*this);
        }
        return *this;
    }

    T* operator=(CComPtr<T>& lp)
    {
        if(*this!=lp)
        {
            CComPtr(lp).Swap(*this);
        }
        return *this;
    }

    CComPtr(CComPtr<T>&& lp) : CComPtrBase<T>()
    {
        lp.Swap(*this);
    }

    T* operator=(CComPtr<T>&& lp)
    {
        if (*this != lp)
        {
            CComPtr(static_cast<CComPtr&&>(lp)).Swap(*this);
        }
        return *this;
    }

    operator T*()
    {
        return this->p;
    }
    T** operator &()
    {
        return &this->p;
    }
};

template <class T>
inline bool CComPtrBase<T>::IsEqualObject(IUnknown* pOther)
{
    if (p == NULL && pOther == NULL)
        return true;	// They are both NULL objects

    if (p == NULL || pOther == NULL)
        return false;	// One is NULL the other is not

    CComPtr<IUnknown> punk1;
    CComPtr<IUnknown> punk2;
    p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
    pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
    return punk1 == punk2;
}


class CComBSTR
{
public:
    BSTR m_str;

    CComBSTR()
    {
        m_str = NULL;
    }

    CComBSTR(LPCOLESTR pSrc)
    {
        if (pSrc == NULL)
        {
            m_str = NULL;
        }
        else
        {
            m_str = ::SysAllocString(pSrc);
        }
    }

    CComBSTR(const CComBSTR& src)
    {
        m_str = src.Copy();
    }

    CComBSTR(_In_ REFGUID guid)
    {
        OLECHAR szGUID[64];
        int result = ::StringFromGUID2(guid, szGUID, 64);
        if (result)
            m_str = ::SysAllocString(szGUID);
    }

    CComBSTR(CComBSTR&& src)
    {
        m_str = src.m_str;
        src.m_str = NULL;
    }

    ~CComBSTR()
    {
        ::SysFreeString(m_str);
    }

    BSTR* operator&()
    {
        return &m_str;
    }

    unsigned int Length() const
    {
        return ::SysStringLen(m_str);
    }

    unsigned int ByteLength() const
    {
        return ::SysStringByteLen(m_str);
    }

    operator BSTR() const
    {
        return m_str;
    }

    // Copy
    BSTR Copy() const
    {
        if (!*this)
        {
            return NULL;
        }
        else if (m_str != NULL)
        {
            return ::SysAllocStringByteLen((char*)m_str, ::SysStringByteLen(m_str));
        }
        else
        {
            return ::SysAllocStringByteLen(NULL, 0);
        }
    }
    
    HRESULT CopyTo(BSTR* pbstr) const
    {
        if (pbstr == NULL)
        {
            return E_POINTER;
        }
        *pbstr = Copy();

        if ((*pbstr == NULL) && (m_str != NULL))
        {
            return E_OUTOFMEMORY;
        }
        return S_OK;
    }

    // copy BSTR to VARIANT
    HRESULT CopyTo(_Out_ VARIANT *pvarDest) const
    {
        HRESULT hRes = E_POINTER;
        if (pvarDest != NULL)
        {
            pvarDest->vt = VT_BSTR;
            pvarDest->bstrVal = Copy();

            if (pvarDest->bstrVal == NULL && m_str != NULL)
            {
                hRes = E_OUTOFMEMORY;
            }
            else
            {
                hRes = S_OK;
            }
        }
        return hRes;
    }
};