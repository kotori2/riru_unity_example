//
// Created by kotori0 on 2020/2/5.
//

#ifndef RIRU_MODULETEMPLATE_MASTER_HOOK_MAIN_H
#define RIRU_MODULETEMPLATE_MASTER_HOOK_MAIN_H
#include <jni.h>
#include <android/log.h>

static int enable_hack;
static const char* game_name = "com.company.game"; // TODO:EDIT THIS
static unsigned long base_addr = 0;
static void* il2cpp_handle = nullptr;
int isGame(JNIEnv *env, jstring appDataDir);
unsigned long get_module_base(const char* module_name);
void *hack_thread(void *arg);

#define UnityVersion 2020.2.4f1

// UnityVersion Compatible list
// 5.3.0f4     | 5.3.0f4 - 5.3.1f1         | v16
// 5.3.2f1     | 5.3.2f1                   | v19
// 5.3.3f1     | 5.3.3f1 - 5.3.4f1         | v20
// 5.3.5f1     | 5.3.5f1                   | v21
// 5.3.6f1     | 5.3.6f1                   | v21
// 5.3.7f1     | 5.3.7f1 - 5.3.8f2         | v21
// 5.4.0f3     | 5.4.0f3                   | v21
// 5.4.1f1     | 5.4.1f1 - 5.4.3f1         | v21
// 5.4.4f1     | 5.4.4f1 - 5.4.6f3         | v21
// 5.5.0f3     | 5.5.0f3                   | v22
// 5.5.1f1     | 5.5.1f1 - 5.5.6f1         | v22
// 5.6.0f3     | 5.6.0f3 - 5.6.7f1         | v23
// 2017.1.0f3  | 2017.1.0f3 - 2017.1.2f1   | v24
// 2017.1.3f1  | 2017.1.3f1 - 2017.1.5f1   | v24
// 2017.2.0f3  | 2017.2.0f3                | v24
// 2017.2.1f1  | 2017.2.1f1 - 2017.4.40f1  | v24
// 2018.1.0f2  | 2018.1.0f2 - 2018.1.9f2   | v24
// 2018.2.0f2  | 2018.2.0f2 - 2018.2.21f1  | v24
// 2018.3.0f2  | 2018.3.0f2 - 2018.3.7f1   | v24.1
// 2018.3.8f1  | 2018.3.8f1 - 2018.4.36f1  | v24.1
// 2019.1.0f2  | 2019.1.0f2 - 2019.2.21f1  | v24.2
// 2019.3.0f6  | 2019.3.0f6 - 2019.3.6f1   | v24.2
// 2019.3.7f1  | 2019.3.7f1 - 2019.4.14f1  | v24.3
// 2019.4.15f1 | 2019.4.15f1 - 2019.4.20f1 | v24.4
// 2019.4.21f1 | 2019.4.21f1 - 2019.4.29f1 | v24.5
// 2020.1.0f1  | 2020.1.0f1 - 2020.1.10f1  | v24.3
// 2020.1.11f1 | 2020.1.11f1 - 2020.1.17f1 | v24.4
// 2020.2.0f1  | 2020.2.0f1 - 2020.2.3f1   | v27
// 2020.2.4f1  | 2020.2.4f1 - 2020.3.15f2  | v27.1
// 2021.1.0f1  | 2021.1.0f1 - 2021.1.16f1  | v27.2

#define STR(x) #x
#define STRINGIFY_MACRO(x) STR(x)
#define EXPAND(x) x
#define IL2CPPHEADER(a, b, c) STRINGIFY_MACRO(EXPAND(a)EXPAND(b)EXPAND(c))
#define IL2CPPAPIDIR il2cppapi/
#define IL2CPPCLASS IL2CPPHEADER(IL2CPPAPIDIR, UnityVersion, /il2cpp-class.h)
#define IL2CPPAPI IL2CPPHEADER(IL2CPPAPIDIR, UnityVersion, /il2cpp-api-functions.h)

#define LOG_TAG "UNITYHOOK"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

#endif //RIRU_MODULETEMPLATE_MASTER_HOOK_MAIN_H
