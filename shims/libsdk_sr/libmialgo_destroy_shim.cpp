// Defers MIADestroySession to a background thread.
//
// The A13 algo engine tears down hundreds of nodes synchronously (5-16s),
// blocking every Photo -> Video/Pro/Document mode switch. The HAL session is
// already closed at this point, so returning immediately is safe. Destroys are
// serialized with a mutex. Set persist.vendor.camera.sync_destroy=1 to revert
// to synchronous destroy.
#include <dlfcn.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/system_properties.h>

#include <log/log.h>

#define SHIM_TAG "libsdk_sr_shim"

typedef int (*MiaDestroyFn)(void*);

static pthread_mutex_t gDestroyLock = PTHREAD_MUTEX_INITIALIZER;

static bool SyncDestroyRequested() {
    // Escape hatch, checked once per destroy call (cheap).
    char value[PROP_VALUE_MAX] = {};
    __system_property_get("persist.vendor.camera.sync_destroy", value);
    return value[0] == '1';
}

static void* DestroyWorker(void* arg) {
    void* session = arg;
    pthread_mutex_lock(&gDestroyLock);
    MiaDestroyFn real =
        reinterpret_cast<MiaDestroyFn>(dlsym(RTLD_NEXT, "MIADestroySession"));
    if (real != nullptr) {
        __android_log_print(ANDROID_LOG_INFO, SHIM_TAG,
                            "async MIADestroySession %p begin", session);
        real(session);
        __android_log_print(ANDROID_LOG_INFO, SHIM_TAG,
                            "async MIADestroySession %p done", session);
    } else {
        __android_log_print(ANDROID_LOG_ERROR, SHIM_TAG,
                            "RTLD_NEXT MIADestroySession not found, session %p leaked",
                            session);
    }
    pthread_mutex_unlock(&gDestroyLock);
    return nullptr;
}

extern "C" int MIADestroySession(void* session) {
    if (session == nullptr) {
        return 0;
    }
    if (SyncDestroyRequested()) {
        MiaDestroyFn real =
            reinterpret_cast<MiaDestroyFn>(dlsym(RTLD_NEXT, "MIADestroySession"));
        return real != nullptr ? real(session) : 0;
    }
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int rc = pthread_create(&thread, &attr, DestroyWorker, session);
    pthread_attr_destroy(&attr);
    if (rc != 0) {
        __android_log_print(ANDROID_LOG_WARN, SHIM_TAG,
                            "pthread_create failed (%d), destroying synchronously", rc);
        MiaDestroyFn real =
            reinterpret_cast<MiaDestroyFn>(dlsym(RTLD_NEXT, "MIADestroySession"));
        return real != nullptr ? real(session) : 0;
    }
    __android_log_print(ANDROID_LOG_INFO, SHIM_TAG,
                        "MIADestroySession %p deferred to background", session);
    return 0;
}
