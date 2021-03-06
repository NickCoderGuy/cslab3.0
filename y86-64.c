#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

const int MAX_MEM_SIZE  = (1 << 13);

void fetchStage(int *icode, int *ifun, int *rA, int *rB, wordType *valC, wordType *valP) {
    
    pc = getPC();

    byteType a = getByteFromMemory(pc);

    *icode = (a >> 4) & 0xf;
    *ifun = (a & 0xf);


    if(*icode == JXX || *icode == CALL){

        *valC = getWordFromMemory(pc + 1);
        *valP = (pc + 9);

    }

    if(*icode == RET){

        *valP = (pc + 1);

    }



    if(*icode == NOP){

        
        *valP = (pc + 1);

    }

    if(*icode == PUSHQ){
	
        a = getByteFromMemory(pc + 1);
        *rA = ((a >> 4) & 0xf);
        *rB = (a & 0xf);
        *valP = (pc + 2);

    }
	
    if(*icode == OPQ || *icode == RRMOVQ){

        a = getByteFromMemory(pc + 1);
        *rA = ((a >> 4) & 0xf);
        *rB = (a & 0xf);
        
        *valP = (pc + 2);

    }


    if(*icode == IRMOVQ || *icode == RMMOVQ || *icode == MRMOVQ){
	
        a = getByteFromMemory(pc + 1);
        *rA = ((a >> 4) & 0xf);
        *rB = (a & 0xf);

        *valC = getWordFromMemory(pc + 2);

        *valP = (pc + 10);

    }

    if(*icode == POPQ){
	
        a = getByteFromMemory(pc + 1);
        *rA = ((a >> 4) & 0xf);
        *rB = (a & 0xf);
        *valP = (pc + 2);

    }

    if(*icode == HALT){
        *valP = (pc + 1);
        setStatus(STAT_HLT);
    }


	

}

void decodeStage(int icode, int rA, int rB, wordType *valA, wordType *valB) {
	

    if(icode == PUSHQ){

        *valA = (wordType) getRegister(rA);
        *valB = (wordType) getRegister(RSP);

    }

    if(icode == CALL){
    
    	*valB = (wordType) getRegister(RSP);
    
    }

    if(icode == POPQ || icode == RET){

        *valA = (wordType) getRegister(RSP);
        *valB = (wordType) getRegister(RSP);

    }

    if(icode == OPQ || icode == RMMOVQ){

        *valA = getRegister(rA);
        *valB = getRegister(rB);

    }

    if(icode == RRMOVQ){

        *valA = getRegister(rA);

    }

    if(icode == MRMOVQ){
    
    	*valB = getRegister(rB);
    
    }

}

void executeStage(int icode, int ifun, wordType valA, wordType valB, wordType valC, wordType *valE, bool *Cnd) {
  

    if(icode == PUSHQ || icode == CALL){

        *valE = (wordType) (valB + (-8));

    }

    if(icode == RMMOVQ || icode == MRMOVQ){
    
    	*valE = valB + valC;

    }

    if(icode == POPQ || icode == RET){

        *valE = (wordType) (valB + 8);

    }
    
     if(icode == IRMOVQ){

        *valE = valC;

    }


    if(icode == JXX){
    
    	*Cnd = Cond(ifun);
    
    }

    if(icode == OPQ){

        if(ifun == ADD){

            *valE = valB + valA;
		
	    if(((valB < 0) == (valA < 0)) && (((*valE) < 0) != (valA < 0)) && (*valE < 0)){

                setFlags(TRUE,FALSE,TRUE);
                
            }


	    else if((valB + valA) == 0){

                setFlags(FALSE,TRUE,FALSE);

            }
	    else if((valB + valA) < 0){

                setFlags(TRUE,FALSE,FALSE);

            }

	    else if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0))){

                setFlags(FALSE,FALSE,TRUE);

            }

	    else{
	    
		setFlags(FALSE,FALSE,FALSE);

	    }

        }

        if(ifun == SUB){

            *valE = valB - valA;
	    
	    if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0)) && (*valE < 0)){

                setFlags(TRUE,FALSE,TRUE);
                
            }


	    else if((valB - valA) == 0){

                setFlags(FALSE,TRUE,FALSE);

            }
	    else if((valB - valA) < 0){

                setFlags(TRUE,FALSE,FALSE);

            }

	    else if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0))){

                setFlags(FALSE,FALSE,TRUE);

            }

	     else{

                setFlags(FALSE,FALSE,FALSE);

            }


        }
        if(ifun == XOR){

            *valE = valB ^ valA;
		
	    if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0)) && (*valE < 0)){

                setFlags(TRUE,FALSE,TRUE);
                
            }

	    else if((valB ^ valA) == 0){

                setFlags(FALSE,TRUE,FALSE);

            }
	    else if((valB ^ valA) < 0){

                setFlags(TRUE,FALSE,FALSE);

            }

	    else if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0))){

                setFlags(FALSE,FALSE,TRUE);

            }

	     else{

                setFlags(FALSE,FALSE,FALSE);

            }


        }

        if(ifun == AND){

            *valE = valB & valA;
		
	    if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0)) && (*valE < 0)){

                setFlags(TRUE,FALSE,TRUE);
                
            }

	    else if((valB & valA) == 0){

                setFlags(FALSE,TRUE,FALSE);

            }
	    else if((valB & valA) < 0){

                setFlags(TRUE,FALSE,FALSE);

            }

	    else if(((valB < 0) == (valA < 0)) && ((*valE < 0) != (valA < 0))){

                setFlags(FALSE,FALSE,TRUE);

            }

	     else{

                setFlags(FALSE,FALSE,FALSE);

            }


        }
        
    }

    if(icode == RRMOVQ){

        *valE = valA;

    }

}

void memoryStage(int icode, wordType valA, wordType valP, wordType valE, wordType *valM) {


    if(icode == PUSHQ){

        setWordInMemory(valE,valA);

    }

    if(icode == POPQ || icode == RET){

        setWordInMemory(*valM, getWordFromMemory(valA));

    }

    if(icode == RMMOVQ){

        setWordInMemory(valE,valA);

    }

    if(icode == MRMOVQ){

        setWordInMemory(*valM, getWordFromMemory(valE));

    }

    if(icode == CALL){
    
    	setWordInMemory(valE,valP);
    
    }

   

}

void writebackStage(int icode, int rA, int rB, wordType valE, wordType valM) {
 

    if(icode == PUSHQ || icode == CALL || icode == RET){

        setRegister(RSP,valE);

    }

    if(icode == POPQ){

        setRegister(RSP,valE);
        setRegister(rA,valM);

    }

    if(icode == IRMOVQ || icode == OPQ || icode == RRMOVQ){

        setRegister(rB,valE);

    }

    if(icode == MRMOVQ){

        setRegister(rA,valM);

    }

}

void pcUpdateStage(int icode, wordType valC, wordType valP, bool Cnd, wordType valM) {
	
	if(icode != JXX){
	    setPC(valP);
	}

	if(icode == JXX){
		
		if(Cnd == TRUE){
		
			setPC(valC);
		
		}
		else{
		
			setPC(valP);
		
		}

	}

}

void stepMachine(int stepMode) {
  /* FETCH STAGE */
  int icode = 0, ifun = 0;
  int rA = 0, rB = 0;
  wordType valC = 0;
  wordType valP = 0;
 
  /* DECODE STAGE */
  wordType valA = 0;
  wordType valB = 0;

  /* EXECUTE STAGE */
  wordType valE = 0;
  bool Cnd = 0;

  /* MEMORY STAGE */
  wordType valM = 0;

  fetchStage(&icode, &ifun, &rA, &rB, &valC, &valP);
  applyStageStepMode(stepMode, "Fetch", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

  decodeStage(icode, rA, rB, &valA, &valB);
  applyStageStepMode(stepMode, "Decode", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  executeStage(icode, ifun, valA, valB, valC, &valE, &Cnd);
  applyStageStepMode(stepMode, "Execute", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  memoryStage(icode, valA, valP, valE, &valM);
  applyStageStepMode(stepMode, "Memory", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  writebackStage(icode, rA, rB, valE, valM);
  applyStageStepMode(stepMode, "Writeback", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);
  
  pcUpdateStage(icode, valC, valP, Cnd, valM);
  applyStageStepMode(stepMode, "PC update", icode, ifun, rA, rB, valC, valP, valA, valB, valE, Cnd, valM);

  incrementCycleCounter();
}

/** 
 * main
 * */
int main(int argc, char **argv) {
  int stepMode = 0;
  FILE *input = parseCommandLine(argc, argv, &stepMode);

  initializeMemory(MAX_MEM_SIZE);
  initializeRegisters();
  loadMemory(input);

  applyStepMode(stepMode);
  while (getStatus() != STAT_HLT) {
    stepMachine(stepMode);
    applyStepMode(stepMode);
#ifdef DEBUG
    printMachineState();
    printf("\n");
#endif
  }
  printMachineState();
  return 0;
}
