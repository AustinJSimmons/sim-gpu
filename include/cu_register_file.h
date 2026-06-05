/*
 * This will hold the definition of our GPUs Register File
 * Multiple RFBs will compose the entire register file.
 *
 * It will contain 32 registers accessable per thread.
 *
 **/

#ifndef RF_H
#define RF_H

#include "cu_rf_bank.h"
#include <string>

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

template <int REG_PER_THREAD, int NUM_THREADS> SC_MODULE(RF) {
  //

  // Need a pointer instantiated to hold our array of banks
  RFB<REG_PER_THREAD, NUM_THREADS> *banks[NUM_THREADS];

  // Ports
  sc_in<bool> clk;
  sc_in<bool> global_write_enable;
  sc_in<sc_uint<5>> global_write_addr;
  sc_in<sc_int<32>> global_write_data_in[NUM_THREADS];
  sc_in<sc_bv<NUM_THREADS>> global_write_mask;
  sc_in<sc_uint<5>> global_read_addr;
  sc_out<sc_int<32>> global_read_data_out[NUM_THREADS];

  // Internal Signals (wires)
  sc_signal<bool> bank_we[NUM_THREADS];
  sc_signal<sc_int<32>> bank_outputs[NUM_THREADS];
  sc_signal<sc_uint<REG_PER_THREAD>> read_addr;
  sc_signal<sc_uint<REG_PER_THREAD>> write_addr;

  // Process
  void decode_mux() {
    read_addr.write(global_read_addr.read());
    write_addr.write(global_write_addr.read());
    for (int i = 0; i < NUM_THREADS; i++) {
      if (global_write_enable.read() == global_write_mask.read().bit(i)) {
        bank_we[i].write(global_write_enable.read());
      } else {
        bank_we[i].write(false);
      }

      global_read_data_out[i].write(bank_outputs[i].read());
    }
  }

  SC_CTOR(RF) {
    // Need some way to instantiate the number of banks
    // number of banks = number of threads per CU in SIMT
    for (int i = 0; i < NUM_THREADS; i++) {
      std::string bank = "bank_" + std::to_string(i);
      banks[i] = new RFB<REG_PER_THREAD, NUM_THREADS>(bank.c_str());

      banks[i]->clk(this->clk);
      banks[i]->write_address(write_addr);
      banks[i]->read_address(read_addr);
      banks[i]->write_mask(this->global_write_mask);
      banks[i]->write_enable(bank_we[i]);

      banks[i]->bank_in(this->global_write_data_in[i]);
      banks[i]->bank_out(bank_outputs[i]);
    }

    SC_METHOD(decode_mux);
    sensitive << global_write_addr << global_read_addr << global_write_enable;
    for (int i = 0; i < NUM_THREADS; i++) {
      sensitive << bank_outputs[i];
    }
  }

  ~RF(){};
};

#endif
