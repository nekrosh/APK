#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define screen_w 80

unsigned char global_attribute = 0x07;
struct attrch {
	unsigned char chr;
	unsigned char attr;
};
void outputICRegs();

void interrupt(*oldInt0) (...);
void interrupt(*oldInt1) (...); //keyboard
void interrupt(*oldInt2) (...);
void interrupt(*oldInt3) (...);
void interrupt(*oldInt4) (...);
void interrupt(*oldInt5) (...);
void interrupt(*oldInt6) (...);
void interrupt(*oldInt7) (...);
void interrupt(*oldInt8) (...);
void interrupt(*oldInt9) (...);
void interrupt(*oldIntA) (...);
void interrupt(*oldIntB) (...);
void interrupt(*oldIntC) (...);
void interrupt(*oldIntD) (...);
void interrupt(*oldIntE) (...);
void interrupt(*oldIntF) (...);

void interrupt newInt0(...) { outputICRegs(); oldInt0(); }
void interrupt newInt1(...) { global_attribute++; outputICRegs(); oldInt1(); }
void interrupt newInt2(...) { outputICRegs(); oldInt2(); }
void interrupt newInt3(...) { outputICRegs(); oldInt3(); }
void interrupt newInt4(...) { outputICRegs(); oldInt4(); }
void interrupt newInt5(...) { outputICRegs(); oldInt5(); }
void interrupt newInt6(...) { outputICRegs(); oldInt6(); }
void interrupt newInt7(...) { outputICRegs(); oldInt7(); }
void interrupt newInt8(...) { outputICRegs(); oldInt8(); }
void interrupt newInt9(...) { outputICRegs(); oldInt9(); }
void interrupt newIntA(...) { outputICRegs(); oldIntA(); }
void interrupt newIntB(...) { outputICRegs(); oldIntB(); }
void interrupt newIntD(...) { outputICRegs(); oldIntD(); }
void interrupt newIntC(...) { outputICRegs(); oldIntC(); }
void interrupt newIntE(...) { outputICRegs(); oldIntE(); }
void interrupt newIntF(...) { outputICRegs(); oldIntF(); }

void init() {
	oldInt0 = getvect(0x08);
	oldInt1 = getvect(0x09);
	oldInt2 = getvect(0x0A);
	oldInt3 = getvect(0x0B);
	oldInt4 = getvect(0x0C);
	oldInt5 = getvect(0x0D);
	oldInt6 = getvect(0x0E);
	oldInt7 = getvect(0x0F);

	oldInt8 = getvect(0x70);
	oldInt9 = getvect(0x71);
	oldIntA = getvect(0x72);
	oldIntB = getvect(0x73);
	oldIntC = getvect(0x74);
	oldIntD = getvect(0x75);
	oldIntE = getvect(0x76);
	oldIntF = getvect(0x77);


	setvect(0x08, newInt0);
	setvect(0x09, newInt1);
	setvect(0x0A, newInt2);
	setvect(0x0B, newInt3);
	setvect(0x0C, newInt4);
	setvect(0x0D, newInt5);
	setvect(0x0E, newInt6);
	setvect(0x0F, newInt7);

	setvect(0xA8, newInt8);
	setvect(0xA9, newInt9);
	setvect(0xAA, newIntA);
	setvect(0xAB, newIntB);
	setvect(0xAC, newIntC);
	setvect(0xAD, newIntD);
	setvect(0xAE, newIntE);
	setvect(0xAF, newIntF);

	_disable();
	outp(0x20, 0x11);
	outp(0x21, 0x08);
	outp(0x21, 0x04);
	outp(0x21, 0x01);

	outp(0xA0, 0x11);
	outp(0xA1, 0x70);
	outp(0xA1, 0x02);
	outp(0xA1, 0x01);
	_enable();
}

void my_print(attrch far* screen, int val) {
	for (int i = 0; i < 8; i++) {
		attrch temp = { '0' + (val & 1), global_attribute };
		*screen = temp;
		val >> 1;
		screen++;
	}
}

void outputICRegs() {
	attrch far* screen = (attrch far*)MK_FP(0xB800, 0);
	my_print(screen, inp(0x21));
	screen+= screen_w;
	outp(0x20, 0x0A);
	my_print(screen, inp(0x20));
	screen+=screen_w;
	outp(0x20, 0x0B);
	my_print(screen, inp(0x20));
	screen += screen_w;
	

	screen += screen_w;
	my_print(screen, inp(0xA1));
	screen += screen_w;
	outp(0xA0, 0x0A);
	my_print(screen, inp(0xA0));
	screen += screen_w;
	outp(0xA0, 0x0B);
	my_print(screen, inp(0xA0));
}

int main() {
	init();
	clrscr();

	unsigned far* fp;
	FP_SEG(fp) = _psp;
	FP_OFF(fp) = 0x2c;
	_dos_freemem(*fp);
	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1);
	return 0;
}
