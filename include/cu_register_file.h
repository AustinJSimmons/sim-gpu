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

template <int NUM_BANKS, int REG_PER_THREAD, int NUM_THREADS> SC_MODULE(RF) {
  // Need to calc the DEPTH of our banks at compile time
  static constexpr int BANK_DEPTH = REG_PER_THREAD / NUM_BANKS;

  // Need a pointer instantiated to hold our array of banks
  sc_vector<RFB<BANK_DEPTH, NUM_THREADS>> banks[NUM_BANKS];

  // Ports
  sc_in<bool> clk;
  sc_in<bool> global_write_enable;
  sc_in<sc_uint<5>> global_write_addr;
  sc_in<sc_int<32>> global_write_data_in[NUM_THREADS];
  sc_in<sc_bv<NUM_THREADS>> global_write_mask;
  sc_in<sc_uint<5>> global_read_addr;
  sc_out<sc_int<32>> global_read_data_out[NUM_THREADS];

  // Internal Signals (wires)
  sc_signal<bool> bank_wr_e[NUM_BANKS];
  sc_signal<sc_int<32>> bank_outputs[NUM_BANKS][NUM_THREADS];
  sc_signal<sc_uint<BANK_DEPTH>> read_row;
  sc_signal<sc_uint<BANK_DEPTH>> write_row;

  // Process
  void decode_mux() {
    read_row.write(global_read_addr.read() >> 2);
    write_row.write(global_write_addr.read() >> 2);
    for (int i = 0; i < NUM_BANKS; i++) {
      if (global_write_addr.read() % NUM_BANKS == i) {
        bank_wr_e[i].write(global_write_enable.read());
      } else {
        bank_wr_e[i].write(false);
      }

      if (global_read_addr.read() % NUM_BANKS == i) {
        for (int j = 0; j < NUM_THREADS; j++) {
          global_read_data_out[j].write(bank_outputs[i][j].read());
        }
      }
    }
  }

  SC_CTOR(RF) {
    // Need some way to instantiate the number of banks
    for (int i = 0; i < NUM_BANKS; i++) {
      std::string bank = "bank_" + std::to_string(i);
      banks[i] = new RFB<BANK_DEPTH, NUM_THREADS>(bank);

      banks[i]->clk(this->clk);
      banks[i]->write_address(write_row);
      banks[i]->read_address(read_row);
      banks[i]->write_mask(global_write_mask);
      banks[i]->write_enable(bank_wr_e[i]);

      for (int j = 0; j < NUM_THREADS; j++) {
        banks[i]->bank_in[j](this->global_write_data_in[j]);
        banks[i]->bank_out[j](bank_outputs[i][j]);
      }
    }

    SC_METHOD(decode_mux);
    sensitive << global_write_addr << global_read_addr << global_write_enable;
  }

  ~RF();
};

#endif
