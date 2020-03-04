//
// Created by kotori0 on 2020/2/5.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <dlfcn.h>
#include <whale.h>
#include <string>
#include <ios>
#include <sstream>
#include "hook_main.h"
#include "enhanced_dlfcn.h"
#include "il2cpp.h"
#include <iomanip>
#include <sys/system_properties.h>

int isGame(JNIEnv *env, jstring appDataDir) {
    if (!appDataDir)
        return 0;

    const char *app_data_dir = env->GetStringUTFChars(appDataDir, NULL);

    int user = 0;
    static char package_name[256];
    if (sscanf(app_data_dir, "/data/%*[^/]/%d/%s", &user, package_name) != 2) {
        if (sscanf(app_data_dir, "/data/%*[^/]/%s", package_name) != 1) {
            package_name[0] = '\0';
            LOGW("can't parse %s", app_data_dir);
            return 0;
        }
    }
    env->ReleaseStringUTFChars(appDataDir, app_data_dir);
    if (strcmp(package_name, game_name) == 0) {
        LOGD("detect game: %s", package_name);
        return 1;
    }
    else {
        return 0;
    }
}

unsigned long get_module_base(const char* module_name)
{
    FILE *fp;
    unsigned long addr = 0;
    char *pch;
    char filename[32];
    char line[1024];

    snprintf(filename, sizeof(filename), "/proc/self/maps");

    fp = fopen(filename, "r");

    if (fp != NULL) {
        while (fgets(line, sizeof(line), fp)) {
            if (strstr(line, module_name) && strstr(line, "r-xp")) {
                pch = strtok(line, "-");
                addr = strtoul(pch, NULL, 16);
                if (addr == 0x8000)
                    addr = 0;
                break;
            }
        }
        fclose(fp);
    }
    return addr;
}

struct cSharpByteArray {
    size_t idkwhatisthis[3];
    size_t length;
    char buf[4096];
};

struct cSharpString {
    size_t address;
    size_t nothing;
    int length;
    char buf[4096];
};

typedef void* (*dlopen_type)(const char* name,
                             int flags,
                             //const void* extinfo,
                             const void* caller_addr);
dlopen_type dlopen_backup = nullptr;
void* dlopen_(const char* name,
              int flags,
              //const void* extinfo,
              const void* caller_addr){

    void* handle = dlopen_backup(name, flags, /*extinfo,*/ caller_addr);
    if(!il2cpp_handle){
        LOGI("dlopen: %s", name);
        if(strstr(name, "libil2cpp.so")){
            il2cpp_handle = handle;
            LOGI("Got il2cpp handle at %lx", (long)il2cpp_handle);
        }
    }
    return handle;
}

typedef size_t (*Hook)(char* instance, cSharpByteArray* src);
Hook backup = nullptr;
size_t hook(char* instance, cSharpByteArray *src){
    if(backup == nullptr){
        LOGE("backup DOES NOT EXIST");
    }
    size_t r = backup(instance, src);
    return r;
}

void hook_each(unsigned long rel_addr, void* hook, void** backup){
    LOGI("Installing hook at %lx", rel_addr);
    unsigned long addr = /*base_addr + */rel_addr;

    //设置属性可写
    void* page_start = (void*)(addr - addr % PAGE_SIZE);
    if (-1 == mprotect(page_start, PAGE_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC)) {
        LOGE("mprotect failed(%d)", errno);
        return ;
    }

    WInlineHookFunction(
            reinterpret_cast<void*>(addr),
            hook,
            backup);
    mprotect(page_start, PAGE_SIZE, PROT_READ | PROT_EXEC);
}

void *hack_thread(void *arg)
{
    LOGI("hack thread: %d", gettid());

    void *dl = dlopen("libdl.so", RTLD_LAZY);
    void* __loader_dlopen = dlsym(dl, "__loader_dlopen");
    hook_each((unsigned long)__loader_dlopen, (void*)dlopen_, (void**)&dlopen_backup);

    while (true)
    {
        base_addr = get_module_base("libil2cpp.so");
        if (base_addr != 0 && il2cpp_handle != nullptr) {
            break;
        }
    }
    LOGD("detect libil2cpp.so %lx, start sleep", base_addr);
    il2cpp_domain_get_ il2cpp_domain_get = (il2cpp_domain_get_)dlsym(il2cpp_handle, "il2cpp_domain_get");
    il2cpp_domain_get_assemblies_ il2cpp_domain_get_assemblies = (il2cpp_domain_get_assemblies_)dlsym(il2cpp_handle, "il2cpp_domain_get_assemblies");
    il2cpp_assembly_get_image_ il2cpp_assembly_get_image = (il2cpp_assembly_get_image_)dlsym(il2cpp_handle, "il2cpp_assembly_get_image");
    il2cpp_class_from_name_ il2cpp_class_from_name = (il2cpp_class_from_name_)dlsym(il2cpp_handle, "il2cpp_class_from_name");
    il2cpp_class_get_method_from_name_ il2cpp_class_get_method_from_name = (il2cpp_class_get_method_from_name_)dlsym(il2cpp_handle, "il2cpp_class_get_method_from_name");
    sleep(2);
    LOGD("hack game begin");
    Il2CppDomain* domain = il2cpp_domain_get();
    unsigned long ass_len = 0;
    const Il2CppAssembly** assembly_list = il2cpp_domain_get_assemblies(domain, &ass_len);
    while(strcmp((*assembly_list)->aname.name, "Assembly-CSharp") != 0){
        LOGD("Assembly name: %s", (*assembly_list)->aname.name);
        assembly_list++;
    }
    const Il2CppImage* image = il2cpp_assembly_get_image(*assembly_list);
    Il2CppClass* clazz = il2cpp_class_from_name(image, "Namespace", "Classname");

    hook_each((unsigned long)il2cpp_class_get_method_from_name(clazz, "Your Method", 1)->methodPointer, (void*)hook, (void**)&backup);

    LOGD("hack game finish");
    return NULL;
}