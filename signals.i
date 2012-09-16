
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

typedef char *va_list;

#pragma used+

char getchar(void);
void putchar(char c);
void puts(char *str);
void putsf(char flash *str);

char *gets(char *str,unsigned int len);

void printf(char flash *fmtstr,...);
void sprintf(char *str, char flash *fmtstr,...);
void snprintf(char *str, unsigned int size, char flash *fmtstr,...);
void vprintf (char flash * fmtstr, va_list argptr);
void vsprintf (char *str, char flash * fmtstr, va_list argptr);
void vsnprintf (char *str, unsigned int size, char flash * fmtstr, va_list argptr);
signed char scanf(char flash *fmtstr,...);
signed char sscanf(char *str, char flash *fmtstr,...);

#pragma used-

#pragma library stdio.lib

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

struct st_signal {
unsigned char buz_mode;      
unsigned char buz_timer;
unsigned char buz_status;

unsigned char red_mode;
unsigned char red_timer;
unsigned char red_status;
unsigned char green_mode;
unsigned char green_timer;
unsigned char green_status;
unsigned char white_mode;
unsigned char white_timer;
unsigned char white_status;
};

extern void update_signal_status(void);         

extern void signal_buz(unsigned char);
extern void signal_green(unsigned char);
extern void signal_red(unsigned char);
extern void signal_white(unsigned char);
extern void signal_printallbytes (void);

struct st_signal signals;      

void signal_printallbytes (void) {
register unsigned char i;
unsigned char *status = &signals.buz_mode;
printf ("Стуктура signals =");
for (i = 0; i < sizeof(signals); i++) {
printf (" %x", *status);
status++;
}
printf ("\r\n");
}

unsigned char init_lamp_status (unsigned char *lamp_mode) {
unsigned char *lamp_timer = lamp_mode + 1;
unsigned char *lamp_status = lamp_mode + 2;
switch (*lamp_mode) {
case 2:
*lamp_timer = 1;
*lamp_status = 1;
break;
case 3:
*lamp_timer = 5;
*lamp_status = 1;
break;
case 4:
*lamp_timer = 3;
*lamp_status = 1;
break;
case 1:
*lamp_status = 1;
break;
default:
*lamp_status = 0;
break;
};
return (*lamp_status);
}

unsigned char update_lamp_status (unsigned char *lamp_mode) {
unsigned char *lamp_timer = lamp_mode + 1;
unsigned char *lamp_status = lamp_mode + 2;
switch (*lamp_mode) {
case 2:
if (*lamp_timer) {
*lamp_timer = *lamp_timer - 1;
if (*lamp_timer == 0) {
if (*lamp_status) {
*lamp_status = 0;
*lamp_timer = 5;
} else {
*lamp_status = 1;
*lamp_timer = 1;
}
}
}
break;
case 3:
if (*lamp_timer) {
*lamp_timer = *lamp_timer - 1;
if (*lamp_timer == 0) {
if (*lamp_status) {
*lamp_status = 0;
*lamp_timer = 1;
} else {
*lamp_status = 1;
*lamp_timer = 5;
}
}
}
break;
case 4:
if (*lamp_timer) {
*lamp_timer = *lamp_timer - 1;
if (*lamp_timer == 0) {
*lamp_status = !(*lamp_status);
*lamp_timer = 3;
}
}
break;
case 1:
*lamp_status = 1;
break;
case 0:
default:
*lamp_status = 0;
*lamp_mode = 0;
break;
};
return (*lamp_status);
}

void update_signal_status(void) {
switch (signals.buz_mode) {
case 2:
case 3:
signals.buz_status = 0;
if (signals.buz_timer) {
signals.buz_timer--;
if (signals.buz_timer) signals.buz_status = 1; else signals.buz_mode = 0;
}
break;
case 4:
if (signals.buz_timer) {
signals.buz_timer--;
if (signals.buz_timer == 0) {
signals.buz_status = !signals.buz_status;
signals.buz_timer = 3;
}
}
break;
default:
signals.buz_status = 0;
signals.buz_mode = 0;
break;
}
if (mode.sound) PORTD.6      = signals.buz_status;
PORTA.3      = update_lamp_status(&signals.red_mode);
PORTB.4    = update_lamp_status(&signals.green_mode);
PORTA.2    = update_lamp_status(&signals.white_mode);

}

void signal_buz(unsigned char signal_mode) {
signals.buz_mode = signal_mode;
switch (signal_mode) {
case 2:
signals.buz_timer = 1;
signals.buz_status = 1;
break;
case 3:
signals.buz_status = 1;
signals.buz_timer = 5;
break;
case 4:
signals.buz_status = 1;
signals.buz_timer = 3;
break;
default:
signals.buz_status = 0;
signals.buz_timer = 0;
break;
};
if (mode.sound) PORTD.6      = signals.buz_status;
}

void signal_red(unsigned char signal_mode) {
signals.red_mode = signal_mode;
PORTA.3      = init_lamp_status(&signals.red_mode);
}

void signal_green(unsigned char signal_mode) {
signals.green_mode = signal_mode;
PORTB.4    = init_lamp_status(&signals.green_mode);
}

void signal_white(unsigned char signal_mode) {
signals.white_mode = signal_mode;
PORTA.2    = init_lamp_status(&signals.white_mode);
}
