/*
 * The collector units are the piece of silicon physically
 * responsible for latching operands over multiple cycles if
 * needed.
 **/
#ifndef OC_UNIT_H
#define OC_UNIT_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

template <int NUM_OPERANDS, int NUM_THREADS> SC_MODULE(OC_UNIT) {
  // Register array
  // NUM_OPERANDS = largest num of operands per single instruction (FMAD) = 3
  sc_int<32> register_array[NUM_OPERANDS][NUM_THREADS];

  struct DispatchPayload {
    sc_int<32> src1;
    sc_int<32> src2;
    sc_int<32> src3;
    sc_bv<4> opcode;

    // SystemC requires overloading operator== for custom structs in signals
  };

  // Ports
  sc_in<sc_int<32>> ocu_in[NUM_THREADS];
  sc_out<DispatchPayload> ocu_out[NUM_THREADS];

  SC_CTOR(OC_UNIT) {}

  ~OC_UNIT() {}
};
#endif // !OC_UNIT_H
