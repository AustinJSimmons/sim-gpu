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
#ifndef LSU_H
#define LSU_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

SC_MODULE(LSU) {
  // Ports
  sc_in<sc_uint<6>> opcode;
  sc_in<sc_int<32>> mem_addr;
  sc_inout<sc_int<32>> sr_dr;

  // Process prototypes
  void execute();

  // Constructor
  SC_CTOR(LSU) {
    SC_METHOD(execute);
    sensitive << opcode << mem_addr << sr_dr;
  }

  // Destructor
  ~LSU();
};

#endif // !
