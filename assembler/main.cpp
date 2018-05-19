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

std::map<std::string, int> addressMap = {
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

    int memoryCount = 0;
	for (std::string line; std::getline(file, line);) {
		std::istringstream lineStream(line);

		std::string instructionCode;
		lineStream >> instructionCode;

		if (noneOperandInstructions.find(instructionCode) == noneOperandInstructions.end() &&
            oneOperandInstruction.find(instructionCode) == oneOperandInstruction.end() &&
            twoOperandInstruction.find(instructionCode) == twoOperandInstruction.end()) {

            // para retirar o ':'' do label.
            instructionCode.pop_back();
            addressMap[instructionCode] = memoryCount;
		} 
        memoryCount += 2;
	}

	file.clear();
	file.seekg(0);
    memoryCount = 0;

    std::ofstream outputFile("output.mif");

    outputFile << "DEPTH = 128;" << std::endl
        << "WIDTH = 8;" << std::endl
        << "ADDRESS_RADIX = BIN; " << std::endl
        << "DATA_RADIX = BIN; " << std::endl
        << "CONTENT" << std::endl
        << "BEGIN" << std::endl
        << std::endl;

    for (std::string line; std::getline(file, line); ) {
        std::istringstream lineStream(line);

        std::string instructionCode;
        lineStream >> instructionCode;
        
        if (noneOperandInstructions.find(instructionCode) == noneOperandInstructions.end() &&
            oneOperandInstruction.find(instructionCode) == oneOperandInstruction.end() &&
            twoOperandInstruction.find(instructionCode) == twoOperandInstruction.end()) {
            lineStream >> instructionCode;
        }

        std::bitset<8> memoryAddressHigh(memoryCount);
        std::bitset<8> memoryAddressLow(memoryCount+1);
        std::bitset<16> dataValue;
        memoryCount += 2;
        if (instructionCode == ".data") {
            int memoryAlocation, initialValue;
            lineStream >> memoryAlocation >> initialValue;

            // std::bitset<8> memoryAddress2(memoryCount);
            dataValue = std::bitset<16>(initialValue);
        } else {
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
                    if (secondOperandAddress == addressMap.end()) {

                    }

                    auto twoInstruction = twoOperandInstruction.find(instructionCode);

                    dataValue = std::bitset<16>(twoInstruction->second->opcode.to_string() +
                        std::bitset<2>(firstOperandAddress->second).to_string() + 
                        std::bitset<9>(
                            secondOperandAddress != addressMap.end() ?
                             secondOperandAddress->second
                            :
                             std::stoi(secondOperand)).to_string());
                    
                } else {
                    dataValue = std::bitset<16>(oneInstruction->second->opcode.to_string() +
                        std::bitset<11>(firstOperandAddress->second).to_string());
                    }
            } else {
                dataValue = std::bitset<16>(noneInstruction->second->opcode);
            }
            
        }
        std::cout << instructionCode << ": "<< dataValue << std::endl;
        std::bitset<8> highValue(dataValue.to_string());
        std::bitset<8> lowValue((dataValue << 8).to_string());

        outputFile << memoryAddressHigh << " : " << highValue << ';' << std::endl;
        outputFile << memoryAddressLow << " : " << lowValue << ';' << std::endl;
    }

    outputFile << '[' << std::bitset<8>(memoryCount) << "..01111111] : " << std::bitset<8>() << ';' << std::endl 
        << "END;";
    return 0;
}
