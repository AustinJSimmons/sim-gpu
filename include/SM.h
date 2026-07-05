/*
 * SM stands for streaming multiprocessor. It is the
 * main processing component of our GPU and also the
 * most complex.
 **/

#ifndef SM_H
#define SM_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

#include "CU.h"
#include "PE.h"
#include "RF.h"

template <int WARP_SIZE, int NUM_REG, int DATA_SIZE> SC_MODULE(SM) {

  // Internal components
  RF<WARP_SIZE, NUM_REG> *register_file;
  CU<WARP_SIZE> *CU;
  sc_vector<PE> PE_array;

  // Internal wiring between components
  // Data Path Lines
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_rf_to_pe_a;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_rf_to_pe_b;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_rf_to_pe_c;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_pe_to_rf_res;

  // Control Path Lines
  sc_signal<sc_uint<5>> wire_rf_rs1_addr;
  sc_signal<sc_uint<5>> wire_rf_rs2_addr;
  sc_signal<sc_uint<5>> wire_rf_rd_addr;

  SC_CTOR(SM)
      : wire_pe_to_rf_res("wire_pe_to_rf_res", WARP_SIZE),
        wire_rf_to_pe_a("wire_rf_to_pe_a", WARP_SIZE),
        wire_rf_to_pe_b("wire_rf_to_pe_b", WARP_SIZE),
        wire_rf_to_pe_c("wire_rf_to_pe_c", WARP_SIZE) {
    register_file = new RF<WARP_SIZE, NUM_REG>("Vectorized_Register_File");

    for (int i = 0; i < WARP_SIZE; i++) {
      // RF -> PE
      register_file->data_out_a[i](wire_rf_to_pe_a[i]);
      PE_array[i].dc_in_a(wire_rf_to_pe_a[i]);
      register_file->data_out_b[i](wire_rf_to_pe_b[i]);
      PE_array[i].dc_in_b(wire_rf_to_pe_b[i]);
      register_file->data_out_c[i](wire_rf_to_pe_c[i]);
      PE_array[i].dc_in_c(wire_rf_to_pe_c[i]);
      // PE -> RF
      PE_array[i].dc_out(wire_pe_to_rf_res[i]);
      register_file->data_in_write(wire_pe_to_rf_res[i]);

      // Control Path
    }
  };

  ~SM() { delete register_file; };
};

#endif // !SM_H
