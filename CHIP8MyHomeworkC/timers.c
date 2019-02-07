#pragma once

unsigned char getDelayTimer();
unsigned char getSoundTimer();
void setDelayTimer(unsigned char);
void setSoundTimer(unsigned char);

unsigned char delayTimer;
unsigned char soundTimer;

unsigned char getDelayTimer(){
	return delayTimer;
}

unsigned char getSoundTimer(){
	return soundTimer;
}

void setDelayTimer(unsigned char c){
	delayTimer = c;
}

void setSoundTimer(unsigned char c){
	soundTimer = c;
}