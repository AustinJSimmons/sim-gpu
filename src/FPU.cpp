#include "FPU.h"
#include "helpers.h"
#include <cmath>

void FPU::execute() {
  sc_bv<4> op = op_switch.read();
  int a = src1.read();
  int b = src2.read();
  int c = src3.read();

  float f_a = typePunning<float>(a);
  float f_b = typePunning<float>(b);
  float f_c = typePunning<float>(c);
  float f_r = 0.0f;

  switch (op.to_uint()) {
  case 0b0001:
    f_r = f_a + f_b;
    break;
  case 0b0010:
    f_r = f_a - f_b;
    break;
  case 0b0011:
    f_r = f_a * f_b;
    break;
  case 0b0100:
    f_r = f_a / f_b;
    break;
  case 0b0101:
    f_r = std::fmaf(f_a, f_b, f_c);
    break;
  case 0b0110:
    f_r = std::min(f_a, f_b);
    break;
  case 0b0111:
    f_r = std::max(f_a, f_b);
    break;
  default:
    f_r = 0.0f;
  };

  int res = typePunning<int>(f_r);

  result.write(res);
}

FPU::~FPU() {}
