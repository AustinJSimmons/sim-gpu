#include "Assembler.h"
#include "SM.h"
#include "helpers.h" // Assuming you use this for typePunning floats
#include <iostream>
#include <systemc.h>
#include <vector>

// Define our SM template parameters
#define WARP_SIZE 32
#define MEM_SIZE 1024
#define NUM_INSTRUCTIONS 256
#define NUM_REG 32
#define DATA_SIZE 32

int sc_main(int argc, char *argv[]) {
  // 1. Instantiate the Clock and the Streaming Multiprocessor
  sc_clock sysclk("sysclk", 10, SC_NS);
  SM<WARP_SIZE, MEM_SIZE, NUM_INSTRUCTIONS, NUM_REG, DATA_SIZE> sm(
      "Streaming_Multiprocessor");
  sm.clk(sysclk);

  // Assembler temp stuff
  std::vector<uint32_t> kernel_bin;
  std::string kernel_path = "../../test/test_kernel.asm";
  kernel_bin = Assembler::compile(kernel_path);

  if (kernel_bin.empty()) {
    std::cout << "Failed to compile assembly" << std::endl;
    return 1;
  }

  // ==========================================
  // 2. Load the Kernel into the Instruction Cache
  // ==========================================
  sm.cu->i_cache[0] = 0x000000C0; // LOAD R3, [R0 + 0]
  sm.cu->i_cache[1] = 0x00000900; // LOAD R4, [R1 + 0]
  sm.cu->i_cache[2] = 0x000520CF; // FMUL R5, R3, R4
  sm.cu->i_cache[3] = 0x00001141; // STORE R5, [R2 + 0]

  // ==========================================
  // 3. Initialize Memory & Register Pointers
  // ==========================================
  // Array A starts at Mem[0], Array B at Mem[32], Array C at Mem[64]
  for (int t = 0; t < WARP_SIZE; t++) {
    // Thread 't' Registers
    sm.register_file->registers[t][0] = 0 + t;  // R0: pointer to A[t]
    sm.register_file->registers[t][1] = 32 + t; // R1: pointer to B[t]
    sm.register_file->registers[t][2] = 64 + t; // R2: pointer to C[t]

    // Thread 't' Data (Let's compute 2.5f * 4.0f = 10.0f)
    // We use type punning to store floats in our 32-bit integer memory
    sm.lsu->global_mem[0 + t] = typePunning<int>(2.5f);
    sm.lsu->global_mem[32 + t] = typePunning<int>(4.0f);
  }

  // ==========================================
  // 4. Run the Simulation!
  // ==========================================
  std::cout << "\n[SIM] Starting Kernel Execution..." << std::endl;

  // 4 instructions = 4 clock cycles
  sc_start(40, SC_NS);

  std::cout << "[SIM] Kernel Complete.\n" << std::endl;

  // ==========================================
  // 5. Verify the Results
  // ==========================================
  int errors = 0;
  for (int t = 0; t < WARP_SIZE; t++) {
    int raw_result = sm.lsu->global_mem[64 + t];
    float float_result = typePunning<float>(raw_result);

    if (float_result != 10.0f) {
      std::cout << "Thread " << t << " FAILED! Expected 10.0, got "
                << float_result << std::endl;
      errors++;
    }
  }

  if (errors == 0) {
    std::cout << "SUCCESS! All 32 threads computed C[i] = A[i] * B[i]"
              << std::endl;
  } else {
    std::cout << "FAILED: " << errors << " threads failed to compute correctly"
              << std::endl;
    return 1;
  }

  return 0;
}
