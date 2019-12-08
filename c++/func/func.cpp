#include <c_plus_plus_primer.h>
#include <cstdarg>
#include "func.h"

using std::va_list;

static int test_func_op();

//////////////////////////////////////////////////////////////////////////
int TEST_func(int argc, char **argv)
{
	int ret = 0;

	ret = test_func_op();

	return ret;
}

static void dummy(int *p)
{

}

static void print_array(int (&arr)[5])
{
	for (auto elem : arr) {
		cout << elem << endl;
	}
}

static void error_msg(initializer_list<string> &il)
{
	for (auto beg = il.begin(); beg != il.end(); ++beg)
		cout << *beg << " ";
	cout << endl;
}

static void ellipsis(char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

static const string & return_literal()
{
	// return "Empty";	//Empty先转换成一个临时的string对象，再返回该临时对象的引用
}

static vector<string> return_initilized_list()
{
	return {"hello", "world2"};
}

static auto trailling_return(int i) -> int (*)[10]
{
	static int a[10];

	return &a;
};

static int(*c_style_return(int i))[10]
{
    static int a[10];

    return &a;
}

static int odd[] = {1, 3, 5, 7, 9};
static int even[] = {0, 2, 4, 6, 8}; 
static decltype(odd) *return_array_ptr(int i)		//decltype数组时不会自动把数组名转换为指针
{
	return (i&1) ? &odd : &even;
}

static void decl_print(double)
{

}
static void decl_print(const string&)
{

}

static void decl_screen(unsigned int width, unsigned int height, char backgound)
{
	cout << "width = " << width << ", height = " << height << ", background = " << backgound << endl;
}

//可以分多次声明来定义默认参数，但是不能重复
//获取默认参数值是在声明所在的域内获取
static unsigned int decl_width = 120;
static void decl_screen(unsigned int width, unsigned int height=80, char background = 'R');
static void decl_screen(unsigned int width = decl_width, unsigned int height, char background);

static int* return_pfunc(int arg1, int arg2)
{
	cout << "arg1: " << arg1 << endl
		<< "arg2: " << arg2 << endl;

	return nullptr;
}

static auto trailing_pfunc(int arg) -> decltype(return_pfunc) *
{
	cout << __FUNCTION__ << " " << __LINE__  << " " << arg << endl;

	return return_pfunc;
}

static int test_func_op()
{
	int i = 3;
	const int ci = 4;

	dummy(&i);
//	dummy(&ci);

	int m_arr[5]{0,1};
	print_array(m_arr);

	initializer_list<string> il{"hello", "world"};
	auto il_size = il.size();
	error_msg(il);

	// ellipsis("%d: %s %s\n", 1001, "hello", "world");

	const string &s2 = return_literal();
	auto s2_size = s2.size();
//	const string s3 = s2;

	vector<string> vs4 = return_initilized_list();

	for (string s : vs4) {
		cout << s << " ";
	}
	cout << endl;

	int (*pa)[10] = trailling_return(0);

	decltype(odd)* pa2 = return_array_ptr(0);
	pa2 = return_array_ptr(1);

//	void decl_print(int);		//一旦加上该声明，函数外的decl_print声明(定义)即被隐藏，函数声明有作用域
	decl_print("hello");

	static unsigned int decl_width = 80;	//获取默认参数值时不受该定义影响，因为不在同一个作用域
	decl_screen();
	
	typedef int*(FUNC)(int, int);
	typedef int*(*PFUNC)(int, int);

	using FUNC2 = int*(int, int);
	using PFUNC2 = int*(*)(int, int);

	typedef decltype(return_pfunc) FUNC3;
	typedef decltype(return_pfunc) *PFUNC3;

//	FUNC f1(int);		//函数类型作返回值时不会自动转换为函数指针类型
//	FUNC2 f2(int);
//	FUNC3 f3(int);

	PFUNC pf1(int);
	PFUNC2 pf2(int);
	PFUNC3 pf3(int);

	trailing_pfunc(1)(10, 20);

	return 0;
}

int main(int argc, char *argv[])
{
    
    return 0;
}
