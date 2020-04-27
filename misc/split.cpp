#include <vector>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

static bool inline isDelimiter(const std::string& delimiter, char c)
{
    return delimiter.find(c) != std::string::npos;
}


std::vector<std::string>& split(const std::string& str, std::vector<std::string>& result)
{
    std::string::size_type pos, prev = 0;
    std::string s;
    std::string cmdDelimiter = " ";
    std::string argDelimiter = ", ";

    pos = str.find_first_of(cmdDelimiter);

    if (pos == std::string::npos) {
        result.emplace_back(std::move(str));
        return result;
    }

    result.emplace_back(str, 0, pos);
    prev = pos + 1;

    while ((pos = str.find_first_of(argDelimiter, prev)) != std::string::npos) {
        if (pos > prev) {
            size_t b = prev;
            size_t e = pos;
            while (b < e) {
                if (isspace(str[b]) || isDelimiter(argDelimiter, str[b]))        ++b;
                else if (isspace(str[e]) || isDelimiter(argDelimiter, str[e]))   --e;
                else                        break;
            }

            if (b <= e) {
                result.emplace_back(str, b, e - b + 1);
            }
        }

        prev = pos + 1;
    }

    while (prev < str.size()) {
        if (isspace(str[prev]) || isDelimiter(argDelimiter, str[prev]))
            ++prev;
        else
            break;
    }

    if (prev < str.size())
        result.emplace_back(str, prev, str.size() - prev);

    return result;
}

void fmtTime()
{
    struct timeval tv;
    struct tm* tm;
    gettimeofday(&tv, nullptr);

    tm = localtime(&tv.tv_sec);

    char buf[40];
    snprintf(buf, sizeof(buf), "%02d-%02d %02d:%02d:%02d.%03d",
            tm->tm_mon + 1,
            tm->tm_mday,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec,
            tv.tv_usec/1000 
            );

    printf("time: %s\n", buf);
}


int main(int argc, char *argv[])
{
    const char* buf = "videoShow 1 2 3\n";    
    //const char buf[] = "videoShow";    

    std::string s(buf, 10);

    printf("s.size:%d\n", s.size());

    std::vector<std::string> result; 
    result = split(buf, result);

    for_each(result.begin(), result.end(), [](std::string& p) {printf("##:size=%ld,%s\n", p.size(), p.c_str()); });

    fmtTime();

    return 0;
}	
