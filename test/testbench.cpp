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

  SC_THREAD(test_alu);
}

/*
 * test_alu()
 * a function used to test each opcode the ALU
 * might receive.
 * - ADD 0x03
 * - SUB 0x04
 * - MUL 0x05
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

  sc_stop();
}
