/*
 * This file will act as a test suite for functional tests of each
 * individual component.
 **/

// #include "sysc/kernel/sc_module.h"
#include "systemc"
using namespace sc_core;
using namespace sc_dt;

#include "ALU.h"
#include "FPU.h"

#ifndef TEST_H
#define TEST_H

SC_MODULE(TestBench) {
  int errorCount;
  std::string testMode;

  // Define shared ports of all components being tested.
  sc_signal<bool> clk;
  sc_signal<sc_uint<6>> opcode;
  sc_signal<sc_int<32>> input_a, input_b;
  sc_signal<sc_int<32>> result;

  // Define instances of components being tested.
  ALU *alu1;
  FPU *fpu1;

  // Define test functions.
  void test_alu();
  void test_fpu();

  // Define CTOR

  TestBench(sc_module_name name, std::string testMode);
};

#endif // !TEST_H
