
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

extern unsigned char t_key; 

void init_keys(void);       
void poll_keys(void);       
void poll_key(unsigned char); 
unsigned char key(unsigned char);
extern unsigned char key_treated[7] ; 

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

extern signed char valcoder;

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

struct st_key {
signed char status;     
unsigned char last;     
unsigned char is_work;  
unsigned char range;  
unsigned int on;        
unsigned int off;       
unsigned char reg;      
unsigned char pin;      
} keys[7]= {
{-1, 0, 0, 0, 0, 0, 0x33, 3},     
{-1, 0, 0, 0, 0, 0, 0x33, 5},     
{-1, 0, 0, 0, 0, 0, 0x33, 6},     
{-1, 0, 0, 0, 0, 0, 0x33, 7},     
{-1, 0, 0, 0, 0, 0, 0x33, 2},     
{-1, 0, 0, 0, 0, 0, 0x33, 4},     
{-1, 0, 0, 0, 0, 0, 0x36, 1}      

};

unsigned char t_key = 0; 

unsigned char key(unsigned char i) {

if (i <= 3 ) {
if (keys[i].status == -1         ) {
return 1           ;
} else {
if (keys[i].status == 0            ) {

if (key_treated[i] == 0) {
signal_buz(2); 
return 0            ;
} else return 1           ;
}
if (keys[i].status == 1           ) {
key_treated[i] = 0;

return 1           ;
}   
} 
} else {
if (keys[i].status == -1         ) {
return (keys[i].last);
} else {

return (keys[i].status);
}             
}
}

void init_keys(void) {
register unsigned char i;
for (i=0; i<7; i++) {
t_key = 10     ;
while (keys[i].status == -1         ) {
poll_key(i);
if(keys[i].status != -1          && keys[i].last != keys[i].status) {
keys[i].last = keys[i].status;
keys[i].status = -1         ;
if (t_key == 0) break;        
} else {
keys[i].is_work = 1;
}
}

}
}

void poll_keys(void) {
register unsigned char i;
for (i=0; i<7; i++) {
poll_key(i);    

}
}

void poll_key(unsigned char i) {
unsigned char pin_val = 0;
union reg_port {
unsigned char num[2];
unsigned int *p;
} curr_port;

curr_port.num[0] = keys[i].reg; curr_port.num[1] = 0;

pin_val = ((*(curr_port.p)) & ((1 << (keys[i].pin))							))				 >> keys[i].pin; 

switch (keys[i].status) {
case 0            :
if (pin_val == 1           ) {
keys[i].status = -1         ;
GICR &= ~(3<<6);
keys[i].on = 0;
keys[i].off = 1;
keys[i].last = 0            ;    
} 
break;
case 1           : 
if (pin_val == 0            ) {
keys[i].status = -1         ;
GICR &= ~(3<<6);
keys[i].on = 1;
keys[i].off = 0;
keys[i].last = 1           ;    
} 
break;
case -1         :
(pin_val == 0            ) ? keys[i].on++ : keys[i].off++;

if (keys[i].on >= 100 || keys[i].off >= 100) {
keys[i].status = (keys[i].on > keys[i].off) ? 0             : 1           ;

GICR |= (3<<6);

} 
break;
default:
};
}
