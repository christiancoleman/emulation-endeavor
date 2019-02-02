#include <stdio.h>
#include "memory.c"
#include "processor.c"
#include "gpu.c"
#include "input.c"
#include "rom.c"

void startEmulation();
void consoleStart();
void consoleEnd();


int main (int argc, char **argv) {
	consoleStart();

	struct memory m;
	printf("Memory found at: 0x%x\n", &m);
	printf("Memory size is: %d (int bytes) and 0x%x (hex bytes)\n", sizeof(m), sizeof(m));

	consoleEnd();

	return 0;
}

void startEmulation(){
	while(true){
		executeCPU(5);
		//generateInterrupts();
		//emulateGraphics();
		//emulateSound();
		//emulateOtherSoftware();
		//timeSynchronization();
	}
}

void consoleStart(){
	printf("\n");
	printf("###########################################################\n");
	printf("###################### TinyGBoyC ##########################\n");
	printf("###########################################################\n");
	printf("\n");
}

void consoleEnd(){
	printf("\n");
	printf("\n");
	printf("\n");
}