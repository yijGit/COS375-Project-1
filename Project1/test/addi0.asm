main: addi $t0, $zero, 10          # testing addi instruction
      addi $t1, $zero, 20          
      addi $s0, $t0, 1
      addi $s2, $t1, -1
      addi $t2, $zero, -1
      addi $t3, $zero, 10
      addi $t3, $t3, -8
      addi $t3, $t3, -8
     .word 0xfeedfeed
