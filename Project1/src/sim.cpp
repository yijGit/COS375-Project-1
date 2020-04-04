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
  fields.funct = (instruction & mask0);
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

int32_t sign_extend_imm(uint32_t imm)
{
  if (imm & (1 << 15))
  { return imm | 0xffff0000;}
  else
  { return imm; }
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
        // sll
        if (r_fields.funct == 0x0)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rt] << r_fields.shamt;
          cout << "sll" << endl;
          
        }
        // srl
        if (r_fields.funct == 0x02)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rt] >> r_fields.shamt;
          cout << "srl" << endl;
        }
        // add
        if (r_fields.funct == 0x20)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rs] + reg_arr[r_fields.rt];
          cout << "add" << endl;
        }
        // addu
        if (r_fields.funct == 0x21)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rs] + reg_arr[r_fields.rt];
          cout << "addu" << endl;
        }
        // sub
        if (r_fields.funct == 0x22)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rs] - reg_arr[r_fields.rt];
          cout << "sub" << endl;
        }
        // subu
        if (r_fields.funct == 0x23)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rs] - reg_arr[r_fields.rt];
          cout << "subu" << endl;
        }
        // and
        if (r_fields.funct == 0x24)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rs] & reg_arr[r_fields.rt];
          cout << "and" << endl;
        }
        // or
        if (r_fields.funct == 0x25)
        {
          reg_arr[r_fields.rd] = reg_arr[r_fields.rs] | reg_arr[r_fields.rt];
          cout << "or" << endl;
        }
        // nor
        if (r_fields.funct == 0x27)
        {
          reg_arr[r_fields.rd] = ~(reg_arr[r_fields.rs] | reg_arr[r_fields.rt]);
          cout << "nor" << endl;
        }
        // jr
        if (r_fields.funct == 0x08)
        {
          pc = reg_arr[r_fields.rs];
          cout << "jr" << endl;
        }
        // slt
        // NOTE: I am not sure I'm supposed to use sign_extend_imm here.
        // $s and $t need to be signed, so that is why I put them through sign_extend_imm
        if (r_fields.funct == 0x2a)
        {
          reg_arr[r_fields.rd] = (sign_extend_imm(reg_arr[r_fields.rs]) < sign_extend_imm(reg_arr[r_fields.rt])) ? 1 : 0;
          cout << "slt" << endl;
        }
        // sltu
        if (r_fields.funct == 0x2b)
        {
          reg_arr[r_fields.rd] = (reg_arr[r_fields.rs] < reg_arr[r_fields.rt]) ? 1 : 0;
          cout << "sltu" << endl;
        }

        // jr
        if (r_fields.funct == 0x8)
        {
          pc = reg_arr[r_fields.rs];
          cout << "jr" << endl;
        }

        break;
	    }

      case I_TYPE:
      {
	      I_format i_fields = get_I_format(instruction);
        // addi
        if (op_code == 0x8)
        {
          reg_arr[i_fields.rt] = reg_arr[i_fields.rs] + sign_extend_imm(i_fields.imm);
          cout << "addi" << endl;
        }
        // addiu
        if (op_code == 0x9)
        {
          reg_arr[i_fields.rt] = reg_arr[i_fields.rs] + sign_extend_imm(i_fields.imm);
          cout << "addiu" << endl;
        }
        // andi
        if (op_code == 0xc)
        {
          reg_arr[i_fields.rt] = reg_arr[i_fields.rs] & i_fields.imm;
          cout << "andi" << endl;
        }
        // ori
        if (op_code == 0xd)
        {
          reg_arr[i_fields.rt] = reg_arr[i_fields.rs] | i_fields.imm;
          cout << "ori" << endl;
        }
        // lbu
        // NOTE: Don't know if specifying MemEntrySize does anything or if I still need to do bitwise AND 0xff/0xffff to isolate a byte/halfword
        // For now I assumed BYTE_SIZE and HALF_SIZE doesn't do anything and used bitwise AND (&)
        if (op_code == 0x24)
        {
          myMem->getMemValue(0xff & (reg_arr[i_fields.rs] + i_fields.imm), reg_arr[i_fields.rt], BYTE_SIZE);
          cout << "lbu" << endl;
        }
        // lhu
        if (op_code == 0x25)
        {
          myMem->getMemValue(0xffff & (reg_arr[i_fields.rs] + i_fields.imm), reg_arr[i_fields.rt], HALF_SIZE);
          cout << "lhu" << endl;
        }
        // lui
        if (op_code == 0xf)
        {
          reg_arr[i_fields.rt] = i_fields.imm << 16;
          cout << "lui" << endl;
        }
        // lw
        if (op_code == 0x23)
        {
          myMem->getMemValue(reg_arr[i_fields.rs] + sign_extend_imm(i_fields.imm), reg_arr[i_fields.rt], WORD_SIZE);
          cout << "lw" << endl;
        }
        // slti
        if (op_code == 0xa)
        {
          reg_arr[i_fields.rt] = (reg_arr[i_fields.rs] < sign_extend_imm(i_fields.imm)) ? 1 : 0;
          cout << "slti" << endl;
        }
        // sltiu
        if (op_code == 0xb)
        {
          reg_arr[i_fields.rt] = (reg_arr[i_fields.rs] < i_fields.imm) ? 1 : 0;
          cout << "sltiu" << endl;
        }
        // sb
        if (op_code == 0x28)
        {
          myMem->setMemValue(reg_arr[i_fields.rs] + i_fields.imm, 0xff & reg_arr[i_fields.rt], BYTE_SIZE);
          cout << "sb" << endl;
        }
        // sh
        if (op_code == 0x29)
        {
          myMem->setMemValue(reg_arr[i_fields.rs] + i_fields.imm, 0xffff & reg_arr[i_fields.rt], HALF_SIZE);
          cout << "sh" << endl;
        }
        // sw
        if (op_code == 0x2b)
        {
          myMem->setMemValue(reg_arr[i_fields.rs] + sign_extend_imm(i_fields.imm), reg_arr[i_fields.rt], WORD_SIZE);
          cout << "sw" << endl;
        }
        // beq
        if (op_code == 0x4)
        {
          if (reg_arr[i_fields.rs] == reg_arr[i_fields.rt])
          { pc += (sign_extend_imm(i_fields.imm) << 2);
          cout << "beq" << endl;
          }
        }
        // bne
        if (op_code == 0x5)
        {
          if (reg_arr[i_fields.rs] != reg_arr[i_fields.rt])
          { pc += (sign_extend_imm(i_fields.imm) << 2);
          cout << "beq" << endl;
          }
        }

        break;
	    }

      case J_TYPE:
      {
	      J_format j_fields = get_J_format(instruction);

        // j
        if (op_code == 0x2)
        {
          pc = ((pc >> 28) << 28) | (j_fields.addr << 2);
          cout << "j" << endl;
        }
        // jal
        if (op_code == 0x3)
        {
          reg_arr[31] = pc + 4; // pc already incremented by 4 earlier, so total += 8
          pc = ((pc >> 28) << 28) | (j_fields.addr << 2); // pc = jumpadr
          cout << "jal" << endl;
        }

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
