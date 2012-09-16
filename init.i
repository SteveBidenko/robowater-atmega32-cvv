
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

#asm
   .equ __i2c_port=0x15 ;PORTC
   .equ __sda_bit=1
   .equ __scl_bit=0
#endasm

#pragma used+
void i2c_init(void);
unsigned char i2c_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
unsigned char i2c_write(unsigned char data);
#pragma used-

#pragma used+

unsigned char rtc_read(unsigned char address);
void rtc_write(unsigned char address,unsigned char data);
void rtc_init(unsigned char rs,unsigned char sqwe,unsigned char out);
void rtc_get_time(unsigned char *hour,unsigned char *min,unsigned char *sec);
void rtc_set_time(unsigned char hour,unsigned char min,unsigned char sec);
void rtc_get_date(unsigned char *date,unsigned char *month,unsigned char *year);
void rtc_set_date(unsigned char date,unsigned char month,unsigned char year);

#pragma used-
#pragma library ds1307.lib

#asm
   .equ __w1_port=0x18 ;PORTB
   .equ __w1_bit=0
#endasm

#pragma used+
unsigned char w1_init(void);
unsigned char w1_read(void);
unsigned char w1_write(unsigned char data);
unsigned char w1_search(unsigned char cmd,void *p);
unsigned char w1_dow_crc8(void *p,unsigned char n);
#pragma used-

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

#pragma used+

unsigned char cabs(signed char x);
unsigned int abs(int x);
unsigned long labs(long x);
float fabs(float x);
int atoi(char *str);
long int atol(char *str);
float atof(char *str);
void itoa(int n,char *str);
void ltoa(long int n,char *str);
void ftoa(float n,unsigned char decimals,char *str);
void ftoe(float n,unsigned char decimals,char *str);
void srand(int seed);
int rand(void);
void *malloc(unsigned int size);
void *calloc(unsigned int num, unsigned int size);
void *realloc(void *ptr, unsigned int size); 
void free(void *ptr);

#pragma used-
#pragma library stdlib.lib

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

extern struct __ds1820_scratch_pad_struct {
unsigned char 	temp_lsb,temp_msb;
signed char 	temp_high,temp_low;
unsigned char 	conf,
res1,res2,res3,
crc;
} __ds1820_scratch_pad;
extern struct st_terms {
int t;               
unsigned char id;   
int t_last;          
unsigned char err;  
} termometers[4];
extern byte ds1820_rom_codes[4][9];

unsigned char sync_ds1820_eeprom(void);
void sync_eeprom_ds1820(void);
void read_all_terms(unsigned char measure_mode);
unsigned char ds1820_select(unsigned char *addr);
unsigned char ds1820_read_spd(unsigned char *addr);
unsigned char ds1820_write_spd(unsigned char *addr);
unsigned char ds1820_run_measure(unsigned char *addr);
int ds1820_temperature(unsigned char *addr);            
int ds1820_temperature_10(unsigned char *addr);         
int ds1820_temperature_10lh(unsigned char *addr, signed char *temp_low, signed char *temp_high);
unsigned char ds1820_set_alarm(unsigned char *addr, signed char temp_low, signed char temp_high);
signed char ds1820_get_alarm(unsigned char *addr, signed char *temp_low, signed char *temp_high);
unsigned char ds1820_set_resolution(unsigned char *addr, unsigned char resolution);
signed char ds1820_get_resolution(unsigned char *addr, unsigned char *resolution);

unsigned char *ds1820_show_spd();
#pragma used-

extern signed char valcoder;

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

typedef char alarm_text[16      ];

struct st_alarm {
char datestamp[12];  
int val_preset;             
int val_alarm;              
alarm_text text;            
};

extern void alarm_reg (int reg_preset, int reg_alarm, char *text_alarm, unsigned char);
extern signed char alarm_unreg (unsigned char);
extern void alarm_all_print (void);
extern char *get_alert_str(unsigned char);
extern void update_alert_menu (void);

typedef char lcd_str[16];        

enum en_type {
e_empty = 0,              
e_clatsman,               
e_winter,                 
e_isfour,                 
e_percent,                
e_temperature,            
e_room,                   
e_world,                  
e_water,                  
e_coef,                   
e_scale,                  
e_stime,                  
e_mtime,                  
e_minute,                 
e_hour,                   
e_day,                    
e_month,                  
e_year,                   
e_dt,                     
e_time,                   
e_date,                   
e_sync,                   
e_alarm,                  
e_alert,                  
e_password                

};

struct st_parameter {

signed int val_data;                
enum en_type val_type;              
unsigned char can_edit;             

unsigned char str_num;              
};
struct st_array_pos {
signed char line0;          
signed char line1;          
unsigned char lcd;          
struct st_parameter *menu;  
unsigned char maximum;      

struct st_parameter *par;   
int val_data;               
unsigned char level;        
};

extern char linestr[];                

extern struct st_parameter main_menu[7];
extern struct st_parameter parameters[11];
extern struct st_parameter alerts[12        ];
extern struct st_array_pos curr_menu;                

extern void sync_set_par(unsigned char);
extern void init_curr_menu(struct st_parameter *lmenu, unsigned char lmax);
extern void lcd_init_edit(void);
extern void lcd_save_edit(void);
extern void lcd_esc_edit(void);
extern void lcd_menu(signed char);
extern void lcd_edit(signed char);

extern char *param_str(unsigned char, struct st_parameter parameter[]);
extern char *par_str(struct st_parameter *, unsigned char, int);
extern char *getmenustr(unsigned char menu_num_pp);

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

#pragma used+
char eeprom_write(unsigned char eeprom_adresse, unsigned char eeprom_input, unsigned char bank);
char eeprom_read(unsigned char eeprom_adresse, unsigned char bank);

void eeprom_write_struct (unsigned char *struct_data, unsigned char size);

void eeprom_read_struct (unsigned char *struct_data, unsigned char size);

void eeprom_write_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);

void eeprom_read_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);
#pragma used-

extern unsigned char t_key; 

void init_keys(void);       
void poll_keys(void);       
void poll_key(unsigned char); 
unsigned char key(unsigned char);
extern unsigned char key_treated[7] ; 

unsigned int read_adc(unsigned char);

byte timer1_valcoder = 0;
unsigned int timer_start = 0;
byte timer_stop = 0;
byte timer_fan = 0;
byte count_fan = 0;

interrupt [4] void ext_int2_isr(void) {
if (timer_fan) timer_fan--;
if (timer_start) {
timer_start--;

if (!timer_start && (mode.run == 1)) {         
mode.fan = 1;
printf ("ПУСК \r\n");
mode.run=3;                
count_fan = 0;
signal_green(1);
}
}
if (timer_stop) {
timer_stop--;

if (!timer_stop && (mode.run == 2)) {          
mode.run = 0;
mode.fan = 0;           

signal_green(0);
}
}

if (timer1_valcoder) {
timer1_valcoder--;

if (!timer1_valcoder) {
if (event)
timer1_valcoder++;
else
event = ev_timer;

}
}
if (!mode.stop_sync_dt) get_cur_dt (0);
read_all_terms(0);

parameters[7].val_data = read_adc(0)/4;
parameters[5].val_data = read_adc(1)/4;
if (time_integration) time_integration--;
if (event == ev_none) {
event = ev_secunda;
}

}

interrupt [12] void timer0_ovf_isr(void) {

}

interrupt [10] void timer1_ovf_isr(void) {
update_signal_status();
if (t_key) t_key--;
}

interrupt [6] void timer2_ovf_isr(void) {

}

unsigned int read_adc(unsigned char adc_input) {
ADMUX=adc_input | (0xC0 & 0xff);
delay_us(10);   
ADCSRA|=0x40;   
while ((ADCSRA & 0x10)==0); 
ADCSRA|=0x10;
return ADCW;
}
#pragma used+
void set_cur_dt (void) {
#asm("cli")
rtc_set_time(s_dt.cHH, s_dt.cMM, s_dt.cSS);
rtc_set_date(s_dt.cdd, s_dt.cmo, s_dt.cyy);
#asm("sei")
delay_ms (200);
get_cur_dt(1);

}

void get_cur_dt (unsigned char force) {
if (force) {
rtc_init(0,1,0);            
}

rtc_get_time(&s_dt.cHH,&s_dt.cMM,&s_dt.cSS);

rtc_get_date(&s_dt.cdd,&s_dt.cmo,&s_dt.cyy);
}

#pragma used-

void init_uart(void) {
word bauddiv;

UCSRA=0x00; UCSRC=0x86; UCSRB = (1<<3            ) | (1<<4            );
bauddiv = ((3686400 + (115200UL * 8L))/(115200UL * 16L) - 1);
UBRRL = (unsigned char)bauddiv;
UBRRH = bauddiv >> 8;
}

void init_terms(void) {
printf ("Поиск всех термометров на шине 1-Wire. Найдено: ");
prim_par.terms = w1_search(0xf0,ds1820_rom_codes);
delay_ms (200   );
printf ("%d штук\r\n", prim_par.terms);
}

void init(void) {

PORTA=0x00; DDRA=0xFC;

PORTB=0x07; DDRB=0xF8;

PORTC=0xFF; DDRC=0x00;

PORTD=0x0D; DDRD=0xF2;

TCCR0=0x6C; TCNT0=0x00; OCR0=0x00;

TCCR1A=0x00; TCCR1B=0x02; TCNT1H=0x00; TCNT1L=0x00; 
ICR1H=0x00; ICR1L=0x00; 
OCR1AH=0x00; OCR1AL=0x00; OCR1BH=0x00; OCR1BL=0x00;

ASSR=0x00; TCCR2=0x6c; TCNT2=0x00; OCR2=0x00;

GICR|=0xE0; MCUCR=0x06; MCUCSR=0x00; GIFR=0xE0;

TIMSK=0x45;

init_uart ();

ACSR=0x80; SFIOR=0x00;

ADMUX=0xC0 & 0xff; ADCSRA=0xA6; SFIOR&=0x1F;
printf ("Старт инициализации периферии...\r\n"); delay_ms(500);

i2c_init();

mode.stop_sync_dt = 0;      
get_cur_dt(1);                   

init_terms();

lcd_init(0x0C   );

valcoder = 0;

init_curr_menu(&main_menu[0], 7);

signal_green(0); signal_red(0); signal_buz(0); signal_white(0);
} 
