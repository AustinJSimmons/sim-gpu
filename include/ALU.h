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
  // Control Path
  sc_in<sc_bv<6>> op_switch;
  sc_in<sc_bv<3>> mod;

  // Data Path
  sc_in<sc_int<32>> input_a;
  sc_in<sc_int<32>> input_b;

  sc_out<sc_int<32>> result;
  sc_out<bool> pred_out;

  // 2. Process Logic
  void execute();

  // Constructor prototype
  SC_CTOR(ALU) {
    SC_METHOD(execute);
    // Must be sensitive to inputs for testing at least
    sensitive << op_switch << mod << input_a << input_b;
  }

  // Destructor prototype
  ~ALU();
};

#endif
