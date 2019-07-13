#include <c_plus_plus_primer.h>
#include "std_lib.h"
#include "TextQuery.h"

static int test_string_op();
static int test_vector_op();
static int test_io_op();
static int test_file_io();
static int test_string_io();
static int sequential_container_op();
static int test_sequence_container_adapter();
static int test_generic_algorithm();
static void elimDups(vector<string> &vs);
static bool isShorter(const string &s1, const string &s2);
static void print(vector<string> &vs);
static vector<string>::const_iterator biggies(vector<string> &vs, string::size_type sz);
static void test_bind();
static int test_associative_container();
static int test_dynamic_memory(int argc, char **argv);
static void test_manipulator();
static void test_shared_ptr();
static void test_std_remove();
static void test_std_move();
static void test_multi_derived();
extern int test_caller();
extern int test_thread();
extern int test_atomic();
extern int test_move();
extern void testDeferredActions();

//////////////////////////////////////////////////////////////////////////
int TEST_stdlib(int argc, char **argv)
{
    std::cout << "======================"<< std::endl;
    std::cout << __FUNCTION__ << "\n" << std::endl;

    int ret;

     //ret = test_string_op();
    //ret = test_vector_op();
    /* ret = test_io_op(); */
    // ret = test_file_io();
    // ret = test_string_io();
    //ret = sequential_container_op();
    //ret = test_sequence_container_adapter();
    //ret = test_generic_algorithm();
    //ret = test_associative_container();
    /* ret = test_dynamic_memory(argc, argv); */
    // test_manipulator();

    // test_shared_ptr();
    // test_std_remove();
    // test_std_move();
    //test_multi_derived();

    test_caller();
    //test_thread();
    //test_atomic();
    //test_move();
    //testDeferredActions();

    std::cout << "======================\n"<< std::endl;
    return ret;
}

template <typename T>
std::string to_string(const T& n)
{
    std::ostringstream stm;
    stm << n;
    return stm.str();
}

static int test_string_op()
{
    std::string line;

    std::cout << "to_string(123): " << to_string(123) << std::endl;

//  while (getline(std::cin, line)) {
//      if (!line.empty())
//          std::cout << line << std::endl;
//      else
//          cout << "empty input" << endl;
//  }

//  getline(cin, line);
// //	auto size = line.size();
//  decltype(line.size()) size = line.size();
//  cout << "size is " << size << endl;
//
//  decltype(string().size()) punct_cnt = 0;
//  for (auto &c : line) {
//      if (ispunct(c))
//          ++punct_cnt;
//      c = toupper(c);
//  }
//  cout << punct_cnt << " punctuation characters in " << line << endl;


//  const string hexdigits = "0123456789ABCDEF";
//  string results;
//  decltype(hexdigits.size()) n;
//  while (cin >> n) {
//  p    if (n < hexdigits.size())
//          results += hexdigits[n];
//  }
//  cout << results << endl;

    const char* cp = "Hello world";
    char noNull[] = {'H', 'i'};
    string s1(cp);
    string s2(noNull, 2);	//第一个参数为char*类型，则第二个参数代表长度
    cout << s2 << endl;
    string s4(cp + 6, 5);
    cout << s4 << endl;
    string s5(s1, 6, 5);
    cout << s5 << endl;
    string s6(s1, 6);		//第一个参数是string类型，则第二个参数代表起始位置
    cout << s6 << endl;

    string ss1 = s1.substr(0, 5);
    cout << ss1 << endl;
    string ss2 = s1.substr(6);
    cout << ss2 << endl;

    s1.insert(5, "wo");
    s1.erase(s1.size() - 7);
    auto s1_it = s1.insert(0, "houren");
    cout << s1_it << endl;
    s1.insert(0, s2, 0, s2.size());
    s1.replace(0, 2, "whr");	//把指定范围的字符串替换为新的字符串
    cout << s1 << endl;

    string numbers("0123456789");
    string str_to_find("fdfs3r32fsfs324e32rewf23rrrrrr44");
    string::size_type pos = 0;
    while ((pos = str_to_find.find_first_of(numbers, pos)) != string::npos) {
        cout << "found number at index: " << pos << ", element is " << str_to_find[pos] << endl;
        ++pos;
    }

    auto convert_num = std::stol(numbers);
    convert_num = std::stol(std::to_string(convert_num));

    return 0;
}

static int test_vector_op()
{
    vector <vector<int>> vvec1, vvec2;

    vvec2 = vvec1;

    vector <string> articles{1, "ab"};	//10不能用来初始化string，所以当做成员个数，结果是10个"ab"
    string s;
//  while (cin >> s) {
//      articles.push_back(s);
//  }

    for(auto s : articles)
        std::cout << s << endl;

    auto it_b = articles.begin();
    auto it_e = articles.end();

    string s2("some string");
    for (auto it = s2.begin(); it != s2.end() && !isspace(*it); ++it) {
        *it = toupper(*it);
    }
    cout << s2 << endl;

    const char a4[6]{"abcde"};
    cout <<a4 << endl;

    int ia[3] = {1, 2, 3};
    decltype(ia) ia2{ 4, 5 };
    ia2[0] = 6;
    auto pia  = ia;
    int *beg = std::begin(ia);		//begin end 函数
    int *e2 = std::end(ia);

    auto diff = e2 - beg;

    vector<int> via(std::begin(ia), std::end(ia));
    vector<int> via2(ia, ia+sizeof(ia)/sizeof(*ia));
    vector<int> via3 = {std::begin(ia), std::end(ia)};
    for(auto i : via3)
        std::cout << i << endl;

    int m_array[3][4];
    int m_cnt = 0;
    for (auto &row : m_array) {
        for (auto &col : row) {
            col = m_cnt;
            m_cnt++;
        }
    }

    for (auto &row : m_array) {
        for (auto &col : row) {
            cout << col << ' ';
        }
        cout << endl;
    }

    decltype(&m_cnt) p = 0;		//&取地址符返回右值

    return 0;
}

static void iterate_array(const std::array<int, 10> &a, string s)
{
    cout << s;
    for (auto &c : a)
        cout << c << " ";
    cout << endl;
}

static void iterate_vector_string(const std::vector<std::string> &v, string s = string())
{
    cout << s;
    for (auto &s : v)
        cout << s << " ";
    cout << endl;
}

static int test_io_op()
{
    /* badbit: stream is corrupted
     * failbit: IO operation failed
     * eofbit: a stream hit end-of-file
     * goodbit: not in error state, the value is guranteed to be zero.
     */

    bool status;
    auto old_state = cin.rdstate();
    cin.clear();

    status = cin.good();
    status = cin.eof();
    status = cin.fail();
    status = cin.bad();
    cin.clear();

    /* cin.setstate(); */
    auto rd_state = cin.rdstate();
    std::cout << "rd_state = " << rd_state << std::endl;

    auto bit = cin.failbit;

    cin.clear(cin.eofbit);
    cin.clear(cin.badbit);
    status = cin.eof();

    cin.clear(cin.badbit);
    status = cin.bad();		//badbit置位时返回true
    status = cin.fail();	//failbit或badbit置位时返回true

    cin.sync_with_stdio(false);
    cin.sync();
    int v;
    cin >> v;		//input EOF
    status = cin.fail();
    status = cin.eof();
    old_state = cin.rdstate();
    status = (old_state & cin.eofbit) && (old_state & cin.failbit) && !(old_state & cin.badbit);

    cout << "hello1" << endl;
    cout << "hello2" << std::flush;
    cout << "hello3" << std::ends;
    cout << "hello4" << std::endl;

    cout << "wang";
    //cin >> v;

    cin.tie(&cout);	//把cin绑定到cout
    std::ostream *old_tie = cin.tie(nullptr);
    cin.clear();
    while (cin >> v)
        cout << v << std::flush;
    cout << endl;
    if (cin.eof())
        cout << "EOF" << endl;
    if (cin.fail())
        cout << "FAIL" << endl;
    if (cin.bad())
        cout << "BAD" << endl;
    cin.tie(&cerr);
    cin.tie(old_tie);

    return 0;
}

static int test_file_io()
{
    string filename("test.txt");
    bool open_status;

    /* std::fstream f; */
    /* std::iostream io; */


    std::ofstream out_file;
    out_file.open(filename, std::ofstream::out | std::ofstream::in);
    /* out_file.open(filename, std::ofstream::out | std::ofstream::app); */
    open_status = out_file.is_open();
    if (!open_status)
        cout << "open failed!" << endl;
    if (out_file) {
        out_file << "hello world" << std::endl;
    }
    out_file.close();

//  out_file.open(filename, std::ofstream::in);
//  status = out_file.is_open();

    string s_buf;
    std::ifstream in_file;
    in_file.open(filename, std::ifstream::in);
#if 0
    while (in_file) {
        if (getline(in_file, s_buf))
            cout << s_buf << endl;
    }
#else
    while (getline(in_file, s_buf))
        cout << s_buf << endl;
#endif
    out_file.close();

    return 0;
}

static int test_string_io()
{
    std::stringstream ss;
    ss << "hello world!";

    string s;
    while (ss >>s) {
        cout << "s = " << s << std::endl;
    }

    cout << ss.str() << std::endl;
    return 0;
}



/* sequential container type:
 * vector
 * deque
 * list
 * forward_list
 * array
 * string*/

/* sequential container adapter:
 * stack
 * queue
 * priority_queue
 */
static int sequential_container_op()
{
    std::cout << "test list::splice" << std::endl;
    std::list<int> la;
    la.push_back(1);
    la.push_back(2);
    la.push_back(3);
    la.push_back(4);

    std::list<int> lb;
    lb.push_back(34);
    lb.splice(lb.begin(), la, ++la.begin(), la.end());
    std::for_each(la.begin(), la.end(), [](int d){std::cout << d << " ";});
    std::cout << std::endl;

    std::for_each(lb.begin(), lb.end(), [](int d){std::cout << d << " ";});
    std::cout << std::endl;

    return 0;

    std::list<string> authors = {"Miltion", "Shakespeare", "Austen"};
    std::vector<const char*> articles = {"a", "an", "the"};

    std::list<string> list2(authors);	//拷贝构造时，类型必须完全一致
    //std::vector<string> articles2 = articles;
    std::forward_list<std::string> words(articles.cbegin(), articles.cend());	//此时类型可以不一致

    std::array<int, 10> a1 = { 1, 2, 3 };
    std::array<int, 10> a2 = a1;
    a2 = { 4, 5, 6 };
    iterate_array(a1, "a1: ");
    iterate_array(a2, "a2: ");
    cout << endl;

    //std::swap(a1, a2);
    a1.swap(a2);
    iterate_array(a1, "a1: ");
    iterate_array(a2, "a2: ");
    cout << endl;

    words.assign(articles.cbegin(), articles.cend());
    words.assign(initializer_list<string>{"wang", "hou", "ren"});
    words.assign(3, "wo");
//  for (auto &s : words) {
//      cout << s << " ";
//  }
//  cout << endl;

    string str;
    str.insert(str.begin(), 'b');
    cout << str << endl;

    std::vector<std::string> svec;
    svec.insert(svec.end(), 2, "ha");
    svec.insert(svec.begin(), { "Aa", "Bb" });
    auto end_it = svec.end();

    std::vector<std::string> svec2{"hello", "world", "2016"};
#if _cplusplus > 201103L
    auto ret = svec.insert(end_it-1, svec2.begin()+1, svec2.end());
#endif
//  if (ret == end_it)
//      cout << "insert failed" << endl;
    svec.emplace_back("12345");
    svec.emplace(svec.begin() + 1, "67890");
    iterate_vector_string(svec);

    string s1 = svec.back();
    string s2 = *--svec.end();
    if (s1 == s2)
        cout << "equal string: " << s1 << endl;

    string s3 = svec.at(0);
    string s4 = svec.front();
    if (s3 == s4)
        cout << "equal front string: " << s3 << endl;

    auto it = svec.erase(svec.end()-1);
    if (it != svec.end())
        cout << *it << endl;
    else
        cout << "off-the-end iterator" << endl;

    std::forward_list<int> f_list;
    auto f_it = f_list.before_begin();
    for (int i = 0; i < 10; ++i) {
        f_it = f_list.insert_after(f_it, i);
        //	f_list.push_front(i);
    }

    for (auto f_curit = f_list.begin(), f_it = f_list.before_begin(); f_curit != f_list.end();) {
        if (*f_curit % 2)
            f_curit = f_list.erase_after(f_it);
        else {
            f_it = f_curit;
            ++f_curit;
        }
    }

    for (auto &i : f_list) {
        cout << i << " ";
    }
    cout << endl;

    vector<int> v1;
    auto cap = v1.capacity();
    v1.insert(v1.begin(), 0);
    cap = v1.capacity();
    v1.reserve(10);
    cap = v1.capacity();
    v1.shrink_to_fit();
    cap = v1.capacity();

    vector<int> v2{10, 20, 30, 40, 50, 60};
#if _cplusplus > 201103L
    auto v1_it = v1.insert(v1.begin(), v2.begin(), v2.begin() + (v2.end() - v2.begin()) / 2);	//返回插入的第一个元素迭代器
    cout << *v1_it << endl;		//10
#endif
    std::forward_list<int> flist2;
    auto fl2_it = flist2.insert_after(flist2.before_begin(), v2.begin(), v2.begin() + (v2.end() - v2.begin()) / 2);	//返回插入的最后一个元素迭代器
    cout << *fl2_it << endl;	//30

    int fl2_v = *flist2.begin();

    //移动flist.begin()之后的那个元素移动到了flist3，为什么移动的不是flist2.begin()本身？因为forward_list不能方便的操作迭代器之前的那个元素
    std::forward_list<int> flist3{77, 78, 79};
    flist3.splice_after(flist3.before_begin(), flist2, flist2.begin());
    return 0;
}

class Item {
    friend std::ostream& operator<<(std::ostream &, Item&);
    friend bool operator<(const Item&, const Item&);
public:
    Item() = default;
    Item(int x, int y) : m_x(x), m_y(y) {}
    bool operator<(const Item& rhs);

private:
    int m_x = 0;
    int m_y = 0;
};

std::ostream& operator<<(std::ostream &os, Item& item)
{
    os << "(" << item.m_x << ", " << item.m_y << ")";
    return os;
}

bool Item::operator<(const Item& rhs)
{
    return m_x < rhs.m_x;
}

bool operator<(const Item& lhs, const Item& rhs)	// "<"决定优先级顺序
{
    return lhs.m_x < rhs.m_x;
}

static int test_sequence_container_adapter()
{
    std::stack<int> stk;
    std::queue<int> que;
    std::priority_queue<int> prique;
    std::stack<int, vector<int>> stk_vec;

    for (size_t ix = 0; ix != 10; ++ix)
        stk.push(ix);

    while (!stk.empty()) {
        int value = stk.top();
        stk_vec.push(value);
        stk.pop();
    }

    std::queue<Item> que_item;
    for (size_t ix = 0; ix != 10; ++ix) {
        que_item.emplace(ix, ix);
    }
    Item itm = que_item.front();
    cout << itm << endl;
    itm = que_item.back();
    cout << itm << endl;
    que_item.pop();
    itm = que_item.front();
    cout << itm << endl;

    std::priority_queue<Item> prique_item;
    for (size_t ix = 0; ix != 10; ++ix) {
        prique_item.emplace(ix, ix);
    }
    itm = prique_item.top();
    cout << itm << endl;

    prique_item.emplace(10, 10);
    itm = prique_item.top();
    cout << itm << endl;

    return 0;
}

/* generic algorithms
 * algorithm never execute container operations
 */
static int test_generic_algorithm()
{
    std::string s("hello world");

    //find
    auto ret = std::find(s.cbegin(), s.cend(), 'k');
    if (ret != s.cend())
        cout << *ret << endl;

    int a[] = { 13, 2, 72, 13, 38};
    auto ret2 = std::find(std::begin(a), std::end(a), 72);
#if _cplusplus > 201103L
    if (ret2 != std::cend(a))
        cout << *ret2 << endl;
#endif
    //accumulate
    auto sum = std::accumulate(std::begin(a), std::end(a), 0);
    cout << sum << endl;

    //equal
    std::array<int, sizeof(a) / sizeof(*a)> a2{13, 2};
    auto ret4 = std::equal(std::begin(a), std::end(a), a2.begin());
    cout << ret4 << endl;

    //fill
    std::fill(a2.begin(), a2.end(), 8);

    //fill_n
    vector<int> vec;
    auto it = std::back_inserter(vec);
    *it = 32;

    std::fill_n(std::back_inserter(vec), 10, 0);
    cout << "vec size is " << vec.size() << endl;

    //copy
    vec.resize(0);
    std::copy(std::begin(a), std::end(a), back_inserter(vec));

    //replace
    std::replace(vec.begin(), vec.end(), 13, 28);

    //replace_copy
    vec.resize(0);
    std::replace_copy(std::begin(a), std::end(a), back_inserter(vec), 13, 28);

    string input_text{"the quick red fox jumps over the slow red turtle"};
    vector<string> story_text;
    std::stringstream input_st(input_text);
    string si;
    while (input_st >> si) {
        story_text.push_back(si);
    }

    //sort
    std::sort(story_text.begin(), story_text.end());

    //unique
    auto ret5 = std::unique(story_text.begin(), story_text.end());
    story_text.erase(ret5, story_text.end());

    print(story_text);

    vector<string> story_text_2;
    std::copy(story_text.begin(), story_text.end(), std::back_inserter(story_text_2));
    std::stable_sort(story_text.begin(), story_text.end(), isShorter);
    std::sort(story_text_2.begin(), story_text_2.end(), isShorter);
    print(story_text);
    print(story_text_2);

    auto ret6 = biggies(story_text_2, 4);
    std::for_each(ret6, story_text_2.cend(), [](const string &s)->void {cout << s << " "; });
    cout << endl;

    //lambda
    {
        int i1 = 1;
        int ii = 10;
        {
            /*const*/ int i2 = 2;
            int ii = 20;
            auto f = [=, &i2] ()  mutable {cout << ++ii <<  " " << ++i2 << endl; };
            f();
        }
    }

    //bind
    test_bind();

    //insert iterator
    {
        std::list<int> lst = { 1, 2, 3, 4 };
        std::deque<int> lst2, lst3;
        std::copy(lst.begin(), lst.end(), std::front_inserter(lst2));

        *std::front_inserter(lst2) = 5;
        *std::back_inserter(lst2) = 0;
        *std::inserter(lst2, lst2.begin()+ (lst2.end()-lst2.begin())/2) = 10;

        for_each(lst2.begin(), lst2.end(), [](int v) {cout << v << " "; });
        cout << endl;
    }

    //istream/ostream iterator
    {
        std::ostream_iterator<int> oit(cout, " ");
        oit = 1;
        *oit = 2;
        cout << endl;
        std::vector<int> vec= { 1, 2, 3, 4, 5 };
        std::copy(vec.begin(), vec.end(), oit);
        cout << endl;

        //	std::istream_iterator<int> iit(cin);
        //	std::istream_iterator<int> eof;

//		iit++;	//在这里要输入两次, 因为++ --是读取下一个值，在读取下一个值之前，先要读取得到当前值
//      int n = -1;
//      n = *iit++;
//      cout << n << endl;
//
//      assert(++iit == eof);

//      cout << "begin loop" << endl;
//      vec.clear();
//      int vec_size;
//      while (iit != eof) {
//          vec.push_back(*iit++);
//          vec_size = vec.size();
//      }
//      std::copy(vec.begin(), vec.end(), oit);
//      cout << endl;
    }

    //reverse iterator
    {
        string line{"first, middle, last"};
        auto it = std::find(line.cbegin(), line.cend(), ',');
        cout << string(line.cbegin(), it) << endl;

        auto rit = std::find(line.crbegin(), line.crend(), ',');
        cout << string(rit.base(), line.cend()) << endl;	//base成员函数得到正向的迭代器
    }

    return 0;
}

static void elimDups(vector<string> &vs)
{
    std::sort(vs.begin(), vs.end());
    auto end_uniq = std::unique(vs.begin(), vs.end());
    vs.erase(end_uniq, vs.end());
}

static void print(vector<string> &vs)
{
    string s_out;

    for (auto &s : vs)
        s_out += s + " ";

    cout << s_out << endl;
}

static bool isShorter(const string &s1, const string &s2)
{
    return s1.size() < s2.size();
}

static bool check_size(const string &s, string::size_type sz)
{
    return s.size() >= sz;
}

static vector<string>::const_iterator biggies(vector<string> &vs, string::size_type sz)
{
#if 0
    //use lambda
    return find_if(vs.begin(), vs.end(), [sz](const string &s)->bool {return s.size() >= sz;});
#else
    //use bind
    return find_if(vs.begin(), vs.end(), std::bind(check_size, _1, sz));

#endif

}

static initializer_list<int> bind_routine(double &d, int &i, std::ostream &os)
{
    d++;
    i++;

    os << i << " " << d << endl;

    return { i };
}

static void test_bind()
{
    int a = 3;
    double d = 3.14;

    //传给bind的非占位符参数默认是按值传递
    auto f = std::bind(bind_routine, _1, std::ref(a), std::ref(cout));

    auto d2 = f(d);
    assert(*d2.begin() == a);
//	assert(d2 == d);
}

/* associative_containers:
 * map multimap
 * set multist
 */
static int test_associative_container()
{
    std::map<string, string> authors = { {"Joyce", "James"}, {"Austen", "Jane"} };
    std::set<int> excludes = {1, 3, 5, 8};

    auto it = authors.find("Austen");
    if (it != authors.end())
        cout << it->second << endl;

    auto it2 = excludes.find(3);
    if (it2 != excludes.end()) {
        cout << *it2 << endl;
    }

    Item i;
    std::set<Item> s2;

    std::multiset<std::string, decltype(isShorter)*> booktore(isShorter);

    std::pair<string, int> p2 = std::make_pair("12", 3);
    cout << p2.first << p2.second << endl;

    authors.insert(authors.end(), {"Joyc2", "houren"});
    authors.insert(authors.end(), { "Joyc3", "houren" });

    auto r3 = authors.at("Joyc3");
    cout << "r3 = " << r3 << endl;

    const std::map<string, int> p3{ {"a", 1}, {"b", 2} };


    return 0;
}

/* dynamic memory
 * smart pointers:
 * shared_ptr
 * unique_ptr
 * weak_ptr
 */

class Unit {
public:
    //Unit() { cout << "constructor Unit" << endl; }
    ~Unit() { cout << "destructor Unit" << endl; }
};

static std::shared_ptr<Unit> shared_ptr_routine()
{
    std::shared_ptr<Unit> p_unit = std::make_shared<Unit>();

    return p_unit;
}

static void shared_ptr_process(std::shared_ptr<Unit> p)
{
    cout << "process" << endl;
}

static void test_text_query()
{
    std::ifstream is("input.txt");
    TextQuery tq(is);

    QueryResult r = tq.query("whr");
    print(cout, r);
}

static int test_dynamic_memory(int argc, char ** argv)
{
    std::shared_ptr<int> p1 = std::make_shared<int>(8);
    *p1 = 16;
    //	cout << *p1 << endl;

    //	auto p3 = shared_ptr_routine();
    //  std::shared_ptr<Unit> p4(p3.get());	//自身的拷贝才会增加引用计数
    //  p3.reset();
    //  cout << p3.use_count() << endl;
    //  p4.reset();

    //  Unit* pu = new Unit();
    //  shared_ptr_process(std::shared_ptr<Unit>(pu));	//只有一个引用

    std::shared_ptr<Unit> p4;

    //  std::unique_ptr<Unit> up(new Unit());
    //  p4.reset(up.release());

    char *cp = new char[0];		//off-the-end pointer
    char *cp2 = new char[0];
    int *ip = new int[0];
    //cout << *cp << endl;

    delete[] cp;
    delete[] cp2;
    delete[] ip;

    std::unique_ptr<Unit[]> up2(new Unit[3]);
    auto &u = up2[0];
    up2.reset();

    std::allocator<Unit> a;
    Unit* aup = a.allocate(3);
    a.construct(aup);
    a.destroy(aup);
    a.deallocate(aup, 3);

    std::vector<int> v(20);
    std::allocator<int> a2;
    auto aup2 = a2.allocate(v.size() * 2);
    auto p2 = std::uninitialized_copy(v.begin(), v.end(), aup2);

#if __cplusplus > 201103L
    auto size1 = std::uninitialized_fill_n(p2, v.size(), 40) - aup2;
    cout << size1 << endl;
#endif


    std::shared_ptr<int> p3(new int{ 8 });
    cout << *p3 << endl;

    test_text_query();

    test_manipulator();

    return 0;
}

static void test_manipulator()
{
    std::cout << "default boolean value:" << true << false <<
        "\nalpha boolean value:" << std::boolalpha << true << false << endl;
}

class A {
public:
    // A() = default;
    ~A() {
        std::cout << "dtor A" << std::endl;
    }
};

static void test_shared_ptr()
{
    std::shared_ptr<A> p1(new A, [](A*p){std::cout << "delete point" << std::endl; delete p;});

    {
        std::shared_ptr<A> p2(p1.get());

        std::weak_ptr<A> wp = p2;

        if (auto p = wp.lock()) {
            auto p2 = wp.lock();

            std::cout << "get sp success " << p.get() << std::endl;
            std::cout << "p use count: " << p.use_count() << std::endl;

        }

        std::cout << "p2 " << p2.get() << std::endl;

        std::cout << "before leave inner test" << std::endl;
        std::cout << "p2 use count: " << p2.use_count() << std::endl;

    }
    std::cout << "after leave inner test" << std::endl;
}

static void test_std_remove()
{
    std::vector<int> v = {2, 1, 2, 3, 4, 2, 5};
    auto ret = std::remove(v.begin(), v.end(), 2); 
    // std::cout << *ret << std::endl;
    // v.erase(std::remove(v.begin(), v.end(), 2), v.end());

    for (auto it = v.begin(); it != ret; ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for (; ret != v.end(); ++ret) {
        std::cout << *ret << " "; 
    }
    std::cout << std::endl;
}

class MA {
public:
    MA() {
        mData = new int[10];
        std::cout << "new mData:" << mData << std::endl;
        for (int i = 0; i < 10; ++i) {
            mData[i] = i;
        }
    }

    int *mData;
};

static void test_std_move()
{
    MA a;

    MA a2(std::move(a));

    for (int i = 0; i < 10; ++i) {
        std::cout << a2.mData[i] << " ";
    } 
    std::cout << std::endl;

    std::cout << "a::mData:" << a.mData << " a2::mData:" << a2.mData << std::endl;

}

class TBase {
public:
    int mInfo = 10;
};

class TBase2 {
public:
    TBase2(int d) : mInfo2(d) {}

    int mInfo2;
};

class TDerived : public TBase, public TBase2 {
public:
    TDerived() :TBase(), TBase2(mInfo) {
        std::cout << mInfo2 << std::endl;
    }
};

static void test_multi_derived()
{
    TDerived td;
}

