
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

void delay_us(unsigned int n);
void delay_ms(unsigned int n);

#pragma used-

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
unsigned char w1_init(void);
unsigned char w1_read(void);
unsigned char w1_write(unsigned char data);
unsigned char w1_search(unsigned char cmd,void *p);
unsigned char w1_dow_crc8(void *p,unsigned char n);
#pragma used-

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
char eeprom_write(unsigned char eeprom_adresse, unsigned char eeprom_input, unsigned char bank);
char eeprom_read(unsigned char eeprom_adresse, unsigned char bank);

void eeprom_write_struct (unsigned char *struct_data, unsigned char size);

void eeprom_read_struct (unsigned char *struct_data, unsigned char size);

void eeprom_write_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);

void eeprom_read_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);
#pragma used-

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

extern unsigned char t_key; 

void init_keys(void);       
void poll_keys(void);       
void poll_key(unsigned char); 
unsigned char key(unsigned char);
extern unsigned char key_treated[7] ; 

unsigned char key_treated[7] = {0,0,0,0,0,0,0} ; 
struct st_mode mode = {0, 0, 0, 0, 0, 0, 1, 0.0,0};  
struct st_datetime s_dt;

struct st_eeprom_par prim_par={
{{(int)0, 0}, {(int)0, 0}, {(int)0, 0}, {(int)0, 0}},
0xFF, 205, 0x7F, 0x7F,
(int)10, (int)0, (int)0,
(int)180, (int)100,
(int)1600, (int)5000, 
(int)-2000, (int)1500, (int)2200,
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
1,  
0,  
0,   
4, 

{{0x28,0x95,0x0f,0x7e,0x03,0x00,0x00,0x72,0x01}, 
{0x28,0xc1,0x1a,0x7e,0x03,0x00,0x00,0x2e,0x01}, 
{0x28,0x1a,0x17,0x7e,0x03,0x00,0x00,0xfe,0x01}, 
{0x28,0xe8,0x2a,0x7e,0x03,0x00,0x00,0x8a,0x01}} 

};
unsigned int time_integration=0;
int tmp_delta;
byte tap_angle_min = 0;   

enum en_event event;                          

void printallterms(void); void lcd_primary_screen(void);
void print_prim_par(unsigned char *, unsigned char);

void check_serial(void);
void check_peripheral(void);
void event_processing(void);
void mode_processing(void);
void update_P(int);
void update_PID(int error, int iMin, int iMax);
void init_new_terms(unsigned char);

void main(void) {

byte size_prim_par;

init();                  
#asm("sei")             
printf ("Инициализация кнопок...\r\n"); 
init_keys();
printf("Старт Ньютона металоизделия П2_1. %u.%02u. Найдено %u термометров.\r\n", 4, 1, prim_par.terms);

size_prim_par = sizeof(prim_par);

if (!key(3)    && !key(2)    ) {
unsigned char term_status;
prim_par.terms = 4;

term_status = sync_ds1820_eeprom();
eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);

printf("Запись в EEPROM заводских установок.\r\n Неопознанных термометров = %u\r\n", term_status);
} else {
unsigned char size_addr = size_prim_par - sizeof(prim_par.addr);
if (!key(3)   ) {
prim_par.terms = 4;
printf("Запись в EEPROM установок по умолчанию.\r\n");
eeprom_write_struct ((unsigned char *)&prim_par, size_addr);
}
if (!key(2)    ) {
unsigned char term_status;
eeprom_read_struct ((char *)&prim_par, size_addr);
prim_par.terms = 4;

term_status = sync_ds1820_eeprom();
printf("Запись в EEPROM %u новых термометров.\r\n", term_status);
eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
}
}

eeprom_read_struct ((char *)&prim_par, size_prim_par);

read_all_terms(1);

sync_set_par(1); 
update_alert_menu();        

printallterms();
lcd_primary_screen();       

if (prim_par.season) signal_white(1); else signal_white(0);
parameters[8].val_data = prim_par.fan_speed;

tmp_delta = abs(prim_par.TA_in_Min) + 500L      ;  
mode.k_angle_limit = (78.0     / tmp_delta) * 1000; 
if (parameters[2].val_data < 500L      ) {    
tap_angle_min = ((long int)((500L       - parameters[2].val_data) * mode.k_angle_limit))/1000;   
};     
if (parameters[6].val_data < tap_angle_min) parameters[6].val_data = tap_angle_min;
else parameters[6].val_data = prim_par.tap_angle;     
printf("Установка вентилятора. %u. Установка крана %u .\r\n", parameters[8].val_data, parameters[6].val_data);
while(1) {

check_serial();
poll_keys();

 check_peripheral();
 event_processing();
 mode_processing();
}; 

} 

void check_peripheral(void) {

if ((abs(valcoder)-1) >= 0) {        

if (valcoder < 0)
event = ev_left;
else
event = ev_right;
}                                 

if ((event == ev_none) && (!key(2)    )) event = ev_enter;
if ((event == ev_none) && (!key(3)   )) event = ev_cancel;

if ((event == ev_none) &&  (!key(0)    )) event = ev_stop;

if ((event == ev_none) && (main_menu[5].can_edit == 0) && (!key(1)    )) event = ev_start;

if ((event == ev_none) && (!prim_par.alert_status[0]) && (key(4)   )) event = ev_alarm1; 
if ((event == ev_none) && (!prim_par.alert_status[1]) && (key(5)   )) event = ev_alarm2; 
if ((event == ev_none) && (!prim_par.alert_status[11]) && (!key(6)   )) event = ev_filter; 

if ((event == ev_none) && (!prim_par.alert_status[7]) && (termometers[0].err >= 5))

event = ev_term1_nf; 
if ((event == ev_none) && (!prim_par.alert_status[8]) && (termometers[1].err >= 5)) 

event = ev_term2_nf;
if ((event == ev_none) && (!prim_par.alert_status[9]) && (termometers[2].err >= 5))  

event = ev_term3_nf;
if ((event == ev_none) && (!prim_par.alert_status[10]) && (termometers[3].err >= 5))  

event = ev_term4_nf;        

if (parameters[2].val_data < 500L      ) {
if ((event == ev_none) && !(prim_par.alert_status[8] || prim_par.alert_status[2]) && 
(termometers[1].t < (prim_par.TA_in_Min-5))) 
event = ev_freezing1;
if ((event == ev_none) && !(prim_par.alert_status[7] || prim_par.alert_status[3]) && 
(termometers[0].t < prim_par.TA_out_Min)) 
event = ev_freezing2;
if ((event == ev_none) && !(prim_par.alert_status[10] || prim_par.alert_status[5]) && 
(termometers[3].t < prim_par.TW_out_Min)) 
event = ev_freezing3;
}    
if ((event == ev_none) && (mode.initrun)) {
mode.initrun -= 4;
if ((mode.initrun) && (main_menu[5].can_edit == 0)) event = ev_start;         
else
event = ev_stop;
mode.initrun = 0;
}

}

void event_processing(void) {

switch (event) {
case ev_secunda:                
main_menu[0].val_data = read_term(0);       
switch (mode.menu) {
case 0: lcd_primary_screen(); break;
case 1: lcd_menu(0); break;
default: ;

}

if (mode.print && (mode.run ==1)) printf("-");
if (mode.print && (mode.run ==3)) printf ("До следующего измерения ПУСК: %u\r\n", time_integration);
if (mode.print && (mode.run ==0)) printf ("До следующего измерения СТОП: %u\r\n", timer_fan);
event = ev_none;            
break;
case ev_left:                   
case ev_right:                  

timer1_valcoder = 60      ; 
valcoder = 0;
signal_buz(0);

switch (mode.menu) {
case 0:
sync_set_par(1);
lcd_menu(mode.menu++); break;   
case 1: lcd_menu(event-2); break;       
case 2: lcd_edit(event-2); break;       
default: ;                              
}

event = ev_none;            
break;
case ev_enter:                  

key_treated[2]=1;
event = ev_none;            
signal_buz(2);
timer1_valcoder = 60      ;     
switch (mode.menu) {

case 0:
sync_set_par(1);
lcd_menu(mode.menu++); break;           
case 1: lcd_init_edit(); break;             
case 2: lcd_save_edit(); mode.menu = 1; break;
};
break;
case ev_timer:

if (mode.menu) timer1_valcoder = 60      ; 
case ev_cancel:
if (event == ev_cancel) {
signal_buz(2);
key_treated[3]=1;
timer1_valcoder = 60      ;     
}
event = ev_none;            

switch (mode.menu) {
case 0: lcd_primary_screen(); break;
case 1: --mode.menu; lcd_esc_edit(); break;
case 2: --mode.menu; lcd_esc_edit(); break;
case 3: mode.menu = 1; break;
};
lcd_clrscr();
break;
case ev_start:      
key_treated[1]=1;
printf ("Нажата кнопка ПУСК. ");  
switch (mode.run) {
case 0: { 
if (prim_par.season) {
signal_white(1);
mode.pomp = 1;
mode.run = 1;                
timer_start = prim_par.T_z; 
parameters[6].val_data = 255      ;
printf("Включен режим Прогрев. LIMIT = %d, Время прогрева = %d\r\n", mode.k_angle_limit,prim_par.T_z);
signal_green(2);
} else {
signal_white(0);
mode.pomp = 0;
mode.run = 3;
mode.fan = 1;
signal_green(1);
printf("Включен режим Пуск\r\n");
};   
parameters[8].val_data = prim_par.fan_speed;
if (parameters[8].val_data <= 102             ) parameters[8].val_data = 102             ;
count_fan = 0; 
time_integration = 0;
signal_buz(3); 
break;
} 
case 1: signal_buz(2); signal_green(2); printf("Режим Прогрев.\r\n"); 
break; 
case 2: signal_buz(2); signal_green(3); printf("Режим Остановки.\r\n"); 
break; 
case 3: signal_buz(2); signal_green(1); printf("Режим Пуск.\r\n");
break; 
default: break;
};  
event = ev_none;
break;
case ev_stop:
key_treated[0]=1;
printf ("Нажата кнопка СТОП. ");
switch (mode.run) {
case 0: signal_buz(2); time_integration = 0; signal_green(0); printf(" Режим СТОП.\r\n"); break; 
case 1: mode.run = 2;
timer_start = 0;
signal_green(2);  signal_buz(3);
printf ("Включен режим Остановки\r\n");
timer_stop = 10                 ;    
time_integration = 0; 
break; 
case 2: signal_buz(2); time_integration = 0; printf("Режим Остановки.\r\n"); break; 
case 3: mode.run = 2; 
signal_green(3); signal_buz(3);
printf ("Включен режим Остановки\r\n");
timer_stop = 10                 ;    
time_integration = 0; break; 
default: break;
}
mode.fan = 0;  
event = ev_none;
break;
case ev_alarm1:   
signal_red(1); signal_buz(4);  
mode.run = 0;
mode.fan = 0;
if (prim_par.season) mode.pomp = 1; 
parameters[6].val_data = 255      ;
signal_green(0);
alarm_reg(0, 1, get_alert_str(0), 0);
printf ("Авария: %s\r\n", get_alert_str(0));
event = ev_none;
break;
case ev_alarm2:   
signal_red(1); signal_buz(4);
mode.run = 0;
mode.fan = 0;
mode.pomp = 1;
parameters[6].val_data = 255      ;
alarm_reg(0, 1, get_alert_str(1), 1);
printf ("АВАРИЯ: %s\r\n", get_alert_str(1));
event = ev_none;
break;
case ev_freezing1:  
alarm_reg(0, 1, get_alert_str(2), 2);

if (prim_par.season) mode.pomp = 1;
parameters[6].val_data = 255      ;
printf ("АВАРИЯ: %s\r\n", get_alert_str(2));
event = ev_none;
break;
case ev_freezing2: 
alarm_reg(0, 1, get_alert_str(3), 3);
signal_red(1); signal_buz(4);
signal_green(0);
mode.run = 0;  
mode.fan = 0;
if (prim_par.season) mode.pomp = 1;
parameters[6].val_data = 255      ;
printf ("АВАРИЯ: %s\r\n", get_alert_str(3));
event = ev_none;
break;
case ev_freezing3: 
alarm_reg(0, 1, get_alert_str(5), 5);
signal_red(1); signal_buz(4);
signal_green(0);
mode.run = 0;  
mode.fan = 0;
if (prim_par.season) mode.pomp = 1;
parameters[6].val_data = 255      ;
printf ("АВАРИЯ: %s\r\n", get_alert_str(5));
event = ev_none;    
break;
case ev_term1_nf:   
alarm_reg(5, termometers[0].err, get_alert_str(7), 7);
signal_red(1); signal_buz(4);
signal_green(0);
mode.run = 0;
mode.fan = 0;
if (prim_par.season) mode.pomp = 1;
parameters[6].val_data = 255      ;
printf ("АВАРИЯ: %s\r\n", get_alert_str(7));
event = ev_none;
break;
case ev_term2_nf:  
alarm_reg(5, termometers[1].err, get_alert_str(8), 8);
signal_red(3); signal_buz(4);
signal_green(0);
mode.run = 0;
mode.fan = 0;
if (prim_par.season) mode.pomp = 1;
parameters[6].val_data = 255      ;
printf ("АВАРИЯ: %s\r\n", get_alert_str(8));
event = ev_none;
break;
case ev_term3_nf:  

alarm_reg(5, termometers[2].err, get_alert_str(9), 9);
printf ("ПРЕДУПРЕЖДЕНИЕ: %s\r\n", get_alert_str(9));
event = ev_none;
break;
case ev_term4_nf:  
alarm_reg(5, termometers[3].err, get_alert_str(10), 10);
signal_red(3); signal_buz(4);
signal_green(0);
mode.run = 0;
mode.fan = 0;
if (prim_par.season) mode.pomp = 1;
parameters[6].val_data = 255      ;
printf ("АВАРИЯ: %s\r\n", get_alert_str(10));
event = ev_none;
break;            
case ev_filter:  
alarm_reg(0, 1, get_alert_str(11), 11);
event = ev_none;
printf ("АВАРИЯ: %s\r\n", get_alert_str(11));
signal_red(2);
break;
default:
break;
};
}

void mode_processing(void) {

int water_out_bound;
int water_out_bound_1;
static int error_w_stop = 0;

parameters[0].val_data = prim_par.TA_out_prs;
if (mode.run == 5) {
} else {
OCR0 = (unsigned char)parameters[6].val_data;
OCR2 = (unsigned char)parameters[8].val_data; 

PORTD.4     = mode.fan;
parameters[9].val_data = mode.fan;
PORTD.5      = mode.pomp;
parameters[10].val_data = mode.pomp;

switch (mode.run) {
case 0:
time_integration = 0;
if (prim_par.season) {

if (prim_par.season && (parameters[2].val_data < 500L      )) {

tap_angle_min = ((long int)((500L       - parameters[2].val_data) * mode.k_angle_limit))/1000;   
if (tap_angle_min < 100) tap_angle_min = 100 ; 
}    

error_w_stop = (prim_par.TW_out_Stop - parameters[4].val_data)/100;
water_out_bound = prim_par.TW_out_Stop - 200;
water_out_bound_1 = prim_par.TW_out_Stop - 500;

if ((parameters[4].val_data <= water_out_bound) && (timer_fan == 0)) {
timer_fan = 20            ;

parameters[6].val_data = parameters[6].val_data + error_w_stop;
if (parameters[6].val_data < tap_angle_min) parameters[6].val_data = tap_angle_min ;   
if (parameters[6].val_data > 255      ) parameters[6].val_data = 255      ;         
if (parameters[4].val_data < water_out_bound_1) {

signal_white(3);
mode.pomp = 1;
} else {
signal_white(1);
mode.pomp = 0;
}
if (mode.print) printf("Разогрев калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, parameters[6].val_data, parameters[5].val_data, tap_angle_min, parameters[4].val_data/10);    
}

water_out_bound = prim_par.TW_out_Stop + 250;
water_out_bound_1 = prim_par.TW_out_Stop + 500;
if ((parameters[4].val_data > water_out_bound) && (timer_fan == 0)) {
timer_fan = 20            ;

if (parameters[6].val_data < tap_angle_min) parameters[6].val_data = tap_angle_min;         
if (parameters[6].val_data > 255      ) parameters[6].val_data = 255      ;         
if ((parameters[4].val_data >= water_out_bound) && (mode.pomp == 1)) {

signal_white(1);
mode.pomp = 0;
};
if (parameters[4].val_data >= water_out_bound_1)  parameters[6].val_data = parameters[6].val_data + error_w_stop;
if (mode.print) printf("Охлаждение калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, parameters[6].val_data, parameters[5].val_data, tap_angle_min, parameters[4].val_data/10);      
}

if (timer_fan == 0) {
timer_fan = 20            ;
if (parameters[3].val_data >= prim_par.TW_out_Stop)  mode.pomp = 0;

if (mode.print) printf("Поддержание калорифера: %d,  Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, parameters[6].val_data, parameters[5].val_data, tap_angle_min, parameters[4].val_data/10);
}
} else {
signal_white(0);
mode.pomp = 0;

}
signal_green(0);   
break;

case 3:

if (prim_par.season) 
signal_white(1);  
else { 
signal_white(0);
parameters[8].val_data = prim_par.fan_speed;
}

if (1 == 0) {
update_P(parameters[0].val_data - parameters[1].val_data); 
time_integration = prim_par.T_int;
} 
if (time_integration == 0) {
update_PID(parameters[0].val_data - parameters[1].val_data, -5000, 5000); 
time_integration = prim_par.T_int;
if (prim_par.season && (parameters[2].val_data < 500L      )) {

if ((parameters[6].val_data == 255      ) && ((parameters[1].val_data - 200) < parameters[0].val_data)) {
count_fan++;
if (count_fan == 5          ) {
parameters[8].val_data = parameters[8].val_data - 10            ;
count_fan = 0;
if  (parameters[8].val_data <= 102             ) parameters[8].val_data = 102             ;

if (mode.print) printf("Понижение скорости вентилятора расчетное : %d, измеренное : %d, Счетчик циклов :%d, POM_T :%d \r\n",  parameters[8].val_data, parameters[7].val_data, count_fan, parameters[1].val_data);   
}
} else  {
if ((parameters[8].val_data < prim_par.fan_speed) && ((parameters[1].val_data + 300) > parameters[0].val_data)) {   
count_fan++;
if (count_fan == 5          ) {
parameters[8].val_data = parameters[8].val_data + 10            ;
count_fan = 0;

if  (parameters[8].val_data > prim_par.fan_speed ) parameters[8].val_data = prim_par.fan_speed;

if (mode.print) printf("Увеличение скорости вентилятора расчетная: %d, измеренная : %d, Счетчик циклов :%d, Заданная скорость :%d \r\n",  parameters[8].val_data, parameters[7].val_data, count_fan, prim_par.fan_speed);
}

} else count_fan = 0;
}

if (mode.print) printf("Скорость вентилятора расчетная: %d, измеренная : %d, Заданная скорость: %d, POM_T: %d, TAP_ANGLE_MIN = %d \r\n",  parameters[8].val_data, parameters[7].val_data, prim_par.fan_speed, parameters[1].val_data, tap_angle_min); 
}
} 
signal_green(1);

break;
default:
break;
};
}
}
void check_range(void) {

if (parameters[2].val_data < 500L      )  tap_angle_min = ((long int)((500L       - parameters[2].val_data) * mode.k_angle_limit))/1000;

if (parameters[6].val_data < tap_angle_min)
parameters[6].val_data = tap_angle_min;
else          
if (parameters[6].val_data > 255      ) 
parameters[6].val_data = 255      ;

}

void update_P(int error) {

if ((parameters[6].val_data >=0) && (parameters[6].val_data <= 255      ))
parameters[6].val_data = parameters[6].val_data + (error / 100) * prim_par.Ku;
check_range();
printf("Разность температур: %d, TAP_ANGLE :%d, time_integration :%d \r\n",  error, ((parameters[6].val_data*100)/0xFF),time_integration);
}
void update_PID(int error, int iMin, int iMax) {
static int dState = 0, iState = 0;
int pTerm, dTerm, iTerm;
int result;

pTerm = (prim_par.Ku * error) / 10;    
iState += error;                

if (parameters[8].val_data >= prim_par.fan_speed) {
if ((iState > iMax) || (iState < iMin)) { 
if (iState > iMax) 
iState = iMax;
else 
iState = iMin;
}    
iTerm = (prim_par.Ki * iState) / 100;    
dTerm = prim_par.Kd * (parameters[1].val_data - dState);
dState = parameters[1].val_data;
result = pTerm + iTerm - dTerm;
parameters[6].val_data += result / 100;
check_range();
}
if (!mode.print)

printf("%02u:%02u:%02u, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n",
s_dt.cHH, s_dt.cMM, s_dt.cSS, 
parameters[0].val_data, parameters[1].val_data, error, 
parameters[6].val_data, parameters[5].val_data, tap_angle_min, result,
pTerm, iTerm, dTerm,
parameters[8].val_data, parameters[7].val_data);          
else
printf("Delta (x100) = %d, result = %d, TAP_ANGLE = %d, pTerm = %d, iTerm = %d, dTerm = %d\r\n", 
error, result, parameters[6].val_data, pTerm, iTerm, dTerm, parameters[1].val_data);
}

void printallterms(void) {
int term;
register byte i;
unsigned char resolution;
signed char th, tl;
unsigned char *spd;

if(!prim_par.terms) return;			
spd = ds1820_show_spd();         

for(i=0; i<prim_par.terms; i++) {
ds1820_get_resolution(&ds1820_rom_codes[i][0], &resolution);
term = ds1820_temperature(&ds1820_rom_codes[i][0]);
tl = th = 0;
ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
printf(" t%-u = %i(%-i.%-u)C[%02X%02X:%02x]%d:%d; ", i+1, term, term/100, abs(term%100), spd[1], spd[0], resolution, tl, th);
}
printf("\r\n");
}

int read_term(byte num) {

return termometers[num].t;
};

void lcd_primary_screen(void) {
char run_mod [8];
register unsigned char i;
unsigned char c_alerts = 0;
unsigned char sign = '+';
int lcd_term = main_menu[0].val_data;

for (i = 0; i < 12        ; i++) {

if (prim_par.alert_status[i] && i != 9) c_alerts++;
}
switch (mode.run) {
case 0: sprintf(run_mod, "СТОП   "); break; 
case 1: sprintf(run_mod, "ПРОГРЕВ"); break; 
case 2: sprintf(run_mod, "ОСТАНОВ"); break; 
case 3: sprintf(run_mod, "ПУСК   "); break; 
default: break;
};
if (c_alerts)
sprintf(run_mod, "Авария ");
else {
signal_red(0);  signal_buz(0); 
}
lcd_command(0x0C   );       
lcd_gotoxy(0,0);        
sprintf(linestr, "%02u:%02u:%02u %02u.%02u  ", s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo);
lcd_puts(linestr);
lcd_gotoxy(0,1);                

if(prim_par.terms) {
sign = (lcd_term < 0) ? '-' : '+';
sprintf(linestr, "t=%c%02u.%01uC %s", sign, abs(lcd_term)/100, (abs(lcd_term)%100)/10, run_mod);

} else {
sprintf(linestr, "Нет термометров");
}
lcd_puts(linestr);

}

void print_prim_par(unsigned char *struct_data, unsigned char size) {
register unsigned char i;
unsigned char s_byte; 
printf("Состояние prim_par [%d bytes]: ", size);
for (i=0; i<size; i++) {
s_byte = *struct_data++;
if (i < (size - 36)) 
printf(" %d", s_byte);
else 
printf(" %X", s_byte);

}
printf("\r\n");
}

void init_new_terms(unsigned char number) {
register unsigned char i;
unsigned char is_found = 0, is_new = 0;
signed char th, tl, index;
unsigned char newterms;

newterms = w1_search(0xf0, ds1820_rom_codes);
delay_ms (200   );
printallterms();

for(i = 0; i < newterms; i++) {
ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
index = (th == -69) ? abs(tl) - 2 : -1;

if (!is_found) is_found = (index == number) ? i + 1 : 0;

if (!is_new) is_new = (index == -1) ? 0 : i + 1;
}
printf ("Задан N%u (number), нашли N%u (is_found), свободный N%u (is_new)\r\n", number, is_found, is_new);

if (is_found) {
printf ("Термометр N%u существует по порядковому номеру %u\r\n", number, is_found);
return;
}

if (!is_new) {
printf ("Свободных термометров нет\r\n");
return;
}

tl = -2 - number; i = is_new - 1;
printf ("Новый TL = %d[%02x] по номеру %u\r\n", tl, tl, i);
if (ds1820_set_alarm(&ds1820_rom_codes[i][0], tl, -69)) {
printf ("Проинициализировали термометр N%u по порядковому номеру %u\r\n", number, is_new);
delay_ms (200   );

#asm
            JMP  __RESET
        #endasm
}
printf ("Не могу проинициализировать новый термометр N%u\r\n", number);
return;
}
void ask_turn_off(void) {
printf("Выключите питание и подключите все оборудование\r\n");
while(1);
}

void init_force_term(signed char number) {

byte inbyte = 0;

#asm("cli")
printf("Отключите все термометры, кроме [%02x] и нажмите клавишу SHIFT-'~'\r\nВыход из режима - выключение питания\r\n", number);
do
if (UCSRA & (1<<7            )) inbyte = UDR;
while (inbyte != 0x7E);
prim_par.terms = w1_search(0xf0, ds1820_rom_codes);
delay_ms (200   );
printallterms();
if (!prim_par.terms)
printf("Термометры не найдены\r\n");
else
if (ds1820_set_alarm(&ds1820_rom_codes[0][0], number, -69))
printf("Термометр [%02x] успешно проинициализирован\r\n", number);
else
printf("Не могу проинициализировать новый термометр [%02x]\r\n", number);
ask_turn_off();
#asm("sei")
}
void set_term(signed char number, signed char sign) {
if (sign == -69) {
if (ds1820_set_alarm(&ds1820_rom_codes[number][0], -2 - number, sign))
printf("Термометр [%02x] успешно проинициализирован\r\n", number);
else
printf("Не могу проинициализировать термометр [%02x]\r\n", number);
} else { 
if (ds1820_set_alarm(&ds1820_rom_codes[number][0], 1, sign))
printf("Термометр [%02x] успешно очищен\r\n", number);
else
printf("Не могу очистить термометр [%02x]\r\n", number);
}
}

void toggle_sound(void) {
if (mode.sound)
printf("Звук выключен\r\n");
else
printf("Звук включен\r\n");
mode.sound = !mode.sound;
}

void toggle_print(void) {
mode.print = !mode.print;
if (mode.print)
printf("Печать включена\r\n"); 

else
printf("Печать выключена\r\n Время, Температура заданная, Температура измеренная, Delta, Угол крана расчетный, Угол кранаизмеренный, Угол ограничения, result, pTerm, iTerm, dTerm, Обороты расчетные, Обороты измеренныее\r\n");

}

void check_serial(void) {
byte inbyte;    
register byte i;

if (UCSRA & (1<<7            )) { 
inbyte = UDR;
switch (inbyte) {
case 0x7A:   
printf("Время: %02u:%02u:%02u, дата:%02u.%02u.%02u, найдено %u термометров\r\n",
s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo, s_dt.cyy, prim_par.terms);
printallterms();
break;
case 0x73:       
toggle_sound(); break;   
case 0x77:       
printf ("Cаабытие = %u\r\n", (unsigned char)event);
break;
case 0x78:  
toggle_print(); break;

case 0x65:  
alarm_all_print();
break;
case 0x64:  
for (i=0; i < 12        ; i++) {
if (!(alarm_unreg(i))) {
printf ("Удалили активную тревогу: %s\r\n", get_alert_str(i));
}
}
printf ("Инициализируем все термометры!\r\n");
read_all_terms(1);
break;
case 0x63:                
print_prim_par((unsigned char *)&prim_par, sizeof(prim_par));
break;
case 0x6D:                
for(i=0; i<11; i++) printf("%s\t", param_str(i, parameters));
printf("\r\n");
break;
case 0x6E:                
for(i=0; i<7; i++) printf("%s\t", param_str(i, main_menu));
printf("\r\n");
break;
case 0x4a:              
poll_keys();
break;
case 0x75:                
init_force_term(0xFE); break;
case 0x69:                
init_force_term(0xFD); break;
case 0x6F:                
init_force_term(0xFC); break;
case 0x70:                
init_force_term(0xFB); break;
case 0x30:              
printf("Всем найденым термометрам прописываем 0x69 0xFE - Термометр в помещении\r\n");
init_new_terms(0);   
break;
case 0x31:              
printf("Всем найденым термометрам прописываем 0x69 0xFD - Термометр на улице\r\n");
init_new_terms(1);
break;
case 0x32:              
printf("Всем найденым термометрам прописываем 0x69 0xFC - Термометр на подаче воды\r\n");
init_new_terms(2);
break;
case 0x33:              
printf("Всем найденым термометрам прописываем 0x69 0xFB - Термометр на обратке воды\r\n");
init_new_terms(3);
break;
case 0x3f:              
set_term(3, 1); break;
case 0x3e:              
set_term(2, 1); break;
case 0x3c:              
set_term(1, 1); break;
case 0x4d:              
set_term(0, 1); break;
case 0x22:              
set_term(3, -69); break;
case 0x3a:              
set_term(2, -69); break;
case 0x4c:              
set_term(1, -69); break;
case 0x4b:              
set_term(0, -69); break;
case 0x39:              
signal_printallbytes();
break;
default:
printf("Нажат символ 0x%x\r\n", inbyte);
};
}
}
