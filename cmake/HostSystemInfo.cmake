# Creates the file "HostSystemInfo.env" in the binary directory (./build) 
# that contains environment variables for the host system. 
# The default environment variable prefix ist "BUILD_HOST".
# 
# - BUILD_HOST_OPERATING_SYSTEM: the name of the operating system of the host system (e.g. "macOS")
# - BUILD_HOST_PROCESSOR: the name of the processor of the host system (e.g. "AppleM1")
# - BUILD_HOST_PROCESSOR_ARCHITECTURE: the name of the processor architecture of the host system (e.g. "arm64")
# - BUILD_HOST_COMPILER: the name of the compiler currently used on the host system (e.g. "AppleClang")
# - BUILD_HOST_VECTOR_EXTENSION: the name of the vector extension used on the host system (e.g. "")

set(BUILD_HOST_OUTPUT_FILE_NAME "${CMAKE_BINARY_DIR}/HostSystemInfo.env")
set(BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX "BUILD_HOST")

# Add environment variable BUILD_OPERATING_SYSTEM (e.g. "macOS")
cmake_host_system_information(RESULT HOST_OPERATING_SYSTEM QUERY OS_NAME)
string(REPLACE " " "" HOST_OPERATING_SYSTEM "${HOST_OPERATING_SYSTEM}")
message(STATUS "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_OPERATING_SYSTEM=${HOST_OPERATING_SYSTEM}")
file(WRITE "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_OPERATING_SYSTEM=${HOST_OPERATING_SYSTEM}\n")

# Add environment variable BUILD_HOST_PROCESSOR (e.g. "AppleM1")
cmake_host_system_information(RESULT HOST_PROCESSOR QUERY PROCESSOR_NAME)
string(REPLACE " " "" HOST_PROCESSOR "${HOST_PROCESSOR}")
message(STATUS "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_PROCESSOR=${HOST_PROCESSOR}")
file(APPEND "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_PROCESSOR=${HOST_PROCESSOR}\n")

# Add environment variable BUILD_HOST_PROCESSOR_DESCRIPTION (e.g. "arm64")
cmake_host_system_information(RESULT HOST_PROCESSOR_DESCRIPTION QUERY PROCESSOR_DESCRIPTION)
string(REPLACE " " "" HOST_PROCESSOR_DESCRIPTION "${HOST_PROCESSOR_DESCRIPTION}")
message(STATUS "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_PROCESSOR_DESCRIPTION=${HOST_PROCESSOR_DESCRIPTION}")
file(APPEND "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_PROCESSOR_DESCRIPTION=${HOST_PROCESSOR_DESCRIPTION}\n")

# Add environment variable BUILD_HOST_PROCESSOR_ARCHITECTURE (e.g. "arm64")
string(REPLACE " " "" HOST_PROCESSOR_ARCHITECTURE "${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_PROCESSOR_ARCHITECTURE=${HOST_PROCESSOR_ARCHITECTURE}")
file(APPEND "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_PROCESSOR_ARCHITECTURE=${HOST_PROCESSOR_ARCHITECTURE}\n")

# Add environment variable BUILD_HOST_COMPILER (e.g. "AppleClang")
string(REPLACE " " "" HOST_COMPILER "${CMAKE_CXX_COMPILER_ID}")
message(STATUS "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_COMPILER=${HOST_COMPILER}")
file(APPEND "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_COMPILER=${HOST_COMPILER}\n")

# Detect vector extension of the host system ---------------------------------------------------
set(HOST_VECTOR_EXTENSION " ")

# Detect SSE vector extension of the host system
cmake_host_system_information(RESULT HOST_HAS_SSE QUERY HAS_SSE)
if(HOST_HAS_SSE)
  set(HOST_VECTOR_EXTENSION "SSE")
  message(VERBOSE "HOST_HAS_SSE=${HOST_HAS_SSE}")
endif()

# Detect SSE2 vector extension of the host system
cmake_host_system_information(RESULT HOST_HAS_SSE2 QUERY HAS_SSE2)
if(HOST_HAS_SSE2)
  set(HOST_VECTOR_EXTENSION "SSE2")
  message(VERBOSE "SSE2 supported")
endif()

# Load CMake module (./cmake/findAVX.cmake) to detects CPU instruction set extensions for vectorization (AVX, AVX2, AVX512)
# Reference: https://gist.github.com/UnaNancyOwen/263c243ae1e05a2f9d0e
include(findAVX)

# Detect AVX vector extension of the host system
if (CXX_AVX_FOUND)
  set(HOST_VECTOR_EXTENSION "AVX")
  message(VERBOSE "AXV supported with flags: ${CXX_AVX_FLAGS}")
endif()

# Detect AVX2 vector extension of the host system
if (CXX_AVX2_FOUND)
  set(HOST_VECTOR_EXTENSION "AVX2")
  message(VERBOSE "AXV2 supported with flags: ${CXX_AVX2_FLAGS}")
endif()

# Detect AVX512 vector extension of the host system
if(CXX_AVX512_FOUND)
 set(HOST_VECTOR_EXTENSION "AVX512")
 message(VERBOSE "AXV512 supported with flags: ${CXX_AVX512_FLAGS}")
endif()

# Write vector extension environment variable to the output file
message(STATUS "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_VECTOR_EXTENSION=${HOST_VECTOR_EXTENSION}")
file(APPEND "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BUILD_HOST_ENVIRONMENT_VARIABLE_PREFIX}_VECTOR_EXTENSION=${HOST_VECTOR_EXTENSION}\n")
# ------------------------------------------------------------------------------------------------