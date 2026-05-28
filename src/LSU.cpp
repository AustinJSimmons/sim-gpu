#include "LSU.h"

void LSU::execute() {
  sc_uint<6> op = opcode.read();
  sc_int<32> base = base_reg.read();
  sc_int<32> dest = dest_reg.read();
  sc_int<16> os = offset.read();

  switch (op) {
  case 0x00:
    break;
  case 0x01:
    break;
  }
}
