// Bypass CL_INVALID_BINARY in libsdk_sr.so ocl_runtime.cpp:834; creates return fake handle, rest no-op.
#include <log/log.h>
#include <stdint.h>

#define SHIM_TAG "libsdk_sr_shim"
static void* const kFakeHandle = reinterpret_cast<void*>(0x53484152);

static void* FakeCreate(const char* who) {
    __android_log_print(ANDROID_LOG_WARN, SHIM_TAG, "%s bypassed, fake handle", who);
    return kFakeHandle;
}

extern "C" {

// Creates return fake handle; run/destroy/config are no-op success.
void* sr_create_ex()          { return FakeCreate("sr_create_ex"); }
void* st_sr_create_ex()       { return FakeCreate("st_sr_create_ex"); }
void* sr_create()             { return FakeCreate("sr_create"); }
void* st_sr_create()          { return FakeCreate("st_sr_create"); }
void* srsuper_create()        { return FakeCreate("srsuper_create"); }
void* st_srsuper_create()     { return FakeCreate("st_srsuper_create"); }
int sr_run()                  { return 0; }
int sr_run_ex()               { return 0; }
int sr_run_scale()            { return 0; }
int sr_run_scale_ex()         { return 0; }
int sr_run_preview_raw()      { return 0; }
int st_sr_run()               { return 0; }
int st_sr_run_ex()            { return 0; }
int st_sr_run_preview_raw()   { return 0; }
int st_sr_run_scale()         { return 0; }
int st_sr_run_scale_ex()      { return 0; }
int srsuper_run()             { return 0; }
int st_srsuper_run()          { return 0; }
int sr_destroy()              { return 0; }
int st_sr_destroy()           { return 0; }
int srsuper_destroy()         { return 0; }
int st_srsuper_destroy()      { return 0; }
int sr_destroy_ex()           { return 0; }
int st_sr_destroy_ex()        { return 0; }
int st_sr_process_ex()        { return 0; }
int sr_process_ex()           { return 0; }
int sr_set_extention()        { return 0; }
int st_sr_set_extention()     { return 0; }
int sr_set_extention_ptr()    { return 0; }
int st_sr_set_extention_ptr() { return 0; }
int sr_set_callback()         { return 0; }
int st_sr_set_callback()      { return 0; }
int sr_set_log_level()        { return 0; }
int st_sr_set_log_level()     { return 0; }
int sr_getversion()           { return 0; }
int st_sr_getversion()        { return 0; }
int srsuper_getversion()      { return 0; }
int st_srsuper_getversion()   { return 0; }

}  // extern "C"
