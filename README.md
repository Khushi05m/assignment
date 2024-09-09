# assignment

how the program works

This program is designed to take hexadecimal machine code and convert it into human-readable assembly instructions. The cool thing is that you can either input this hex code manually or from a file, and the program will then output the corresponding assembly code either to the console or into a file.


 used component of the program

1. Opcodes and Registers 
   The program knows what instructions like MOVE, ADD, and jump commands (JMP, JPz, etc.) look like in machine code. It also knows the registers (like A, B, C, etc.) that the instructions work with. These are all mapped out using unordered_map structures.

2. Converting Hex to Instructions  
   The main job of the program is to take a hexadecimal string and translate it into an assembly command. For example, the hex string `01A50A` should be translated into `MOVE C, 10`. This is done in the decodeInstruction function, which carefully decodes the hex string based on specific patterns
   
4. File and Console Input/Output 
   When you run the program, it asks whether you want to provide the input manually or from a file. If you choose a file, you type in the filename, and the program reads the hex data from that file. After processing, it writes the results to another file that you specify. If you don't use a file, you can input hex commands directly into the console.

 How the Program Works (Step by Step):

1. Startup  
   The program first asks if you want to input your data from a file or type it in manually. You say either "yes" (for file) or "no" (for manual input).

2. Reading Input  
   - If you chose a file, you provide the filename (like `input.txt`), and the program opens that file and reads each line of hex code.
   - If you chose manual input, you type in the hex code directly into the console until you’re done.

3. Processing the Hex 
   For each hex string, the program decodes it. It checks if the instruction is something like a `JMP` (which has extra data like an address) or if it's a `MOVE` or `ADD` (which works with registers and values). The program then converts that hex into a meaningful assembly instruction like `MOVE A, 5`.

4. Output 
   - If you chose to write to a file, the decoded instructions are saved into a file you specify (e.g., `output.txt`).
   - If you chose to output to the console, the decoded instructions are printed directly to your screen.

Example Flow:

Let's say you have a hex command like 01A50A. This represents a MOVE instruction, telling register `C` to hold the value `10`. Here's what happens:

1. You choose to input from a file, and the program reads the line 01A50A.
2. The program recognizes that 01 corresponds to the MOVE opcode.
3. It breaks down the hex code and figus out that A5 corresponds to register C, and 0A is the immediate value 10.
4. The program decodes it as `MOVE C, 10 and writes this to the output file (or prints it to the console).

 Debugging and Feedback:
I've added some debug lines so that if something goes wrong, like the input file doesn't open, you’ll see an error message. It’s also set up to confirm when it has processed commands or written results to the output file.

