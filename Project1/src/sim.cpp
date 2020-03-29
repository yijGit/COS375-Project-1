#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstdio>
#include "MemoryStore.h"
#include "RegisterInfo.h"

using namespace std;

enum boolean {FALSE, TRUE};

int main(int argc, char *argv[])
{
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

  // Point	the	program	counter	to	the	first	instruction
  while (TRUE)
  {
    // Fetch	current	instruction	from	memory@PC
    // Determine	the	instruction	type
    // Get	the	operands
    /*switch (instruction	type)
    {
      case 0xfeedfeed:
        // RegisterInfo	reg;
        // Fill	reg	with	the	current	contents	of	the	registers
        dumpRegisterState(reg);
        dumpMemoryState(myMem);
        return 0;
      case INSTR1:
        // Perform	operation	and	update	destination
        // register/memory/PC
        break;
      // ...
      default:
        fprintf(stderr, "Illegal	operation...");
        exit(127);
    }*/
    break;
  }

  dumpRegisterState(reg);
  dumpMemoryState(myMem);

  delete myMem;
  return 0;
}
