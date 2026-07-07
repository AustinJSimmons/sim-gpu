/*
 * Assembler.h is used in test modules to make it easier to write custom
 * ASM instructions from my ISA to test kernels on the simulated GPU.
 *
 * Pseudo code 1st attempt:
 * 1. open .asm file
 * 2. read line by line
 *  i. First token = ISA keyword before first space
 *  ii. Based on ISA keyword parse the rest of the line based on instruction
 * type iii. convert keyword, registers, offsets etc into 32bit binary
 * instruction iv. store in an array or vector of some data type that can hold
 * 32bit binary instructions uint32?
 * 3. repeat i - iv until EOF
 * 4. return array/vector of 32bit instructions
 *
 * Problems: Nothing to handle labels for branch instructions.
 *
 * Solution: Two pass assembler architecture
 * 1st pass: Build symbol tabel for labels
 * 2nd pass: Generate Binary
 *
 * 1st pass pseudo code:
 * 1. open .asm file
 * 2. read file line by line
 *  i. track current instruction count and increment on every valid instruction
 * found ii. When a label is found TEMPLATE: (':' marks them) save it to a
 * hash-table/map with the current instruction count repeat till EOF
 *
 * 2nd pass pseudo code:
 * 1. Read line
 * 2. Check keyword:
 *  i. if keyword is M or S type, decode normally
 *  ii. else if keyword is C type JUMP or BLT etc look at target and calculate
 * offset
 * 3. store instruction in 32bit vector or array
 * repeat until EOF
 *
 **/

#ifndef ASSEMBLER_H
#define ASSEMBLER_H

class Assembler {
private:
public:
  // We need a function to compile kernel.asm files using our custom ISA.
};

#endif // !ASSEMBLER_H
