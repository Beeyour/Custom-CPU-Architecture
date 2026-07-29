#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

// ============================================================================
// 1. Instruction Formats & Metadata Definitions
// ============================================================================

enum FormatType {
    FMT_NO_ARG,
    FMT_SINGLE_DST,
    FMT_SINGLE_SRC,
    FMT_DST_SRC,
    FMT_DST_IMM16,
    FMT_DST_MEM,
    FMT_MEM_SRC,
    FMT_DST_IMM4,
    FMT_JUMP_LABEL,
    FMT_LOOP_REG_LABEL
};

struct InstSpec {
    uint16_t opcode;
    FormatType format;
    uint16_t wordCount;
};

// Complete Instruction Set Mapping (Opcodes 0x00 to 0x39)
const std::map<std::string, InstSpec> ISA = {
    // Group 1: Data Movement, Stack & I/O (0x00 - 0x09)
    {"BOOT", {0x00, FMT_NO_ARG, 1}},
    {"NOP", {0x00, FMT_NO_ARG, 1}},
    {"LOAD", {0x01, FMT_DST_MEM, 2}},
    {"STOR", {0x02, FMT_MEM_SRC, 2}},
    {"MOV", {0x03, FMT_DST_SRC, 1}},
    {"MOVI", {0x04, FMT_DST_IMM16, 2}},
    {"PUSH", {0x05, FMT_SINGLE_SRC, 1}},
    {"POP", {0x06, FMT_SINGLE_DST, 1}},
    {"XCHG", {0x07, FMT_DST_SRC, 1}},
    {"INN", {0x08, FMT_SINGLE_DST, 1}},
    {"OUTT", {0x09, FMT_SINGLE_SRC, 1}},

    // Group 2: ALU & Comparison Operations (0x0A - 0x27)
    {"ADD", {0x0A, FMT_DST_SRC, 1}},
    {"ADDI", {0x0B, FMT_DST_IMM16, 2}},
    {"ADDM", {0x0C, FMT_DST_MEM, 2}},
    {"SUB", {0x0D, FMT_DST_SRC, 1}},
    {"SUBI", {0x0E, FMT_DST_IMM16, 2}},
    {"SUBM", {0x0F, FMT_DST_MEM, 2}},
    {"MUL", {0x10, FMT_DST_SRC, 1}},
    {"MULI", {0x11, FMT_DST_IMM16, 2}},
    {"DIV", {0x12, FMT_DST_SRC, 1}},
    {"DIVI", {0x13, FMT_DST_IMM16, 2}},
    {"INC", {0x14, FMT_SINGLE_DST, 1}},
    {"DEC", {0x15, FMT_SINGLE_DST, 1}},
    {"ANDD", {0x16, FMT_DST_SRC, 1}},
    {"ANDI", {0x17, FMT_DST_IMM16, 2}},
    {"ORR", {0x18, FMT_DST_SRC, 1}},
    {"ORI", {0x19, FMT_DST_IMM16, 2}},
    {"XORR", {0x1A, FMT_DST_SRC, 1}},
    {"XORI", {0x1B, FMT_DST_IMM16, 2}},
    {"NOTT", {0x1C, FMT_SINGLE_DST, 1}},
    {"SHL", {0x1D, FMT_DST_SRC, 1}},
    {"SHLI", {0x1E, FMT_DST_IMM4, 1}},
    {"SHR", {0x1F, FMT_DST_SRC, 1}},
    {"SHRI", {0x20, FMT_DST_IMM4, 1}},
    {"ROLL", {0x21, FMT_DST_SRC, 1}},
    {"ROLI", {0x22, FMT_DST_IMM4, 1}},
    {"RORR", {0x23, FMT_DST_SRC, 1}},
    {"RORI", {0x24, FMT_DST_IMM4, 1}},
    {"CMP", {0x25, FMT_DST_SRC, 1}},
    {"CMPI", {0x26, FMT_DST_IMM16, 2}},
    {"TEST", {0x27, FMT_DST_SRC, 1}},

    // Group 3: Control Flow & Branching (0x28 - 0x35)
    {"JMP", {0x28, FMT_JUMP_LABEL, 2}},
    {"JZ", {0x29, FMT_JUMP_LABEL, 2}},
    {"JE", {0x29, FMT_JUMP_LABEL, 2}},
    {"JNZ", {0x2A, FMT_JUMP_LABEL, 2}},
    {"JNE", {0x2A, FMT_JUMP_LABEL, 2}},
    {"JC", {0x2B, FMT_JUMP_LABEL, 2}},
    {"JNC", {0x2C, FMT_JUMP_LABEL, 2}},
    {"JS", {0x2D, FMT_JUMP_LABEL, 2}},
    {"JNS", {0x2E, FMT_JUMP_LABEL, 2}},
    {"JA", {0x2F, FMT_JUMP_LABEL, 2}},
    {"JG", {0x2F, FMT_JUMP_LABEL, 2}},
    {"REF", {0x30, FMT_JUMP_LABEL, 2}},
    {"JL", {0x30, FMT_JUMP_LABEL, 2}},
    {"JAE", {0x31, FMT_JUMP_LABEL, 2}},
    {"JBE", {0x32, FMT_JUMP_LABEL, 2}},
    {"CALL", {0x33, FMT_JUMP_LABEL, 2}},
    {"RET", {0x34, FMT_NO_ARG, 1}},
    {"LOOP", {0x35, FMT_LOOP_REG_LABEL, 2}},

    // Group 4: Flags & System Operations (0x36 - 0x39)
    {"CLC", {0x36, FMT_NO_ARG, 1}},
    {"STC", {0x37, FMT_NO_ARG, 1}},
    {"CLR", {0x38, FMT_SINGLE_DST, 1}},
    {"HALT", {0x39, FMT_NO_ARG, 1}}};

const std::unordered_map<std::string, uint16_t> REGISTERS = {{"AX", 0x0}, {"BX", 0x1}, {"CX", 0x2}, {"DX", 0x3},   {"BP", 0x4},
                                                             {"DI", 0x5}, {"SI", 0x6}, {"SP", 0x7}, {"FLAG", 0xF}, {"FLAGS", 0xF}};

std::unordered_map<std::string, uint16_t> symbolTable;

// ============================================================================
// 2. String & Tokenization Helpers
// ============================================================================

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string removeComments(const std::string& line) {
    size_t pos = line.find(';');
    if (pos != std::string::npos) return line.substr(0, pos);
    pos = line.find("//");
    if (pos != std::string::npos) return line.substr(0, pos);
    return line;
}

std::string toUpper(std::string str) {
    for (char& c : str) c = static_cast<char>(std::toupper(c));
    return str;
}

uint16_t getRegisterCode(const std::string& regName) {
    std::string upperReg = toUpper(regName);
    if (REGISTERS.find(upperReg) == REGISTERS.end()) {
        throw std::invalid_argument("Unknown register ID: '" + regName + "'");
    }
    return REGISTERS.at(upperReg);
}

uint16_t buildWord(uint16_t opcode, uint16_t dest, uint16_t src) {
    return (opcode & 0x3F) | ((dest & 0x0F) << 6) | ((src & 0x0F) << 10);
}

std::vector<std::string> tokenize(const std::string& line) {
    std::string clean = removeComments(line);
    std::replace(clean.begin(), clean.end(), ',', ' ');

    std::vector<std::string> tokens;
    std::string current;
    bool inBracket = false;

    for (char c : clean) {
        if (c == '[')
            inBracket = true;
        else if (c == ']')
            inBracket = false;

        // إذا كانت مسافة ونحن لسنا داخل أقواس، ننهي الكلمة
        if (std::isspace(c) && !inBracket) {
            if (!current.empty()) {
                tokens.push_back(trim(current));
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) {
        tokens.push_back(trim(current));
    }
    return tokens;
}

// Split data arguments by comma while preserving quotes and DUP parens
std::vector<std::string> splitDataArgs(const std::string& str) {
    std::vector<std::string> args;
    std::string current;
    bool inSingleQuote = false;
    bool inDoubleQuote = false;
    int parenDepth = 0;

    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i];
        if (c == '\'' && !inDoubleQuote)
            inSingleQuote = !inSingleQuote;
        else if (c == '"' && !inSingleQuote)
            inDoubleQuote = !inDoubleQuote;
        else if (c == '(' && !inSingleQuote && !inDoubleQuote)
            parenDepth++;
        else if (c == ')' && !inSingleQuote && !inDoubleQuote)
            parenDepth--;

        if (c == ',' && !inSingleQuote && !inDoubleQuote && parenDepth == 0) {
            args.push_back(trim(current));
            current.clear();
        } else {
            current += c;
        }
    }
    if (!trim(current).empty()) {
        args.push_back(trim(current));
    }
    return args;
}

// ============================================================================
// 3. Advanced Numerical & Data Parsing Engine
// ============================================================================

uint16_t parseLiteralOrLabel(std::string str, bool pass2) {
    str.erase(std::remove(str.begin(), str.end(), '['), str.end());
    str.erase(std::remove(str.begin(), str.end(), ']'), str.end());
    str = trim(str);

    // ==========================================
    // دعم العمليات الحسابية (مثل: word1 + 1)
    // ==========================================
    size_t plusPos = str.rfind('+');
    if (plusPos != std::string::npos) {
        return parseLiteralOrLabel(str.substr(0, plusPos), pass2) + parseLiteralOrLabel(str.substr(plusPos + 1), pass2);
    }
    size_t minusPos = str.rfind('-');
    if (minusPos != std::string::npos && minusPos > 0) {
        return parseLiteralOrLabel(str.substr(0, minusPos), pass2) - parseLiteralOrLabel(str.substr(minusPos + 1), pass2);
    }

    // Remove underscore separators
    std::string cleanStr;
    for (char c : str)
        if (c != '_') cleanStr += c;

    std::string upperStr = toUpper(cleanStr);

    // Symbol / Label Lookup
    if (symbolTable.find(upperStr) != symbolTable.end()) {
        return symbolTable.at(upperStr);
    }

    // Binary Literal (e.g. 01011010b or 0b01011010)
    if (!upperStr.empty() && upperStr.back() == 'B' && upperStr.find("0X") == std::string::npos) {
        try {
            return static_cast<uint16_t>(std::stoul(upperStr.substr(0, upperStr.size() - 1), nullptr, 2));
        } catch (...) {
        }
    }
    if (upperStr.rfind("0B", 0) == 0) {
        try {
            return static_cast<uint16_t>(std::stoul(upperStr.substr(2), nullptr, 2));
        } catch (...) {
        }
    }

    // Hexadecimal Parsing (0x1234 or 1234h)
    if (upperStr.rfind("0X", 0) == 0) {
        return static_cast<uint16_t>(std::stoul(upperStr.substr(2), nullptr, 16));
    }
    if (!upperStr.empty() && upperStr.back() == 'H') {
        return static_cast<uint16_t>(std::stoul(upperStr.substr(0, upperStr.size() - 1), nullptr, 16));
    }

    // Decimal Parsing
    try {
        return static_cast<uint16_t>(std::stoi(cleanStr));
    } catch (...) {
        if (pass2) {
            throw std::invalid_argument("Unresolved label or invalid literal: '" + str + "'");
        }
        return 0x0000;
    }
}
// Parse DW Data Elements (Supports numbers, strings, DUP)
std::vector<uint16_t> parseDataElement(std::string arg, bool pass2) {
    std::vector<uint16_t> words;
    arg = trim(arg);

    // 1. Handle DUP expression: e.g., "5 dup(0)" or "3 dup('a')"
    std::string upperArg = toUpper(arg);
    size_t dupPos = upperArg.find(" DUP");
    if (dupPos == std::string::npos) dupPos = upperArg.find("\tDUP");
    if (dupPos == std::string::npos && upperArg.find("DUP(") != std::string::npos) {
        dupPos = upperArg.find("DUP(");
    }

    if (dupPos != std::string::npos) {
        std::string countStr = trim(arg.substr(0, dupPos));
        size_t openParen = arg.find('(', dupPos);
        size_t closeParen = arg.rfind(')');
        if (openParen != std::string::npos && closeParen != std::string::npos && closeParen > openParen) {
            std::string valStr = trim(arg.substr(openParen + 1, closeParen - openParen - 1));
            uint16_t count = parseLiteralOrLabel(countStr, pass2);
            std::vector<uint16_t> innerWords = parseDataElement(valStr, pass2);
            for (uint16_t i = 0; i < count; ++i) {
                for (uint16_t w : innerWords) {
                    words.push_back(w);
                }
            }
            return words;
        }
    }

    // 2. Handle String / Character Literals: 'hhfs' or "b"
    if ((arg.front() == '\'' && arg.back() == '\'') || (arg.front() == '"' && arg.back() == '"')) {
        std::string content = arg.substr(1, arg.length() - 2);
        if (content.empty()) {
            words.push_back(0x0000);
            return words;
        }
        for (size_t i = 0; i < content.length(); i += 2) {
            uint16_t word = static_cast<uint8_t>(content[i]);
            if (i + 1 < content.length()) {
                word = (word << 8) | static_cast<uint8_t>(content[i + 1]);
            }
            words.push_back(word);
        }
        return words;
    }

    // 3. Handle Standard Numeric Literals / Labels
    words.push_back(parseLiteralOrLabel(arg, pass2));
    return words;
}

// Parse entire line containing DW data
std::vector<uint16_t> parseDWLine(const std::string& dataStr, bool pass2) {
    std::vector<uint16_t> lineWords;
    std::vector<std::string> args = splitDataArgs(dataStr);
    for (const auto& arg : args) {
        std::vector<uint16_t> parsed = parseDataElement(arg, pass2);
        lineWords.insert(lineWords.end(), parsed.begin(), parsed.end());
    }
    return lineWords;
}

// ============================================================================
// 4. Main Two-Pass Assembly Pipeline
// ============================================================================

int main(int argc, char* argv[]) {
    std::string inputPath = "programs/program.asm";
    std::string outputPath = "programs/memory.hex";

    // Only override paths if they do NOT start with '-' (ignoring flag arguments)
    if (argc >= 2 && argv[1][0] != '-') inputPath = argv[1];
    if (argc >= 3 && argv[2][0] != '-') outputPath = argv[2];

    std::cout << "========================================================\n";
    std::cout << " 16-Bit Custom Processor Assembler Pipeline\n";
    std::cout << " Source: " << inputPath << "\n";
    std::cout << " Target: " << outputPath << "\n";
    std::cout << "========================================================\n";

    // ------------------------------------------------------------------------
    // PASS 1: Symbol Table & Address Calculation
    // ------------------------------------------------------------------------
    {
        std::ifstream pass1File(inputPath);
        if (!pass1File.is_open()) {
            std::cerr << "Fatal Error: Unable to open input assembly file: " << inputPath << "\n";
            return 1;
        }

        std::string line;
        uint16_t currentAddress = 0;
        int lineNum = 0;

        while (std::getline(pass1File, line)) {
            lineNum++;
            std::string cleanLine = removeComments(line);
            cleanLine = trim(cleanLine);
            if (cleanLine.empty()) continue;

            // Extract Label if present
            size_t colonPos = cleanLine.find(':');
            if (colonPos != std::string::npos) {
                std::string labelName = trim(cleanLine.substr(0, colonPos));
                labelName = toUpper(labelName);

                if (symbolTable.count(labelName)) {
                    std::cerr << "Pass 1 Error (Line " << lineNum << "): Duplicate label definition '" << labelName << "'\n";
                    return 1;
                }

                symbolTable[labelName] = currentAddress;
                cleanLine = trim(cleanLine.substr(colonPos + 1));
            }

            if (cleanLine.empty()) continue;

            std::vector<std::string> tokens = tokenize(cleanLine);
            if (tokens.empty()) continue;

            std::string mnemonic = toUpper(tokens[0]);

            // Check for Data Directive (DW) or Implicit Data Definition
            if (mnemonic == "DW") {
                size_t dwPos = cleanLine.find("DW");
                if (dwPos == std::string::npos) dwPos = cleanLine.find("dw");
                std::string dataBody = cleanLine.substr(dwPos + 2);
                std::vector<uint16_t> dwWords = parseDWLine(dataBody, false);
                currentAddress += dwWords.size();
            } else if (ISA.find(mnemonic) != ISA.end()) {
                currentAddress += ISA.at(mnemonic).wordCount;
            } else {
                // Check if it's an implicit DW without the DW keyword
                std::vector<uint16_t> implicitDW = parseDWLine(cleanLine, false);
                currentAddress += implicitDW.size();
            }
        }
        pass1File.close();
    }

    // ------------------------------------------------------------------------
    // PASS 2: Machine Code Generation
    // ------------------------------------------------------------------------
    std::vector<uint16_t> machineCode;
    {
        std::ifstream pass2File(inputPath);
        std::string line;
        int lineNum = 0;

        while (std::getline(pass2File, line)) {
            lineNum++;
            std::string cleanLine = removeComments(line);
            cleanLine = trim(cleanLine);
            if (cleanLine.empty()) continue;

            // Strip Label
            size_t colonPos = cleanLine.find(':');
            if (colonPos != std::string::npos) {
                cleanLine = trim(cleanLine.substr(colonPos + 1));
            }
            if (cleanLine.empty()) continue;

            std::vector<std::string> tokens = tokenize(cleanLine);
            if (tokens.empty()) continue;

            std::string mnemonic = toUpper(tokens[0]);

            // Process Data Directives (DW)
            if (mnemonic == "DW") {
                size_t dwPos = cleanLine.find("DW");
                if (dwPos == std::string::npos) dwPos = cleanLine.find("dw");
                std::string dataBody = cleanLine.substr(dwPos + 2);
                std::vector<uint16_t> dwWords = parseDWLine(dataBody, true);
                machineCode.insert(machineCode.end(), dwWords.begin(), dwWords.end());
                continue;
            }

            // Process Standard Instructions
            if (ISA.find(mnemonic) != ISA.end()) {
                InstSpec spec = ISA.at(mnemonic);
                try {
                    switch (spec.format) {
                        case FMT_NO_ARG:
                            machineCode.push_back(buildWord(spec.opcode, 0, 0));
                            break;

                        case FMT_SINGLE_DST:
                            if (tokens.size() < 2) throw std::invalid_argument("Missing destination register argument");
                            machineCode.push_back(buildWord(spec.opcode, getRegisterCode(tokens[1]), 0));
                            break;

                        case FMT_SINGLE_SRC:
                            if (tokens.size() < 2) throw std::invalid_argument("Missing source register argument");
                            machineCode.push_back(buildWord(spec.opcode, 0, getRegisterCode(tokens[1])));
                            break;

                        case FMT_DST_SRC:
                            if (tokens.size() < 3) throw std::invalid_argument("Requires <DST> and <SRC> arguments");
                            machineCode.push_back(buildWord(spec.opcode, getRegisterCode(tokens[1]), getRegisterCode(tokens[2])));
                            break;

                        case FMT_DST_IMM16:
                        case FMT_DST_MEM:
                            if (tokens.size() < 3) throw std::invalid_argument("Requires <DST> and <IMM16/ADDR> arguments");
                            machineCode.push_back(buildWord(spec.opcode, getRegisterCode(tokens[1]), 0));
                            machineCode.push_back(parseLiteralOrLabel(tokens[2], true));
                            break;

                        case FMT_MEM_SRC:
                            if (tokens.size() < 3) throw std::invalid_argument("Requires [<ADDR>] and <SRC> arguments");
                            machineCode.push_back(buildWord(spec.opcode, 0, getRegisterCode(tokens[2])));
                            machineCode.push_back(parseLiteralOrLabel(tokens[1], true));
                            break;

                        case FMT_DST_IMM4:
                            if (tokens.size() < 3) throw std::invalid_argument("Requires <DST> and <IMM4> arguments");
                            {
                                uint16_t imm4 = parseLiteralOrLabel(tokens[2], true) & 0x0F;
                                machineCode.push_back(buildWord(spec.opcode, getRegisterCode(tokens[1]), imm4));
                            }
                            break;

                        case FMT_JUMP_LABEL:
                            if (tokens.size() < 2) throw std::invalid_argument("Missing target <LABEL/ADDR>");
                            machineCode.push_back(buildWord(spec.opcode, 0, 0));
                            machineCode.push_back(parseLiteralOrLabel(tokens[1], true));
                            break;

                        case FMT_LOOP_REG_LABEL:
                            if (tokens.size() < 3) throw std::invalid_argument("Requires <REG> and <LABEL/ADDR>");
                            machineCode.push_back(buildWord(spec.opcode, getRegisterCode(tokens[1]), 0));
                            machineCode.push_back(parseLiteralOrLabel(tokens[2], true));
                            break;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Syntax Error (Line " << lineNum << "): " << line << "\n";
                    std::cerr << "  Details: " << e.what() << "\n";
                    return 1;
                }
            } else {
                // Implicit Data Definition
                std::vector<uint16_t> implicitWords = parseDWLine(cleanLine, true);
                machineCode.insert(machineCode.end(), implicitWords.begin(), implicitWords.end());
            }
        }
        pass2File.close();
    }

    // Append 0xFFFF sentinel marker to halt execution
    machineCode.push_back(0xFFFF);

    // Pad memory image to full 16-word blocks
    while (machineCode.size() % 16 != 0) {
        machineCode.push_back(0x0000);
    }

    // Write Logisim Evolution v3.0 Hex Image
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "Fatal Error: Unable to write destination file: " << outputPath << "\n";
        return 1;
    }

    outFile << "v3.0 hex words addressed\n";
    for (size_t i = 0; i < machineCode.size(); i += 16) {
        outFile << std::hex << std::setfill('0') << std::setw(2) << i << ":";
        for (size_t j = 0; j < 16 && (i + j) < machineCode.size(); ++j) {
            outFile << " " << std::setfill('0') << std::setw(4) << machineCode[i + j];
        }
        outFile << "\n";
    }
    outFile.close();

    std::cout << "SUCCESS: Processed " << machineCode.size() << " hex words successfully.\n";
    std::cout << "Output File Generated: " << outputPath << "\n";
    return 0;
}