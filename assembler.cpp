#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <iomanip>
using namespace std;

// Register mapping
unordered_map<string, int> registers = {
    {"A", 0b000},
    {"B", 0b001},
    {"C", 0b010},
    {"D", 0b011},
    {"E", 0b100}
};

// Opcodes mapping
unordered_map<string, int> opcodes = {
    {"ADD", 0b00},
    {"MOVE", 0b01},
    {"STORE", 0b01},
    {"LOAD", 0b01},
    {"JMP", 0b10000000},
    {"JPZ", 0b10000001},
    {"JPP", 0b10000010},
    {"JPN", 0b10000011}
};

// Helper function to convert int to hex
string intToHex(int value, int width = 2) {
    stringstream ss;
    ss << setfill('0') << setw(width) << hex << value;
    return ss.str();
}

// Parse and process the command
vector<string> parseCommand(const string &command) {
    vector<string> tokens;
    istringstream stream(command);
    string token;

    while (stream >> token) {
        // Find and handle commas
        size_t commaPos = token.find(',');
        if (commaPos != string::npos) {
            // Split around the comma
            tokens.push_back(token.substr(0, commaPos)); // Before the comma
            tokens.push_back(token.substr(commaPos + 1)); // After the comma
        } else {
            tokens.push_back(token);
        }
    }
    return tokens;
}


// Function to process ADD, MOVE, STORE, LOAD, and JMP instructions
string processInstruction(const vector<string>& tokens) {
    if (tokens.empty()) return "Invalid command format";

    string instruction = tokens[0];
    if (opcodes.find(instruction) == opcodes.end()) {
        return "Invalid Instruction";
    }

    int opcode = opcodes[instruction];
    if (instruction == "ADD" || instruction == "MOVE") {
        if (tokens.size() != 3) return "Invalid Instruction Format";

        string destReg = tokens[1];
        string srcOrVal = tokens[2];

        if (registers.find(destReg) == registers.end()) return "Invalid Destination Register";

        int destRegCode = registers[destReg];
        int instrCode;

        // ADD/MOVE with register
        if (registers.find(srcOrVal) != registers.end()) {
            int srcRegCode = registers[srcOrVal];
            instrCode = (opcode << 6) | (destRegCode << 3) | srcRegCode;
            return intToHex(instrCode);
        } else { // ADD/MOVE with immediate value
            try {
                int value = stoi(srcOrVal); // Convert the string to an integer
                if (value < 0 || value > 255) return "Value out of range"; // Limit to 8-bit values

                instrCode = (opcode << 6) | (destRegCode << 3) | 0b101; // Use '101' as per spec to indicate a constant
                return intToHex(instrCode) + " " + intToHex(value); // Instruction and value in hex
            } catch (const invalid_argument& e) {
                return "Invalid value format";
            }
        }

    } else if (instruction == "STORE" || instruction == "LOAD") {
        if (tokens.size() != 3) return "Invalid Instruction Format";

        string addr = tokens[1];
        string reg = tokens[2];

        if (registers.find(reg) == registers.end()) return "Invalid Register";

        int regCode = registers[reg];
        int address = stoi(addr);

        int instrCode = (opcode << 6) | (0b110 << 3) | regCode;

        // Convert address to two bytes (16-bit address split)
        int addrLSB = address & 0xFF;
        int addrMSB = (address >> 8) & 0xFF;

        return intToHex(instrCode) + " " + intToHex(addrLSB) + " " + intToHex(addrMSB);

    } else if (instruction == "JMP" || instruction == "JPZ" || instruction == "JPP" || instruction == "JPN") {
        if (tokens.size() != 2) return "Invalid Instruction Format";

        string addr = tokens[1];
        int address = stoi(addr);

        int instrCode = opcode;

        // Convert address to two bytes (16-bit address split)
        int addrLSB = address & 0xFF;
        int addrMSB = (address >> 8) & 0xFF;

        return intToHex(instrCode) + " " + intToHex(addrLSB) + " " + intToHex(addrMSB);
    }

    return "Unknown error";
}

int main() {
    string input;
    cout << "Enter assembly command: ";
    getline(cin, input);

    vector<string> tokens = parseCommand(input);
    if (tokens.empty()) {
        cerr << "Invalid command" << endl;
        return 1;
    }

    string result = processInstruction(tokens);
    cout << "Hexadecimal: " << result << endl;

    return 0;
}
