#include "StrVec.h"

void swap(StrVec& lhs, StrVec& rhs)
{
	using std::swap;

	swap(lhs.elements, rhs.elements);
	swap(lhs.first_free, rhs.first_free);
	swap(lhs.cap, rhs.cap);
}

StrVec::StrVec(const StrVec&s)
{
	auto newdata = alloc_n_copy(s.elements, s.first_free);
	elements = newdata.first;
	first_free = cap = newdata.second;
}

StrVec::StrVec(StrVec&& s) noexcept /*: elements(nullptr), first_free(nullptr), cap(nullptr)*/
{
	swap(*this, s);
}

StrVec& StrVec::operator=(const StrVec& s) &
{
	auto data = alloc_n_copy(elements, first_free);
	free();
	elements = data.first;
	first_free = cap = data.second;

	return *this;
}

StrVec& StrVec::operator=(StrVec&& s) noexcept
{
	if (this != &s)
		swap(*this, s);

	return *this;
}

StrVec::~StrVec()
{
	free();
}

void StrVec::push_back(const std::string& s)
{
	check_n_alloc();
	alloc.construct(first_free++, s);
}

std::pair<std::string*, std::string*> StrVec::alloc_n_copy(const std::string* b, const std::string* e)
{
	auto data = alloc.allocate(e - b);
	return{ data, uninitialized_copy(b, e, data) };
}

void StrVec::free()
{
	if (elements) {
		for (auto p = first_free; p != elements; )
			alloc.destroy(--p);
		alloc.deallocate(elements, cap - elements);
	}
}

void StrVec::reallocate()
{
	auto newSize = capacity() != 0 ? 2 * capacity() : 1;
	auto newData = alloc.allocate(newSize);

#if 0
	auto dest = newData;
	auto elem = elements;
	for (auto i = 0; i != size(); ++i) {
		alloc.construct(dest++, std::move(*elem++));
	}
#else
	auto dest = uninitialized_copy(make_move_iterator(begin()), make_move_iterator(end()), newData);
#endif
	free();
	elements = newData;
	first_free = dest;
	cap = elements + newSize;
}

int main(int argc, char *argv[])
{
    
    return 0;
}
