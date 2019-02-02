#pragma once

#include <stdio.h>

void loadRom();
int getSizeOfFile(FILE*);

void loadRom(){

	printf("\n");
	printf("######################################################\n");
	printf("################### LOADING ROM ######################\n");
	printf("######################################################\n\n");

	/*
		After you have initialized the emulator, load the program into the memory (use fopen in binary mode)
		and start filling the memory at location: 0x200 == 512.
	*/

	//https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm

	FILE * fp;

	fp = fopen("TETRIS", "rb"); // OPEN IN BINARY MODE

	int sz = getSizeOfFile(fp);

	for(int i = 0, j = 0x200; i < sz; i++, j++){
		memory[j] = fgetc(fp);
	}

	fclose(fp);
}

int getSizeOfFile(FILE* fp){
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET);
	printf("Size: %i (0x%x)\n\n", sz, sz);
	return sz;
}