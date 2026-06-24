/*
 * This will hold the definition of our GPUs Register File Banks
 * Multiple RFBs will compose the entire register file.
 *
 * It will contain 32 registers accessable per thread.
 *
 **/

#ifndef RFB_H
#define RFB_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

template <int REG_PER_THREAD, int NUM_THREADS> SC_MODULE(RFB) {
  sc_int<32> mem_array[REG_PER_THREAD];

  sc_in<bool> clk;

  sc_in<sc_bv<NUM_THREADS>> write_mask; // Only writes to threads needing writes
  sc_in<sc_uint<REG_PER_THREAD>> write_address; // Register addr
  sc_in<bool> write_enable;
  sc_in<sc_int<32>> bank_in;

  sc_in<sc_uint<REG_PER_THREAD>> read_address;
  sc_out<sc_int<32>> bank_out;
  // Processes
  void execute() {
    for (int i = 0; i < NUM_THREADS; i++) {
      // Always dump reads
      bank_out.write(mem_array[read_address.read()]);
      if (write_enable.read()) {
        mem_array[write_address.read()] = bank_in.read();
      }
    }
  }

  SC_CTOR(RFB) {
    SC_METHOD(execute);
    sensitive << clk.pos();
  }

  ~RFB(){};
};

#endif
