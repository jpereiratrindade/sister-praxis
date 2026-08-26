cmake_minimum_required(VERSION 3.25)
project(sister_praxis VERSION 0.2.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SISTER_PRAXIS_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

function(sister_praxis_strict target)
  target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
  if(SISTER_PRAXIS_WARNINGS_AS_ERRORS)
    target_compile_options(${target} PRIVATE -Werror)
  endif()
endfunction()

add_library(sister_praxis_core
  src/domain.cpp
  src/audit.cpp
)
target_include_directories(sister_praxis_core PUBLIC include)
sister_praxis_strict(sister_praxis_core)

add_library(sister_praxis_runtime
  src/runtime.cpp
)
target_include_directories(sister_praxis_runtime PUBLIC include)
target_link_libraries(sister_praxis_runtime PUBLIC sister_praxis_core)
sister_praxis_strict(sister_praxis_runtime)

add_library(sister_praxis_http
  src/http.cpp
)
target_include_directories(sister_praxis_http PUBLIC include)
target_link_libraries(sister_praxis_http PUBLIC sister_praxis_runtime)
sister_praxis_strict(sister_praxis_http)

add_executable(sister-praxis-cli src/main.cpp)
target_link_libraries(sister-praxis-cli PRIVATE sister_praxis_runtime)
sister_praxis_strict(sister-praxis-cli)

add_executable(sister-praxis-http src/http_main.cpp)
target_link_libraries(sister-praxis-http PRIVATE sister_praxis_http)
sister_praxis_strict(sister-praxis-http)

enable_testing()

add_executable(sister_praxis_core_test tests/core_test.cpp)
target_link_libraries(sister_praxis_core_test PRIVATE sister_praxis_core)
sister_praxis_strict(sister_praxis_core_test)
add_test(NAME sister_praxis_core_test COMMAND sister_praxis_core_test)

add_executable(sister_praxis_runtime_test tests/runtime_test.cpp)
target_link_libraries(sister_praxis_runtime_test PRIVATE sister_praxis_runtime)
sister_praxis_strict(sister_praxis_runtime_test)
add_test(NAME sister_praxis_runtime_test COMMAND sister_praxis_runtime_test)

add_executable(sister_praxis_http_test tests/http_test.cpp)
target_link_libraries(sister_praxis_http_test PRIVATE sister_praxis_http)
sister_praxis_strict(sister_praxis_http_test)
add_test(NAME sister_praxis_http_test COMMAND sister_praxis_http_test)

find_package(Python3 COMPONENTS Interpreter REQUIRED)
add_test(
  NAME sister_praxis_runtime_entrypoint_test
  COMMAND ${Python3_EXECUTABLE}
          ${CMAKE_SOURCE_DIR}/tests/component_runtime_contract_test.py
)
