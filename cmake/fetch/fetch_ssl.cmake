# cmake_minimum_required(VERSION 3.8)
# project(test-app)

# include(FetchContent)

# set(OPENSSL_FETCH_INFO
#         URL https://www.openssl.org/source/openssl-1.1.1h.tar.gz
#         URL_HASH SHA256=5c9ca8774bd7b03e5784f26ae9e9e6d749c9da2438545077e6b3d755a06595d9
#         )

# #set(OPENSSL_USE_STATIC_LIBS ON)
# #set(OPENSSL_USE_STATIC_LIBS OFF)

# FetchContent_Declare(
#         openssl
#         GIT_REPOSITORY https://github.com/jc-lab/openssl-cmake.git
#         GIT_TAG        39af37e0964d71c516da5b1836849dd0a03df7a4 # Change to the latest commit ID
# )
# FetchContent_GetProperties(openssl)
# if (NOT openssl_POPULATED)
#     FetchContent_Populate(openssl)
#     add_subdirectory(${openssl_SOURCE_DIR} ${openssl_BINARY_DIR})
# endif ()

# add_executable(test-app main.cc)
# target_link_libraries(test-app
#     PRIVATE
#     OpenSSL::Crypto
#     OpenSSL::SSL
# )