#include <functional>
#include <stdio>
#include <stdlib>
#include <string>

#include "Function.h"

typedef const std::map<std::string, int> InstructionCode;
typedef const std::map<int, std::function> InstructionType;

InstructionType instructions = {
    0, &Function::stop,
    1, &Function::load
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