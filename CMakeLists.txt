###################
###### CMake ######
###################
cmake_minimum_required(VERSION 3.26)
message("CMAKE_VERSION = ${CMAKE_VERSION}")

file(GLOB MODULE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/cmake/*/" LIST_DIRECTORIES true) # "${PROJECT_SOURCE_DIR}/cmake"
list(APPEND CMAKE_MODULE_PATH "${MODULE_DIRS}")

#####################
###### VERSION ######
#####################
include(git_version_detect)

####################
###### Project #####
####################
project(ipfilter 
    VERSION ${GITVERSIONDETECT_VERSION_MAJOR}.${GITVERSIONDETECT_VERSION_MINOR}.${GITVERSIONDETECT_VERSION_PATCH}
    DESCRIPTION "otus course cpp developer - hw2"
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message("CMAKE_C_COMPILER: ${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER: ${CMAKE_CXX_COMPILER}")
message("CMAKE_GENERATOR: ${CMAKE_GENERATOR}")

##########################
###### Dependencies ######
##########################

## BOOST
set(BOOST_ENABLE_CMAKE ON)
set(Boost_FIND_QUIETLY OFF)
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
# include(fetch_boost) # https://github.com/boostorg/program_options.git
# set(CMAKE_FIND_DEBUG_MODE ON)
# find_package(Boost 1.82 REQUIRED COMPONENTS program_options)
find_package(Boost REQUIRED COMPONENTS program_options)
# set(CMAKE_FIND_DEBUG_MODE OFF)
message("Boost_LIBRARY_DIRS: ${Boost_LIBRARY_DIRS}")
message("Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")

include_directories(${Boost_INCLUDE_DIRS})
# link_directories(${Boost_LIBRARY_DIR})

# (file)Boost.ProgramOptions + (compiler opton)libboost_program_options

## GTEST
# include(fetch_gtest)
find_package(GTest 1.13 REQUIRED)

## FMT
# include(fetch_fmt) - replaced by cxx23 standard

## RANGE
# include(fetch_rangev3) - replaced by cxx23 standard

##############################
######  Compile settings #####
##############################
# enable all warnings and treat them as errors
add_compile_options(-Wall -Wextra -Wpedantic -Werror)


#######################
###### Clang-tidy #####
#######################
# # if clang-tidy is not already set, it is searched for here
# # If using a CMake version above 3.19 it is recommended to configure clang-tidy in a preset
# if(ENABLE_CLANG_TIDY)
#     if(NOT CMAKE_CXX_CLANG_TIDY)
#         find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
#         if(CLANG_TIDY_EXECUTABLE)
#             list(APPEND CLANG_TIDY_COMMAND "${CLANG_TIDY_EXECUTABLE}"
#     "-checks=-*,readability-*,modernize-use-nullptr,-extra-arg=-std=c++2b"
#             )
#         endif()
#     else()
#         message(WARNING "Failed to find CLANG_TIDY.")        
#     endif()
# endif()

############################
###### Copy RESOURCES ######
############################
file(CREATE_LINK ${CMAKE_CURRENT_LIST_DIR}/tests/ip_filter.tsv ${CMAKE_CURRENT_BINARY_DIR}/src/ip/tests/ip_filter.tsv COPY_ON_ERROR SYMBOLIC)

###########################
###### code_coverage ######
###########################
if(ENABLE_COVERAGE) # todo: maybe combined with BUILD_TESTING, DEBUG ...
    include(code_coverage)
endif()

######################################
###### Enable tests at top level #####
######################################
if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    include(CTest)
    enable_testing() # todo: check whether include(CTest) does the same ... 
endif()

######################
###### Libraries #####
######################
add_subdirectory(src/info) # static
add_subdirectory(src/ip) # shared + header_only (just as a workout - no real need... yet))

########################
###### Application #####
########################
add_executable(ip_filter)
target_sources(
    ip_filter
    PRIVATE 
        "src/main.cpp"
)

target_compile_features(
    ip_filter
    PRIVATE
        cxx_std_23 # to be sure
)

target_link_libraries(
    ip_filter
    # PRIVATE
        Boost::program_options
        info_static
        ip_shared
)

target_include_directories(
    ip_filter
    PRIVATE src src/info/include src/ip/include
)

# if (ENABLE_CLANG_TIDY)
#     set_target_properties(
#         ip_filter
#         PROPERTIES
#             CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
#     )    
# endif()

# add_custom_target(
#     create_hash_target ALL
#     COMMAND cmake -P create_md5sum 
#         $<TARGET_FILE:tests/ip_filter.tsv>
#         ${CMAKE_CURRENT_BINARY_DIR}/ip_filter_result.md5
# )

# add_custom_command(TARGET ip_filter
#     POST_BUILD
#         COMMAND cmake -P create_md5sum 
#         $<TARGET_FILE:tests/ip_filter.tsv>
#         ${CMAKE_CURRENT_BINARY_DIR}/ip_filter_result.md5
# )

# include(GNUInstallDirs)
# message("CMAKE_INSTALL_LOCAL_ONLY = ${CMAKE_INSTALL_LOCAL_ONLY}")
# install(TARGETS ip_filter ip_shared 
#     RUNTIME DESTINATION "${CMAKE_CURRENT_LIST_DIR}/bin"
#     LIBRARY DESTINATION "${CMAKE_CURRENT_LIST_DIR}/lib"
# )
install(TARGETS ip_shared ip_filter) # DESTINATION "${CMAKE_CURRENT_LIST_DIR}/bin")
# install(TARGETS ip_filter RUNTIME_DEPENDENCIES DESTINATION "${CMAKE_CURRENT_LIST_DIR}/bin")