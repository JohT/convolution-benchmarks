# Creates the file "BenchmarkChart.env" in the binary directory (./build) 
# that contains environment variables for the benchmark chart creation. 
# The default environment variable prefix ist "BENCHMARK_CHART".
# 
# - BENCHMARK_CHART_GROUP: Name of the final folder that will contain the chart and its data grouped together (e.g. "AppleClang-macOS-arm64")
# - BENCHMARK_CHART_DATA_FILE: Benchmark results XML filename with path (e.g. "./build/benchmark/benchmark-results.xml")

# Options to overwrite the used CPU vectorization extension of the build host system.
option(ENFORCE_VECTOR_EXTENSION_SSE2 "Enforces SSE2 vector extension if available (Default=OFF)")
if (ENFORCE_VECTOR_EXTENSION_SSE2)
  message(VERBOSE "Enforcing SSE2 vector extension if available")
endif()

option(ENFORCE_VECTOR_EXTENSION_AVX "Enforces AVX vector extension if available (Default=OFF)")
if (ENFORCE_VECTOR_EXTENSION_AVX)
  message(VERBOSE "Enforcing AVX vector extension if available")
endif()

option(ENFORCE_VECTOR_EXTENSION_AVX2 "Enforces AVX2 vector extension if available (Default=OFF)")
if (ENFORCE_VECTOR_EXTENSION_AVX2)
  message(VERBOSE "Enforcing AVX2 vector extension if available")
endif()

set(BUILD_HOST_OUTPUT_FILE_NAME "${CMAKE_BINARY_DIR}/BenchmarkChart.env")
set(BENCHMARK_CHART_VARIABLENAME_PREFIX "BENCHMARK_CHART")

# Write benchmark (XML) data filename environment variable to the output file
file(TO_NATIVE_PATH "${CMAKE_BINARY_DIR}/benchmark/benchmark-report.xml" BENCHMARK_CHART_DATA_FILE)
message(VERBOSE "${BENCHMARK_CHART_VARIABLENAME_PREFIX}_DATA_FILE=${BENCHMARK_CHART_DATA_FILE}")
file(WRITE "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BENCHMARK_CHART_VARIABLENAME_PREFIX}_DATA_FILE=${BENCHMARK_CHART_DATA_FILE}\n")

# Detect HOST_OPERATING_SYSTEM (e.g. "macOS")
cmake_host_system_information(RESULT HOST_OPERATING_SYSTEM QUERY OS_NAME)
string(REPLACE " " "" HOST_OPERATING_SYSTEM "${HOST_OPERATING_SYSTEM}")
message(VERBOSE "HOST_OPERATING_SYSTEM=${HOST_OPERATING_SYSTEM}")

# Detect HOST_PROCESSOR (e.g. "AppleM1")
cmake_host_system_information(RESULT HOST_PROCESSOR QUERY PROCESSOR_NAME)
string(REPLACE " " "" HOST_PROCESSOR "${HOST_PROCESSOR}")
message(VERBOSE "HOST_PROCESSOR=${HOST_PROCESSOR}")

# Detect HOST_PROCESSOR_DESCRIPTION (e.g. "arm64")
cmake_host_system_information(RESULT HOST_PROCESSOR_DESCRIPTION QUERY PROCESSOR_DESCRIPTION)
string(REPLACE " " "" HOST_PROCESSOR_DESCRIPTION "${HOST_PROCESSOR_DESCRIPTION}")
message(VERBOSE "HOST_PROCESSOR_DESCRIPTION=${HOST_PROCESSOR_DESCRIPTION}")

# Detect HOST_PROCESSOR_ARCHITECTURE (e.g. "arm64")
string(REPLACE " " "" HOST_PROCESSOR_ARCHITECTURE "${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(VERBOSE "HOST_PROCESSOR_ARCHITECTURE=${HOST_PROCESSOR_ARCHITECTURE}")

# Detect HOST_COMPILER (e.g. "AppleClang")
string(REPLACE " " "" HOST_COMPILER "${CMAKE_CXX_COMPILER_ID}")
message(VERBOSE "HOST_COMPILER=${HOST_COMPILER}")

# Detect vector extension of the host system ---------------------------------------------------
set(HOST_VECTOR_EXTENSION " ")

# Detect SSE vector extension of the host system
cmake_host_system_information(RESULT HOST_HAS_SSE QUERY HAS_SSE)
if (HOST_HAS_SSE)
  set(HOST_VECTOR_EXTENSION "SSE")
  message(VERBOSE "HOST_HAS_SSE=${HOST_HAS_SSE}")
endif()

# Detect SSE2 vector extension of the host system
cmake_host_system_information(RESULT HOST_HAS_SSE2 QUERY HAS_SSE2)
if (HOST_HAS_SSE2)
  set(HOST_VECTOR_EXTENSION "SSE2")
  message(VERBOSE "SSE2 supported")
endif()

# Load CMake module (./cmake/findAVX.cmake) to detects CPU instruction set extensions for vectorization (AVX, AVX2, AVX512)
# Reference: https://gist.github.com/UnaNancyOwen/263c243ae1e05a2f9d0e
include(findAVX)

# Detect AVX vector extension of the host system
if (CXX_AVX_FOUND AND NOT ENFORCE_VECTOR_EXTENSION_SSE2)
  set(HOST_VECTOR_EXTENSION "AVX")
  message(VERBOSE "AXV supported with flags: ${CXX_AVX_FLAGS}")
endif()

# Detect AVX2 vector extension of the host system
if (CXX_AVX2_FOUND AND NOT ENFORCE_VECTOR_EXTENSION_SSE2 AND NOT ENFORCE_VECTOR_EXTENSION_AVX)
  set(HOST_VECTOR_EXTENSION "AVX2")
  message(VERBOSE "AXV2 supported with flags: ${CXX_AVX2_FLAGS}")
endif()

# Detect AVX512 vector extension of the host system
if (CXX_AVX512_FOUND AND NOT ENFORCE_VECTOR_EXTENSION_SSE2 AND NOT ENFORCE_VECTOR_EXTENSION_AVX AND NOT ENFORCE_VECTOR_EXTENSION_AVX2)
  set(HOST_VECTOR_EXTENSION "AVX512")
  message(VERBOSE "AXV512 supported with flags: ${CXX_AVX512_FLAGS}")
endif()
# ------------------------------------------------------------------------------------------------

# Write benchmark chart directory environment variable to the output file
set(BENCHMARK_CHART_GROUP "${HOST_COMPILER}-${HOST_OPERATING_SYSTEM}-${HOST_PROCESSOR_ARCHITECTURE}${HOST_VECTOR_EXTENSION}")
message(VERBOSE "${BENCHMARK_CHART_VARIABLENAME_PREFIX}_GROUP=${BENCHMARK_CHART_GROUP}")
file(APPEND "${BUILD_HOST_OUTPUT_FILE_NAME}" "${BENCHMARK_CHART_VARIABLENAME_PREFIX}_GROUP=${BENCHMARK_CHART_GROUP}\n")