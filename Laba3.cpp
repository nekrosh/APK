
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#define NEW_MASTER_BASE_ADDR 0x08
#define NEW_SLAVE_BASE_ADDR  0x70

const short attr1 = 0x5E00;
const short attr2 = 0x5C00;

short attr;

void interrupt(*old_irq0)(...);
void interrupt(*old_irq1)(...);
void interrupt(*old_irq2)(...);
void interrupt(*old_irq3)(...);
void interrupt(*old_irq4)(...);
void interrupt(*old_irq5)(...);
void interrupt(*old_irq6)(...);
void interrupt(*old_irq7)(...);
void interrupt(*old_irq8)(...);
void interrupt(*old_irq9)(...);
void interrupt(*old_irq10)(...);
void interrupt(*old_irq11)(...);
void interrupt(*old_irq12)(...);
void interrupt(*old_irq13)(...);
void interrupt(*old_irq14)(...);
void interrupt(*old_irq15)(...);

void init(void);
void output_regs(void);
void print_reg(short far* screen, short val);
void output_symb(short far* screen, short symb);

void interrupt new_irq0(...);
void interrupt new_irq1(...);
void interrupt new_irq2(...);
void interrupt new_irq3(...);
void interrupt new_irq4(...);
void interrupt new_irq5(...);
void interrupt new_irq6(...);
void interrupt new_irq7(...);
void interrupt new_irq8(...);
void interrupt new_irq9(...);
void interrupt new_irq10(...);
void interrupt new_irq11(...);
void interrupt new_irq12(...);
void interrupt new_irq13(...);
void interrupt new_irq14(...);
void interrupt new_irq15(...);

int main()
{
	unsigned far* fp;
	attr = attr1;

	init();
	clrscr();
	FP_SEG(fp) = _psp; // получаем сегмент
	FP_OFF(fp) = 0x2c; // смещение сегмента данных с переменными среды
	_dos_freemem(*fp);	// освобождение памяти
	_dos_keep(0, (_DS - _CS) + (_SP / 16) + 1); // резидентной

	return 0;
}

void init()
{
	//Старые оброботчики
	old_irq0 = getvect(0x08);
	old_irq1 = getvect(0x09);
	old_irq2 = getvect(0x0A);
	old_irq3 = getvect(0x0B);
	old_irq4 = getvect(0x0C);
	old_irq5 = getvect(0x0D);
	old_irq6 = getvect(0x0E);
	old_irq7 = getvect(0x0F);
	old_irq8 = getvect(0x70);
	old_irq9 = getvect(0x71);
	old_irq10 = getvect(0x72);
	old_irq11 = getvect(0x73);
	old_irq12 = getvect(0x74);
	old_irq13 = getvect(0x75);
	old_irq14 = getvect(0x76);
	old_irq15 = getvect(0x77);
	// Свои обработчики
	setvect(0x08, &new_irq0);
	setvect(0x09, &new_irq1);
	setvect(0x0A, &new_irq2);
	setvect(0x0B, &new_irq3);
	setvect(0x0C, &new_irq4);
	setvect(0x0D, &new_irq5);
	setvect(0x0E, &new_irq6);
	setvect(0x0F, &new_irq7);
	setvect(0x70, &new_irq8);
	setvect(0x71, &new_irq9);
	setvect(0x72, &new_irq10);
	setvect(0x73, &new_irq11);
	setvect(0x74, &new_irq12);
	setvect(0x75, &new_irq13);
	setvect(0x76, &new_irq14);
	setvect(0x77, &new_irq15);

	_disable();//запрет прерывания

	outp(0x20, 0x11);                 // icw1	
	outp(0x21, NEW_MASTER_BASE_ADDR); // icw2 
	outp(0xA1, NEW_SLAVE_BASE_ADDR);  // icw2 
	outp(0x21, 0x04);				  // icw3 
	outp(0xA1, 0x02); 				  // icw3 
	outp(0x21, 0x01);				  // icw4 
	outp(0xA1, 0x01);                 // icw4  


	//Разрешить прерывания
	_enable();
}

void print_symb(short far* screen, short symb)
{
	*screen = attr;
	*screen |= symb;
}

void print_reg(short far* screen, short val)
{
	int i;
	for (i = 7; i >= 0; i--)
	{
		print_symb(screen, val % 2 + '0');
		screen++;
		val /= 2;
	}
}

void output_regs()
{
	short far* screen = (short far*)MK_FP(0xB800, 0);

	print_reg(screen, inp(0x21)); // master mask register
	screen += 8;
	print_symb(screen, 0x20);
	screen++;

	outp(0x20, 0x0A);
	print_reg(screen, inp(0x20)); // master request register
	screen += 8;
	print_symb(screen, 0x20);
	screen++;

	outp(0x20, 0x0B);
	print_reg(screen, inp(0x20)); // master service register
	screen += 8;
	print_symb(screen, 0x20);
	screen++;

	FP_OFF(screen) = 0xA0;

	print_reg(screen, inp(0xA1)); // slave mask register
	screen += 8;
	print_symb(screen, 0x20);
	screen++;

	outp(0xA0, 0x0A);
	print_reg(screen, inp(0xA0)); // slave request register
	screen += 8;
	print_symb(screen, 0x20);
	screen++;

	outp(0xA0, 0x0B);
	print_reg(screen, inp(0xA0)); // slave service register*/
}

void interrupt new_irq0(...) { old_irq0();  output_regs(); }
void interrupt new_irq1(...); // keyboard
void interrupt new_irq2(...) { old_irq2();  output_regs(); }
void interrupt new_irq3(...) { old_irq3();  output_regs(); }
void interrupt new_irq4(...) { old_irq4();  output_regs(); }
void interrupt new_irq5(...) { old_irq5();  output_regs(); }
void interrupt new_irq6(...) { old_irq6();  output_regs(); }
void interrupt new_irq7(...) { old_irq7();  output_regs(); }
void interrupt new_irq8(...) { old_irq8();  output_regs(); }
void interrupt new_irq9(...) { old_irq9();  output_regs(); }
void interrupt new_irq10(...) { old_irq10(); output_regs(); }
void interrupt new_irq11(...) { old_irq11(); output_regs(); }
void interrupt new_irq12(...) { old_irq12(); output_regs(); }
void interrupt new_irq13(...) { old_irq13(); output_regs(); }
void interrupt new_irq14(...) { old_irq14(); output_regs(); }
void interrupt new_irq15(...) { old_irq15(); output_regs(); }

void interrupt new_irq1(...)
{
	if (attr == attr1)
	{
		attr = attr2;
	}
	else
	{
		attr = attr1;
	}
	output_regs();
	old_irq1();
	
	
}
