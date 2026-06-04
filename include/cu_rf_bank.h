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

template <int BANK_DEPTH, int NUM_THREADS> SC_MODULE(RFB) {
  // Register array should be a flat 1D array of rows to represent
  // the hardware reality.
  // BANK_DEPTH gives us the number registers (rows) each bank will
  // be responsible for.
  // NUM_THREADS determines the width of our rows so that threads have
  // their own 32 bit registers to access.
  sc_signal<sc_int<32>> mem_array[BANK_DEPTH][NUM_THREADS];

  // Control signals for selecting thread specific registers.
  sc_in<bool> clk;

  sc_in<sc_bv<NUM_THREADS>> write_mask;
  sc_in<sc_uint<BANK_DEPTH>> write_address; // Row address
  sc_in<bool> write_enable;
  sc_in<sc_int<32>> bank_in[NUM_THREADS];

  sc_in<sc_uint<BANK_DEPTH>> read_address;
  sc_out<sc_int<32>> bank_out[NUM_THREADS];

  // Processes
  void execute() {
    for (int i = 0; i < NUM_THREADS; i++) {
      // Always dump reads
      bank_out[i].write(mem_array[read_address.read()][i]);

      if (write_enable.read()) {
        if (write_mask.read()[i]) {
          mem_array[write_address.read()][i].write(bank_in[i].read());
        }
      }
    }
  }

  SC_CTOR(RFB) {
    SC_METHOD(execute);
    sensitive << clk.pos();
  }

  ~RFB();
};

#endif
