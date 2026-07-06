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
#ifndef FPU_H
#define FPU_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(FPU) {
  // Ports
  sc_in<sc_bv<6>> op_switch;
  sc_in<sc_int<32>> src1;
  sc_in<sc_int<32>> src2;
  sc_in<sc_int<32>> src3; // for FMAD
  sc_inout<sc_int<32>> result;

  // Process prototypes
  void execute();

  // Constructor
  SC_CTOR(FPU) {
    SC_METHOD(execute);
    sensitive << op_switch << src1 << src2;
  }

  // Destructor
  ~FPU();
};

#endif // !
