#include <iostream>
#include <map>
#include <execinfo.h>
#include <memory>
#include <cxxabi.h>
#include <string.h>
#include <signal.h>
#include "../utime.h"

//////////////////////////////////////////
using GMap = std::map<std::string, std::function<int()>>;
static GMap& getGroutineMap()
{
    static GMap g;
    return g;
}


bool registerRoutine(std::string name, std::function<int()> routine)
{
    auto ret = getGroutineMap().emplace(name, routine);

    return ret.second;
}


const char kMangledSymbolPrefix[] = "_Z";
// Characters that can be used for symbols, generated by Ruby:
// (('a'..'z').to_a+('A'..'Z').to_a+('0'..'9').to_a + ['_']).join
const char kSymbolCharacters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
// Demangles C++ symbols in the given text. Example:
// "out/Debug/base_unittests(_ZN10StackTraceC1Ev+0x20) [0x817778c]"
// =>
// "out/Debug/base_unittests(StackTrace::StackTrace()+0x20) [0x817778c]"
void DemangleSymbol (std::string* symbol)
{
    std::string::size_type search_from = 0;
    while (search_from < symbol->size ()) {
        // Look for the start of a mangled symbol from search_from
        std::string::size_type mangled_start = symbol->find (kMangledSymbolPrefix, search_from);
        if (mangled_start == std::string::npos) {
            break; // Mangled symbol not found
        }
        // Look for the end of the mangled symbol
        std::string::size_type mangled_end = symbol->find_first_not_of (kSymbolCharacters, mangled_start);
        if (mangled_end == std::string::npos) {
            mangled_end = symbol->size ();
        }
        std::string mangled_symbol = std::move (symbol->substr (mangled_start, mangled_end - mangled_start));
        // Try to demangle the mangled symbol candidate
        int status = -4; // some arbitrary value to eliminate the compiler warning
        std::unique_ptr<char, void(*)(void*)> demangled_symbol {
            abi::__cxa_demangle (mangled_symbol.c_str (), nullptr, 0, &status),
            std::free
        };
        // 0 Demangling is success
        if (0 == status) {
            // Remove the mangled symbol
            symbol->erase (mangled_start, mangled_end - mangled_start);
            // Insert the demangled symbol
            symbol->insert (mangled_start, demangled_symbol.get ());
            // Next time, we will start right after the demangled symbol
            search_from = mangled_start + strlen (demangled_symbol.get ());
        }
        else {
            // Failed to demangle. Retry after the "_Z" we just found
            search_from = mangled_start + 2;
        }
    }
}


void sig_handler(int sig)
{
#define BACKTRACE_SIZE 30
    void* array[BACKTRACE_SIZE];

    std::cout << "\033[1m\033[31m ==BACKTRACE== \033[0m" << std::endl;
    int size = backtrace(array, BACKTRACE_SIZE);

    char** strings = backtrace_symbols(array, size);
    for (int i = 0; i < size; ++i) {
        std::string symbol(strings[i]);
        DemangleSymbol(&symbol); 
    	printf("%s\n", symbol.c_str());    
    }
	free(strings);

    signal(sig, SIG_DFL);
}



int main(int argc, char *argv[]) 
{
    struct sigaction act;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    act.sa_handler = sig_handler;
    int err = sigaction(SIGSEGV, &act, NULL);
    if (err < 0) {
        perror("sigaction");
        exit(1);
    }


    for (auto& d : getGroutineMap()) {
        std::cout << "\033[32mRUN\033[0m " << d.first <<std::endl;
        d.second();
    } 

    return 0;
}

