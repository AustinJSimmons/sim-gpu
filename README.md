# sim-gpu

A project inspired by tiny-gpu. The aim is to essentially design and simulate a
gpu from scratch in C++. Really just to hammer into my brain C++ concepts while
having a interesting and fun structured end goal and consequently learn lots
about how GPUs actually work on a hardware level. This is an in progress project
so this README will be messy at times until I understand the general direction to
go.

## Table-Of-Contents

- [Overview](#overview)
- [Architecture](#architecture)
  - [GPU Architecture (High Level)](#gpu-architecture)
  - [Compute Unit Architecture](#compute-unit-architecture)
  - [ISA Attempt](#isa-attempt)
- [Simulation](#simulation)
- [TODO](#todo)

## Overview

Since this project is for self-education, the architecture section will
contain a combination of diagrams and explanations of various parts of
GPU/computer architecture. Each will be updated as my understanding grows.
The final goal being a one stop shop for details on how modern GPUs function.

We'll start with a high level GPU architecture diagram and then move to deeper,
and deeper levels of granularity. Stopping just short of the logic gate level.
Where possible I will try and answer the following questions about each component:

1. What does it do?
2. What are its component parts?
3. Why does a GPU need it?

Hopefully answering these questions can let any reader (myself included) get a
deeper understanding of how GPUs work and how their underlying architecture is
designed.

## Architecture

### GPU Architecture

![High Level GPU Diagram](docs/High-Level-GPU-Architecture.drawio.svg)

#### Compute Unit Architecture

Compute units are roughly analogous to CPU cores. They handle parallel
execution of instructions across many PEs (Processing Elements). In my design
they contain a Control Unit, register file, L1 Cache, and many PEs. Some modern
examples would be NVIDEAs SMs (Streaming Multiprocessors) and AMDs CUs (Compute
Units). While they function essentially like cores in a CPU, the big difference
in a GPU is the parallel execution of instructions.

##### Control Flow of the CU

1. Program (maybe warp level?) instructions are written to CU control unit
   instruction cache.
2. The decoder fetches instructions from the instruction cache.
3. The decoder decodes these instructions into hardware control signal
   instructions.
4. These instructions are put in a FiFo (or something similar) queue which,
   accessible by the dispatcher.
5. The dispatcher sends out the control signal instructions to the scheduler
   which decides what resources are available to carry out said instructions.
6. The scheduler then informs the dispatcher where to send the in hardware.

##### Control Unit

![High Level Compute Unit Architecture Diagram](docs/Compute-Unit.drawio.svg)

The control unit is essentially the brain of each of our Compute Units. Its
responsible for fetching and decoding program instructions and dispatching these
to each PE for execution. It contains an L1/Instruction cache, decoder, scheduler
and dispatcher.

##### Processing Element

Processing Elements are the GPUs workhorse. They receive and execute instructions
to transform, and move data across registers and memory. It contains (in my design)
an ALU (Arithmetic Logic Unit), FPU and LSU.

###### ALU

The Arithmetic Logic Unit is responsible for executing arithmetic operations
(ADD, MUL, SUB) and logic operations (AND, OR, NOT). Its also responsible for
moving data between registers. Its made up of a series of logic gates
specialized to perform these actions.

###### FPU

The Floating Point Unit is responsible for executing floating point operations
(FMUL, FADD, FMAD). Like the ALU its made up of specialized logic gates to perform
these operations. GPUs need floating point units because a lot of floating point
math is used in graphics pipelines and AI models.

##### Register File

In our simulation, we have an instruction set that assigns 5 bits per register in
an instruction. Thus we have $2^5 = 32$ addressable registers per thread.
We'll call these R0 -> R31. In my first iteration we just want to execute a single
thread on a set of 2 PEs and so our register file will only have 32 addressable
registers that can be accessed by the thread but this can be scaled up.

### ISA Attempt

For this project I plan to simulate a 32bit instruction set below the table of planned
supported operators will be a couple bitmaps describing the layout of specific instruction
types.

| Mnemonic | Op-code | Instruction Type | Description |
| :--- | :--- | :--- | :--- |
| `LOAD` | `0x00` | S-Type | (base_reg * offset) = mem_addr -> rd |
| `STORE` | `0x01` | S-Type | src-> (base_reg * offset) = mem_addr |
| `MOV` | `0x02` | M-Type | src1 -> rd |
| `ADD` | `0x03` | M-Type | rd = src1 + src2 |
| `SUB` | `0x04` | M-Type | rd = src1 - src2 |
| `MUL` | `0x05` | M-Type | rd *= src2 |
| `` | `0x06` | -Type | |
| `AND` | `0x07` | M-Type | rd = src1 AND src2 |
| `OR` | `0x08` | M-Type | rd = src1 OR src2 |
| `NOT` | `0x09` | M-Type | rd = NOT rd |
| `XOR` | `0x0A` | M-Type | rd = src1 XOR src2 |
| `LS` | `0x0B` | M-Type | rd = src1 << src2 |
| `RS` | `0x0C` | M-Type | rd = src1 >> src2 |
| `FADD` | `0x0D` | M-Type | rd = src1 + src2 |
| `FSUB` | `0x0E` | M-Type | rd = src1 - src2 |
| `FMUL` | `0x0F` | M-Type | rd = src1 * src2 |
| `FDIV` | `0x10` | M-Type | rd = src1/src2 |
| `FMAD` | `0x11` | M-Type | rd = (rd * src1) + src2 |
| `FMIN` | `0x12` | M-Type | rd = MIN(src1, src2) |
| `FMAX` | `0x13` | M-Type | rd = MAX(src1, src2) |
| `` | `0x14` | -Type | |
| `` | `0x15` | -Type | |
| `` | `0x16` | -Type | |
| `` | `0x17` | -Type | |
| `` | `0x18` | -Type | |
| `` | `0x19` | -Type | |
| `` | `0x1A` | -Type | |
| `` | `0x1B` | -Type | |
| `` | `0x1C` | -Type | |
| `` | `0x1D` | -Type | |
| `` | `0x1E` | -Type | |
| `` | `0x1F` | -Type | |
| `` | `0x20` | -Type | |
| `` | `0x21` | -Type | |
| `JUMP` | `0x22` | J-Type | idk yet |

#### S-Type Instruction

S-type instructions like LOAD and STORE access memory using a base register
holding a 32bit memory address times the immediate offset. This is used to find
where to store data from a source register or what to load into a destination register.

![S-Type Bitmap layout](docs/S-Type.svg#gh-light-mode-only)
![S-Type Bitmap layout](docs/S-Type-W.svg#gh-dark-mode-only)

#### M-Type Instruction

M-type instructions access up to three registers (src1, src2 and rd) and performs
some operation on the values within each, storing the output at the destination
register. MOV just copies the contents of src1 to the rd.

![M-Type Bitmap layout](docs/M-Type.svg#gh-light-mode-only)
![M-Type Bitmap layout](docs/M-Type-W.svg#gh-dark-mode-only)

#### C-Type Instruction

C-Type instructions use predicates and a memory address to jump around in
the instruction cache. Still don't know if this is useful for my GPU simulation
yet nor how it works.

![C-Type Bitmap layout](docs/J-Type.svg#gh-light-mode-only)
![C-Type Bitmap layout](docs/C-Type-W.svg#gh-dark-mode-only)

## Simulation

## TODO

- [x] Figure out how to functionally test modules in systemc.
- [ ] Get a functional simulation done first without worrying about cycle timing
      accuracy.
  - [ ] Processing Element simulated and tested (Functional)
    - [x] ALU simulated and tested
    - [x] FPU simulated and tested
    - [ ] LSU simulated and tested
    - [ ] SFU ~ If needed
  - [ ] Compute unit/SM simulated and tested
    - [ ] Control unit simulated and tested
      - [ ] Instruction Cache
      - [ ] Decoder
      - [ ] Scheduler
      - [ ] Dispatcher
    - [ ] Register File Simulated and tested
    - [ ] L1 Cache simulated and tested
  - [ ] GPU simulated and tested
    - [ ] Scheduler (warp?) simulated and tested
    - [ ] L2 Cache
    - [ ] Memory Controller
    - [ ] High-speed Bus?
- [ ] Implement Cycle Accurate simulation methods after functional Simulation is
      complete.
