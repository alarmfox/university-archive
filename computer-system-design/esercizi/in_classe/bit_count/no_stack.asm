*programma principale

        ORG     $8000
MAIN    MOVE.B  A,D0
        JSR     COUNT_FN
        MOVE.B  A+1,D0
        MOVE.B  D2,D4
        JSR     COUNT_FN
FINE    JMP     FINE

*funzione che conta i bit alti in un byte
*accetta in ingresso il byte da controllare D0
          ORG     $8200
COUNT_FN  CLR.B   D1        *i ciclo
          CLR.B   D2        *risultato
          LEA     MASK,A0   *carico le maschere dalla memoria in A0
LOOP      CMP.B   #N,D1     
          BGE     STOP
          MOVE.B  (A0)+,D3  *copio la machera da testare in D3
          AND.B   D0,D3     *confronto la word con la maschera
          BEQ.S   CONT      *la AND mette a 0 il bit eq del registro di stato
          ADD.B   #1,D2     *aggiungo 1 al risultato
CONT      ADD.B   #1,D1     *aggiunto 1 alla i 
          JMP     LOOP      
STOP      RTS

*area dati
          ORG     $8100
A         DC.W    1023
N         EQU     8 
MASK      DC.B    $80,$40,$20,$10,$8,$4,$2,$1 
          END     MAIN
