LOOP_START:
  LOAD R8, [R0 + 0]       // Fetch A[row][k]
  LOAD R9, [R1 + 0]       // Fetch B[k][t]
  
  FMAD R5, R8, R9, R5     // Accumulate: R5 = (R8 * R9) + R5
  
  ADD R0, R0, 1           // Move A pointer right by 1 column
  ADD R1, R1, 32          // Move B pointer down by 1 row (Stride = 32)
  ADD R4, R4, 1           // k = k + 1
  
  CMP.LT P0, R4, R6       // Is k < 32? (Save answer to Predicate 0)
  JUMP P0, LOOP_START       // If P0 is True, jump backward!

  STORE R5, [R2 + 0]      // Loop finished! Save accumulator to Matrix C memory

END_OF_PROGRAM:
