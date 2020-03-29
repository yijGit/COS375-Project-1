main(argc, argv) {
  // Create	a	memory	store	called	myMem
  // Initialize	registers	to	have	value	0
  // Read	bytes	of	binary	file passed	as	parameter into appropriate	memory	locations
  // Point	the	program	counter	to	the	first	instruction
  while (TRUE) {
    // Fetch	current	instruction	from	memory@PC
    // Determine	the	instruction	type
    // Get	the	operands
    switch (instruction	type)	{
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
      fprintf(stderr, ”Illegal	operation...”);
      exit(127);
    }
  }
}
