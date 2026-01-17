cmake_minimum_required (VERSION 3.21)
project (Proto2-Project-01 LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

find_package(Protobuf CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/proto/main_proto.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

add_library(proto2_project_01_pmessages ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(proto2_project_01_pmessages PUBLIC protobuf::libprotobuf)
target_include_directories(proto2_project_01_pmessages PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
target_compile_features(proto2_project_01_pmessages PUBLIC cxx_std_17)

add_library(server
    Proto2-Project-01/Server/server.cpp
    Proto2-Project-01/Server/server.h
)
target_include_directories(server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Proto2-Project-01/Server
)
target_link_libraries(server PUBLIC proto2_project_01_pmessages)
target_compile_features(server PUBLIC cxx_std_17)

add_executable(proto2_tests
    tests/client_tests.cpp
    tests/server_tests.cpp
)

add_dependencies(proto2_tests proto2_project_01_pmessages)

target_link_libraries(proto2_tests PRIVATE server GTest::gtest GTest::gtest_main)

# --- Client app
add_executable(client_app
    Proto2-Project-01/Client/client.cpp
)
target_link_libraries(client_app PRIVATE proto2_project_01_pmessages)
add_dependencies(client_app proto2_project_01_pmessages)

# --- Server app wrapper (reads request.bin, writes response.bin)   
add_executable(server_app
    Proto2-Project-01/Server/server_app.cpp
)
target_link_libraries(server_app PRIVATE server)
add_dependencies(server_app proto2_project_01_pmessages)


include(GoogleTest)
gtest_discover_tests(proto2_tests DISCOVERY_MODE PRE_TEST DISCOVERY_TIMEOUT 30)

#add_test(NAME proto2_tests COMMAND proto2_tests)



