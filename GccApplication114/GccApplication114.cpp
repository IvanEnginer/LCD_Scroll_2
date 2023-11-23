#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "TWI.h"
#include "LCD_I2C.h"
char unit[][32] =
{
	"Unit 1",
	"Unit 2",
	"Unit 3",
	"Unit 4",
	"Unit 5",
	"Unit 6",
	"Unit 7",
	"Unit 8",
	"Unit 9",
	"Unit 10",
	"Unit 11",
	"Unit 12",
	"Unit 13",
	"Unit 14",
	"Unit 15"
};
int NewState,OldState,vol,upState,dowState;

ISR(TIMER1_COMPA_vect)
{
	NewState=PINA&0b00000011;
	if (NewState!=OldState)
	{
		switch(OldState)
		{
			case 2:
			{
				if(NewState == 3) upState++;
				if(NewState == 0)dowState++;
				break;
			}
			case 0:
			{
				if(NewState == 2) upState++;
				if(NewState == 1) dowState++;
				break;
			}
			case 1:
			{
				if(NewState == 0) upState++;
				if(NewState == 3) dowState++;
				break;
			}
			case 3:
			{
				if(NewState == 1) upState++;
				if(NewState == 2) dowState++;
				break;
			}
		}
		OldState=NewState;
	}
	if (upState >= 4)
	{
		lcd_clear();
		lcd_putstring(unit[vol++]);
		if (vol==15)
		{
			vol=13;
		}
		lcd_gotoxy(0,1);
		lcd_putstring(unit[vol++]);
		if (vol==15)
		{
			vol=13;
		}
		upState = 0;
	}
	if (dowState >= 4)
	{
		if (vol==0)
		{
			vol=2;
			lcd_gotoxy(0,0);
			lcd_putstring(unit[vol--]);
		}		
		lcd_clear();
		lcd_putstring(unit[vol--]);
		if (vol==0)
		{
			lcd_gotoxy(0,1);
			lcd_putstring(unit[vol--]);
		}
		lcd_gotoxy(0,1);
		lcd_putstring(unit[vol--]);
		dowState = 0;
	}
	TCNT1H=0x00;
	TCNT1L=0x00;
}

int main(void)
{
	I2C_Init();
	lcd_init();
	lcd_clear();
	lcd_putstring(unit[0]);
	lcd_gotoxy(0,1);
	lcd_putstring(unit[1]);
	PORTA=0x03;
	DDRA=0x00;
	PORTB=0x00;
	DDRB=0xFF;
	TCCR1A=0x00;
	TCCR1B=0x0A;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x03;
	OCR1AL=0xE8;
	OCR1BH=0x00;
	OCR1BL=0x00;
	TIMSK=0x10;
	sei();
	while(1)
	{
	}
}