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

// Mapeamento de todos os operadores sem operandos com seus respectivos opcodes.
const std::map<std::string, std::bitset<16>> noneOperandInstructions = {
    {"return", std::bitset<16>("1000000000000000")},
    {"stop", std::bitset<16>("0000000000000000")}
};

// Mapeamento de todos os operadores de um operando com seus respectivos opcodes.
const std::map<std::string, std::bitset<5>> oneOperandInstructions = {
    {"copytop", std::bitset<5>("10110")},
    {"call", std::bitset<5>("01111")},
    {"push", std::bitset<5>("01101")},
    {"pop", std::bitset<5>("01110")},
    {"read", std::bitset<5>("00011")},
    {"write", std::bitset<5>("00100")},
    {"jump", std::bitset<5>("01001")}
};

// Mapeamento de todos os operadores de dois operandos com seus respectivos opcodes.
const std::map<std::string, std::bitset<5>> twoOperandInstructions = {
    {"store_i", std::bitset<5>("10101")},
    {"load_i", std::bitset<5>("10100")},
    {"load_c", std::bitset<5>("10011")},
    {"store_s", std::bitset<5>("10010")},
    {"load_s", std::bitset<5>("10001")},
    {"move", std::bitset<5>("01100")},
    {"divide", std::bitset<5>("01000")},
    {"multiply", std::bitset<5>("00111")},
    {"subtract", std::bitset<5>("00110")},
    {"load", std::bitset<5>("00001")},
    {"store", std::bitset<5>("00010")},
    {"add", std::bitset<5>("00101")},
    {"jmpz", std::bitset<5>("01010")},
    {"jmpn", std::bitset<5>("01011")}
};

// Mapeamento de todas as variaveis e seus respectivos endereços.
// (Registro ou Label).
std::map<std::string, int> addressMap = {
    {"A0", 0},
    {"A1", 1},
    {"A2", 2},
    {"A3", 3}
};

// Primeira passagem pelo arquivo para que se armazene os endereços dos labels e do .data.
void firstFilePass(std::ifstream& file) {
    int memoryCount = 0;
    for (std::string line; std::getline(file, line);) {
        std::istringstream lineStream(line);

        std::string instructionCode;
        lineStream >> instructionCode;

        // Verifica se não é uma instrução, mas sim label.
        if (noneOperandInstructions.find(instructionCode) == noneOperandInstructions.end() &&
            oneOperandInstructions.find(instructionCode) == oneOperandInstructions.end() &&
            twoOperandInstructions.find(instructionCode) == twoOperandInstructions.end()) {

            // para retirar o ':'' do label.
            instructionCode.pop_back();
            addressMap[instructionCode] = memoryCount;
        }

        // Passa para proxima posição de memória disponível.
        memoryCount += 2;
    }
}

// Insere no dataValue o valor da instrução, sendo o opcode + [[primeiro operador] + segundo operador]
void getMemoryValue(const std::string& instructionCode, std::istringstream& lineStream,
    std::bitset<16>& dataValue) {
    // Caso for um .data, é lido depois a memória de alocação, que com a mudança na
    // especificação é sempre 2 (16 bits).
    // Caso contrário é uma instrução.
    if (instructionCode == ".data") {
        int memoryAlocation, initialValue;
        lineStream >> memoryAlocation >> initialValue;

        dataValue = std::bitset<16>(initialValue);
    } else {
        auto noneInstruction = noneOperandInstructions.find(instructionCode);
        // Se não for uma instrução que não tem operador, então pode ser uma
        // instrução com um ou dois operadores.
        if (noneInstruction == noneOperandInstructions.end()) {
            std::string firstOperand;
            lineStream >> firstOperand;
            
            // Par do primeiro operador e seu respectivo endereço de memória.
            auto firstOperandAddress = addressMap.find(firstOperand);
            auto oneInstruction = oneOperandInstructions.find(instructionCode);
            // Caso não seja uma instrução de um operador, é lido o segundo operador.
            if (oneInstruction == oneOperandInstructions.end()) {
                std::string secondOperand;
                lineStream >> secondOperand;

                // Par do segundo operador e seu respectivo endereço de memória.
                auto secondOperandAddress = addressMap.find(secondOperand);
                auto twoInstruction = twoOperandInstructions.find(instructionCode);

                // Armazena o valor como sendo o opcode, primeiro operador e segundo operador.
                // O segundo operador pode ser uma constante, então pode não estar na memória,
                // portanto deve-se verificar se está ou não na memória, se não estiver, então
                // é uma constante inteira.
                dataValue = std::bitset<16>(twoInstruction->second.to_string() +
                    std::bitset<2>(firstOperandAddress->second).to_string() + 
                    std::bitset<9>(
                        secondOperandAddress != addressMap.end() ?
                         secondOperandAddress->second
                        :
                         std::stoi(secondOperand)).to_string());
                
            } else {
                // Armazena o valor como sendo o opcode e o primeiro operador.
                dataValue = std::bitset<16>(oneInstruction->second.to_string() +
                    std::bitset<11>(firstOperandAddress->second).to_string());
                }
        } else {
            // Armazena o valor como sendo o opcode apenas.
            dataValue = std::bitset<16>(noneInstruction->second);
        }
    }
}

// Passa pela segundo vez no arquivo, realmente montando ele no arquivo de saída.
// Retorna o contador de memória.
int secondFilePass(std::ifstream& file, std::ofstream& outputFile) {
    int memoryCount = 0;
    for (std::string line; std::getline(file, line); ) {
        std::istringstream lineStream(line);

        // Le a primeira string, que pode ser uma instrução ou um label.
        std::string instructionCode;
        lineStream >> instructionCode;
        
        // Caso for um label, le a próxima string, que será a instrução.
        if (noneOperandInstructions.find(instructionCode) == noneOperandInstructions.end() &&
            oneOperandInstructions.find(instructionCode) == oneOperandInstructions.end() &&
            twoOperandInstructions.find(instructionCode) == twoOperandInstructions.end()) {
            lineStream >> instructionCode;
        }

        // Pega cada célula da memória, sendo a parte alta e a parte baixa.
        std::bitset<8> memoryAddressHigh(memoryCount);
        std::bitset<8> memoryAddressLow(memoryCount+1);

        // O valor propriamente dito da célula da memória correspondente.
        std::bitset<16> dataValue;
        getMemoryValue(instructionCode, lineStream, dataValue);
        // Para ir para a próxima posição de memória (próxima instrução).
        memoryCount += 2;
        
        std::bitset<8> highValue(dataValue.to_string());
        std::bitset<8> lowValue((dataValue << 8).to_string());

        outputFile << memoryAddressHigh << " : " << highValue << "; -- " << line << std::endl;
        outputFile << memoryAddressLow << " : " << lowValue << ';' << std::endl;
    }

    return memoryCount;
}

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

    firstFilePass(file);

    // Limpa o arquivo e começa a leitura novamente (segunda passada).
	file.clear();
	file.seekg(0);

    std::ofstream outputFile("output.mif");

    // Cabeçalho do .mif para identificação da memória.
    outputFile << "DEPTH = 128;" << std::endl
        << "WIDTH = 8;" << std::endl
        << "ADDRESS_RADIX = BIN; " << std::endl
        << "DATA_RADIX = BIN; " << std::endl
        << "CONTENT" << std::endl
        << "BEGIN" << std::endl
        << std::endl;

    int memoryCount = secondFilePass(file, outputFile);

    outputFile << '[' << std::bitset<8>(memoryCount) << "..01111111] : " << std::bitset<8>() << ';' << std::endl 
        << "END;";
    return 0;
}
