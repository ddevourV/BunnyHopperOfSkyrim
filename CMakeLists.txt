cmake_minimum_required(VERSION 3.25)
project(BunnyHopperOfSkyrim VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

include(FetchContent)

# rapidcsv (header-only): download only
FetchContent_Declare(
  rapidcsv
  GIT_REPOSITORY https://github.com/d99kris/rapidcsv.git
  GIT_TAG        master
  GIT_SHALLOW    TRUE
)
FetchContent_GetProperties(rapidcsv)
if (NOT rapidcsv_POPULATED)
  FetchContent_Populate(rapidcsv)
endif()
set(RAPIDCSV_INCLUDE_DIRS "${rapidcsv_SOURCE_DIR}/src" CACHE PATH "" FORCE)

# CommonLibSSE-NG
FetchContent_Declare(
  CommonLibSSE
  GIT_REPOSITORY https://github.com/CharmedBaryon/CommonLibSSE-NG.git
  GIT_TAG        main
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(CommonLibSSE)

# sources
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "include/*.h" "include/*.hpp")

add_library(BunnyHopperOfSkyrim SHARED
  ${SOURCES}
  ${HEADERS}
)

target_link_libraries(BunnyHopperOfSkyrim PRIVATE CommonLibSSE::CommonLibSSE)

# ✅ IMPORTANT: headers are in /include
target_include_directories(BunnyHopperOfSkyrim PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

set_target_properties(BunnyHopperOfSkyrim PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out"
)
