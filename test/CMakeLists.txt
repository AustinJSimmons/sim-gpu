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
add_test(NAME ALU_Test_MUL COMMAND test-gpu MUL)
add_test(NAME ALU_Test_AND COMMAND test-gpu ANND)
add_test(NAME ALU_Test_OR COMMAND test-gpu ORR)
add_test(NAME ALU_Test_XOR COMMAND test-gpu XORR)
add_test(NAME ALU_Test_NOT COMMAND test-gpu NOTT)
add_test(NAME ALU_Test_LS COMMAND test-gpu LS)
add_test(NAME ALU_Test_RS COMMAND test-gpu RS)
add_test(NAME FPU_Test_FADD COMMAND test-gpu FADD)
add_test(NAME FPU_Test_FSUB COMMAND test-gpu FSUB)
add_test(NAME FPU_Test_FMUL COMMAND test-gpu FMUL)
add_test(NAME FPU_Test_FDIV COMMAND test-gpu FDIV)
add_test(NAME FPU_Test_FMAD COMMAND test-gpu FMAD)
add_test(NAME FPU_Test_FMIN COMMAND test-gpu FMIN)
add_test(NAME FPU_Test_FMAX COMMAND test-gpu FMAX)
add_test(NAME PE_Test_ADD COMMAND test-gpu PADD)
add_test(NAME PE_Test_FADD COMMAND test-gpu PFAD)

