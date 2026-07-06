/*
 * CU stands for Control Unit in this case.
 *
 * Defines a systemc module that holds parts of our control unit,
 * which is part of the greater Streaming Multiprocessor or SM.
 **/

#ifndef CU_H
#define CU_H

#include "systemc.h"
using namespace sc_core;
using namespace sc_dt;

template <int WARP_SIZE, int NUM_INSTRUCTION> SC_MODULE(CU) {
  // Clock in to execute pipeline
  sc_in_clk clk;
  // I-Cache Instruction cache
  sc_bv<32> i_cache[NUM_INSTRUCTION];

  // PC (Program Counter)
  sc_signal<sc_uint<32>> pc_reg;

  sc_out<sc_bv<6>> opcode;
  sc_out<sc_uint<5>> rs1_addr;
  sc_out<sc_uint<5>> rs2_addr;
  sc_out<sc_uint<5>> rd_addr;
  sc_out<sc_uint<16>> offset;
  sc_out<sc_bv<4>> preds;
  sc_out<sc_uint<22>> c_offset;
  sc_out<bool> is_load;
  sc_out<bool> is_store;
  sc_out<bool> is_fpu_op;
  sc_vector<sc_out<bool>> rf_write_enable;
  sc_vector<sc_out<bool>> active_mask;

  SC_CTOR(CU)
      : rf_write_enable("rf_write_enable", WARP_SIZE),
        active_mask("active_mask", WARP_SIZE) {
    pc_reg.write(0); // init the pc reg
    SC_METHOD(execute_pipeline);
    sensitive << clk.pos();

    dont_initialize();
  };

  void execute_pipeline() {

    // Fetch
    sc_bv<32> raw_instruction = i_cache[pc_reg.read()];
    sc_bv<6> op_val = raw_instruction.range(5, 0);
    // 1. Determine Memory operations for LSU
    is_load.write(op_val.to_uint() == 0x00);
    is_store.write(op_val.to_uint() == 0x01);

    // 2. Determine if it's an FPU operation (Based on your ISA, 0x0D to 0x13)
    is_fpu_op.write(op_val.to_uint() >= 0x0D && op_val.to_uint() <= 0x13);

    // 3. Determine if instruction writes to RF (LOAD or Math)
    bool inst_writes_to_rf =
        (op_val.to_uint() == 0x00) ||
        (op_val.to_uint() >= 0x02 && op_val.to_uint() <= 0x13);

    // 4. Drive the per-thread masks
    for (int i = 0; i < WARP_SIZE; i++) {
      // For a simple Phase 1 functional test, all threads in the warp are
      // active
      bool thread_is_active = true;

      active_mask[i].write(thread_is_active);
      rf_write_enable[i].write(inst_writes_to_rf && thread_is_active);
    }
    // Decode and dispatch
    opcode.write(raw_instruction.range(5, 0));
    if (op_val.to_uint() <= 0x01) {
      rs1_addr.write(raw_instruction.range(15, 11).to_uint());
      rs2_addr.write(raw_instruction.range(10, 6).to_uint());
      rd_addr.write(raw_instruction.range(10, 6).to_uint());
      offset.write(raw_instruction.range(31, 16).to_uint());

    } else if (0x02 <= op_val.to_uint() && op_val.to_uint() <= 0x13) {
      rs1_addr.write(raw_instruction.range(10, 6).to_uint());
      rs2_addr.write(raw_instruction.range(15, 11).to_uint());
      rd_addr.write(raw_instruction.range(20, 16).to_uint());

    } else {
      preds.write(raw_instruction.range(9, 6));
      c_offset.write(raw_instruction.range(31, 10).to_uint());
    }

    pc_reg.write(pc_reg.read() + 1);
  }

  ~CU(){};
};

#endif // !CU_H
