#include <NBMacros.h>
#include <NBLog.h>

NBLOG_DEFINE();

CONSTRUCTOR_DEFINE(loadPrerequestPlugin) {
    NBLOG_INIT(NULL, NBLOG_MODE_CONSOLE);
}

DESTRUCTOR_DEFINE(unloadPrerequestPlugin) {
    NBLOG_DEINIT();
}