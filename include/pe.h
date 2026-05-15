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
#ifndef PE_H
#define PE_H

#include <systemc.h>
// Must include child Modules

SC_MODULE(ProcessingElement) {
  // 1. Ports (how it connects to the rest of the GPU)

  // 2. Internal Signals (Wiring)

  // 3. Child Modules (Internal Components)

  // Constructor prototype
  SC_CTOR(ProcessingElement);

  // Destructor prototype
  ~ProcessingElement();
};

#endif
