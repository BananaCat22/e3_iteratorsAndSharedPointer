###############################################################################
cmake_minimum_required(VERSION 3.17.0) # version can be different
cmake_policy(SET CMP0135 NEW) # avoid depricated "old" behavior warnings

###############################################################################
set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-g -O0 -std=c++11 -stdlib=libc++ -U__STRICT_ANSI__ -fsanitize=address -fprofile-instr-generate -fcoverage-mapping")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

###############################################################################
project(memoryoperations) 

include_directories(googletest/include) # this is so we can #include <gtest/gtest.h>
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

###############################################################################
add_executable(tests) 
target_include_directories(tests PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB ALL_CXX_SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/*.c*
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/tests/*.c*
    ${PROJECT_SOURCE_DIR}/tests/*.h*
)
target_sources(tests PUBLIC 
   ${ALL_CXX_SOURCE_FILES}
)
target_link_libraries(tests PRIVATE gtest) # link google test to this executable
set_property(TARGET tests PROPERTY CXX_STANDARD 11)

###############################################################################
add_custom_target(coverage
    COMMAND tests
    COMMAND llvm-profdata merge -sparse default.profraw -o cumulative.profdata
    COMMAND llvm-cov show -ignore-filename-regex='../google*/*' -ignore-filename-regex='../json/*' -format="html" ./tests -instr-profile=cumulative.profdata > coverageLines.html
    COMMAND llvm-cov report -show-region-summary=false -ignore-filename-regex='../google*/*' -ignore-filename-regex='../json/*'  ./tests -instr-profile=cumulative.profdata > coverageSummary.txt
    COMMAND cat coverageSummary.txt
)
add_dependencies(coverage tests)

enable_testing()
include(GoogleTest)
gtest_discover_tests(tests)

###############################################################################
# Add target to build documents 
set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
#set the output directory of the documentation
set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
# sanity check...
message("Doxygen Output ${DOXYGEN_OUTPUT_DIR}")
find_package(Doxygen)

###############################################################################
if(DOXYGEN_FOUND)
    # create the output directory where the documentation will live
    file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})
    # configure our Doxygen configuration file. This will be the input to the doxygen
    # executable
    configure_file(${DOXYGEN_INPUT}
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

   # now add the custom target. This will create a build target called 'document' 
   # in your project
   ADD_CUSTOM_TARGET(document
   COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
   COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}
   COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
      WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR}
   COMMAND ${CMAKE_COMMAND} -E echo "Done."
   WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

endif(DOXYGEN_FOUND)

###############################################################################
# Add target to format code 
if(NOT CLANG_FORMAT)
    find_program(CLANG_FORMAT "clang-format")
endif()
if(CLANG_FORMAT)
    file(GLOB ALL_CXX_SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/src/*.c*
        ${PROJECT_SOURCE_DIR}/include/*.h
        ${PROJECT_SOURCE_DIR}/tests/*.c*
        ${PROJECT_SOURCE_DIR}/tests/*.h*
    )
    add_custom_target(
        ${PROJECT_NAME}_format
        COMMAND clang-format -i -style=file ${ALL_CXX_SOURCE_FILES}
    )

    if(NOT TARGET format)
        add_custom_target(format)
    endif()

    add_dependencies(format ${PROJECT_NAME}_format)
endif()

# Format code _before_ building so that formatting will not change
# the code after compiling the code. If the code is modified after
# compile, then the build system will think the code needs to be
# compiled again.
add_dependencies(tests format)
