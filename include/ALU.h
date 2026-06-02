/*
 * Header files will be used to define the following for each component:
 * Structural Module:
 * 1. External Ports (Pins - how it connects to the rest of the GPU)
 * 2. Internal Signals (Wiring of component internals)
 * 3. Child Modules (The internal components)
 * 4. Constructor/Destructor
 *
 * Child Module:
 * 1. Ports (Pins)
 * 2. Internal State (Registers)
 * 3. Process prototypes (Logic)
 * 4. Constructor/Destructor
 * **/
#ifndef ALU_H
#define ALU_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(ALU) {
  // 1. Ports (how it connects to the rest of the GPU)
  // For input: At the most basic level I see the ALU needing
  // in_a, in_b for our two input operands and some in_opp for
  // our oppcode being executed as well as a clk input.
  sc_in<sc_bv<4>> op_switch;
  sc_in<sc_int<32>> input_a;
  sc_in<sc_int<32>> input_b;

  // For output: I see us needing a result output and maybe a
  // few flag outputs to handle things like zero flags, overflow
  // negative signs etc..
  sc_out<sc_int<32>> result;

  // 2. Process Logic
  void execute();

  // Constructor prototype
  SC_CTOR(ALU) {
    SC_METHOD(execute);
    // Must be sensitive to inputs for testing at least
    sensitive << op_switch << input_a << input_b;
  }

  // Destructor prototype
  ~ALU();
};

#endif
