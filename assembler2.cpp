#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <iomanip>
#include <vector>
using namespace std;

// Opcodes mapping (reverse)
unordered_map<int, string> opcodeToInstruction = {
    {0b00, "ADD"},
    {0b01, "MOVE"},
    {0b10000000, "JMP"},
    {0b10000001, "JPZ"},
    {0b10000010, "JPP"},
    {0b10000011, "JPN"}
};

// Register mapping (reverse)
unordered_map<int, string> regToName = {
    {0b000, "A"},
    {0b001, "B"},
    {0b010, "C"},
    {0b011, "D"},
    {0b100, "E"}
};

// Helper function to convert hex string to int
int hexToInt(const string& hexStr) {
    int value;
    stringstream ss;
    ss << hex << hexStr;
    ss >> value;
    return value;
}

// Helper function to reverse the endianness of a string
string reverseEndian(const string& hexStr) {
    string reversedHex;
    for (int i = hexStr.length() - 2; i >= 0; i -= 2) {
        reversedHex += hexStr.substr(i, 2);
    }
    return reversedHex;
}

// Function to decode a single hexadecimal instruction back to assembly
string decodeInstruction(const string& hexStr, bool littleEndian) {
    string finalHexStr = littleEndian ? reverseEndian(hexStr) : hexStr;

    if (finalHexStr.length() < 4) {
        return "Invalid input";
    }

    // Convert the first two characters of the hex string to the instruction word
    int instructionWord = hexToInt(finalHexStr.substr(0, 2));

    // Check if the opcode matches JMP or other jump-related instructions
    if (instructionWord >= 0x80) {
        int opcode = instructionWord;
        
        if (opcode == 0b10000000) { // JMP
            if (finalHexStr.length() >= 6) {
                int addr = hexToInt(finalHexStr.substr(2, 4)); // Extract the address
                return "JMP " + to_string(addr);
            } else {
                return "Incomplete input for JMP";
            }
        } else if (opcode == 0b10000001) { // JPZ
            if (finalHexStr.length() >= 6) {
                int addr = hexToInt(finalHexStr.substr(2, 4));
                return "JPZ " + to_string(addr);
            } else {
                return "Incomplete input for JPZ";
            }
        } else if (opcode == 0b10000010) { // JPP
            if (finalHexStr.length() >= 6) {
                int addr = hexToInt(finalHexStr.substr(2, 4));
                return "JPP " + to_string(addr);
            } else {
                return "Incomplete input for JPP";
            }
        } else if (opcode == 0b10000011) { // JPN
            if (finalHexStr.length() >= 6) {
                int addr = hexToInt(finalHexStr.substr(2, 4));
                return "JPN " + to_string(addr);
            } else {
                return "Incomplete input for JPN";
            }
        } else {
            return "Unknown jump-related opcode";
        }
    }

    // Decode based on other instructions like ADD and MOVE
    int opcode = (instructionWord >> 6) & 0b11;
    if (opcodeToInstruction.find(opcode) == opcodeToInstruction.end()) {
        return "Unknown opcode";
    }

    string instruction = opcodeToInstruction[opcode];

    if (instruction == "ADD" || instruction == "MOVE") {
        int destReg = (instructionWord >> 3) & 0b111;
        int srcOrVal = instructionWord & 0b111;

        if (regToName.find(destReg) == regToName.end()) return "Unknown destination register";
        string destRegStr = regToName[destReg];

        // Check if there is an immediate value (extension word) or a register
        if (finalHexStr.length() == 6) { // Immediate value is present (e.g., 01A50A)
            int value = hexToInt(finalHexStr.substr(4, 2));
            return instruction + " " + destRegStr + ", " + to_string(value);
        } else if (regToName.find(srcOrVal) != regToName.end()) { // Source is a register
            return instruction + " " + destRegStr + ", " + regToName[srcOrVal];
        } else {
            return "Unknown source register or value";
        }
    }

    return "Unknown error";
}

// Function to process multiple hexadecimal commands and decode them
vector<string> processCommands(const vector<string>& hexCommands, bool littleEndian) {
    vector<string> results;
    for (size_t i = 0; i < hexCommands.size(); i++) {
        string hexStr = hexCommands[i];

        results.push_back(decodeInstruction(hexStr, littleEndian));
    }
    return results;
}

// Function to read input from a file
vector<string> readHexFromFile(const string& filename) {
    ifstream file(filename);
    vector<string> hexCommands;
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string hexStr;
        while (ss >> hexStr) {
            hexCommands.push_back(hexStr);
        }
    }

    file.close();
    return hexCommands;
}

// Function to write output to a file
void writeAssemblyToFile(const string& filename, const vector<string>& assemblyCommands) {
    ofstream file(filename);
    for (const auto& command : assemblyCommands) {
        file << command << endl;
    }
    file.close();
}

int main() {
    vector<string> hexCommands;
    bool littleEndian = false;

    // Ask if input is from file or manual
    string inputOption, outputFile;
    cout << "Would you like to input from a file (yes/no)? ";
    cin >> inputOption;
    bool fromFile = (inputOption == "yes");

    if (fromFile) {
        string inputFile;
        cout << "Enter input filename: ";
        cin >> inputFile;
        hexCommands = readHexFromFile(inputFile);

        cout << "Enter output filename: ";
        cin >> outputFile;
    } else {
        string input;
        cout << "Enter hexadecimal commands (type 'done' to finish): ";
        while (cin >> input && input != "done") {
            hexCommands.push_back(input);
        }
    }

    // Ask for endian option
    string endianOption;
    cout << "Would you like the output in little endian (yes/no)? ";
    cin >> endianOption;
    littleEndian = (endianOption == "yes");

    // Process commands
    vector<string> results = processCommands(hexCommands, littleEndian);

    if (fromFile) {
        // Write results to output file
        writeAssemblyToFile(outputFile, results);
        cout << "Results written to " << outputFile << endl;
    } else {
        // Print results to console
        for (const auto& result : results) {
            cout << "Assembly: " << result << endl;
        }
    }

    return 0;
}
