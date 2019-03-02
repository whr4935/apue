#include <iostream>
#include <cstring>
#include <algorithm>

class MyString {
public:
    friend void swap(MyString&, MyString&) noexcept;
    explicit MyString(int len = 0);
    ~MyString();
    MyString(const char* str);
    MyString(const MyString& str);
    MyString(MyString&& str) noexcept;
    MyString& operator=(MyString);

    const char* c_str() const;
    size_t size() const;

private:
    char* mStr = nullptr;
    size_t mSize = 0;
};

MyString::MyString(int len)
    : mSize(len)
{
    if (mSize) {
        mStr = new char[mSize+1];
        memset(mStr, 0, mSize);
    }
}

MyString::~MyString()
{
    if (mStr)
        std::cout << "dtor: " << mStr << std::endl;

    delete[] mStr;
}

MyString::MyString(const char* str)
{
    if (str) {
        mSize = strlen(str);
        if (mSize) {
            mStr = new char[mSize+1];
            strcpy(mStr, str);
        }
    }
}

MyString::MyString(const MyString& s)
{
    if (s.mStr) {
        mStr = new char[s.mSize+1];
        strcpy(mStr, s.mStr);
    }
}

MyString::MyString(MyString&& s) noexcept
    : mStr(s.mStr)
    , mSize(s.mSize)
{
    s.mStr = nullptr;
    s.mSize = 0;
}

//可以正确处理赋值给自己的情况，左右引用都行
MyString& MyString::operator=(MyString s)
{
    swap(*this, s);
    return *this;
}

const char* MyString::c_str() const
{
    return mStr;
}

size_t MyString::size() const
{
    return mSize;
}

void swap(MyString& l, MyString& r) noexcept
{
    using std::swap;

    swap(l.mStr, r.mStr);
    swap(l.mSize, r.mSize);
}

std::ostream& operator<<(std::ostream&out, const MyString& s)
{
    out << s.c_str();
    return out;
}

int test_mystring(int argc, char *argv[])
{
    MyString s1;
    MyString s2("ddddddd");
    MyString s3("abc");

    //s2 = std::move(s3);
    s2 = std::move(s2);
    std::cout << s2.c_str() << std::endl;

    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    std::cout.clear();

    std::cout << "exit" << std::endl;
    return 0;
}
