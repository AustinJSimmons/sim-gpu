/*
 * This file will act as a test suite for functional tests of each
 * individual component.
 **/

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#include "ALU.h"

#ifndef TEST_H
#define TEST_H

SC_MODULE(testBench) {
  // Define shared ports of all components being tested.
  sc_signal<bool> clk;
  sc_signal<sc_uint<4>> opcode;
  sc_signal<sc_int<32>> input_a, input_b;
  sc_signal<sc_int<32>> result;

  // Define instances of components being tested.
  ALU *alu1;

  // Define test functions.
  void test_alu();

  // Define CTOR

  SC_CTOR(testBench) {
    // Connect component to test
    alu1 = new ALU("ALU_1");
    alu1->clk(clk);
    alu1->input_a(input_a);
    alu1->input_b(input_b);
    alu1->result(result);

    // Create a thread to run each test
    SC_THREAD(test_alu);
  }
};

#endif // !TEST_H
