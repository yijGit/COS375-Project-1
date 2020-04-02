#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include "MemoryStore.h"
#include "RegisterInfo.h"
#include "EndianHelpers.h"

using namespace std;

enum boolean {FALSE, TRUE};

struct R_format
{
  uint32_t rs, rt, rd, shamt, funct;
};

struct I_format
{
  uint32_t rs, rt, imm;
};

struct J_format
{
  uint32_t addr;
};

// add instruction types as we go along
enum ins_t {HALT, R_TYPE, I_TYPE, J_TYPE};

uint32_t get_opcode(uint32_t instruction)
{
  return instruction >> 26;
}

R_format get_R_format(uint32_t instruction)
{
  struct R_format fields;
  uint32_t mask21 = 31 << 21; // mask for bits 21-25
  uint32_t mask16 = 31 << 16; // mask for bits 16-20
  uint32_t mask11 = 31 << 11; // mask for bits 11-15
  uint32_t mask6 = 31 << 6; // mask for bits 6-10
  uint32_t mask0 = 63; // mask for bits 0-5
  fields.rs = (instruction & mask21) >> 21;
  fields.rt = (instruction & mask16) >> 16;
  fields.rd = (instruction & mask11) >> 11;
  fields.shamt = (instruction & mask6) >> 6;
  fields.funct = instruction & mask0;
  return fields;
}

I_format get_I_format(uint32_t instruction)
{
  struct I_format fields;
  uint32_t mask21 = 31 << 21; // mask for bits 21-25
  uint32_t mask16 = 31 << 16; // mask for bits 16-20
  uint32_t mask0 = ((1 << 16) - 1); // mask for bits 0-15, equal to 2^16 - 1
  fields.rs = (instruction & mask21) >> 21;
  fields.rt = (instruction & mask16) >> 16;
  fields.imm = instruction & mask0;
  return fields;
}

J_format get_J_format(uint32_t instruction)
{
    struct J_format fields;
    fields.addr = instruction & ((1 << 26) - 1); // extract bits 0-25
    return fields;
}

void transfer_registers(uint32_t reg_arr[], RegisterInfo &reg)
{
  reg.at = reg_arr[1];
  reg.gp = reg_arr[28];
  reg.sp = reg_arr[29];
  reg.fp = reg_arr[30];
  reg.ra = reg_arr[31];

  for (int i = 0; i < V_REG_SIZE; i++)
  {
    reg.v[i] = reg_arr[2 + i];
  }
  for (int i = 0; i < A_REG_SIZE; i++)
  {
    reg.a[i] = reg_arr[4 + i];
  }
  for (int i = 0; i < T_REG_SIZE - 2; i++)
  {
    reg.t[i] = reg_arr[8 + i];
  }
  reg.t[8] = reg_arr[24];
  reg.t[9] = reg_arr[25];
  for (int i = 0; i < S_REG_SIZE; i++)
  {
    reg.s[i] = reg_arr[16 + i];
  }
  for (int i = 0; i < K_REG_SIZE; i++)
  {
    reg.k[i] = reg_arr[26 + i];
  }
  return;
}

int main(int argc, char *argv[])
{
  FILE *pFile;
  uint32_t word, instruction, op_code;
  long lSize;
  uint32_t pc;
  ins_t instr_type;
  // Create	a	memory	store	called	myMem
  MemoryStore *myMem = createMemoryStore();
  // Initialize	registers	to	have	value	0
  uint32_t reg_arr[32];
  RegisterInfo reg;
  for (int i = 0; i < 32; i++)
  {
    reg_arr[i] = 0;
  }


  // Read	bytes	of	binary	file passed	as	parameter into appropriate	memory	locations
  pFile = fopen(argv[1], "rb");
  if (pFile==NULL) {fputs("File error", stderr); exit(1);}
  fseek(pFile, 0, SEEK_END);
  lSize = ftell(pFile);
  rewind(pFile);
  cout << lSize << "\n";
  //fread(word, 4, 1, pFile);
  for (long i = 0; i < lSize; i+= 4)
  {
    fread(reinterpret_cast<char *>(&word), sizeof(word), 1, pFile);
    word = ConvertWordToBigEndian(word);
    myMem->setMemValue((uint32_t) i, word, WORD_SIZE);
    cout << hex << setfill('0') << setw(8) << word << endl;
  }

  // Point	the	program	counter	to	the	first	instruction
  pc = 0;

  while (TRUE)
  {
    // Fetch	current	instruction	from	memory@PC
    myMem->getMemValue(pc, instruction, WORD_SIZE);
    pc += 4;
    // Determine	the	instruction	type
    if (instruction == 0xfeedfeed)
      {instr_type = HALT;}
    else
      {
        op_code = get_opcode(instruction);
        if (op_code == 0x0)
           { instr_type = R_TYPE;}
        else if ((op_code == 0x2) || (op_code == 0x3))
           { instr_type = J_TYPE;}
        else
           {instr_type = I_TYPE;}
      }
    // Get	the	operands
    switch (instr_type)
    {
      case HALT:
        // RegisterInfo	reg;
        // Fill	reg	with	the	current	contents	of	the	registers
      {
        transfer_registers(reg_arr, reg);
	      dumpRegisterState(reg);
        dumpMemoryState(myMem);
        cout << "HALT" << endl;
        return 0;
	    }

      case R_TYPE:
        // Perform	operation	and	update	destination
        // register/memory/PC
      {
	      R_format r_fields = get_R_format(instruction);
        cout << "R" << endl;
        break;
	    }

      case I_TYPE:
      {
	      I_format i_fields = get_I_format(instruction);
        // addi
        if (op_code == 0x8)
        {
          reg_arr[i_fields.rt] = reg_arr[i_fields.rs] + i_fields.imm; // Note: still need to add sign extension
          cout << "addi" << endl;
        }
        cout << "I" << endl;
        break;
	    }

      case J_TYPE:
      {
	      J_format j_fields = get_J_format(instruction);
        cout << "J" << endl;
        break;
	    }

      default:
      {
	      fprintf(stderr, "Illegal	operation...");
        exit(127);
	    }
    }
  }

  dumpRegisterState(reg);
  dumpMemoryState(myMem);

  delete myMem;
  return 0;
}
