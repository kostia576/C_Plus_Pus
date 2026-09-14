cmake_minimum_required(VERSION 3.14)  

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION_STR "0.0.${PATCH_VERSION}")  

project(helloworld VERSION 0.0.${PATCH_VERSION} LANGUAGES CXX)

# ==========================================================
# Опции сборки
# ==========================================================
option(WITH_BOOST_TEST "Build Boost.Test tests" ON)
option(WITH_GTEST "Build GoogleTest tests" OFF) 

# ==========================================================
# Основная программа
# ==========================================================
add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp ip_utils.cpp)  

set_target_properties(helloworld_cli helloworld PROPERTIES
    CXX_STANDARD 17  # ← Обновил до 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(helloworld_cli PRIVATE helloworld)

configure_file(version.h.in version.h)

# ==========================================================
# Boost.Test (опционально)
# ==========================================================
if(WITH_BOOST_TEST)
   
    if(POLICY CMP0167)
        cmake_policy(SET CMP0167 NEW)
    endif()
    
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    
    add_executable(test_version test_version.cpp)
    
    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_compile_definitions(test_version PRIVATE BOOST_TEST_DYN_LINK)
    target_include_directories(test_version PRIVATE ${Boost_INCLUDE_DIR})
    target_link_libraries(test_version PRIVATE ${Boost_LIBRARIES} helloworld)
    

    enable_testing()
    add_test(NAME test_version COMMAND test_version)
endif()

# ==========================================================
# GoogleTest (опционально)
# ==========================================================
if(WITH_GTEST)
   
    find_package(GTest CONFIG REQUIRED)
    
    add_executable(ip_utils_tests 
        tests/ip_utils_tests.cpp
    )
    
    set_target_properties(ip_utils_tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )
    
    target_link_libraries(ip_utils_tests PRIVATE 
        GTest::gtest 
        GTest::gtest_main 
        helloworld  
    )
    
    
    enable_testing()
    include(GoogleTest)
    gtest_discover_tests(ip_utils_tests)
endif()

# ==========================================================
# Компиляторные предупреждения
# ==========================================================
if(MSVC)
    target_compile_options(helloworld_cli PRIVATE /W4)
    target_compile_options(helloworld PRIVATE /W4)
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE /W4)
    endif()
    if(WITH_GTEST)
        target_compile_options(ip_utils_tests PRIVATE /W4)
    endif()
else()
    target_compile_options(helloworld_cli PRIVATE -Wall -Wextra -pedantic -Werror)
    target_compile_options(helloworld PRIVATE -Wall -Wextra -pedantic -Werror)
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
    if(WITH_GTEST)
        target_compile_options(ip_utils_tests PRIVATE -Wall -Wextra -pedantic -Werror)
    endif()
endif()

# ==========================================================
# Установка и пакетирование
# ==========================================================
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT example@example.com)
include(CPack)