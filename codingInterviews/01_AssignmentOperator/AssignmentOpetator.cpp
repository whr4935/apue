#include <iostream>
#include <string.h>

class CMyString
{
public:
    CMyString(const char* pData = nullptr);
    CMyString(const CMyString& str);
    ~CMyString() {
        delete [] m_pData;
    }

    void dump() {
        std::cout << m_pData << std::endl;

    }

    CMyString& operator=(const CMyString& r);

private:
    char* m_pData;
};

CMyString::CMyString(const char* pData)
    : m_pData(strdup(pData))
{

}

CMyString& CMyString::operator=(const CMyString& r)
{
    if (&r == this)
        return *this;

    if (r.m_pData != nullptr) {

        if (m_pData) {
            delete [] m_pData;
            m_pData = nullptr;
        }

        m_pData = new char[strlen(r.m_pData+1)];
        strcpy(m_pData, r.m_pData);
    }

    return *this;
}

int main(int argc, char **argv)
{

    CMyString c1("hello c1");
    CMyString c2("hello c2");

    c1 = c2;

    c1.dump();

    return 0;
}
