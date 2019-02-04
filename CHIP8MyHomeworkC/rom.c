#pragma once

#include <stdio.h>

char * ROM_NAME = "_TETRIS";

void loadRom();
int getSizeOfFile(FILE*);

void loadRom(){

	printf("\n");
	printf("######################################################\n");
	printf("################### LOADING ROM ######################\n");
	printf("######################################################\n\n");

	FILE * fp;

	fp = fopen(ROM_NAME, "rb"); // b is binary mode

	int sz = getSizeOfFile(fp);

	for(int i = 0, j = 0x200; i < sz; i++, j++){
		memory[j] = fgetc(fp);
	}

	fclose(fp);
}

int getSizeOfFile(FILE* fp){
	fseek(fp, 0L, SEEK_END);
	int sz = ftell(fp);
	fseek(fp, 0L, SEEK_SET); // return to the beginning
	printf("Size: %i (0x%x)\n", sz, sz);
	return sz;
}