main: addiu $t0, $zero, 10          # testing the other add instructions
      addiu $t1, $zero, 5
      addiu $t2, $zero, -11
      add  $s0, $t0, $t1
      addu $s1, $t0, $t1
      add  $s2, $t0, $t2
      addu $s3, $t0, $t2
      add  $s4, $s3, $s3
      addu $s5, $s4, $s4
     .word 0xfeedfeed
