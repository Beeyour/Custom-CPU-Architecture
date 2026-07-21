#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum InstType { NO_ARGS, REG_REG, REG_IMM, REG_MEM, MEM_REG };

struct InstDef {
    uint16_t opcode;
    InstType type;
};

const std::map<std::string, InstDef> INSTRUCTIONS = {{"BOOT", {0x00, NO_ARGS}}, {"LOAD", {0x01, REG_MEM}}, {"STOR", {0x02, MEM_REG}}, {"MOV", {0x03, REG_REG}},
                                                     {"MOVI", {0x04, REG_IMM}}, {"ADD", {0x05, REG_REG}},  {"ADDI", {0x06, REG_IMM}}, {"ADDM", {0x07, REG_MEM}},
                                                     {"SUB", {0x08, REG_REG}},  {"SUBI", {0x09, REG_IMM}}, {"SUBM", {0x0A, REG_MEM}}};

const std::map<std::string, uint16_t> REGISTERS = {{"AX", 0x0}, {"BX", 0x1}, {"CX", 0x2}, {"DX", 0x3}, {"BP", 0x4}, {"DI", 0x5}, {"SI", 0x6}, {"SP", 0x7}};

uint16_t getRegisterCode(const std::string& regName) {
    if (REGISTERS.find(regName) == REGISTERS.end()) {
        throw std::invalid_argument("Unknown register: " + regName);
    }
    return REGISTERS.at(regName);
}

// Layout aligned with your specification: [xx: 15-14][Src: 13-10][Dest: 9-6][Opcode: 5-0]
uint16_t buildInstruction(uint16_t opcode, uint16_t dest, uint16_t src) {
    return (opcode & 0x3F) | ((dest & 0x0F) << 6) | ((src & 0x0F) << 10);
}

std::vector<std::string> tokenize(const std::string& line) {
    std::string cleanLine = line;
    std::replace(cleanLine.begin(), cleanLine.end(), ',', ' ');

    std::stringstream ss(cleanLine);
    std::vector<std::string> tokens;
    std::string token;

    while (ss >> token) {
        for (char& c : token) c = std::toupper(c);
        tokens.push_back(token);
    }
    return tokens;
}

uint16_t parseValue(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), '['), str.end());
    str.erase(std::remove(str.begin(), str.end(), ']'), str.end());

    if (str.find("0X") == 0) {
        return static_cast<uint16_t>(std::stoul(str.substr(2), nullptr, 16));
    } else if (!str.empty() && str.back() == 'H') {
        str.pop_back();
        return static_cast<uint16_t>(std::stoul(str, nullptr, 16));
    }
    return static_cast<uint16_t>(std::stoul(str));
}

int main() {
    std::ifstream inputFile("program.asm");
    std::ofstream outputFile("memory.hex");

    if (!inputFile.is_open() || !outputFile.is_open()) {
        std::cerr << "Error: File handling failure." << std::endl;
        return 1;
    }

    std::vector<uint16_t> memory;
    std::string line;
    int lineNumber = 0;

    while (std::getline(inputFile, line)) {
        lineNumber++;
        if (line.empty() || line[0] == ';') continue;

        std::vector<std::string> tokens = tokenize(line);
        if (tokens.empty()) continue;

        std::string cmd = tokens[0];

        if (INSTRUCTIONS.count(cmd)) {
            try {
                InstDef def = INSTRUCTIONS.at(cmd);
                uint16_t word1 = 0;
                uint16_t word2 = 0;
                bool requiresSecondWord = false;

                switch (def.type) {
                    case NO_ARGS:
                        word1 = buildInstruction(def.opcode, 0, 0);
                        break;
                    case REG_REG:
                        if (tokens.size() < 3) throw std::invalid_argument("Missing arguments");
                        word1 = buildInstruction(def.opcode, getRegisterCode(tokens[1]), getRegisterCode(tokens[2]));
                        break;
                    case REG_IMM:
                    case REG_MEM:
                        if (tokens.size() < 3) throw std::invalid_argument("Missing arguments");
                        word1 = buildInstruction(def.opcode, getRegisterCode(tokens[1]), 0);
                        word2 = parseValue(tokens[2]);
                        requiresSecondWord = true;
                        break;
                    case MEM_REG:
                        if (tokens.size() < 3) throw std::invalid_argument("Missing arguments");
                        word1 = buildInstruction(def.opcode, 0, getRegisterCode(tokens[2]));
                        word2 = parseValue(tokens[1]);
                        requiresSecondWord = true;
                        break;
                }

                memory.push_back(word1);
                if (requiresSecondWord) {
                    memory.push_back(word2);
                }
            } catch (const std::exception& e) {
                std::cerr << "Syntax Error at line " << lineNumber << ": " << line << "\n"
                          << "Details: " << e.what() << std::endl;
                return 1;
            }
        } else {
            std::cerr << "Unknown instruction at line " << lineNumber << ": " << cmd << std::endl;
            return 1;
        }
    }

    // Append FFFF sentinel marker to signify execution end
    memory.push_back(0xFFFF);

    while (memory.size() % 16 != 0) {
        memory.push_back(0x0000);
    }

    outputFile << "v3.0 hex words addressed\n";
    for (size_t i = 0; i < memory.size(); i += 16) {
        outputFile << std::hex << std::setfill('0') << std::setw(2) << i << ":";
        for (size_t j = 0; j < 16 && (i + j) < memory.size(); ++j) {
            outputFile << " " << std::setfill('0') << std::setw(4) << memory[i + j];
        }
        outputFile << "\n";
    }

    inputFile.close();
    outputFile.close();
    std::cout << "Assembly successful. File generated: memory.hex" << std::endl;
    return 0;
}