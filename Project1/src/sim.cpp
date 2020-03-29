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


  cout << "Hello, world!";
  dumpRegisterState(reg);
  dumpMemoryState(myMem);

  delete myMem;
  return 0;
}
