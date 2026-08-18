cmake_minimum_required(VERSION 3.25)
project(sister_praxis VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SISTER_PRAXIS_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

add_library(sister_praxis_core
  src/domain.cpp
  src/audit.cpp
)
target_include_directories(sister_praxis_core PUBLIC include)

target_compile_options(sister_praxis_core PRIVATE
  -Wall -Wextra -Wpedantic -Wconversion -Wshadow
)
if(SISTER_PRAXIS_WARNINGS_AS_ERRORS)
  target_compile_options(sister_praxis_core PRIVATE -Werror)
endif()

add_executable(sister-praxis-cli src/main.cpp)
target_link_libraries(sister-praxis-cli PRIVATE sister_praxis_core)
target_compile_options(sister-praxis-cli PRIVATE -Wall -Wextra -Wpedantic)
if(SISTER_PRAXIS_WARNINGS_AS_ERRORS)
  target_compile_options(sister-praxis-cli PRIVATE -Werror)
endif()

enable_testing()
add_executable(sister_praxis_core_test tests/core_test.cpp)
target_link_libraries(sister_praxis_core_test PRIVATE sister_praxis_core)
target_compile_options(sister_praxis_core_test PRIVATE -Wall -Wextra -Wpedantic)
if(SISTER_PRAXIS_WARNINGS_AS_ERRORS)
  target_compile_options(sister_praxis_core_test PRIVATE -Werror)
endif()
add_test(NAME sister_praxis_core_test COMMAND sister_praxis_core_test)
