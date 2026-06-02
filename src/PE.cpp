#include "PE.h"

void PE::demux() {
  // Check opcode decide where to send PE internal signals
  bool is_fpu = is_fpu_switch.read();
  sc_int<32> a = dc_in_a.read();
  sc_int<32> b = dc_in_b.read();

  // switch (is_fpu) {
  // case 0x0D:
  // case 0x0E:
  // case 0x0F:
  // case 0x10:
  // case 0x11:
  // case 0x12:
  // case 0x13:
  //   fpu_in_a.write(a);
  //   fpu_in_b.write(b);
  //   fpu_op.write(op);
  //   demux_is_fpu.write(true);
  //   break;
  // default:
  //   alu_in_a.write(a);
  //   alu_in_b.write(b);
  //   alu_op.write(op);
  //   demux_is_fpu.write(false);
  //   break;
  // }

  if (is_fpu) {
    fpu_in_a.write(a);
    fpu_in_b.write(b);
    demux_is_fpu.write(is_fpu);
  } else {
    alu_in_a.write(a);
    alu_in_b.write(b);
    demux_is_fpu.write(is_fpu);
  }
}

void PE::mux() {
  if (demux_is_fpu.read()) {
    dc_out.write(fpu_result.read());
  } else {
    dc_out.write(alu_result.read());
  }
}

PE::~PE() {}
