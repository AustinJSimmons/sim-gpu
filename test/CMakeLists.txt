
add_executable(test-gpu
  test_main.cpp
  testbench.cpp
)
target_link_libraries(test-gpu PUBLIC
  ${SystemC_LIBRARIES}
  component_lib
)
# Add Tests here when added to testbench.
add_test(NAME ALU_Test_ADD COMMAND test-gpu ADD)
add_test(NAME ALU_Test_SUB COMMAND test-gpu SUB)
