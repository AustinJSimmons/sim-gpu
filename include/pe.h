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
  sc_in<bool> is_fpu_switch;
  sc_in<sc_bv<6>> opcode_in;
  sc_in<sc_bv<3>> mod;
  sc_in<sc_int<32>> dc_in_a;
  sc_in<sc_int<32>> dc_in_b;
  sc_in<sc_int<32>> dc_in_c;
  sc_out<sc_int<32>> dc_out;
  sc_out<bool> pred_out;

  // 2. Internal Signals (Wiring)
  sc_signal<sc_bv<6>> alu_op;
  sc_signal<sc_bv<3>> alu_mod;
  sc_signal<sc_int<32>> alu_in_a;
  sc_signal<sc_int<32>> alu_in_b;
  sc_signal<sc_int<32>> alu_result;
  sc_signal<sc_bv<6>> fpu_op;
  sc_signal<sc_int<32>> fpu_in_a;
  sc_signal<sc_int<32>> fpu_in_b;
  sc_signal<sc_int<32>> fpu_in_c; // for FMAD
  sc_signal<sc_int<32>> fpu_result;

  void demux();
  void mux(); // mux?

  // 3. Child Modules (Internal Components)
  ALU *alu;
  FPU *fpu;

  // Constructor prototype
  SC_CTOR(PE) {
    alu = new ALU("ALU");
    alu->input_a(alu_in_a);
    alu->input_b(alu_in_b);
    alu->op_switch(alu_op);
    alu->result(alu_result);
    alu->mod(alu_mod);
    alu->pred_out(pred_out);
    fpu = new FPU("FPU");
    fpu->src1(fpu_in_a);
    fpu->src2(fpu_in_b);
    fpu->src3(fpu_in_c);
    fpu->op_switch(fpu_op);
    fpu->result(fpu_result);

    SC_METHOD(demux);
    sensitive << is_fpu_switch << opcode_in << dc_in_a << dc_in_b << dc_in_c;

    SC_METHOD(mux);
    sensitive << fpu_result << alu_result;
  }

  // Destructor prototype
  ~PE();
};

#endif
