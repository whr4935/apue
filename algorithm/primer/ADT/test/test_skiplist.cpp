#include "../skiplist.h"
#include <utils/utils_main/utils_main.h>
#include <utils/utils.h>

int test_skipList()
{
    TRACE();
    SkipList<int, int> *s = new SkipList<int, int>();

    BenchMark b;
    benchmark_begin(&b);
    for (int i = 0; i < 10; ++i) {
        s->insertNode(i, i);
    }
    benchmark_end(&b, "insertNode");
    //std::cout << "insert OK" << std::endl;

    auto r = s->findNode(8);
    std::cout << "findNode " << 8 << ", get " << r.first << " " << r.second << std::endl;
    s->dump();

    std::cout << "deleteNode test" << std::endl;
    bool r2 = s->deleteNode(8);
    std::cout << "deleteNode " << 8 << " return " << r2 << std::endl;

    s->dump();

    delete s;

    return 0;
}
AUTO_RUN(test_skipList);
