#include "TextQuery.h"

TextQuery::TextQuery(std::ifstream& is) : file(new vector<std::string>)
{
    string text;

    while (getline(is, text)) {
        file->push_back(text);
        int n = file->size() - 1;
        std::istringstream line(text);
        string word;
        while (line >> word) {
            auto &lines = wm[word];
            if (!lines) {
                lines = std::make_shared<std::set<line_no>>();
                //lines.reset(new std::set<line_no>);
            }
            lines->insert(n);
        }
    }
}

QueryResult TextQuery::query(const std::string& s) const
{
    static std::shared_ptr<std::set<line_no>> no_data(new std::set<line_no>);

    /* auto &sought = wm.find(s); */
    /* if (sought != wm.end()) { */
    /*  return QueryResult(s, sought->second, file); */
    /* } else { */
    /*  return QueryResult(s, no_data, file); */
    //}
}

std::ostream& print(std::ostream &os, const QueryResult& r)
{
    cout << r.sought << " occurs " << r.lines->size() << " time(s)" << endl;
    for (auto n : *r.lines) {
        cout << "(line " << n+1 << "): " << *(r.file->begin() + n) << endl;
    }

    return os;
}
