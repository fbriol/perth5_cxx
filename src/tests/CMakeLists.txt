include(CTest)

enable_testing()

# Create a new test suite.
macro(ADD_TESTCASE testname sources)
  set(libraries ${ARGN})
  add_executable(test_${testname} ${sources})
  target_include_directories(test_${testname}
                             PRIVATE ${CMAKE_BINARY_DIR}/include)
  target_link_libraries(test_${testname} GTest::gtest_main ${libraries})
  add_test(NAME test_${testname}
           COMMAND test_${testname} --gtest_output=xml:test_${testname}.xml)
endmacro()

# test_love_numbers
file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/love_numbers.cpp")
add_testcase(love_numbers "${src}")

# test_constituent
file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/consitutent.cpp")
add_testcase(constituent "${src}" perth)

# test_inference
file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/inference.cpp")
add_testcase(inference "${src}" perth)

# test_delta_t
file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/delta_t.cpp")
add_testcase(delta_t "${src}")

# test_doodson
file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/doodson.cpp")
add_testcase(doodson "${src}" perth)

# test_nodal_corrections
file(GLOB src "${CMAKE_CURRENT_SOURCE_DIR}/nodal_corrections.cpp")
add_testcase(nodal_corrections "${src}" perth)
