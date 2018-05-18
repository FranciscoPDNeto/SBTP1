#include <bitset>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class NoneOperandInstruction {
public:
    std::bitset<16> opcode;

    NoneOperandInstruction(const std::bitset<16> opcode) 
        : opcode(opcode) {
    }
};

class OneOperandInstruction {
public:
    std::bitset<5> opcode;
    std::bitset<11> firstOperand;

    OneOperandInstruction(const std::bitset<5> opcode) 
        : opcode(opcode) {
    }

    OneOperandInstruction(const std::bitset<5> opcode, const std::bitset<11> operand) : opcode(opcode), 
        firstOperand(operand) {
    }
};

class TwoOperandInstruction {
public:
    std::bitset<5> opcode;
    std::bitset<2> firstOperand;
    std::bitset<9> secondOperand;

    TwoOperandInstruction(const std::bitset<5> opcode) 
        : opcode(opcode) {
    }

    TwoOperandInstruction(const std::bitset<5> opcode, const std::bitset<2> firstOperand, const std::bitset<9> secondOperand) 
        : opcode(opcode), firstOperand(firstOperand), secondOperand(secondOperand) {
    }
};

const std::map<std::string, std::shared_ptr<NoneOperandInstruction>> noneOperandInstructions = {
    {"return", std::make_shared<NoneOperandInstruction>(std::bitset<16>("1000000000000000"))},
    {"stop", std::make_shared<NoneOperandInstruction>(std::bitset<16>("0000000000000000"))}
};
std::map<std::string, std::shared_ptr<OneOperandInstruction>> oneOperandInstruction = {
    {"copytop", std::make_shared<OneOperandInstruction>(std::bitset<5>("10110"))},
    {"call", std::make_shared<OneOperandInstruction>(std::bitset<5>("01111"))},
    {"push", std::make_shared<OneOperandInstruction>(std::bitset<5>("01101"))},
    {"pop", std::make_shared<OneOperandInstruction>(std::bitset<5>("01110"))},
    {"read", std::make_shared<OneOperandInstruction>(std::bitset<5>("00011"))},
    {"write", std::make_shared<OneOperandInstruction>(std::bitset<5>("00100"))},
    {"jump", std::make_shared<OneOperandInstruction>(std::bitset<5>("01001"))}
};
std::map<std::string, std::shared_ptr<TwoOperandInstruction>> twoOperandInstruction = {
    {"store_i", std::make_shared<TwoOperandInstruction>(std::bitset<5>("10101"))},
    {"load_i", std::make_shared<TwoOperandInstruction>(std::bitset<5>("10100"))},
    {"load_c", std::make_shared<TwoOperandInstruction>(std::bitset<5>("10011"))},
    {"store_s", std::make_shared<TwoOperandInstruction>(std::bitset<5>("10010"))},
    {"load_s", std::make_shared<TwoOperandInstruction>(std::bitset<5>("10001"))},
    {"move", std::make_shared<TwoOperandInstruction>(std::bitset<5>("01100"))},
    {"divide", std::make_shared<TwoOperandInstruction>(std::bitset<5>("01000"))},
    {"multiply", std::make_shared<TwoOperandInstruction>(std::bitset<5>("00111"))},
    {"subtract", std::make_shared<TwoOperandInstruction>(std::bitset<5>("00110"))},
    {"load", std::make_shared<TwoOperandInstruction>(std::bitset<5>("00001"))},
    {"store", std::make_shared<TwoOperandInstruction>(std::bitset<5>("00010"))},
    {"add", std::make_shared<TwoOperandInstruction>(std::bitset<5>("00101"))},
    {"jmpz", std::make_shared<TwoOperandInstruction>(std::bitset<5>("01010"))},
    {"jmpn", std::make_shared<TwoOperandInstruction>(std::bitset<5>("01011"))}
};

std::map<std::string, short> addressMap = {
    {"A0", 0},
    {"A1", 1},
    {"A2", 2},
    {"A3", 3}
};

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        std::cerr << "An argument is needed." << std::endl;
        exit(1);
    }
    std::string filename(argv[1]);
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file." << std::endl;
        exit(1);
    }

	for (std::string line; std::getline(file, line);) {
		std::istringstream lineStream(line);

		std::string instructionCode;
		lineStream >> instructionCode;

		if (instructionCode.front() == '_') {
			// addressMap[instrutionCode];
		}
	}

	file.clear();
	file.seekg(0);
    for (std::string line; std::getline(file, line); ) {
        std::istringstream lineStream(line);

        std::string instructionCode;
        lineStream >> instructionCode;

        auto label = addressMap.find(instructionCode);
        if (label != addressMap.end()) {
            lineStream >> instructionCode;
        }

        auto noneInstruction = noneOperandInstructions.find(instructionCode);
        if (noneInstruction == noneOperandInstructions.end()) {
	    std::string firstOperand;
	    lineStream >> firstOperand;
            
            auto firstOperandAddress = addressMap.find(firstOperand);
            auto oneInstruction = oneOperandInstruction.find(instructionCode);
            if (oneInstruction == oneOperandInstruction.end()) {
                std::string secondOperand;
                lineStream >> secondOperand;

                auto secondOperandAddress = addressMap.find(secondOperand);

                auto twoInstruction = twoOperandInstruction.find(instructionCode);

                twoInstruction->second->firstOperand = firstOperandAddress->second;
                twoInstruction->second->secondOperand = secondOperandAddress->second;
            } else {
                oneInstruction->second->firstOperand = firstOperandAddress->second;
            }

        }     
    }
    return 0;
}
