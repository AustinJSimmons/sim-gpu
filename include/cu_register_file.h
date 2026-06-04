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

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

template <int NUM_BANKS, int REG_PER_THREAD, int NUM_THREADS> SC_MODULE(RF) {
  // Need to calc the DEPTH of our banks at compile time
  static constexpr int BANK_DEPTH = REG_PER_THREAD / NUM_BANKS;

  // Need a pointer instantiated to hold our array of banks
  sc_vector<RFB<BANK_DEPTH, NUM_THREADS>> banks[NUM_BANKS];

  SC_CTOR(RF) {
    // Need some way to instantiate the number of banks
    for (int i = 0; i < NUM_BANKS; i++) {
      banks[i] = new RFB<BANK_DEPTH, NUM_THREADS>;
    }
  }

  ~RF();
};

#endif
