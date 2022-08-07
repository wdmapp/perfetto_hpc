
cmake_minimum_required(VERSION 3.17)

project(perfetto_hpc LANGUAGES C CXX)

include(cmake/CPM.cmake)

# download perfetto and provide it as perfetto::perfetto target
CPMAddPackage("gh:google/perfetto@28.0")
find_package(Threads REQUIRED)
add_library(perfetto STATIC ${perfetto_SOURCE_DIR}/sdk/perfetto.cc)
target_include_directories(perfetto PUBLIC ${perfetto_SOURCE_DIR}/sdk)
target_link_libraries(perfetto PUBLIC Threads::Threads)
target_compile_features(perfetto PUBLIC cxx_std_14)
add_library(perfetto::perfetto ALIAS perfetto)

find_package(MPI REQUIRED)
CPMAddPackage("gh:LLNL/wrap#master")
set(WRAP "${wrap_SOURCE_DIR}/wrap.py")
include("${wrap_SOURCE_DIR}/WrapConfig.cmake")

add_subdirectory(perfetto_hpc)
