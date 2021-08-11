cmake_minimum_required (VERSION 3.8)

project ("MCGeForce")

add_library (MCGeForce SHARED "MCGeForce.cpp" "MCGeForce.h")

target_include_directories(MCGeForce PUBLIC "include" "include/gfesdk" "$ENV{JAVA_HOME}/../include" "$ENV{JAVA_HOME}/../include/win32")
target_link_libraries(MCGeForce PUBLIC "${PROJECT_SOURCE_DIR}/lib/win64/GfeSDK.lib")