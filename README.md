# sim-gpu

A project inspired by tiny-gpu. The aim is to essentially design and simulate a
gpu from scratch in C++. Really just to hammer into my brain C++ concepts while
having a interesting and fun structured end goal and consequently learn lots
about how GPUs actually work on a hardware level. This is an in progress project
so this README will be messy at times until I understand the general direction to
go.

### Table-Of-Contents

- [Overview](#overview)
- [Architecture](#architecture)
  - [GPU Architecture (High Level)](#gpu-architecture)
  - [Compute Unit Architecture](#compute-unit)
  - [ISA Attempt](#isa-attempt)
- [Simulation](#simulation)
- [TODO](#todo)

# Overview

# Architecture

## GPU Architecture

![High Level GPU Diagram](docs/High-Level-GPU-Architecture.drawio.svg)

## Compute Unit Architecture

![High Level Compute Unit Architecture Diagram](docs/Compute-Unit.drawio.svg)

## ISA Attempt

For this project I plan to simulate a 32bit instruction set below the table of planned
supported operators will be a couple bitmaps describing the layout of specific instruction
types.

| Mnemonic | Op-code | Instruction Type | Description |
| :--- | :--- | :--- | :--- |
| `LOAD` | `0x00` | S-Type | reg_src ->  mem_addr |
| `STORE` | `0x01` | S-Type | mem_addr -> reg_dest |
| `MOV` | `0x02` | M-Type | reg_src -> reg_dest |
| `ADD` | `0x03` | M-Type | reg_dest = reg_dest + reg_src |
| `SUB` | `0x04` | M-Type | reg_dest = reg_dest - reg_src |
| `MUL` | `0x05` | M-Type | reg_dest *= reg_src |
| `JUMP` | `0x06` | J-Type | idk yet but involves mem_addr and predicates |
| `AND` | `0x07` | M-Type | reg_dest = reg_dest AND reg_src |
| `OR` | `0x08` | M-Type | reg_dest = reg_dest OR reg_src |
| `NOT` | `0x09` | M-Type | reg_dest = NOT reg_dest |
| `XOR` | `0x09` | M-Type | reg_dest = reg_dest XOR reg_src |

#### S-Type Instruction

S-type instructions like LOAD and STORE access memory and either a source or destination
register.

![S-Type Bitmap layout](docs/S-Type.svg#gh-light-mode-only)
![S-Type Bitmap layout](docs/S-type-white.svg#gh-dark-mode-only)

#### M-Type Instruction

M-type instructions access two registers (source and destination) and performs
some operation on the values within each, storing the output at the destination
register. MOV just copies the contents of the source to the destination.

![M-Type Bitmap layout](docs/M-Type.svg#gh-light-mode-only)
![M-Type Bitmap layout](docs/M-type-white.svg#gh-dark-mode-only)

#### J-Type Instruction

J-Type instructions use predicates and a memory address to jump around in
memory. Still don't know if this is useful for my GPU simulation yet nor how it
works.

![J-Type Bitmap layout](docs/J-Type.svg#gh-light-mode-only)
![J-Type Bitmap layout](docs/J-type-white.svg#gh-dark-mode-only)

# Simulation

# TODO (High Level)

- [ ] Look into SystemC more in depth for discrete event simulation.
- [ ] Design a basic GPU in SystemC using cycle accurate modeling ideally.
