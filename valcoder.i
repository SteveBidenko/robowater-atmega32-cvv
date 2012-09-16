
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

extern signed char valcoder;

signed char valcoder;              

interrupt [2] void ext_int0_isr(void) {

if ((PIND.3)) valcoder--; else valcoder++;

GIFR |= (1<<6);

GICR &= ~(1<<6);

}
interrupt [3] void ext_int1_isr(void) {

GICR |= (1<<6);

}
