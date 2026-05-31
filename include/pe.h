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
#ifndef PE_H
#define PE_H

#include <systemc.h>
// Must include child Modules
#include "ALU.h"
#include "FPU.h"

SC_MODULE(PE) {
  // 1. Ports (how it connects to the rest of the GPU)
  sc_in<sc_uint<6>> opcode;
  sc_in<sc_int<32>> dc_in_a;
  sc_in<sc_int<32>> dc_in_b;
  sc_inout<sc_int<32>> dc_out;

  // 2. Internal Signals (Wiring)
  sc_signal<sc_uint<6>> alu_op;
  sc_signal<sc_int<32>> alu_in_a;
  sc_signal<sc_int<32>> alu_in_b;
  sc_signal<sc_int<32>> alu_result;
  sc_signal<sc_uint<6>> fpu_op;
  sc_signal<sc_int<32>> fpu_in_a;
  sc_signal<sc_int<32>> fpu_in_b;
  sc_signal<sc_int<32>> fpu_result;

  void demux();

  // 3. Child Modules (Internal Components)
  ALU *alu;
  FPU *fpu;

  // Constructor prototype
  SC_CTOR(PE) {
    alu = new ALU("ALU");
    alu->input_a(alu_in_a);
    alu->input_b(alu_in_b);
    alu->opcode(alu_op);
    alu->result(alu_result);
    fpu = new FPU("FPU");
    fpu->src1(fpu_in_a);
    fpu->src2(fpu_in_b);
    fpu->opcode(fpu_op);
    fpu->result(fpu_result);

    SC_METHOD(demux);
    sensitive << opcode << dc_in_a << dc_in_b;
  }

  // Destructor prototype
  ~PE();
};

#endif
