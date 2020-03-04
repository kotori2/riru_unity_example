cmake_minimum_required(VERSION 3.4.1)

if (NOT DEFINED MODULE_NAME)
    message(FATAL_ERROR "MODULE_NAME is not set")
endif ()

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_CXX_STANDARD 14)
enable_language(ASM)
SET(CMAKE_ASM_FLAGS "${CFLAGS}  -x assembler-with-cpp")

if (CMAKE_SYSTEM_NAME MATCHES "^Android")
    set(KERNEL "Linux")
    set(PLATFORM "Android")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -s -Wl,--gc-sections")
    add_definitions("-DWHALE_ANDROID_AUTO_LOAD")
    message("Building Whale for Android(${CMAKE_SYSTEM_PROCESSOR})...")
elseif (PLATFORM STREQUAL "IOS")
    set(KERNEL "Darwin")
    add_definitions("-DDARWIN")
    include_directories(/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/usr/include)
    if (IOS_ARCH STREQUAL "arm64")
        set(CMAKE_SYSTEM_PROCESSOR "aarch64")
    elseif (IOS_ARCH MATCHES "arm")
        set(CMAKE_SYSTEM_PROCESSOR "arm")
    endif ()
    message("Building Whale for IOS(${CMAKE_SYSTEM_PROCESSOR})...")
else ()
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(KERNEL "Darwin")
        add_definitions("-DDARWIN")
    endif ()
    message("Building Whale for ${CMAKE_SYSTEM_NAME}(${CMAKE_SYSTEM_PROCESSOR})...")
endif ()

include_directories(whale/src/libffi whale/src/libffi/platform_include)
set(WHALE_SOURCES
        whale/src/whale.cc
        whale/src/interceptor.cc
        whale/src/dbi/hook_common.cc
        whale/src/platform/memory.cc
        whale/src/assembler/assembler.cc
        whale/src/assembler/memory_region.cc
        whale/src/dbi/instruction_set.cc
        whale/src/libffi/closures.c
        whale/src/libffi/debug.c
        whale/src/libffi/dlmalloc.c
        whale/src/libffi/java_raw_api.c
        whale/src/libffi/prep_cif.c
        whale/src/libffi/raw_api.c
        whale/src/libffi/types.c
        whale/src/libffi/ffi_cxx.cc
        )

set(WHALE_LINUX_SOURCES
        whale/src/platform/linux/elf_image.cc
        whale/src/platform/linux/process_map.cc
        )
set(WHALE_ANDROID_ART
        whale/src/android/art/native_on_load.cc
        whale/src/android/art/art_runtime.cc
        whale/src/android/art/art_symbol_resolver.cc
        whale/src/android/art/java_types.cc
        whale/src/android/art/well_known_classes.cc
        whale/src/android/art/art_method.cc
        whale/src/android/art/scoped_thread_state_change.cc
        whale/src/android/art/art_jni_trampoline.cc
        )

set(WHALE_AARCH32
        whale/src/dbi/arm/decoder_arm.cc
        whale/src/dbi/arm/decoder_thumb.cc
        whale/src/dbi/arm/inline_hook_arm.cc
        whale/src/dbi/arm/instruction_rewriter_arm.cc
        whale/src/libffi/arm/ffi_armv7.c
        whale/src/libffi/arm/sysv_armv7.S
        )

set(WHALE_AARCH64
        whale/src/dbi/arm64/decoder_arm64.cc
        whale/src/dbi/arm64/instruction_rewriter_arm64.cc
        whale/src/dbi/arm64/inline_hook_arm64.cc
        whale/src/libffi/aarch64/ffi_arm64.c
        whale/src/libffi/aarch64/sysv_arm64.S
        )
set(WHALE_DISASSEMBLER_X86_OR_X86_64
        whale/src/dbi/x86/distorm/decoder.c
        whale/src/dbi/x86/distorm/distorm.c
        whale/src/dbi/x86/distorm/instructions.c
        whale/src/dbi/x86/distorm/insts.c
        whale/src/dbi/x86/distorm/mnemonics.c
        whale/src/dbi/x86/distorm/operands.c
        whale/src/dbi/x86/distorm/prefix.c
        whale/src/dbi/x86/distorm/textdefs.c
        whale/src/dbi/x86/distorm/wstring.c
        )

set(WHALE_X86
        whale/src/assembler/x86/assembler_x86.cc
        whale/src/assembler/x86/managed_register_x86.cc
        whale/src/dbi/x86/inline_hook_x86.cc
        whale/src/dbi/x86/intercept_syscall_x86.cc
        whale/src/dbi/x86/instruction_rewriter_x86.cc
        whale/src/libffi/x86/ffi_i386.c
        whale/src/libffi/x86/sysv_i386.S
        )

set(WHALE_X86_64
        whale/src/assembler/x86_64/assembler_x86_64.cc
        whale/src/assembler/x86_64/managed_register_x86_64.cc
        whale/src/dbi/x86_64/inline_hook_x86_64.cc
        whale/src/dbi/x86_64/instruction_rewriter_x86_64.cc
        whale/src/libffi/x86/ffiw64_x86_64.c
        whale/src/libffi/x86/ffi64_x86_64.c
        whale/src/libffi/x86/unix64_x86_64.S
        whale/src/libffi/x86/win64_x86_64.S
        )

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")

    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch armv7")
    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_AARCH32})

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")

    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch arm64")
    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_AARCH64})

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86?)$")

    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch x86")
    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_X86} ${WHALE_DISASSEMBLER_X86_OR_X86_64})

elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64")

    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -arch x86_64")
    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_X86_64} ${WHALE_DISASSEMBLER_X86_OR_X86_64})

endif ()

if (PLATFORM STREQUAL "Android")

    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_ANDROID_ART})

    if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")

        set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_ANDROID_ART_AARCH32})

    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")

        set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_ANDROID_ART_AARCH64})

    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^(i.86|x86?)$")

        set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_ANDROID_ART_X86})

    elseif (CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64")
        set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_ANDROID_ART_X86_64})

    endif ()

endif ()

if (KERNEL STREQUAL "Linux")

    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_LINUX_SOURCES})

elseif (KERNEL STREQUAL "Darwin")
    set(WHALE_SOURCES ${WHALE_SOURCES} ${WHALE_DARWIN_SOURCES})
endif ()

set(LINKER_FLAGS "-ffixed-x18 -Wl,--hash-style=both")
set(C_FLAGS "-Werror=format -fdata-sections -ffunction-sections")

if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(C_FLAGS "${C_FLAGS} -O2 -fvisibility=hidden -fvisibility-inlines-hidden")
    set(LINKER_FLAGS "${LINKER_FLAGS} -Wl,-exclude-libs,ALL -Wl,--gc-sections")
else ()
    set(C_FLAGS "${C_FLAGS} -O0")
endif ()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    include_directories(whale/src/assembler/vixl)
endif ()

add_subdirectory(whale/src/assembler/vixl)
include_directories(whale/src)
add_definitions("-DWHALE_LIBRARY_NAME=\"lib${MODULE_NAME}.so\"")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${C_FLAGS}")

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${LINKER_FLAGS}")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${LINKER_FLAGS}")

add_library(${MODULE_NAME} SHARED main.cpp hook_main.cpp enhanced_dlfcn.c ${WHALE_SOURCES})
target_include_directories(${MODULE_NAME} PUBLIC whale/include)
target_link_libraries(${MODULE_NAME} log)
if (CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm|aarch64)")
    target_link_libraries(${MODULE_NAME} vixl)
endif ()
set_target_properties(${MODULE_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)
