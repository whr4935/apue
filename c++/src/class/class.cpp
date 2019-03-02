#include <c_plus_plus_primer.h>
#include "class.h"
#include "StrVec.h"

//////////////////////////////////////////////////////////////////////////
//函数声明
static int test_sales_data();
static int test_copy_control();
extern int testMemoryModel();
extern int test_mystring(int argc, char**);
extern int testDelegateCtor();
extern int test_using();

//////////////////////////////////////////////////////////////////////////
int TEST_class(int argc, char **argv)
{
    std::cout << "======================"<< std::endl;
    std::cout << __FUNCTION__ << "\n" << std::endl;
    int ret;

    //ret = test_sales_data();
    //ret = test_copy_control();
    //ret = testMemoryModel();
    test_mystring(argc, argv);
    testDelegateCtor();
    test_using();

    std::cout << "======================\n"<< std::endl;
    return ret;
}

class Sales_data {
    //friend 仅仅指定访问权限，并非原型声明
    friend Sales_data add(const Sales_data &lhs, const Sales_data &rhs);
    friend std::ostream& print(std::ostream &os, const Sales_data &item);
    friend std::istream& read(std::istream &is, Sales_data &item);

public:
    Sales_data()  = default ;		//其他成员由in-class initializer初始化
    Sales_data(const std::string &s): bookNo(s) {}
    Sales_data(const std::string &s, unsigned n, double p) :
        bookNo(s), unints_sold(n), revenue(p*n) {}
    Sales_data(std::istream &);

private:
    std::string isbn() const { return bookNo; }
    Sales_data& combine(const Sales_data&);
    double avg_price() const;

    std::string bookNo;
    unsigned unints_sold = 0;
    double revenue = 0.0;
};

//应该添加全局函数的声明
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream& print(std::ostream&, const Sales_data&);
std::istream& read(std::istream&, Sales_data&);

Sales_data::Sales_data(std::istream &in)
{
    read(in, *this);
}

Sales_data& Sales_data::combine(const Sales_data& rhs)
{
    unints_sold += rhs.unints_sold;
    revenue += rhs.revenue;

    return *this;
}

double Sales_data::avg_price() const
{
    if (unints_sold)
        return revenue / unints_sold;
    else
        return 0;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
    Sales_data sum = lhs;
    sum.combine(rhs);
    return sum;
}

std::ostream& print(std::ostream &os, const Sales_data &item)
{
    os << item.isbn() << " " << item.unints_sold << " "
       << item.revenue << " " << item.avg_price();

    return os;
}

std::istream& read(std::istream &is, Sales_data &item)
{
    double price = 0;
    is >> item.bookNo >> item.unints_sold >> price;
    item.revenue = price * item.unints_sold;

    return is;
}

////////////////////////////
class Screen;

class Window_mgr {
public:
    //using ScreenIndex = std::vector<Screen>::size_type;
    typedef std::vector<Screen>::size_type ScreenIndex;
    void clear(ScreenIndex);
    ScreenIndex addScreen(const Screen&);
private:
    std::vector<Screen> screens/*{ Screen(24, 80, ' ') }*/;
};

class Screen {
    //friend class Window_mgr;
    friend void Window_mgr::clear(ScreenIndex);
public:
    //using pos = std::string::size_type;
    typedef std::string::size_type pos;

    Screen() = default;
    Screen(pos ht, pos wd, char c) : height(ht), width(wd), contents(ht*wd, ' ') {}
    char get() const { return contents[cursor]; }
    char get(pos ht, pos wd) const;
    Screen& set(char);
    Screen& set(pos, pos, char);
    Screen& move(pos r, pos c);
    Screen& display(std::ostream &os) { do_display(os); return *this; }
    const Screen& display(std::ostream &os) const { do_display(os); return *this; }
    void some_member() const;

private:
    void do_display(std::ostream &os) const { os << contents; }

private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
    mutable size_t access_ctr;
    std::vector<int> points = {1, 2, 3};
};

char Screen::get(pos r, pos c) const
{
    pos row = r * width;

    return contents[row + c];
}

//inline 成员函数肯定定义在头文件中，建议只在类外成员函数定义前加上inline
inline Screen& Screen::move(pos r, pos c)
{
    pos row = r * width;
    cursor = row + c;
    return *this;
}

void Screen::some_member() const
{
    ++access_ctr;
}

Screen& Screen::set(char c)
{
    contents[cursor] = c;
    return *this;
}

Screen & Screen::set(pos r, pos col, char ch)
{
    // TODO: insert return statement here
    contents[r*width + col] = ch;

    return *this;
}

//返回值在函数名作用域限定符之前，编译器并不知道ScreenIndex是定义在Window_mgr类中
Window_mgr::ScreenIndex Window_mgr::addScreen(const Screen &s)
{
    screens.push_back(s);
    return screens.size() - 1;
}

/////////////////////////////
void Window_mgr::clear(ScreenIndex i)
{
    Screen &s = screens[i];

    s.contents = string(s.height * s.width, ' ');
}

////////////////////////

//虽然定义在类内部，但仍然是全局函数，如果没有此定义，X构造函数会编译失败
//friend接后面的函数并不是原型声明
void f();

struct X {
    friend void f() {

    }

    X() { f(); }
    void g();
    void h();
};

void X::g()
{
    return f();
}

void X::h()
{
    return f();
}

//////////////////////
typedef double Money;
string bal;
class Account {
public:
    Money balance() { return bal; }

private:
    /* typedef double Money; */
    Money bal;
};

///////////////////////
class X2 {
public:
    X2() = default;		//系统合成的构造函数：第一步使用类内初始化；第二步值初始化
    X2(int val): j(val), i(j) {}	//初始化参数列表与类内成员定义顺序相同
    void print() { std::cout << "i = " << i << ", j = " << j << endl; }

private:
    int i = 3;
    int j = 10;
};

class Delegate_constructor {
public:
    Delegate_constructor(int i, int j) : m_i(i), m_j(j) {}
    Delegate_constructor() : Delegate_constructor(0, 0) {}
    Delegate_constructor(int i) : Delegate_constructor(i, 0) {}

private:
    int m_i;
    int m_j;
};

//litera class
class Debug {
public:
    constexpr Debug(bool b = true) : hw(b), io(b), other(b) {}
    constexpr Debug(bool h, bool i, bool o) : hw(h), io(i), other(o) {}
    constexpr bool any() const { return hw || io || other; }
    void set_io(bool b) { io = b; }
    void set_hw(bool b) { hw = b; }
    void set_other(bool b) { other = b; }
private:
    bool hw;
    bool io;
    bool other;
};

class Account2 {
public:
    typedef double RATE;
    void calculate() { amount += amount * interestRate; }
    static double rate() { return interestRate; }
    static void rate(RATE);
private:
    Account2() = default;
    std::string owner;
    double amount;
    static RATE interestRate;
    static RATE initRate();
    static const char period = 30;		//static const可以类内初始化，但是只能是integer类型
    static const char period2;
    static constexpr int period3 = 60;	//staitc constexpr必须类内初始化
    static Account2 ac;					//静态成员可以是当前类类型,作何用？
};

void Account2::rate(RATE r)
{
    interestRate = r;
}

Account2::RATE Account2::initRate()
{
    interestRate = period;

    return interestRate;
}

Account2::RATE Account2::interestRate = initRate();
const char Account2::period;		//在类内初始化了就不要再给初始值
const char Account2::period2 = 30;
constexpr int Account2::period3;

static int test_sales_data()
{
//  Sales_data data(cin);
//  print(cout, data);

    X2 x2(5);
    x2.print();

    X2 x2_default;
    x2_default.print();

    constexpr Debug d;
    constexpr int c_i = d.any();

    return 0;
}

class CopyItem
{
public:
    CopyItem() = default;
    CopyItem(CopyItem& item) {
        cout << "copy constructor" << endl; }
    CopyItem(CopyItem item, int i) {}
    explicit CopyItem(int i) { cout << "i param" << endl; }
};

class NoCopy
{
public:
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;	// =delete可用于任何函数
    NoCopy& operator=(const NoCopy&) = delete;
    ~NoCopy() = default;
};

class PrivateCopy
{
    PrivateCopy(const PrivateCopy&);
    PrivateCopy& operator=(const PrivateCopy&);

public:
    PrivateCopy() = default;

    void mem_func(PrivateCopy* py) { PrivateCopy p; p = *py; }
};

int noCallable(int) = delete;

/* value-like copy assignment operator */
class HasPtrValue
{
public:
    HasPtrValue(const string& s = string()) : ps(new string(s)), i(0) {}
    HasPtrValue(const HasPtrValue &p) : ps(new string(*p.ps)), i(p.i) {}
    HasPtrValue& operator=(const HasPtrValue&);
    ~HasPtrValue() { delete ps; };
private:
    string *ps;
    int i;
};

HasPtrValue& HasPtrValue::operator=(const HasPtrValue& p)
{
    string *temp_p = new string(*p.ps);	//必须先拷贝，避免赋值给自身时出错
    if (ps) {
        delete ps;
    }
    ps = temp_p;
    i = p.i;

    return *this;
}

/* point-like copy assignment operator */
class HasPtr
{
    friend void swap(HasPtr&, HasPtr&);
public:
    HasPtr(const string& s = string()) : ps(new string(s)), i(0), use(new std::size_t(1)) {}
    HasPtr(const HasPtr &p) : ps(p.ps), i(p.i), use(p.use) { ++*use; }
//	HasPtr& operator=(const HasPtr&);
    HasPtr& operator=(HasPtr);
    ~HasPtr();
    int refCount() { return *use; }
private:
    string *ps;
    int i;
    std::size_t *use;
};

#if 0
HasPtr& HasPtr::operator=(const HasPtr& p)
{
    ++*p.use;		//必须先加1，避免赋值给自身时出错
    if (--*use == 0) {
        delete ps;
        delete use;
    }

    ps = p.ps;
    i = p.i;
    use = p.use;

    return *this;
}
#endif

//////////////////////////////////////////////////
/* Assignment operators that use copy and swap are automatically exception
 * safe and correctly handle self-assignment
 */
//////////////////////////////////////////////////
HasPtr& HasPtr::operator=(HasPtr p)
{
    swap(*this, p);		//so cool
    return *this;
}

HasPtr::~HasPtr()
{
    if (--*use == 0) {
        cout << "dtor: " << *ps << endl;

        delete ps;
        delete use;
    }
}

void swap(HasPtr& lhs, HasPtr& rhs)
{
    using std::swap;

    swap(lhs.ps, rhs.ps);
    swap(lhs.i, rhs.i);
    swap(lhs.use, rhs.use);
}

//////////////////////////////////////////////////
//Message Folder example
//////////////////////////////////////////////////
class Message;

class Folder {
public:
    Folder() = default;
    Folder(const Folder&) = delete;
    Folder& operator=(const Folder&) = delete;

    void addMsg(Message*);
    void remMsg(Message*);
    void print();

private:
    std::set<Message*> messages;
};

void Folder::addMsg(Message* pmsg)
{
    messages.insert(pmsg);
}

void Folder::remMsg(Message* pmsg)
{
    messages.erase(pmsg);
}

void Folder::print()
{
    std::cout << messages.size() << std::endl;

    /* for (auto m : messages) { */

    //   }
}

class Message {
    friend class Folder;
public:
    explicit Message(const std::string &str = "") : contents(str) { }
    Message(const Message&);
    /* Message& operator=(const Message&); */
    Message& operator=(Message);
    ~Message();

    void save(Folder&);
    void remove(Folder&);
    void print();

private:
    void add_to_Folders(const Message&);
    void remove_from_Folders();
    void swap(Message&, Message&);

    std::string contents;
    std::set<Folder*> folders;
};

Message::Message(const Message &m) : contents(m.contents), folders(m.folders)
{
    add_to_Folders(m);
}

/* Message& Message::operator=(const Message& rhs) */
/* { */
/*     remove_from_Folders(); */
/*     contents = rhs.contents; */
/*     folders = rhs.folders; */
/*     add_to_Folders(rhs); */

/*     return *this; */
/* } */

Message& Message::operator=(Message rhs)
{
    swap(*this, rhs);
    return *this;
}

Message::~Message()
{
    remove_from_Folders();
}

void Message::save(Folder&f)
{
    folders.insert(&f);
    f.addMsg(this);
}

void Message::remove(Folder&f)
{
    folders.erase(&f);
    f.remMsg(this);
}

void Message::add_to_Folders(const Message &m)
{
    for (auto f : m.folders)
        f->addMsg(this);
}

void Message::remove_from_Folders()
{
    for (auto f : folders)
        f->remMsg(this);
}

void Message::swap(Message& lhs, Message& rhs)
{
    using std::swap;

    for (auto f : lhs.folders)
        f->remMsg(&lhs);
    for (auto f : rhs.folders)
        f->remMsg(&rhs);

    swap(lhs.contents, rhs.contents);
    swap(lhs.folders, rhs.folders);

    for (auto f : lhs.folders)
        f->addMsg(&lhs);
    for (auto f : rhs.folders)
        f->addMsg(&rhs);
}

static int TestMessageFolders()
{
    Message msg;
    Message msg_copy = msg;
    Folder folder;

    msg.save(folder);

    msg_copy = msg;

    folder.print();

    return 0;
}



//////////////////////////////////////////////////
class CTest {
public:
    CTest(int n) {
        m_n = n;
        pi = new int(7);
        cout << "contruct CTest...." << endl;
    }

    CTest(const CTest& rhs) : m_n(rhs.m_n), pi(new int(*rhs.pi)) {
        cout << "Copy construct CTest..." << endl;
    }

    ~CTest() {
        m_n = 111;
        delete pi;
        pi = nullptr;
        cout << "destruct CTest...." << endl;
    }

    int m_n;
    int *pi;
};

CTest Fun()
{
    CTest t(888);
    return (t);
}


static void test_strVec()
{
    StrVec v;
    v.push_back("wang");
    v.push_back("hou");
    v.push_back("ren");
#if __cplusplus > 201103L
    for_each(v.begin(), v.end(), [](auto &it) {cout << it << " "; }), cout << endl;
#endif
    StrVec v2 = std::move(v);
    #if __cplusplus > 201103L
    for_each(v2.begin(), v2.end(), [](auto &it) {cout << it << " "; }), cout << endl;
#endif
    {
        v2 = std::move(v2);
    }

    v = v2;
    StrVec v3 = v;
//	v.operator=(v2);
//	StrVec().operator=(v2);
}

static void test_operator_overload()
{
    std::cout << "test operatr overload" << std::endl;
}



static int test_copy_control()
{
    CopyItem item1;
    CopyItem item2 = item1;
    item1 = item2;
    CopyItem item3(item1);
    CopyItem item4(item3, 0);
//	CopyItem item5 = CopyItem(11);
    CopyItem item6(120);

//	noCallable(1);

    PrivateCopy py;
    //py.mem_func(&py);

    HasPtrValue hv("hello");
    HasPtrValue hv2(hv);
    hv2 = hv2;

    {
        HasPtr h("world");
        HasPtr h2(h);
        HasPtr h3("hello");
        h2 = h;
        cout << h.refCount() << endl;
    }

    //test Message Folder
    TestMessageFolders();

    {
        CTest&& s = Fun();	//s绑定到了按值返回的临时对象上面		vs引用可以绑定到Fun()返回值上，这是不合符标准的！
        int n = *s.pi;
        cout << "n = " << n << endl;
    }

    int && ri2 = 3 + 3;
    decltype(std::move(ri2++)) ri3 = 8;	//decltype只是取得类型，不会去执行语句，像sizeof一样
    cout << "ri2 = " << ri2 << endl;

    cout << endl;
    // test_strVec();

    string s1 = "a value", s2 = "another";
    s1 + s2 = "wow";

    /*
      For non - static member functions declared without a ref - qualifier, an additional rule applies :
      even if the implicit object parameter is not const - qualified, an rvalue can be bound to the parameter
      as long as in all other respects the argument can be converted to the type of the implicit object parameter.
    */
    (s1 + s2).empty();

    //reference qualifier 修饰*this指向的对象，左引用限定该成员函数只能被左值对象调用，相当于T& *const this
    //参数相同的重载成员函数必须要么都加上reference qualifier，要么都不加

    return 0;
}
