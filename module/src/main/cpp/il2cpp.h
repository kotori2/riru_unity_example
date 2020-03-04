#ifndef IL2CPP_H
#define IL2CPP_H

typedef void Il2CppDomain;
typedef void Il2CppImage;
#define PUBLIC_KEY_BYTE_LENGTH 8
struct Il2CppAssemblyName
{
    const char* name;
    const char* culture;
    const char* hash_value;
    const char* public_key;
    uint32_t hash_alg;
    int32_t hash_len;
    uint32_t flags;
    int32_t major;
    int32_t minor;
    int32_t build;
    int32_t revision;
    uint8_t public_key_token[PUBLIC_KEY_BYTE_LENGTH];
};
struct Il2CppAssembly
{
    Il2CppImage* image;
    uint32_t token;
    int32_t referencedAssemblyStart;
    int32_t referencedAssemblyCount;
    Il2CppAssemblyName aname;
};
typedef void Il2CppDomain;
typedef void Il2CppClass;

struct MethodInfo {
    void* methodPointer;
};

typedef Il2CppDomain* (*il2cpp_domain_get_)();
typedef const Il2CppAssembly** (*il2cpp_domain_get_assemblies_) (const Il2CppDomain * domain, unsigned long * size);
typedef const Il2CppImage* (*il2cpp_assembly_get_image_) (const Il2CppAssembly * assembly);
typedef Il2CppClass* (*il2cpp_class_from_name_) (const Il2CppImage * image, const char* namespaze, const char *name);
typedef const MethodInfo* (*il2cpp_class_get_method_from_name_) (Il2CppClass * klass, const char* name, int argsCount);

#endif