
#include <avr/io.h>
#include <util/delay.h>

#define PORT_LCD PORTD	
#define DDR_LCD DDRD

/* установка лог. "1" на линии E */
#define LCD_E_SET    PORT_LCD |=0x02
/* установка лог. "0" на линии E */
#define LCD_E_CLR    PORT_LCD &=0xFD
/* установка лог. "1" на линии RS */
#define LCD_RS_SET   PORT_LCD|=0x01
/* установка лог. "0" на линии RS */
#define LCD_RS_CLR   PORT_LCD&=0xFE

/* макрос, указывающий функции, что
передаются команды */
#define LCD_COMMAND  0
/* макрос, указывающий функции, что
передаются данные */
#define LCD_DATA     1


#define lcd_putc(x)  lcd_putbyte(x, LCD_DATA)


/* инициализация портов,
подключенных к жки */
void init_port()
{
	PORT_LCD=0x00;
    DDR_LCD=0xFF;
}

/* функция передачи тетрады в жки */
void lcd_putnibble(char t)
{
    t<<=4;
    LCD_E_SET;
    _delay_us(50);
    PORTD&=0x0F;
    PORTD|=t;
    LCD_E_CLR;
    _delay_us(50);
}

/* функция передачи байта в жки.
char c - сам байт
char rs - переменная, указывающая что передается:
     rs = 0 - команда (устанавливается линия RS)
	 rs = 1 - данные (сбрасывается линия RS) */
void lcd_putbyte(char c, char rs)
{
    char highc=0;
    highc=c>>4;
	if (rs==LCD_COMMAND) LCD_RS_CLR;
	else                 LCD_RS_SET;
    lcd_putnibble(highc);
    lcd_putnibble(c);
}

void lcd_putchar(char c)
{
    char highc=0;
    highc=c>>4;
	LCD_RS_SET;
    lcd_putnibble(highc);
    lcd_putnibble(c);
}

/* функция инициализации работы жки
в 4-битном режиме, без курсора */
void lcd_init()
{
    _delay_ms(15);
    lcd_putnibble(0b00000011);
    _delay_ms(4);
    lcd_putnibble(0b00000011);
    _delay_us(100);
    lcd_putnibble(0b00000011);
	 _delay_ms(1);
    lcd_putnibble(0b00000010);
	_delay_ms(1);
    lcd_putbyte(0x28, LCD_COMMAND); // стоит (0x28) 5x8 0b00101000 меняем (0x2C) 5x10 0b00101100 
    _delay_ms(1);
    lcd_putbyte(0x0C, LCD_COMMAND);
    _delay_ms(1);
    lcd_putbyte(0x06, LCD_COMMAND);
    _delay_ms(1);
}


/* функция очистки дисплея и возврата
курсора в начальную позицию*/
void lcd_clear()
{
    lcd_putbyte(0x01, LCD_COMMAND);
    _delay_us(1500);
}



/* функция перемещения курсора в заданную позицию
col - номер знакоместа по горизонтальной оси (от 0 до 15)
row - номер строки (0 или 1) */
void lcd_gotoxy(char col, char row)
{
  char adr;
  adr=0x40*row+col;
  adr|=0x80;
  lcd_putbyte(adr, LCD_COMMAND);
}

void lcd_putstring (char stroka[])
{  char i;
   for(i=0;stroka[i]!='\0';i++)
   lcd_putchar(stroka[i]);
}	