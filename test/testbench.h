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
#include "PE.h"
#include "cu_register_file.h"

#ifndef TEST_H
#define TEST_H

SC_MODULE(TestBench) {
  int errorCount;
  std::string testMode;

  // Define shared ports of all components being tested.
  sc_signal<bool> clk, is_fpu;
  sc_signal<sc_bv<4>> opcode_alu, opcode_fpu, opcode_pe;
  sc_signal<sc_int<32>> a_alu, b_alu, a_fpu, b_fpu, c_fpu, a_pe, b_pe, c_pe;
  sc_signal<sc_int<32>> result_alu, result_fpu, result_pe;

  // Define instances of components being tested.
  ALU *alu1;
  FPU *fpu1;
  PE *pe1;
  RF<4, 32, 32> *rf0;

  // Define test functions.
  void test_alu();
  void test_fpu();
  void test_pe();
  void test_rf();

  bool rough_equal();

  // Define CTOR

  TestBench(sc_module_name name, std::string testMode);
};

#endif // !TEST_H
