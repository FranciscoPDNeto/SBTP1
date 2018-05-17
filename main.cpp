#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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

    NoneOperandInstruction(const char *opcode) 
        : opcode(opcode) {
    }
};

class OneOperandInstruction {
public:
    std::string opcode;
    std::string operand;

    OneOperandInstruction(const char *opcode, std::function<void(T)> instructionFunction) 
        : opcode(opcode) {
    }

    OneOperandInstruction(const char *opcode, const char *operand, 
        std::function<void(T)> instructionFunction) : opcode(opcode), 
        operand(operand) {
    }
};

class TwoOperandInstruction {
public:
    std::string opcode;
    std::string firstOperand;
    std::string secondOperand;

    TwoOperandInstruction(const char *opcode) 
        : opcode(opcode) {
    }

    TwoOperandInstruction(const char *opcode, const char *firstOperand, const char *secondOperand) 
        : opcode(opcode), firstOperand(firstOperand), secondOperand(secondOperand) {
    }
};

const std::map<std::string, std::shared_ptr<NoneOperandInstruction>> noneOperandInstructions = {
    {"return", std::make_shared<NoneOperandInstruction>("1000000000000000")},
    {"stop", std::make_shared<NoneOperandInstruction>("0000000000000000")}
};
std::map<std::string, std::shared_ptr<OneOperandInstruction<int>>> oneOperandInstruction = {
    {"copytop", std::make_shared<OneOperandInstruction<int>>("10110")},
    {"call", std::make_shared<OneOperandInstruction<int>>("01111")},
    {"push", std::make_shared<OneOperandInstruction<int>>("01101")},
    {"pop", std::make_shared<OneOperandInstruction<int>>("01110")},
    {"read", std::make_shared<OneOperandInstruction<int>>("00011")},
    {"write", std::make_shared<OneOperandInstruction<int>>("00100")},
    {"jump", std::make_shared<OneOperandInstruction<int>>("01001")}
};
std::map<std::string, std::shared_ptr<TwoOperandInstruction<int,int>>> twoOperandInstruction = {
    {"store_i", std::make_shared<TwoOperandInstruction<int,int>>("10101")},
    {"load_i", std::make_shared<TwoOperandInstruction<int,int>>("10100")},
    {"load_c", std::make_shared<TwoOperandInstruction<int,int>>("10011")},
    {"store_s", std::make_shared<TwoOperandInstruction<int,int>>("10010")},
    {"load_s", std::make_shared<TwoOperandInstruction<int,int>>("10001")},
    {"move", std::make_shared<TwoOperandInstruction<int,int>>("01100")},
    {"divide", std::make_shared<TwoOperandInstruction<int,int>>("01000")},
    {"multiply", std::make_shared<TwoOperandInstruction<int,int>>("00111")},
    {"subtract", std::make_shared<TwoOperandInstruction<int,int>>("00110")},
    {"load", std::make_shared<TwoOperandInstruction<int,int>>("00001")},
    {"store", std::make_shared<TwoOperandInstruction<int,int>>("00010")},
    {"add", std::make_shared<TwoOperandInstruction<int,int>>("00101")},
    {"jmpz", std::make_shared<TwoOperandInstruction<int,int>>("01010")},
    {"jmpn", std::make_shared<TwoOperandInstruction<int,int>>("01011")}
};

std::map<std::string, std::string> addressMap = {
    {"A0", 0},
    {"A1", 1},
    {"A2", 2},
    {"A3", 3}
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

        auto label = addressMap.find(instructionCode);
        if (label != addressMap.end()) {
            lineStream >> instructionCode;
        }
    }
    return 0;
}