#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "Function.h"
/*
class Register {
public:
    std::string name;
    std::string opcode;

    Register(const char *name, const char *opcode) : name(name), opcode(opcode){   
    }
};
*/
class NoneOperandInstruction {
public:
    std::string opcode;
    std::function<void()> instructionFunction;

    NoneOperandInstruction(const char *opcode, std::function<void()> instructionFunction) 
        : opcode(opcode), instructionFunction(instructionFunction){
    }
};

template<typename T>
class OneOperandInstruction {
public:
    std::string opcode;
    std::string operand;
    std::function<void(T)> instructionFunction;
    // std::function<void(int)> instructionFunction;

    OneOperandInstruction(const char *opcode, std::function<void(T)> instructionFunction) 
        : opcode(opcode), instructionFunction(instructionFunction){
    }

    OneOperandInstruction(const char *opcode, const char *operand, 
        std::function<void(T)> instructionFunction) : opcode(opcode), 
        operand(operand), instructionFunction(instructionFunction) {
    }
};

template<typename T, typename U>
class TwoOperandInstruction {
public:
    std::string opcode;
    std::string firstOperand;
    std::string secondOperand;
    std::function<void(T,U)> instructionFunction;

    TwoOperandInstruction(const char *opcode, std::function<void(int, int)> instructionFunction) 
        : opcode(opcode), instructionFunction(instructionFunction) {
    }

    TwoOperandInstruction(const char *opcode, const char *firstOperand, const char *secondOperand, 
        std::function<void(T,U)> instructionFunction) 
        : opcode(opcode), firstOperand(firstOperand), secondOperand(secondOperand), 
        instructionFunction(instructionFunction) {
    }
};

const std::map<std::string, std::shared_ptr<NoneOperandInstruction>> noneOperandInstructions = {
    {"return", std::make_shared<NoneOperandInstruction>("1000000000000000", &Function::returnFunction)},
    {"stop", std::make_shared<NoneOperandInstruction>("0000000000000000", &Function::stop)}
};
std::multimap<std::string, std::shared_ptr<OneOperandInstruction<int>>> oneOperandInstruction = {
    {"copytop", std::make_shared<OneOperandInstruction<int>>("10110", &Function::copytop)},
    {"call", std::make_shared<OneOperandInstruction<int>>("01111", &Function::call)},
    {"push", std::make_shared<OneOperandInstruction<int>>("01101", &Function::push)},
    {"pop", std::make_shared<OneOperandInstruction<int>>("01110", &Function::pop)},
    {"read", std::make_shared<OneOperandInstruction<int>>("00011", &Function::read)},
    {"write", std::make_shared<OneOperandInstruction<int>>("00100", &Function::write)},
    {"jump", std::make_shared<OneOperandInstruction<int>>("01001", &Function::jump)}
};
std::multimap<std::string, std::shared_ptr<TwoOperandInstruction<int,int>>> twoOperandInstruction = {
    {"store_i", std::make_shared<TwoOperandInstruction<int,int>>("10101", &Function::store_i)},
    {"load_i", std::make_shared<TwoOperandInstruction<int,int>>("10100", &Function::load_i)},
    {"load_c", std::make_shared<TwoOperandInstruction<int,int>>("10011", &Function::load_c)},
    {"store_s", std::make_shared<TwoOperandInstruction<int,int>>("10010", &Function::store_s)},
    {"load_s", std::make_shared<TwoOperandInstruction<int,int>>("10001", &Function::load_s)},
    {"move", std::make_shared<TwoOperandInstruction<int,int>>("01100", &Function::move)},
    {"divide", std::make_shared<TwoOperandInstruction<int,int>>("01000", &Function::divide)},
    {"multiply", std::make_shared<TwoOperandInstruction<int,int>>("00111", &Function::multiply)},
    {"subtract", std::make_shared<TwoOperandInstruction<int,int>>("00110", &Function::subtract)},
    {"load", std::make_shared<TwoOperandInstruction<int,int>>("00001", &Function::load)},
    {"store", std::make_shared<TwoOperandInstruction<int,int>>("00010", &Function::store)},
    {"add", std::make_shared<TwoOperandInstruction<int,int>>("00101", &Function::add)},
    {"jmpz", std::make_shared<TwoOperandInstruction<int,int>>("01010", &Function::jmpz)},
    {"jmpn", std::make_shared<TwoOperandInstruction<int,int>>("01011", &Function::jmpn)}
};

const std::map<std::string, std::string> registers = {
    {"A0", "00"},
    {"A1", "01"},
    {"A2", "10"},
    {"A3", "11"}
};

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cerr << "A argument is need." << std::endl;
        exit(1);
    }
    std::string filename(argv[1]);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(1);
    }

    for (std::string line; std::getline(file, line); ) {
        std::istringstream lineStream(line);

        std::string instructionCode;
        lineStream >> instructionCode;

        
    }
    return 0;
}