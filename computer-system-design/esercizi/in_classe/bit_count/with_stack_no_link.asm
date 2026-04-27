* programma principale
		ORG 		$8000
MAIN 		ADDA		#-2,SP 			*riservo una word vuoto per il risultato
		MOVE.B	A,-(SP) 			*push del byte da controllare
		MOVE.B	#N,-(SP)		*push della dimensione del vettore
		MOVE.L	#MASK,-(SP)		*push dell'indirizzo del primo elemento
		JSR		COUNT_FN
		MOVE.W	(SP)+,D0		*prendo il risultato
		ADDA		#-2,SP 			*riservo una word vuoto per il risultato
		MOVE.B	A+1,-(SP) 			*push del byte da controllare
		MOVE.B	#N,-(SP)		*push della dimensione del vettore
		MOVE.L	#MASK,-(SP)		*push dell'indirizzo del primo elemento
		JSR		COUNT_FN
		ADD		(SP)+,D0
FINE 		JMP 		FINE

* funzione che conta i bit alti in un byte.
* accetta in ingresso il byte da controllare e il vettore delle maschere dallo stack.
		ORG		$8200
COUNT_FN	MOVEA.L	RETOFF(SP),A1
		CLR.B		D1		* i cliclo
		CLR.B 		D2		* risultato
		MOVE.L	MASKOFF(SP),A0
		MOVE.B	WORDOFF(SP),D5
		MOVE.B	DIMOFF(SP),D4
LOOP		CMP.B		D4,D1
		BGE 		STOP
		MOVE.B	(A0)+,D3 	* copio la maschera da testare in D3
		AND.B		D5,D3		* confronto la word con la maschera
		BEQ.S		CONT
		ADD		#1,D2		* aggiungo 1 al risultato
CONT		ADD		#1,D1		* aggiungo 1 alla i
		JMP 		LOOP
STOP		MOVE.W	D2,RESOFF(SP)
		ADDA.L	#RESOFF,SP
		JMP (A1)

*area dati
		ORG 		$8100
A		DC.W		1023
N		EQU		8
MASK		DC.B		$80,$40,$20,$10,$8,$4,$2,$1
RETOFF	EQU		0
MASKOFF	EQU		4
DIMOFF	EQU		8
WORDOFF	EQU		9
RESOFF	EQU		10		
		END		MAIN






