//
// Created by beich on 2019/4/16.
//

#ifndef FAKELINKER_SOINFO_H
#define FAKELINKER_SOINFO_H

#include <elf.h>
#include <stdint.h>
#include <link.h>
#include <string>
#include <vector>
#include <unordered_set>

#include "linked_list.h"

#define SOINFO_NAME_LEN 128

#if !defined(__LP64__)
#define __work_around__
#endif

#if defined(__aarch64__) || defined(__x86_64__)
#define USE_RELA 1
#endif


typedef void (* linker_dtor_function_t)();

typedef void (* linker_ctor_function_t)(int, char **, char **);

typedef struct soinfo9 soinfo;
struct android_namespace_t;

class SoinfoListAllocator {
    // non member
public:
    static LinkedListEntry<soinfo> * alloc() {
        // non implement
        assert(false);
    }

    static void free(LinkedListEntry<soinfo> * entry) {
        // non implement
        assert(false);
    }

private:
    DISALLOW_COPY_AND_ASSIGN(SoinfoListAllocator);
};

class NamespaceListAllocator {
    // non member
public:
    static LinkedListEntry<android_namespace_t> * alloc() {
        // non implement
        assert(false);
        return nullptr;
    }

    static void free(LinkedListEntry<android_namespace_t> * entry) {
        // non implement
        assert(false);
    }

private:
    DISALLOW_COPY_AND_ASSIGN(NamespaceListAllocator);
};

typedef LinkedList<soinfo, SoinfoListAllocator> soinfo_list_t;
typedef LinkedList<android_namespace_t, NamespaceListAllocator> android_namespace_list_t;


struct android_namespace_link_t {
    android_namespace_t * const linked_namespace_;
    const std::unordered_set<std::string> shared_lib_sonames_;
    bool allow_all_shared_libs_;
};

struct android_namespace_t {
    const char * name_;
    bool is_isolated_;
    bool is_greylist_enabled_;
    std::vector<std::string> ld_library_paths_;
    std::vector<std::string> default_library_paths_;
    std::vector<std::string> permitted_paths_;
    std::vector<android_namespace_link_t> linked_namespaces_;
    soinfo_list_t soinfo_list;
};

struct soinfo9 {
#if defined(__work_around__)
//private:
    char old_name_[SOINFO_NAME_LEN];
#endif
public:
    const ElfW(Phdr) * phdr;  // ElfW(Phdr)* phdr
    size_t phnum;
#if defined(__work_around__)
    ElfW(Addr) unused0;
#endif
    ElfW(Addr) base;
    size_t size;

#if defined(__work_around__)
    uint32_t unused1;
#endif

    ElfW(Dyn) * dynamic;

#if defined(__work_around__)
    uint32_t unused2;
    uint32_t unused3;
#endif

    soinfo9 * next;
//private:
    uint32_t flags_;

    const char * strtab_;
    ElfW(Sym) * symtab_;

    size_t nbucket_;
    size_t nchain_;
    uint32_t * bucket_;
    uint32_t * chain_;
#if defined(__mips) || !defined(__LP64__)
    ElfW(Addr) ** plt_got_;
#endif

#if defined(USE_RELA)
    ElfW(Rela)* plt_rela_;
    size_t plt_rela_count_;

    ElfW(Rela)* rela_;
    size_t rela_count_;
#else
    ElfW(Rel) * plt_rel_;
    size_t plt_rel_count_;

    ElfW(Rel) * rel_;
    size_t rel_count_;
#endif

    linker_ctor_function_t * preinit_array_;
    size_t preinit_array_count_;

    linker_ctor_function_t * init_array_;
    size_t init_array_count_;

    linker_dtor_function_t * fini_array_;
    size_t fini_array_count_;

    linker_ctor_function_t init_func_;
    linker_dtor_function_t fini_func_;

#if defined(__arm__)
public:
    uint32_t * ARM_exidx;
    size_t ARM_exidx_count;
//private:
#elif defined(__mips__)
    uint32_t mips_symtabno_;
    uint32_t mips_local_gotno_;
    uint32_t mips_gotsym_;

#endif
    size_t ref_count_;
public:
    link_map link_map_head;

    bool constructors_called;
    ElfW(Addr) load_bias;

#if !defined(__LP64__)
    bool has_text_relocations;
#endif
    bool has_DT_SYMBOLIC;

//private:
    uint32_t version_;
    dev_t st_dev_;
    ino_t st_ino_;

    soinfo_list_t children_;
    soinfo_list_t parents_;

    // version >=1
    off64_t file_offset_;
    uint32_t rtld_flags_;
    uint32_t dl_flags_1_;
    size_t strtab_size_;

    // version>=2
    size_t gnu_nbucket_;
    uint32_t * gnu_bucket_;
    uint32_t * gnu_chain_;
    uint32_t gnu_maskwords_;
    uint32_t gnu_shift2_;
    ElfW(Addr) * gnu_bloom_filter_;

    soinfo9 * local_group_root_;
    uint8_t * android_relocs_;
    size_t android_relocs_size_;

    const char * soname_;
    std::string realpath_;

    const ElfW(Versym) * versym_;

    ElfW(Addr) verder_ptr_;
    size_t verdef_cnt_;
    ElfW(Addr) verneed_ptr_;
    size_t verneed_cnt_;

    uint32_t target_sdk_version_;

    // version >=3
    std::vector<std::string> dt_runpath_;
    android_namespace_t * primary_namespace_;
    android_namespace_list_t secondary_namespaces_;
    uintptr_t handle_;
    // version >=4
    ElfW(Relr) * relr_;
    size_t relr_count_;
};

struct soinfo8 {
#if defined(__work_around__)
//private:
    char old_name_[SOINFO_NAME_LEN];
#endif
public:
    const ElfW(Phdr) * phdr;  // ElfW(Phdr)* phdr
    size_t phnum;
#if defined(__work_around__)
    ElfW(Addr) unused0;
#endif
    ElfW(Addr) base;
    size_t size;

#if defined(__work_around__)
    uint32_t unused1;
#endif

    ElfW(Dyn) * dynamic;

#if defined(__work_around__)
    uint32_t unused2;
    uint32_t unused3;
#endif

    soinfo8 * next;
//private:
    uint32_t flags_;

    const char * strtab_;
    ElfW(Sym) * symtab_;

    size_t nbucket_;
    size_t nchain_;
    uint32_t * bucket_;
    uint32_t * chain_;
#if defined(__mips) || !defined(__LP64__)
    ElfW(Addr) ** plt_got_;
#endif

#if defined(USE_RELA)
    ElfW(Rela)* plt_rela_;
    size_t plt_rela_count_;

    ElfW(Rela)* rela_;
    size_t rela_count_;
#else
    ElfW(Rel) * plt_rel_;
    size_t plt_rel_count_;

    ElfW(Rel) * rel_;
    size_t rel_count_;
#endif

    linker_ctor_function_t * preinit_array_;
    size_t preinit_array_count_;

    linker_ctor_function_t * init_array_;
    size_t init_array_count_;

    linker_dtor_function_t * fini_array_;
    size_t fini_array_count_;

    linker_ctor_function_t init_func_;
    linker_dtor_function_t fini_func_;

#if defined(__arm__)
public:
    uint32_t * ARM_exidx;
    size_t ARM_exidx_count;
//private:
#elif defined(__mips__)
    uint32_t mips_symtabno_;
    uint32_t mips_local_gotno_;
    uint32_t mips_gotsym_;

#endif
    size_t ref_count_;
public:
    link_map link_map_head;

    bool constructors_called;
    ElfW(Addr) load_bias;

#if !defined(__LP64__)
    bool has_text_relocations;
#endif
    bool has_DT_SYMBOLIC;

//private:
    uint32_t version_;
    dev_t st_dev_;
    ino_t st_ino_;

    soinfo_list_t children_;
    soinfo_list_t parents_;

    // version >=1
    off64_t file_offset_;
    uint32_t rtld_flags_;
    uint32_t dl_flags_1_;
    size_t strtab_size_;

    // version>=2
    size_t gnu_nbucket_;
    uint32_t * gnu_bucket_;
    uint32_t * gnu_chain_;
    uint32_t gnu_maskwords_;
    uint32_t gnu_shift2_;
    ElfW(Addr) * gnu_bloom_filter_;

    soinfo8 * local_group_root_;
    uint8_t * android_relocs_;
    size_t android_relocs_size_;

    const char * soname_;
    std::string realpath_;

    const ElfW(Versym) * versym_;

    ElfW(Addr) verder_ptr_;
    size_t verdef_cnt_;
    ElfW(Addr) verneed_ptr_;
    size_t verneed_cnt_;

    uint32_t target_sdk_version_;

    // version >=3
    std::vector<std::string> dt_runpath_;
    android_namespace_t * primary_namespace_;
    android_namespace_list_t secondary_namespaces_;
    uintptr_t handle_;
};

typedef void (* linker_function_t)();

struct soinfo7 {
#if defined(__work_around__)
//private:
    char old_name_[SOINFO_NAME_LEN];
#endif
public:
    const ElfW(Phdr) * phdr;  // ElfW(Phdr)* phdr
    size_t phnum;
    ElfW(Addr) entry;
    ElfW(Addr) base;
    size_t size;

#if defined(__work_around__)
    uint32_t unused1;
#endif

    ElfW(Dyn) * dynamic;

#if defined(__work_around__)
    uint32_t unused2;
    uint32_t unused3;
#endif

    soinfo7 * next;
//private:
    uint32_t flags_;

    const char * strtab_;
    ElfW(Sym) * symtab_;

    size_t nbucket_;
    size_t nchain_;
    uint32_t * bucket_;
    uint32_t * chain_;
#if defined(__mips) || !defined(__LP64__)
    ElfW(Addr) ** plt_got_;
#endif

#if defined(USE_RELA)
    ElfW(Rela)* plt_rela_;
    size_t plt_rela_count_;

    ElfW(Rela)* rela_;
    size_t rela_count_;
#else
    ElfW(Rel) * plt_rel_;
    size_t plt_rel_count_;

    ElfW(Rel) * rel_;
    size_t rel_count_;
#endif

    linker_function_t * preinit_array_;
    size_t preinit_array_count_;

    linker_function_t * init_array_;
    size_t init_array_count_;

    linker_function_t * fini_array_;
    size_t fini_array_count_;

    linker_function_t init_func_;
    linker_function_t fini_func_;

#if defined(__arm__)
public:
    uint32_t * ARM_exidx;
    size_t ARM_exidx_count;
//private:
#elif defined(__mips__)
    uint32_t mips_symtabno_;
    uint32_t mips_local_gotno_;
    uint32_t mips_gotsym_;
#endif

    size_t ref_count_;
public:
    link_map link_map_head;
    bool constructors_called;
    ElfW(Addr) load_bias;

#if !defined(__LP64__)
    bool has_text_relocations;
#endif
    bool has_DT_SYMBOLIC;

//private:
    uint32_t version_;
    dev_t st_dev_;
    ino_t st_ino_;

    soinfo_list_t children_;
    soinfo_list_t parents_;

    // version >=1
    off64_t file_offset_;
    uint32_t rtld_flags_;
    uint32_t dl_flags_1_;
    size_t strtab_size_;

    // version>=2
    size_t gnu_nbucket_;
    uint32_t * gnu_bucket_;
    uint32_t * gnu_chain_;
    uint32_t gnu_maskwords_;
    uint32_t gnu_shift2_;
    ElfW(Addr) * gnu_bloom_filter_;

    soinfo7 * local_group_root_;

    uint8_t * android_relocs_;
    size_t android_relocs_size_;

    const char * soname_;
    std::string realpath_;

    const ElfW(Versym) * versym_;

    ElfW(Addr) verder_ptr_;
    size_t verdef_cnt_;

    ElfW(Addr) verneed_ptr_;
    size_t verneed_cnt_;

    uint32_t target_sdk_version_;

    // version >=3
    std::vector<std::string> dt_runpath_;
    android_namespace_t * primary_namespace_;
    android_namespace_list_t secondary_namespaces_;
    uintptr_t handle_;
};




#endif //FAKELINKER_SOINFO_H
