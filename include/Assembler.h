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

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

class Assembler {
private:
public:
  static std::vector<uint32_t> compile(const std::string &filename) {
    std::vector<uint32_t> binary; // Instruction binary

    std::map<std::string, uint32_t> opcodes = {
        {"LOAD", 0x00}, {"STORE", 0x01}, {"MOV", 0x02},  {"ADD", 0x03},
        {"SUB", 0x04},  {"MUL", 0x05},   {"CMP", 0x06},  {"AND", 0x07},
        {"OR", 0x08},   {"NOT", 0x09},   {"XOR", 0x0A},  {"LS", 0x0B},
        {"RS", 0x0C},   {"FADD", 0x0D},  {"FSUB", 0x0E}, {"FMUL", 0x0F},
        {"FDIV", 0x10}, {"FMAD", 0x11},  {"FMIN", 0x12}, {"FMAX", 0x13},
        {"JUMP", 0x22}};

    // Pass 1
    std::cout << "Starting first pass..." << std::endl;
    std::ifstream file(filename);

    if (!file.is_open()) {
      std::cerr << "ASSEMBLER Failed to open " << filename << std::endl;
      std::cerr << "Current Path " << std::filesystem::current_path();
    }

    // Track instruction count
    int instruction_count = 0;
    std::map<int, std::string> labels;
    // Read line by line
    std::string line;
    while (std::getline(file, line)) {
      std::istringstream iss(line);

      std::string label;
      std::string op;

      if (line.back() == ':') {
        std::getline(iss, label, ':');
        labels[instruction_count] = label;
        continue;
      }

      if (line.empty() || line.substr(0, 2) == "//") {
        continue;
      }

      iss >> op; // Gets first part of line before space
      if (opcodes.find(op) != opcodes.end()) {
        instruction_count++;
      }
    }
    std::cout << "First pass complete..." << std::endl;
    std::cout << "Found labels:" << std::endl;
    for (int i = 0; i <= instruction_count; i++) {
      if (labels.find(i) != labels.end()) {
        std::cout << labels.find(i)->second << " -> " << i << std::endl;
      }
    }

    // Pass 2
    file.seekg(0, std::ios::beg);
    file.clear();
    std::cout << "Starting second pass..." << std::endl;

    instruction_count = 0;
    while (std::getline(file, line)) {

      std::istringstream iss(line);

      std::string label;
      std::string op;

      // Skip labels comments and empty lines.
      if (line.empty() || line.substr(0, 2) == "//" || line.back() == ':') {
        continue;
      }

      // M-Type Decode

      // S-Type Decode

      // C-Type Decode
    }

    file.close();
    return binary;
  };
};

#endif // !ASSEMBLER_H
