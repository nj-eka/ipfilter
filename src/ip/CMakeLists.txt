cmake_minimum_required(VERSION 3.26)

project(
    ip_shared
    VERSION 
        ${PROJECT_VERSION}
    DESCRIPTION
        "A simple C++ project to demonstrate creating shared lib in CMake"
    LANGUAGES CXX
)

# set the postfix "d" for the resulting .so or .dll files when building the library in debug mode
set(CMAKE_DEBUG_POSTFIX d)

add_library(ip_shared SHARED)

set_target_properties(ip_shared PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    VERSION ${PROJECT_VERSION} # set properties for the target. VERSION set the library version to the project version
)

target_sources(ip_shared
    PRIVATE
        "src/ipv4_address.cpp"
    PUBLIC
        "include/ip/ipv4_address.hpp"    
        "include/ip/ipv4_filter.hpp"
)

# define the C++ standard needed to compile this library and make it visible to dependers
target_compile_features(
    ip_shared
    PUBLIC cxx_std_23
)

# set the include directories
target_include_directories(
    ip_shared
    PUBLIC include
)

##### SHARED #####
# if using limited visibility, set CXX_VISIBILILTY_PRESET to "hidden"
include(GenerateExportHeader)
set_property(
    TARGET ip_shared
    PROPERTY CXX_VISIBILITY_PRESET "hidden"
)
# Hide inlined functions by default, reducing the size of the library
set_property(
    TARGET ip_shared
    PROPERTY VISIBILITY_INLINES_HIDDEN TRUE
)
# this command generates a header file in the CMAKE_CURRENT_BINARY_DIR which
# sets the visibility attributes according to the compiler settings
generate_export_header(
    ip_shared
    EXPORT_FILE_NAME
    export/ip/export_ipv4.hpp
)
# Add CMAKE_CURRENT_BINARY_DIR to the include path so the generated header can
# be found
target_include_directories(
    ip_shared
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}/export"
)

# An alternative but not recomended way export the symbols is to use
# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE) Which exports all dll symbols by
# default

##### TESTS #######
if(BUILD_TESTING)
    # if(PROJECT_IS_TOP_LEVEL)
    #     include(CTest)
    # endif()
    if(ENABLE_COVERAGE)
        # include(code_coverage)
        append_coverage_compiler_flags()
        # append_coverage_compiler_flags_to_target(ip_shared)
    endif()    
    add_subdirectory(tests)
endif(BUILD_TESTING)