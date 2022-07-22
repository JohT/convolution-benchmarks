add_library(compiler_options INTERFACE)
add_library(${PROJECT_NAME}::CompilerOptions ALIAS compiler_options)

# This file doesn't contain compile options, that are already set by juce::juce_recommended_config_flags:
# https://github.com/juce-framework/JUCE/blob/master/extras/Build/CMake/JUCEHelperTargets.cmake

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang") # Using GNU or Clang compiler ("GNU-style" C++ compiler)
    # -fPIC is needed for position-indipendent code to overcome the following error while linking:
    # "relocation R_X86_64_32S against symbol `stdout@@GLIBC_2.2.5' can not be used when making a shared object; recompile with -fPIC"
    target_compile_options(compiler_options INTERFACE "-fPIC")
    # This option lets the compiler make aggressive, potentially-lossy assumptions about floating-point math
    # https://clang.llvm.org/docs/UsersManual.html#cmdoption-ffast-math
    target_compile_options(compiler_options INTERFACE "-ffast-math")
    # Debugger data in DEBUG mode
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:-g>")
    # No optimization in DEBUG mode. Not meant for benchmarking.
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:-O0>")
    # Full (but stable) Optimization in RELEASE mode
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Release>:-O3>")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")  # Using Microsoft Visual Studio C++
    # /Gy: Allows the compiler to package individual functions in the form of packaged functions (COMDATs). [Microsoft Docs]
    #      The main advantage of this is that if you have identical functions the linker can collapse them all down into one actual piece of code ("COMDAT folding"). [StackOverflow]
    # /nologo: Suppresses the display of the copyright banner when the compiler starts up and display of informational messages during compiling. [Microsoft Docs]
    # /permissive-: Specify standards conformance mode to the compiler. Use this option to help you identify and fix conformance issues in your code, to make it both more correct and more portable. [Microsoft Docs]
    target_compile_options(compiler_options INTERFACE "/Gy;/nologo;/permissive-;")
    # Debugger data in DEBUG mode (MSVC)
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:/Zi>")
    # No optimization in DEBUG mode. Not meant for benchmarking. (MSVC)
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:/Od>")
    # Full (but stable) Optimization in RELEASE mode
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Release>:/O2>")
    # Replaces some function calls with intrinsic or otherwise special forms of the function that help your application run faster. (MSVC)
    target_compile_options(compiler_options INTERFACE "/Oi")
    # Build with Multiple Processes (MSVC)
    target_compile_options(compiler_options INTERFACE "/MP")
    # Exception handling with standard C++ stack unwinding (s) and assume that extern "C" never throw a C++ exception (c) (MSVC)
    target_compile_options(compiler_options INTERFACE "/EHsc")
    # Allows the compiler to reorder, combine, or simplify floating-point operations to optimize floating-point code for speed and space.
    # https://docs.microsoft.com/en-us/cpp/build/reference/fp-specify-floating-point-behavior?view=msvc-170#fast
    target_compile_options(compiler_options INTERFACE "/fp:fast")
endif()

# Sanitizers
set(SANITIZERS "")

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    # Address Sanatizer
    # list(APPEND SANITIZERS "address") # TODO (JohT) Enable this again when failing unit tests are fixed 
    # Undefined Sanatizer
    list(APPEND SANITIZERS "undefined")

    # Thread Sanatizer (cannot be used with address sanatizer)
    list(APPEND SANITIZERS "thread")
endif()

# Enable MSVC Address Sanitizer
# if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC") # Using Microsoft Visual Studio C++
#     Activate Address Sanitizer
#     https://docs.microsoft.com/en-us/cpp/sanitizers/asan?view=msvc-170
#     target_compile_options(compiler_options INTERFACE "/fsanitize=address")
#     To overcome "LNK2038: mismatch detected for 'annotate_vector': value '0' doesn't match value '1'"
#     "_DISABLE_VECTOR_ANNOTATION" is activated.
#     https://docs.microsoft.com/en-us/cpp/sanitizers/error-container-overflow?view=msvc-170
#     target_compile_definitions(compiler_options INTERFACE _DISABLE_VECTOR_ANNOTATION)
# endif()

# Detect CPU instruction set extensions for vectorization (AVX, AVX2, AVX512)
# Specifies the architecture for code generation on x64 [AVX|AVX2|AVX512]
# Introduced around 2013, AVX2 should probably be supported by most PCs nowadays
# except for some mini PC CPUs like the Intel Celeron N3450 (Mid 2016, No AVX).
include(findAVX)

# Enable CLang Vectorization
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    if (CXX_AVX512_FOUND)
        target_compile_options(compiler_options INTERFACE "-mavx512f;-mavx512dq;-mavx512vl;-mavx512bw;-mfma")
    elseif (CXX_AVX2_FOUND)
        target_compile_options(compiler_options INTERFACE "-mavx2;-mfma")
    elseif (CXX_AVX_FOUND) 
        target_compile_options(compiler_options INTERFACE "-mavx")
    endif()
endif()

# Enable MSVC Vectorization
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if (CXX_AVX512_FOUND)
        target_compile_options(compiler_options INTERFACE "/arch:AVX512")    
    elseif (CXX_AVX2_FOUND)
        target_compile_options(compiler_options INTERFACE "/arch:AVX2")
    elseif (CXX_AVX_FOUND) 
        target_compile_options(compiler_options INTERFACE "/arch:AVX")
    endif()
endif()

# Enable CLang Vectorization reports
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    target_compile_options(compiler_options INTERFACE "-Rpass=loop-vectorize")
    target_compile_options(compiler_options INTERFACE "-Rpass-missed=loop-vectorize")
    target_compile_options(compiler_options INTERFACE "-Rpass-analysis=loop-vectorize")
    target_compile_options(compiler_options INTERFACE "-fsave-optimization-record")
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Release>:-gline-tables-only>")
    target_compile_options(compiler_options INTERFACE "-gcolumn-info")
endif()

# Enable MSVC Vectorization Reports
if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Report successfully and unsuccessfully vectorized loops
    target_compile_options(compiler_options INTERFACE "/Qvec-report:2")
endif()

# Enable GNU C Vectorization + Reports
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU") # Using GNU C++ Compiler
    # Enable vectorization (also default at optimization parameter -O3)
    target_compile_options(compiler_options INTERFACE "-ftree-vectorize")
    # Enables Vectorization Report Logging
    # Reference: https://www.gnu.org/software/gcc/projects/tree-ssa/vectorization.html#using
    # Level 0: No output at all.
    # Level 1: Report vectorized loops.
    # Level 2: Also report unvectorized "well-formed" loops and respective reason.
    # Level 3: Also report alignment information (for "well-formed" loops).
    # Level 4: Like level 3 + report for non-well-formed inner-loops.
    # Level 5: Like level 3 + report for all loops.
    # Level 6: Print all vectorizer dump information (equivalent to former vect-debug-details).
    target_compile_options(compiler_options INTERFACE "-ftree-vectorizer-verbose=5")
endif()

# The Memory Sanatizer is only supported by Clang for Linux. 
# Since GNU C++ is used for Linux, it will never be used.
#if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#    list(APPEND SANITIZERS "memory")
#endif()

list(JOIN SANITIZERS ";-fsanitize=" LIST_OF_SANITIZERS)
if((LIST_OF_SANITIZERS) AND (NOT "${LIST_OF_SANITIZERS}" STREQUAL ""))
    set(SANITIZERS_COMPILE_OPTIONS "-fsanitize=${LIST_OF_SANITIZERS};-fno-omit-frame-pointer")
    target_compile_options(compiler_options INTERFACE "$<$<CONFIG:Debug>:${SANITIZERS_COMPILE_OPTIONS}>")
    target_link_libraries(compiler_options INTERFACE "$<$<CONFIG:Debug>:${SANITIZERS_COMPILE_OPTIONS}>")
endif()

# Displays the chosen target's compile options:
get_target_property(FINAL_COMPILE_OPTIONS compiler_options INTERFACE_COMPILE_OPTIONS)
message(STATUS "Compiler definitions added to target compiler_options for compiler ${CMAKE_CXX_COMPILER_ID} and build type ${CMAKE_BUILD_TYPE}: ${FINAL_COMPILE_OPTIONS}")
