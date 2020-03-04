//
// Created by kotori0 on 2020/2/5.
//

#ifndef RIRU_MODULETEMPLATE_MASTER_HOOK_MAIN_H
#define RIRU_MODULETEMPLATE_MASTER_HOOK_MAIN_H
#include <jni.h>
#include <android/log.h>
#include <link.h>
#include <vector>
#include "soinfo.h"

extern "C" {
extern void *enhanced_dlopen(const char *filename, int flags);
extern void *enhanced_dlsym(void *handle, const char *symbol);
extern int enhanced_dlclose(void *handle);
}

static int enable_hack;
static const char* game_name = "com.company.game"; // TODO:EDIT THIS
static unsigned long base_addr = 0;
static void* il2cpp_handle = nullptr;
int isGame(JNIEnv *env, jstring appDataDir);
unsigned long get_module_base(const char* module_name);
void *hack_thread(void *arg);

#define LOG_TAG "UNITYHOOK"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#endif //RIRU_MODULETEMPLATE_MASTER_HOOK_MAIN_H
