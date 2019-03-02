#ifndef _STRVEC_H_
#define _STRVEC_H_

#include <c_plus_plus_primer.h>

class StrVec
{
	friend void swap(StrVec&, StrVec&);
public:
	StrVec() : elements(nullptr), first_free(nullptr), cap(nullptr) {}
	StrVec(const StrVec&);		//没有move构造函数时可以用copy构造函数代替，反过来则不行
	StrVec(StrVec&&) noexcept;
	StrVec& operator=(const StrVec&) &;		
	StrVec& operator=(StrVec&&) noexcept;
	~StrVec();
	void push_back(const std::string&);
	size_t size() const { return first_free - elements; }
	size_t capacity() const { return cap - elements; }
	std::string* begin() const { return elements; }
	std::string* end() const { return first_free; }

private:
	std::allocator<std::string> alloc;
	void check_n_alloc() { if (size() == capacity()) reallocate(); }
	std::pair<std::string*, std::string*> alloc_n_copy(const std::string*, const std::string*);
	void free();
	void reallocate();

	std::string* elements = nullptr;
	std::string* first_free = nullptr;
	std::string* cap = nullptr;
};


#endif // _STRVEC_H_
