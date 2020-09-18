# COS375-Project-1

A high-level simulations for MIPS ISA built for Princeton's COS375: Computer Architecture and Organization. It reads a binary machine code file, interprets the bits in that file as instructions for the MIPS instruction set, and simulates the execution of those instructions. The project was written in C++. 

Below is a short description of the files:  
  **mips-linux-gnu-as** – MIPS assembler. Converts MIPS text assembly to binary ELF files.  
  **mips-linux-gnu-objcopy** – MIPS object file copier/translator. Used to convert ELF files to flat binary files that are read by your simulator.  
  **mips-linux-gnu-objdump** – MIPS object file disassembler. Used to inspect the instructions in a binary ELF file.  
  **EndianHelpers.h** – Signatures of the functions to convert unsigned integers from little-endian to big-endian.  
  **MemoryStore.h** – The interface to the memory abstraction.  
  **RegisterInfo.h** – Signature of the register file dump function and definition of the RegisterInfo struct passed to it as an argument.  
  **UtilityFunctions.o** – A binary file containing the implementations of the memory abstraction and other utility functions.  
  **fib.asm** – An example MIPS assembly test program.  
  **fib_mem_state.out** – The memory state dump of running the compiled version of fib.asm.  
  **fib_reg_state.out** – The register state dump of running the compiled version of fib.as.
