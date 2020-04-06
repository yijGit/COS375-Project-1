main: li $t0, 0             # t0 = 8
      li $t5, 2             # t1 = 16
      li $t7, 16            # t2 = 2
loop: beq $t0, $t5, end     # t3 = 7
      addi $t0, $t0, 1      # t4 = 15
      j loop          
end:  .word 0xfeedfeed 