#include <SilentDream/SilentDream.h>
#include <SilentDream/Log.h>
#include "ArgumentParser.h"


int main(int argc, char **argv)
{
    ArgumentParser argParser(argc, argv);
    if (argParser.initCheck() < 0) {
        return -1;
    }

    Log::initLogMode(argParser.getRunMode());

    return SilentDream::instance()->exec(argParser);
}
