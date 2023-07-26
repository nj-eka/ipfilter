
find_package(GTest REQUIRED) # 1.13.0 EXACT   COMPONENTS gtest gtest_main  gmock 
if(NOT GTest_FOUND)
  message(FATAL_ERROR "Couldn't find gtest")
endif()
# enable_testing() - done at upper level
include(GoogleTest) # to use gtest_discover_tests ...


# include(FindOpenSSL)
# message(STATUS "OPENSSL_FOUND = ${OPENSSL_FOUND}")
# message(STATUS "OPENSSL_INCLUDE_DIR = ${OPENSSL_INCLUDE_DIR}")
# message(STATUS "OPENSSL_CRYPTO_LIBRARY = ${OPENSSL_CRYPTO_LIBRARY}")
# message(STATUS "OPENSSL_CRYPTO_LIBRARIES = ${OPENSSL_CRYPTO_LIBRARIES}")
# message(STATUS "OPENSSL_SSL_LIBRARY = ${OPENSSL_SSL_LIBRARY}")
# message(STATUS "OPENSSL_SSL_LIBRARIES = ${OPENSSL_SSL_LIBRARIES}")
# message(STATUS "OPENSSL_LIBRARIES = ${OPENSSL_LIBRARIES}")
# message(STATUS "OPENSSL_VERSION = ${OPENSSL_VERSION}")
# message(STATUS "OPENSSL_APPLINK_SOURCE = ${OPENSSL_APPLINK_SOURCE}")

# find_package(OpenSSL REQUIRED)
## include_directories(${OPENSSL_INCLUDE_DIRS})
## link_directories(${OPENSSL_LIBRARIES})


add_library(md5 INTERFACE)
target_include_directories(md5 INTERFACE include)
target_compile_features(md5 INTERFACE cxx_std_23)

add_executable(
    ip_unit_tests
        ipv4_address.gtest.cpp
        ipv4_filter.gtest.cpp
        hw2.gtest.cpp
)

set_target_properties(
    ip_unit_tests
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)

target_link_libraries(
    ip_unit_tests
    PRIVATE
        ${PROJECT_NAME} 
        GTest::gtest_main 
        # OpenSSL::Crypto
        md5
)

target_include_directories(
    ip_unit_tests
    PRIVATE
        ${GTEST_INCLUDE_DIRS}
        # ${OPENSSL_INCLUDE_DIR} # PUBLIC
        # "src"
        "include" 
)

target_compile_features(
    ip_unit_tests
    PUBLIC cxx_std_23
)

add_test(ip_unit_tests  ip_unit_tests WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/data")


if (MSVC)
    target_compile_options(
        ip_unit_tests
        PRIVATE  
            /W4
    )
else()
    target_compile_options(
        ip_unit_tests
        PRIVATE
            -Wall -Wextra -pedantic -Werror
    )
endif(MSVC)

##### Coverage ######
if(ENABLE_COVERAGE)
    append_coverage_compiler_flags()
    setup_target_for_coverage_gcovr_html(
        NAME coverage 
        EXECUTABLE ctest --test-dir ${CMAKE_CURRENT_BINARY_DIR} -T Test -T Coverage
        EXCLUDE "/usr/*" 
        DEPENDENCIES ip_shared ip_unit_tests
    )    
    # setup_target_for_coverage_lcov(
    #     NAME coverage
    #     EXECUTABLE ctest --test-dir ${CMAKE_CURRENT_BINARY_DIR} -T Test -T Coverage
    #     EXCLUDE "/usr/*"
    #     DEPENDENCIES ip_shared ip_unit_tests
    # )    
endif()

gtest_discover_tests(ip_unit_tests)
