#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>


void outputClockDurations() {
	outp(0x43, 0xCE);
	for (char c = 0x40; c < 0x43; c++) {
		int temp = inp(c);

		temp += (inp(c) << 8);
		printf("%X \n", temp);
	}
}

void outputBinary(int content, int length) {
	for (int i = 0; i < length; i++) {
		putchar('0' + (content & 1));
		content >>= 1;
	}
	putchar('\n');
}

#define register_len 8
void outputStates() {
	outp(0x43, 0xEE);
	outputBinary(inp(0x40), register_len);
	outputBinary(inp(0x41), register_len);
	outputBinary(inp(0x42), register_len);
}

void setFrequency(unsigned int freq) {


	long base = 1193180, kd;
	int kd_;
	// Óñòàíàâëèâàåì ðåæèì 3 äëÿ âòîðîãî êàíàëà òàéìåðà, 10 11 011 0 – êàíàë, ñòàðøèé+ìëàäøèé áàéòû, ðåæèì, ôîðìàò (äâîè÷íûé)
	outp(0x43, 0xB6);
	kd = base / freq;
	kd_ = kd % 256;
	outp(0x42, kd_);
	kd_ = kd / 256;
	outp(0x42, kd_);
}
void playSound(unsigned int freq, unsigned int dur) {
	setFrequency(freq);
	// Âêëþ÷àåì ãðîìêîãîâîðèòåëü. Ñèãíàë îò
	// êàíàëà 2 òàéìåðà òåïåðü áóäåò ïðîõîäèòü
	// íà âõîä ãðîìêîãîâîðèòåëÿ.

	outp(0x61, inp(0x61) | 3);
	//Ïàóçà
	delay(dur);
	// Âûêëþ÷àåì ãðîìêîãîâîðèòåëü.
	outp(0x61, inp(0x61) & 0xFC);
}

#define melody_length 9
void playRecordedMelody() {
	unsigned int melody[melody_length] = { 329, 329, 329, 415, 523, 659, 587, 523, 659 };
	unsigned int durations[melody_length] = { 200, 100, 200, 400, 200, 200, 400, 200, 400 };
	for (int i = 0; i < melody_length; i++) {
		playSound(melody[i], durations[i]);
	}
}

int getRandomVal(unsigned int maxValue) {

	outp(0x43, 0xB4);
	outp(0x42, maxValue % 256);
	maxValue /= 256;
	outp(0x42, maxValue);


	outp(0x61, inp(0x61) | 0x01);
	outp(0x43, 0x80);


	int temp = inp(0x42);
	temp += (inp(0x42) << 8);

	outp(0x61, inp(0x61) & 0xFE);

	return temp;
}



int main() {
	char option;
	unsigned int maxRandomValue;
	do {
		printf("1. Play melody \n");
		printf("2. Generate random value \n");
		printf("3. Get clock state registers in binary \n");
		printf("4. Get clock line durations in hex \n");
		printf("5. Exit \n");
		option = getchar();

		switch (option) {
		case '1':
			playRecordedMelody();
			break;
		case '2':
			printf("Input high bound value: ");
			scanf("%u", &maxRandomValue);
			printf("\n %u \n", getRandomVal(maxRandomValue));
			break;
		case '3':
			outputStates();
			break;
		case '4':
			outputClockDurations();
			break;
		case '5':
			break;
		default:
			printf("Invalid input. \n");
		}
		fseek(stdin, 0, SEEK_END);
	} while (option != '5');
	return 0;
}
