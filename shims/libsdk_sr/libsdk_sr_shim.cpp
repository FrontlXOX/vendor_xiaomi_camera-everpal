// libsdk_sr_shim.cpp
// Dummy library to satisfy libmialgoengine.so dependencies and bypass OpenCL crashes

#include <stdint.h>

extern "C" {
    // Original stubs
    int st_sr_create_ex()    { return 0; }
    int sr_create_ex()       { return 0; }
    int st_sr_process_ex()   { return 0; }
    int st_sr_destroy_ex()   { return 0; }
    int sr_process_ex()      { return 0; }
    int sr_destroy_ex()      { return 0; }

    // Newly discovered missing symbols
    int sr_destroy()         { return 0; }
    int sr_getversion()      { return 0; }
    int sr_run()             { return 0; }
    int sr_run_scale()       { return 0; }
    int sr_set_extention()   { return 0; }
    int sr_set_extention_ptr() { return 0; }
    int srsuper_create()     { return 0; }
    int srsuper_destroy()    { return 0; }
    int srsuper_getversion() { return 0; }
    int srsuper_run()        { return 0; }
}