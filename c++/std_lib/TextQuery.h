#ifndef _TEXT_QUERY_H
#define	_TEXT_QUERY_H

#include <c_plus_plus_primer.h>

class QueryResult;
class TextQuery
{
public:
	typedef std::vector<string>::size_type line_no;
	TextQuery(std::ifstream&);
	QueryResult query(const std::string&) const;

private:
	std::shared_ptr<vector<string>> file;
	std::map <std::string, std::shared_ptr<std::set<line_no>>> wm;
};

class QueryResult
{
	friend std::ostream& print(std::ostream&, const QueryResult&);
public:
	QueryResult(std::string s, std::shared_ptr<std::set<TextQuery::line_no>> p, 
				std::shared_ptr<std::vector<string>> f) : sought(s), lines(p), file(f) {}

private:
	std::string sought;
	std::shared_ptr<std::set<TextQuery::line_no>> lines;
	std::shared_ptr<std::vector<std::string>> file;
};

#endif