/*
 * cpp files will be used to fill out logic of functions
 * for each module etc...
 **/

// Must include the component header
#include "ALU.h"

void ALU::execute() {
  sc_bv<6> op = op_switch.read();
  sc_int<32> a = input_a.read();
  sc_int<32> b = input_b.read();

  sc_int<32> res = 0;
  // Define different actions for each Oppcode
  switch (op.to_uint()) {
  case 0b0001:
    res = a + b;
    break; // ADD
  case 0b0010:
    res = a - b;
    break; // SUB
  case 0b0011:
    res = a * b;
    break; // MUL
  case 0b0100:
    res = a & b;
    break; // AND
  case 0b0101:
    res = a | b;
    break; // OR
  case 0b0110:
    res = ~a;
    break; // NOT
  case 0b0111:
    res = a ^ b;
    break; // XOR
  case 0b1000:
    res = a << b;
    break;
  case 0b1001:
    res = a >> b;
    break;
  default:
    res = 0;
  }

  result.write(res);
}

ALU::~ALU() {}
