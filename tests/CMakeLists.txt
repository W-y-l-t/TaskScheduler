include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        MyScheduler_tests
        MyScheduler_tests.cpp
        ../lib/utils/MyExceptions.cpp
        ../lib/utils/MyExceptions.hpp
        ../lib/utils/MyTuple.hpp
        ../lib/TaskHelper.hpp
        ../lib/Auxillary.hpp
)

target_link_libraries(
        MyScheduler_tests
        MyScheduler
        GTest::gtest_main
)

target_include_directories(MyScheduler_tests PUBLIC ${PROJECT_SOURCE_DIR})

include(GoogleTest)

gtest_discover_tests(MyScheduler_tests)