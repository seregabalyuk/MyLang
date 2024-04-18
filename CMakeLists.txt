set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(PROJECT_INCLUDES ${CMAKE_SOURCE_DIR}/src)

project(FormalGrammars)

add_executable(main src/main.cpp)
target_include_directories(main PRIVATE ${PROJECT_INCLUDES})

add_executable(genDFA src/RE/genDFA.cpp)
target_include_directories(genDFA PRIVATE ${PROJECT_INCLUDES})

add_executable(сalculator src/сalculator.cpp)
target_include_directories(сalculator PRIVATE ${PROJECT_INCLUDES})
