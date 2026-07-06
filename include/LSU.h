/*
 * Header files will be used to define the following for each component:
 * Structural Module:
 * 1. External Ports (Pins - how it connects to the rest of the GPU)
 * 2. Internal Signals (Wiring of component internals)
 * 3. Child Modules (The internal components)
 * 4. Constructor/Destructor
 *
 * Child Module:
 * 1. Ports (Pins)
 * 2. Internal State (Registers)
 * 3. Process prototypes (Logic)
 * 4. Constructor/Destructor
 * **/
#ifndef LSU_H
#define LSU_H

#include "systemc"
using namespace sc_core;
using namespace sc_dt;

template <int WARP_SIZE, int MEM_SIZE> SC_MODULE(LSU) {
  sc_in_clk clk;

  // Control Path
  sc_in<bool> is_load;
  sc_in<bool> is_store;
  sc_in<sc_uint<16>> offset;
  sc_vector<sc_in<bool>> active_mask;

  // Datapath
  sc_vector<sc_in<sc_int<32>>> base_addr;
  sc_vector<sc_in<sc_int<32>>> store_data;
  sc_vector<sc_out<sc_int<32>>> load_data;

  // GLOBAL MEMORY ~Temporary
  sc_int<32> global_mem[MEM_SIZE];

  // Constructor
  SC_CTOR(LSU)
      : active_mask("active_mask", WARP_SIZE),
        base_addr("base_addr", WARP_SIZE), store_data("store_data", WARP_SIZE),
        load_data("load_data", WARP_SIZE) {
    // Init memory to 0
    for (int i = 0; i < MEM_SIZE; i++) {
      global_mem[i] = 0;
    }
    SC_METHOD(execute);
    // sensitive << clk.pos();
    sensitive << is_load << is_store << offset;
    for (int i = 0; i < WARP_SIZE; i++) {
      sensitive << base_addr[i] << store_data[i] << active_mask[i];
    }
  }

  void execute() {
    // is memory operation?
    if (!is_load && !is_store) {
      return;
    }

    // Do memory operation for all threads in warp
    for (int i = 0; i < WARP_SIZE; i++) {
      if (active_mask[i].read() == true) {
        // Address Generation Unit (AGU) Phase: Base + Offset
        unsigned int effective_addr = base_addr[i].read() + offset.read();
        // Bounds checking to prevent segfaults during simulation
        if (effective_addr < MEM_SIZE) {
          if (is_store.read()) {
            global_mem[effective_addr] = store_data[i].read();
          } else if (is_load.read()) {
            load_data[i].write(global_mem[effective_addr]);
          }

        } else {
          std::cout << "SEGFAULT: Thread " << i
                    << " attempted to access invalid address " << effective_addr
                    << std::endl;
        }
      }
    }
  }

  // Destructor
  ~LSU(){};
};

#endif // !
