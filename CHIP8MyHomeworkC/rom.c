#pragma once

#include <stdio.h>

void loadRom(){

	printf("\n");
	printf("######################################################\n");
	printf("################### LOADING ROM ######N###############\n");
	printf("######################################################\n\n");

	/*
		After you have initialized the emulator, load the program into the memory (use fopen in binary mode)
		and start filling the memory at location: 0x200 == 512.
	*/

	//https://www.tutorialspoint.com/c_standard_library/c_function_fopen.htm

	FILE * fp;

	fp = fopen("TETRIS", "rb"); // OPEN IN BINARY MODE

	fseek(fp, 0L, SEEK_END);

	int sz = ftell(fp);

	printf("Size: %i (0x%x)\n\n", sz, sz);

	fgets(memory, sz, (FILE*)fp);
	printf("Rom: %s\n\n", memory);
}