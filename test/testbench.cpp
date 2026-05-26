/*
 * This file will be used to write test logic for each
 * component as needed.
 **/

#include "testbench.h"
// #include "sysc/kernel/sc_module_name.h"
#include <iostream>
#include <stdio.h>

/*
 * testBench() constructor to make my ctest more intuitive.
 *
 */
TestBench::TestBench(sc_module_name name, std::string testMode)
    : sc_module(name), testMode(testMode) {
  errorCount = 0;
  alu1 = new ALU("ALU_1");
  alu1->opcode(opcode);
  alu1->input_a(input_a);
  alu1->input_b(input_b);
  alu1->result(result);

  fpu1 = new FPU("FPU_1");
  fpu1->opcode(opcode);
  fpu1->src1(input_a);
  fpu1->src2(input_b);
  fpu1->result(result);

  SC_THREAD(test_alu);
  SC_THREAD(test_fpu);
}

/*
 * test_alu()
 * a function used to test each opcode the ALU
 * might receive.
 * */
void TestBench::test_alu() {
  // Init inputs
  input_a.write(0);
  input_b.write(0);
  opcode.write(0x00);
  wait(10, SC_NS);

  // Test ADD 0x03
  if (testMode == "ADD") {

    input_a.write(0);
    input_b.write(1);
    opcode.write(0x03);
    wait(10, SC_NS);

    if (result.read() == 1) {
      std::cout << "Test Passed 0 + 1 = " << result.read() << std::endl;
    } else {
      std::cout << "Test Failed 0 + 1 != " << result.read() << std::endl;
      errorCount++;
    }

    input_a.write(-15);
    input_b.write(20);
    opcode.write(0x03);
    wait(10, SC_NS);

    if (result.read() == 5) {
      std::cout << "Test Passed -15 + 20 = " << result.read() << std::endl;
    } else {
      std::cout << "Test Failed -15 + 20 !=" << result.read() << std::endl;
      errorCount++;
    }
  }

  // Test SUB 0x04
  if (testMode == "SUB") {
    input_a.write(2);
    input_b.write(1);
    opcode.write(0x04);
    wait(10, SC_NS);

    if (result.read() == 1) {
      std::cout << "Test Passed 2 - 1 = " << result.read() << std::endl;
    } else {
      std::cout << "Test Failed 2 - 1 != " << result.read() << std::endl;
      errorCount++;
    }
  }

  // Test MUL 0x05
  if (testMode == "MUL") {
    input_a.write(0);
    input_b.write(2147483647); // max twos compliment
    opcode.write(0x05);
    wait(10, SC_NS);

    if (result.read() != 0) {
      std::cout << "Test Failed MUL by 0" << std::endl;
      errorCount++;
    }
  }

  // Test AND 0x07
  if (testMode == "ANND") {
    input_a.write(1);
    input_b.write(1);
    opcode.write(0x07);
    wait(10, SC_NS);

    if (result.read() != 1) {
      std::cout << "Test Failed 1 AND 1 = 1" << std::endl;
      errorCount++;
    }

    input_a.write(1);
    input_b.write(2);
    opcode.write(0x07);
    wait(10, SC_NS);

    if (result.read() != 0) {
      std::cout << "Test Failed 01 AND 10 = 00" << std::endl;
      errorCount++;
    }
  }

  // Test OR 0x08
  if (testMode == "ORR") {
    input_a.write(1);
    input_b.write(2);
    opcode.write(0x08);
    wait(10, SC_NS);

    if (result.read() != 3) {
      std::cout << "Test Failed 01 OR 10 = 11" << std::endl;
      errorCount++;
    }
  }
  // Test NOT 0x09
  if (testMode == "NOTT") {
    input_a.write(0);
    opcode.write(0x09);
    wait(10, SC_NS);

    if (result.read() != -1) {
      std::cout << "Test Failed NOT 0 = -1 != " << result.read() << std::endl;
      errorCount++;
    }
  }
  // Test XOR 0x0A
  if (testMode == "XORR") {
    input_a.write(1);
    input_b.write(2);
    opcode.write(0x0A);
    wait(10, SC_NS);

    if (result.read() != 3) {
      std::cout << "Test Failed 01 XOR 10 = 11" << std::endl;
      errorCount++;
    }

    input_a.write(1);
    input_b.write(1);
    opcode.write(0x0A);
    wait(10, SC_NS);

    if (result.read() != 0) {
      std::cout << "Test Failed 01 XOR 01 = 11" << std::endl;
      errorCount++;
    }
  }
  // Test LS 0x0B
  if (testMode == "LS") {
    input_a.write(1);
    input_b.write(2);
    opcode.write(0x0B);
    wait(10, SC_NS);

    if (result.read() != 4) {
      std::cout << "Test Failed 0001 << 1 = 0100 = 4" << std::endl;
      errorCount++;
    }
  }
  // Test RS 0x0C
  if (testMode == "RS") {
    input_a.write(4);
    input_b.write(2);
    opcode.write(0x0C);
    wait(10, SC_NS);

    if (result.read() != 1) {
      std::cout << "Test Failed 0100 >> 2 = 0001 = 1" << std::endl;
      errorCount++;
    }
  }

  sc_stop();
}

void TestBench::test_fpu() {
  // Test FADD 0x0D
  // Test FSUB 0x0E
  // Test FMUL 0x0F
  // Test FDIV 0x10
  // Test FMAD 0x11
  // Test FMIN 0x12
  // Test FMAX 0x13
}
