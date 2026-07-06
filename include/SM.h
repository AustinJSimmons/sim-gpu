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
#include "LSU.h"
#include "PE.h"
#include "RF.h"

template <int WARP_SIZE, int MEM_SIZE, int NUM_INSTRUCTIONS, int NUM_REG,
          int DATA_SIZE>
SC_MODULE(SM) {
  sc_in_clk clk;

  // Internal components
  RF<WARP_SIZE, NUM_REG> *register_file;
  CU<WARP_SIZE, NUM_INSTRUCTIONS> *cu;
  LSU<WARP_SIZE, MEM_SIZE> *lsu;
  sc_vector<PE> pe_array;

  // Internal wiring between components
  // Data Path
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_rf_to_pe_a;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_rf_to_pe_b;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_rf_to_pe_c;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_pe_res;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_lsu_res;
  sc_vector<sc_signal<sc_int<DATA_SIZE>>> wire_final_writeback;

  // Control Path Lines
  sc_signal<sc_bv<6>> wire_opcode;
  sc_signal<sc_uint<5>> wire_rs1_addr;
  sc_signal<sc_uint<5>> wire_rs2_addr;
  sc_signal<sc_uint<5>> wire_rd_addr;
  sc_vector<sc_signal<bool>> wire_rf_we;
  sc_vector<sc_signal<bool>> wire_active_mask;

  // LSU specific paths
  sc_signal<bool> wire_lsu_is_load;
  sc_signal<bool> wire_lsu_is_store;

  SC_CTOR(SM)
      : wire_pe_res("wire_pe_res", WARP_SIZE),
        wire_lsu_res("wire_lsu_res", WARP_SIZE),
        wire_final_writeback("wire_final_writeback", WARP_SIZE),
        wire_rf_to_pe_a("wire_rf_to_pe_a", WARP_SIZE),
        wire_rf_to_pe_b("wire_rf_to_pe_b", WARP_SIZE),
        wire_rf_to_pe_c("wire_rf_to_pe_c", WARP_SIZE) {
    cu = new CU<WARP_SIZE, NUM_INSTRUCTIONS>("Control_Unit");
    lsu = new LSU<WARP_SIZE, MEM_SIZE>("Load_Store_Unit");
    register_file = new RF<WARP_SIZE, NUM_REG>("Vectorized_Register_File");

    // Bind system clocks
    cu->clk(clk);
    lsu->clk(clk);

    // CU control path
    cu->opcode(wire_opcode);
    cu->rs1_addr(wire_rs1_addr);
    cu->rs2_addr(wire_rs2_addr);
    cu->rd_addr(wire_rd_addr);

    // RF control path
    register_file->rs1_addr(wire_rs1_addr);
    register_file->rs2_addr(wire_rs2_addr);
    register_file->rd_addr(wire_rd_addr);

    for (int i = 0; i < WARP_SIZE; i++) {
      // RF Write Enables
      cu->rf_write_enable[i](wire_rf_we[i]);
      register_file->write_enable[i](wire_rf_we[i]);

      // Vector Data Path: RF -> PE
      register_file->data_out_a[i](wire_rf_to_pe_a[i]);
      pe_array[i].dc_in_a(wire_rf_to_pe_a[i]);

      register_file->data_out_b[i](wire_rf_to_pe_b[i]);
      pe_array[i].dc_in_b(wire_rf_to_pe_b[i]);

      // Vector Data Path: PE -> Wires
      pe_array[i].dc_out(wire_pe_res[i]);

      // Final Writeback: Wire -> RF
      register_file->data_in_write[i](wire_final_writeback[i]);

      lsu->base_addr_data[i](wire_rf_to_pe_a[i]);
      lsu->store_data[i](wire_rf_to_pe_b[i]);
      lsu->load_data[i](wire_lsu_res[i]);
      lsu->active_mask[i](wire_active_mask[i]);
    }

    SC_METHOD(writeback_mux);
    // Sensitive to any change in PE results, LSU results, or the opcode (to
    // switch the MUX)
    for (int i = 0; i < WARP_SIZE; i++) {
      sensitive << wire_pe_res[i] << wire_lsu_res[i];
    }
    sensitive << wire_opcode;
  };

  void writeback_mux() {
    bool is_load = (wire_opcode.read().to_uint() ==
                    0x00); // Check if instruction is a LOAD

    for (int i = 0; i < WARP_SIZE; i++) {
      if (is_load) {
        // Route memory data to the Register File
        wire_final_writeback[i].write(wire_lsu_res[i].read());
      } else {
        // Route ALU/FPU data to the Register File
        wire_final_writeback[i].write(wire_pe_res[i].read());
      }
    }
  }

  ~SM() { delete register_file; };
};

#endif // !SM_H
