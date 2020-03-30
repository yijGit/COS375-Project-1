#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include "MemoryStore.h"
#include "RegisterInfo.h"
#include "EndianHelpers.h"

using namespace std;

enum boolean {FALSE, TRUE};

// add instruction types as we go along
enum ins_t {HALT, OTHER};

int main(int argc, char *argv[])
{
  FILE *pFile;
  uint32_t word, instruction;
  long lSize;
  uint32_t pc;
  ins_t instr_type;
  // Create	a	memory	store	called	myMem
  MemoryStore *myMem = createMemoryStore();
  // Initialize	registers	to	have	value	0
  RegisterInfo reg;
  reg.at = 0, reg.gp = 0, reg.sp = 0, reg.fp = 0, reg.ra = 0;
  for (int i = 0; i < V_REG_SIZE; i++)
  {
    reg.v[i] = 0;
  }
  for (int i = 0; i < A_REG_SIZE; i++)
  {
    reg.a[i] = 0;
  }
  for (int i = 0; i < T_REG_SIZE; i++)
  {
    reg.t[i] = 0;
  }
  for (int i = 0; i < S_REG_SIZE; i++)
  {
    reg.s[i] = 0;
  }
  for (int i = 0; i < K_REG_SIZE; i++)
  {
    reg.k[i] = 0;
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
    // Determine	the	instruction	type
    if (instruction == 0xfeedfeed)
      {instr_type = HALT;}
    else
      {instr_type = OTHER;}
    // Get	the	operands
    switch (instr_type)
    {
      case HALT:
        // RegisterInfo	reg;
        // Fill	reg	with	the	current	contents	of	the	registers
        dumpRegisterState(reg);
        dumpMemoryState(myMem);
        cout << "HALT" << endl;
        return 0;
      case OTHER:
        // Perform	operation	and	update	destination
        // register/memory/PC
        cout << "other" << endl;
        pc += 4;
        break;
      // ...
      default:
        fprintf(stderr, "Illegal	operation...");
        exit(127);
    }
  }

  dumpRegisterState(reg);
  dumpMemoryState(myMem);

  delete myMem;
  return 0;
}
