#include "PE.h"

void PE::demux() {
  // Check opcode decide where to send PE internal signals
  fpu_in_a.write(dc_in_a.read());
  fpu_in_b.write(dc_in_b.read());
  fpu_in_c.write(dc_in_c.read());

  alu_in_a.write(dc_in_a.read());
  alu_in_b.write(dc_in_b.read());
  alu_mod.write(mod);

  if (is_fpu_switch.read()) {
    fpu_op.write(opcode_in.read());
    alu_op.write(0b0000);
  } else {
    fpu_op.write(0b0000);
    alu_op.write(opcode_in.read());
  }
}

void PE::mux() {
  if (is_fpu_switch.read()) {
    dc_out.write(fpu_result.read());
  } else {
    dc_out.write(alu_result.read());
  }
}

PE::~PE() {}
