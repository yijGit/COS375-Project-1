main: addi $t0, $zero, 8          # t0 = 8
      sll  $t1, $t0, 1            # t1 = 16
      srl  $t2, $t1, 3            # t2 = 2
      addi $t3, $t0, -1           # t3 = 7
      or   $t4, $t0, $t3          # t4 = 15
      ori  $t5, $t1,  2           # t5 = 18
      and  $t6, $t0, $t3          # t6 = 0
      andi $t7, $t4,  6           # t7 = 6
     .word 0xfeedfeed
