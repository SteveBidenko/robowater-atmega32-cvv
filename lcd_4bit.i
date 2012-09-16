
#pragma used+
sfrb TWBR=0;
sfrb TWSR=1;
sfrb TWAR=2;
sfrb TWDR=3;
sfrb ADCL=4;
sfrb ADCH=5;
sfrw ADCW=4;      
sfrb ADCSRA=6;
sfrb ADCSR=6;     
sfrb ADMUX=7;
sfrb ACSR=8;
sfrb UBRRL=9;
sfrb UCSRB=0xa;
sfrb UCSRA=0xb;
sfrb UDR=0xc;
sfrb SPCR=0xd;
sfrb SPSR=0xe;
sfrb SPDR=0xf;
sfrb PIND=0x10;
sfrb DDRD=0x11;
sfrb PORTD=0x12;
sfrb PINC=0x13;
sfrb DDRC=0x14;
sfrb PORTC=0x15;
sfrb PINB=0x16;
sfrb DDRB=0x17;
sfrb PORTB=0x18;
sfrb PINA=0x19;
sfrb DDRA=0x1a;
sfrb PORTA=0x1b;
sfrb EECR=0x1c;
sfrb EEDR=0x1d;
sfrb EEARL=0x1e;
sfrb EEARH=0x1f;
sfrw EEAR=0x1e;   
sfrb UBRRH=0x20;
sfrb UCSRC=0X20;
sfrb WDTCR=0x21;
sfrb ASSR=0x22;
sfrb OCR2=0x23;
sfrb TCNT2=0x24;
sfrb TCCR2=0x25;
sfrb ICR1L=0x26;
sfrb ICR1H=0x27;
sfrb OCR1BL=0x28;
sfrb OCR1BH=0x29;
sfrw OCR1B=0x28;  
sfrb OCR1AL=0x2a;
sfrb OCR1AH=0x2b;
sfrw OCR1A=0x2a;  
sfrb TCNT1L=0x2c;
sfrb TCNT1H=0x2d;
sfrw TCNT1=0x2c;  
sfrb TCCR1B=0x2e;
sfrb TCCR1A=0x2f;
sfrb SFIOR=0x30;
sfrb OSCCAL=0x31;
sfrb TCNT0=0x32;
sfrb TCCR0=0x33;
sfrb MCUCSR=0x34;
sfrb MCUCR=0x35;
sfrb TWCR=0x36;
sfrb SPMCR=0x37;
sfrb TIFR=0x38;
sfrb TIMSK=0x39;
sfrb GIFR=0x3a;
sfrb GICR=0x3b;
sfrb OCR0=0X3c;
sfrb SPL=0x3d;
sfrb SPH=0x3e;
sfrb SREG=0x3f;
#pragma used-

#asm
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x80
	.EQU __sm_mask=0x70
	.EQU __sm_powerdown=0x20
	.EQU __sm_powersave=0x30
	.EQU __sm_standby=0x60
	.EQU __sm_ext_standby=0x70
	.EQU __sm_adc_noise_red=0x10
	.SET power_ctrl_reg=mcucr
	#endif
#endasm

#pragma used+

void delay_us(unsigned int n);
void delay_ms(unsigned int n);

#pragma used-

typedef unsigned char 	byte;	
typedef unsigned int 	word;	
typedef char str_val[16                ];

enum en_event {
ev_none = 0,            
ev_left = 1,            
ev_enter = 2,           
ev_right = 3,           
ev_cancel,              
ev_secunda,             
ev_timer,               
ev_start,               
ev_stop,                
ev_alarm1,              
ev_alarm2,              
ev_filter,              
ev_freezing1,           
ev_freezing2,           
ev_freezing3,           
ev_term1_nf,            
ev_term2_nf,            
ev_term3_nf,            
ev_term4_nf             

};

struct st_datetime {
byte cHH, cMM, cSS;         
byte cyy, cmo, cdd;         
};

struct st_eliminate {
signed int shift;       
signed char scale;      
};

extern struct st_eeprom_par {
struct st_eliminate elims[4];            
byte tap_angle, fan_speed, ADC1, ADC2;        
int Ku, Ki, Kd;                             
int T_z, T_int;                             
int TW_out_Min, TW_out_Stop;                
int TA_in_Min, TA_out_Min, TA_out_prs;    
byte alert_status[12];           
byte season;          
byte alarm;           
byte c_alarm;         
byte terms;           
byte addr[4][9];  
} prim_par;

extern struct st_mode {
byte menu;            
byte run;             

byte initrun;         
byte pomp;            
byte fan;             
byte stop_sync_dt;    
byte sound;           
int k_angle_limit;   
byte ufo[9];          
byte print;           
} mode ;
extern enum en_event event;        

extern void init(void);
extern int read_term(byte);
extern void set_cur_dt (void);
extern void get_cur_dt (unsigned char);

extern struct st_datetime s_dt;

extern struct st_mode mode;
extern unsigned int time_integration;
extern byte timer1_valcoder;
extern unsigned int timer_start;
extern byte timer_stop;
extern byte timer_fan;
extern byte count_fan;

#pragma used+

extern void lcd_init(unsigned char dispAttr); 
extern void lcd_clrscr(void);
extern void lcd_home(void);
extern void lcd_gotoxy(unsigned char x, unsigned char y);
extern void lcd_putc(char c);
extern void lcd_puts(const char *s);
extern void lcd_line_menu(const char *s, unsigned char submenu);

extern void lcd_command(unsigned char cmd);
extern void lcd_data(unsigned char data);
#pragma used-

#pragma used+

static void _lcd_delay(void) {
#asm
    ldi   r31,15
__lcd_delay0:
    dec   r31                   ; 1 cycle
    brne  __lcd_delay0          ; 1 cycle
#endasm
}

static void _long_delay(void) {
#asm
    clr   r26
    clr   r27
__long_delay0:
    sbiw  r26,1         ;2 cycles
    brne  __long_delay0 ;2 cycles
#endasm
}

#pragma used-

static void lcd_e_toggle(void);

static void lcd_e_toggle(void) {
PORTB  |=  (1 << (6));;
delay_us(250);
PORTB  &= ~(1 << (6));;
}

static void lcd_write(byte data, byte rs) {

if (rs) PORTB |=  (1 << (5)); else PORTB &= ~(1 << (5));
PORTB &= ~(1 << (7));

DDRA     |= (1 << (4));
DDRA     |= (1 << (5));
DDRA |= (1 << (6));
DDRA |= (1 << (7));

PORTA &= ~(1 << (7));
PORTA &= ~(1 << (6));
PORTA &= ~(1 << (5));
PORTA &= ~(1 << (4));
if(data & 0x80) PORTA |= (1 << (7));
if(data & 0x40) PORTA |= (1 << (6));
if(data & 0x20) PORTA |= (1 << (5));
if(data & 0x10) PORTA |= (1 << (4));
lcd_e_toggle();

PORTA &= ~(1 << (7));
PORTA &= ~(1 << (6));
PORTA &= ~(1 << (5));
PORTA &= ~(1 << (4));
if(data & 0x08) PORTA |= (1 << (7));
if(data & 0x04) PORTA |= (1 << (6));
if(data & 0x02) PORTA |= (1 << (5));
if(data & 0x01) PORTA |= (1 << (4));
lcd_e_toggle();

PORTA |= (1 << (4));
PORTA |= (1 << (5));
PORTA |= (1 << (6));
PORTA |= (1 << (7));
}

static byte lcd_read(byte rs) {
byte data;

if (rs)
PORTB |=  (1 << (5));                       
else
PORTB &= ~(1 << (5));                        
PORTB |=  (1 << (7));                           

DDRA     &= ~(1 << (4));
DDRA     &= ~(1 << (5));
DDRA &= ~(1 << (6));
DDRA &= ~(1 << (7));

PORTB  |=  (1 << (6));;
delay_us(250);
data = 0;
if ( PINA     & (1 << (4)) ) data |= 0x10;
if ( DDRA     & (1 << (5)) ) data |= 0x20;
if ( DDRA     & (1 << (6)) ) data |= 0x40;
if ( DDRA     & (1 << (7)) ) data |= 0x80;
PORTB  &= ~(1 << (6));;
delay_us(250);                       

PORTB  |=  (1 << (6));;
delay_us(250);
if ( PINA     & (1 << (4)) ) data |= 0x01;
if ( DDRA     & (1 << (5)) ) data |= 0x02;
if ( DDRA     & (1 << (6)) ) data |= 0x04;
if ( DDRA     & (1 << (7)) ) data |= 0x08;
PORTB  &= ~(1 << (6));;
return data;
}

static unsigned char lcd_waitbusy(void) {

while (lcd_read(0) & (1<<7      )) {}
delay_us(10);
return (lcd_read(0));
}

static inline void lcd_newline(byte pos) {
register unsigned char addressCounter;

if ( pos < (0x40) )
addressCounter = 0x40;
else
addressCounter = 0x00;
lcd_command((1<<7      )+addressCounter);
}

void lcd_command(byte cmd) {
lcd_waitbusy();
lcd_write(cmd,0);
}

void lcd_data(byte data) {
lcd_waitbusy();
lcd_write(data,1);
}

void lcd_gotoxy(byte x, byte y) {
if ( y==0 )
lcd_command((1<<7      )+0x00+x);
else
lcd_command((1<<7      )+0x40+x);
}

#pragma used+
int lcd_getxy(void) {
return lcd_waitbusy();
}
#pragma used-

void lcd_clrscr(void) {
lcd_command(1<<0      );
}

void lcd_home(void) {
lcd_command(1<<1      );
}

void lcd_putc(byte c) {
byte pos;

byte lcd_symtable[64] = {   

65,160, 66,161,224, 69,163,164,165,166, 75,167, 77, 72, 79,168, 80, 67, 84,169,170, 88,225,171,172,226,173,174,173,175,176,177,
97,178,179,180,227,101,182,183,184,185,186,187,188,189,111,190,112, 99,191,121,228,120,229,192,193,230,194,195,196,197,198,199
};
pos = lcd_waitbusy();   
if (c=='\n') lcd_newline(pos);
else
if (c < 192) lcd_write(c, 1);
else lcd_write(lcd_symtable[c-192], 1);
}

void lcd_puts(const char *s) {
while (*s != 0) lcd_putc(*s++);
}

void lcd_line_menu(const char *s, byte submenu) {
register byte i;
for(i=0; i<(16-1); i++) {
if (*s != 0) lcd_putc(*s++); else lcd_putc(0x20);
}

if (submenu) lcd_putc('>'); else lcd_putc(0x20);
}

void lcd_init(byte dispAttr) {

DDRB    |= (1 << (5));
DDRB    |= (1 << (7));
DDRB     |= (1 << (6));
DDRA     |= (1 << (4));
DDRA     |= (1 << (5));
DDRA |= (1 << (6));
DDRA |= (1 << (7));
delay_ms(16);        

PORTA |= (1 << (5));  
PORTA |= (1 << (4));  
lcd_e_toggle();
delay_ms(41);         

lcd_e_toggle();
delay_us(64);           

lcd_e_toggle();
delay_us(64);           

PORTA &= ~(1 << (4));   
lcd_e_toggle();
delay_us(64);           

lcd_command(0x28   );      
lcd_command(0x08   );              
lcd_clrscr();                           
lcd_command(((1<<2      ) | (1<<1      ) ));          
lcd_command(dispAttr);                  
}
