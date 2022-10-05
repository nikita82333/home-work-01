cmake_minimum_required(VERSION 3.2)
 
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})
 
project(helloworld VERSION ${PROJECT_VESRION})
 
option(BOOST_TEST_ENABLE "Enable boost tests" ON)
option(GOOGLE_TEST_ENABLE "Enable google tests" ON)
 
configure_file(version.h.in version.h)
 
add_executable(helloworld_cli main.cpp)
add_library(helloworld lib.cpp)
if(BOOST_TEST_ENABLE)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version test_version.cpp)
endif()
 
if(GOOGLE_TEST_ENABLE)
    add_subdirectory(googletest)
    add_executable(g_test_version g_test_version.cpp lib.cpp)
    target_link_libraries(g_test_version gtest_main)
    set_target_properties(g_test_version PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED ON
    )
endif()
 
set_target_properties(helloworld_cli helloworld PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(helloworld
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_link_libraries(helloworld_cli PRIVATE
    helloworld
)
if(BOOST_TEST_ENABLE)
    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )
    set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )
    target_link_libraries(test_version
        ${Boost_LIBRARIES}
        helloworld
    )
endif()
 
if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
    target_compile_options(helloworld PRIVATE
        /W4
    )
    if(BOOST_TEST_ENABLE)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
    if(GOOGLE_TEST_ENABLE)
        target_compile_options(g_test_version PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(BOOST_TEST_ENABLE)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
    if(GOOGLE_TEST_ENABLE)
        target_compile_options(g_test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()
 
 
install(TARGETS helloworld_cli RUNTIME DESTINATION bin)
 
set(CPACK_GENERATOR DEB)
 
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
 
set(CPACK_PACKAGE_CONTACT example@example.com)
 
include(CPack)
 
if(BOOST_TEST_ENABLE)
    enable_testing()
    add_test(test_version test_version)
endif()
 
if(GOOGLE_TEST_ENABLE)
    enable_testing()
    add_test(g_test_version g_test_version)
endif()