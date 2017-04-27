cmake_minimum_required(VERSION 3.5)

project (ci40_project5)

set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)
set (VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
set (PROJECT_VERSION ${VERSION})

include_directories(include)
include_directories(include/awa)

file (GLOB src_awa src/awa/*.c*)
file (GLOB src src/*.c*)

file (GLOB hdrs_awa include/awa/*.h*)
file (GLOB hdrs include/*.c*)

#add_definitions(
#  -std=c++11 # Or -std=c++0x
#    # Other flags
#)

add_compile_options(-std=c++11)

add_executable(ci40_project5_appd ${src_awa} ${src})

# Add library targets
#####################
find_library(LIB_AWA_STATIC libawa.so ${STAGING_DIR}/usr/lib)
find_library(LIB_CONFIG libconfig.so ${STAGING_DIR}/usr/lib)
find_library(LIB_LETMECREATECORE libletmecreate_core.so ${STAGING_DIR}/usr/lib)
find_library(LIB_LETMECREATECLICK libletmecreate_click.so ${STAGING_DIR}/usr/lib)
find_library(LIB_LETMECREATE_BOSCH libletmecreate_bosch.so ${STAGING_DIR}/usr/lib)
find_library(LIB_LETMECREATE_RPI libletmecreate_rpisensehat.so ${STAGING_DIR}/usr/lib)
find_library(LIB_SQLITE3 libsqlite3.so ${STAGING_DIR}/usr/lib)
find_library(LIB_STDCPP libstdc++.so ${STAGING_DIR}/toolchain-mipsel_mips32r2_gcc-5.3.0_musl-1.1.15/lib)

target_link_libraries(ci40_project5_appd ${LIB_CONFIG} ${LIB_AWA_STATIC} ${LIB_LETMECREATECORE} ${LIB_LETMECREATECLICK} ${LIB_LETMECREATE_BOSCH} ${LIB_LETMECREATE_RPI} ${LIB_SQLITE3} ${LIB_STDCPP})

# Add install targets
######################
install(TARGETS ci40_project5_appd RUNTIME DESTINATION bin)
