add_executable(test-gpu testbench.cpp)
target_link_libraries(test-gpu PUBLIC
  ${SystemC_LIBRARIES}
  component_lib
)
add_test(NAME ALU_Test COMMAND test_alu)
