#include <dos.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

struct attrch {
	unsigned char chr;
	unsigned char attr;
};

unsigned int counter;

void interrupt far(*oldIntRTC)(...);
void interrupt far(*oldIntAlarm)(...);

void interrupt far newIntRTC(...) {
	--counter;
	if (oldIntRTC != NULL)	oldIntRTC();
}

int enableTimer(unsigned int delay) {
	counter = delay;
	_disable();
	oldIntRTC = getvect(0x70);
	setvect(0x70, newIntRTC);
	_enable();
	outp(0xA1, inp(0xA1) & 0xFE);
	outp(0x70, 0xB);
	char oldState = inp(0x71);
	outp(0x71, oldState | 0x40);

	while (counter > 0);

	_disable();
	setvect(0x70, oldIntRTC);
	enable();
	outp(0x70, 0xB);
	outp(0x71, oldState);
	printf("Timer finished. \n");
	return 0;
}

void outputBinary(int content, int length = 8) {
	int mask = (1 << (length - 1));
	for (int i = 0; i < length; i++) {
		putchar('0' + ((content & mask) >> (length - 1)));
		content <<= 1;
	}
	putchar('\n');
}

int binaryToBcd(int value) {
	int result = 0;
	for (int i = 0; i < sizeof(value) * 2; i++) {
		result += (value % 10) << (4 * i);
		value /= 10;
	}
	return result;
}



int printRtcTimeDate() {
	outp(0x70, 0xA);
	int state = inp(0x71);
	if (!(state & 0x80)) {
		int ports[] = { 0x09, 0x08, 0x07, 0x06, 0x04, 0x02, 0x00 };

		for (int i = 4; i < 7; i++) {
			outp(0x70, ports[i]);
			char temp = inp(0x71);
			printf("%d%d ", temp >> 4, temp & 0x0F);
		}
		putchar('\n');
	}
	else {
		printf("RTC is not available. \n");
	}
	return 0;
}
unsigned char toBcd(int val)
{
	return (unsigned char)((val / 10 * 16) + (val % 10));
}
int setRtcTimeDate() {
	char data[6];

	unsigned int reg[] = { 0x00, 0x02, 0x04, 0x07, 0x08, 0x09 };
	int temp;
	do {
		fflush(stdin);
		printf("hh: ");
		scanf("%i", &temp);
	} while ((temp > 23 || temp < 0));
	data[2] = toBcd(temp);

	do {
		fflush(stdin);
		printf("mm: ");
		scanf("%i", &temp);
	} while (temp > 59 || temp < 0);
	data[1] = toBcd(temp);

	do {
		fflush(stdin);
		printf("ss: ");
		scanf("%i", &temp);
	} while (temp > 59 || temp < 0);
	data[0] = toBcd(temp);

	int res;
	do {
		outp(0x70, 0x0A);
		res = inp(0x71) & 0x80;
	} while (res);


	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x80);

	for (int i = 0; i < 3; i++)
	{

		outp(0x70, reg[i]);

		outp(0x71, data[i]);
	}


	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0x7F);
	return 0;
}

int disableAlarm() {
	if (oldIntAlarm != NULL)	setvect(0x4A, oldIntAlarm);
	outp(0x70, 0xB);
	outp(0x71, inp(0x71) & 0xDF);
	return 0;
}



#define screen_w 80
void interrupt far newIntAlarm(...) {
	attrch far* screen = (attrch far*)MK_FP(0xB800, 23 * screen_w * 2 + screen_w);
	char message[] = "Alarm!!!! ";
	for (int i = 0; i < sizeof(message); i++) {
		screen->chr = message[i];
		screen++;
	}
	disableAlarm();
}

int enableAlarm() {
	int ports[] = { 0x5, 0x3, 0x1 };
	int bounds[] = { 24, 60, 60 };
	char labels[3][3] = { "hh", "mm", "ss" };
	unsigned int temp;
	outp(0x70, 0xB);
	char oldState = inp(0x71);
	outp(0x71, oldState | 0x04);

	for (int i = 0; i < 3; i++) {
		do {
			fseek(stdin, 0, SEEK_END);
			printf("%s : ", labels[i]);
		} while (!scanf("%ud", &temp) || !(temp < bounds[i]));
		outp(0x70, 0xA);
		if (!(inp(0x71) & 0x80)) {
			outp(0x70, ports[i]);
			outp(0x71, binaryToBcd(temp));
		}
		else {
			--i;
			printf("RTC is not available/ \n");
		}
	}
	outp(0x70, 0xB);
	outp(0x71, oldState);

	_disable();
	oldIntAlarm = getvect(0x4A);
	setvect(0x4A, newIntAlarm);
	_enable();

	outp(0x70, 0xB);
	outp(0x71, inp(0x71) | 0x20);

	outp(0xA1, inp(0xA1) & 0xFE);
	return 0;
}



int main() {
	char action;
	do {
		printf("1. Print  time \n");
		printf("2. Set  time \n");
		printf("3. Timer \n");
		printf("4. Set alarm. \n");
		printf("q. Exit program. \n");
		fseek(stdin, 0, SEEK_END);
		action = getchar();
		switch (action) {
		case '1':
			printRtcTimeDate();
			break;
		case '2':
			setRtcTimeDate();
			break;
		case '3':
			unsigned int delay;
			do {
				fseek(stdin, 0, SEEK_END);
				printf("Timer duration in milliseconds: ");
			} while (!scanf("%ud", &delay) || delay == 0);
			enableTimer(delay);
			break;
		case '4':
			enableAlarm();
			break;
		case 'q':
			break;
		}
	} while (action != 'q');
	return 0;
}