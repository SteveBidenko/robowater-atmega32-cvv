
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

#pragma used+
unsigned char w1_init(void);
unsigned char w1_read(void);
unsigned char w1_write(unsigned char data);
unsigned char w1_search(unsigned char cmd,void *p);
unsigned char w1_dow_crc8(void *p,unsigned char n);
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

byte ds1820_rom_codes[4][9];
struct st_terms termometers[4];  

struct __ds1820_scratch_pad_struct __ds1820_scratch_pad;

unsigned char sync_ds1820_eeprom(void) {
unsigned char result = 0, i, j;
signed char th, tl, get_alarm;

for (i = 0; i < prim_par.terms; i++) {
get_alarm = ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);                        
if (get_alarm > 0) {    

for (j = 0; j < 9; j++)

if (th == -69)
prim_par.addr[abs(tl) - 2][j] = ds1820_rom_codes[i][j];
else {
result++;
mode.ufo[j] = ds1820_rom_codes[i][j];
}
}
}
return result;
}

void sync_eeprom_ds1820(void) {
register byte i, j;

for (i = 0; i < prim_par.terms; i++)
for (j = 0; j < 9; j++)
ds1820_rom_codes[i][j] = prim_par.addr[i][j];
}

void rectification(int unconverted, unsigned char index, unsigned char measure_mode) {

if (index >= 4) return;

termometers[index].t = unconverted;
if (measure_mode == 1) {
if (unconverted == -9999)
termometers[index].err = 5;
else 
termometers[index].err = 0;
termometers[index].t_last = termometers[index].t;
} else {

if (abs(termometers[index].t - termometers[index].t_last) < 1000) {
termometers[index].t_last = termometers[index].t;
termometers[index].err = 0;
} else {
termometers[index].t = termometers[index].t_last;
termometers[index].err++;
}
}
}

void read_all_terms(unsigned char measure_mode) {
register byte i;

if (measure_mode == 1) {
sync_eeprom_ds1820();
for(i = 0; i < prim_par.terms; i++) {
ds1820_set_resolution(&ds1820_rom_codes[i][0], 0b10);
delay_ms (200   ); 

ds1820_run_measure(&ds1820_rom_codes[i][0]); 
delay_ms (200   );
}    
}

for (i = 0; i < prim_par.terms; i++) {
int curr_t = ds1820_temperature_10(&ds1820_rom_codes[i][0]);    
rectification (curr_t, i, measure_mode);

}

POM_T = termometers[0].t;
UL_T = termometers[1].t;
WIN_T = termometers[2].t;
WOUT_T = termometers[3].t;
}
unsigned char *ds1820_show_spd() {
return (char *) &__ds1820_scratch_pad;
}

unsigned char ds1820_select(unsigned char *addr) {
unsigned char i;
if (w1_init()==0) return 0;		
if (addr) {						
w1_write(0x55);				
i=0;
do
w1_write(*(addr++));
while (++i<8);
} else
w1_write(0xcc);
return 1;
}

unsigned char ds1820_read_spd(unsigned char *addr) {
unsigned char i, *p;
unsigned char count = 0, result;
do {
ds1820_select(addr);                	
w1_write(0xbe);							
i = 0;
p = (char *) &__ds1820_scratch_pad;		
do
*(p++)=w1_read();
while (++i<9);

result = !w1_dow_crc8(&__ds1820_scratch_pad,9);
count++;
} while	((result == 0) && (count < 3));
return result;
}

unsigned char ds1820_write_spd(unsigned char *addr) {
register unsigned char i;
unsigned char *p;
if (ds1820_select(addr)==0) return 0;	
w1_write(0x4e);                         
i=0;
p=(char *) &__ds1820_scratch_pad.temp_high; 
do
w1_write(*(p++));
while (++i<3);
return 1;
}

unsigned char ds1820_run_measure(unsigned char *addr) {
if (ds1820_select(addr)==0) return 0;	
w1_write(0x44);
return w1_init();
}

int ds1820_temperature(unsigned char *addr) {
unsigned char values[16] = { 0,6,12,19,25,31,38,44,50,56,63,69,75,81,88,94 };
unsigned char fract; 
int t10; 
if (!ds1820_read_spd(addr)) return -9999;         
fract = __ds1820_scratch_pad.temp_lsb & 0xF;           
t10 = __ds1820_scratch_pad.temp_msb;
t10 = (((t10 << 8) | __ds1820_scratch_pad.temp_lsb) >> 4) * 100; 
t10 += values[fract];
return t10;
}

int ds1820_temperature_10(unsigned char *addr) {
int t10;									
t10 = ds1820_temperature(addr);
(void) ds1820_run_measure(addr);
return t10;
}

int ds1820_temperature_10lh(unsigned char *addr, signed char *temp_low, signed char *temp_high) {
int t10;									
t10 = ds1820_temperature(addr);
*temp_low = __ds1820_scratch_pad.temp_low;  
*temp_high = __ds1820_scratch_pad.temp_high;
(void) ds1820_run_measure(addr);
return t10;
}

unsigned char ds1820_set_alarm(unsigned char *addr, signed char temp_low, signed char temp_high) {
if (!ds1820_read_spd(addr)) return 0;
__ds1820_scratch_pad.temp_low = temp_low;
__ds1820_scratch_pad.temp_high = temp_high;
if (ds1820_write_spd(addr)==0) return 0;
if (!ds1820_read_spd(addr)) return 0;
if ((__ds1820_scratch_pad.temp_low!=temp_low) || (__ds1820_scratch_pad.temp_high!=temp_high)) return 0;
if (ds1820_select(addr)==0) return 0;
w1_write(0x48);                             
delay_ms(15);
return w1_init();
}

signed char ds1820_get_alarm(unsigned char *addr, signed char *temp_low, signed char *temp_high) {
if (!ds1820_read_spd(addr)) return -98;         

*temp_low = __ds1820_scratch_pad.temp_low;  
*temp_high = __ds1820_scratch_pad.temp_high;

return 1;
}

unsigned char ds1820_set_resolution(unsigned char *addr, unsigned char resolution) {
unsigned char temp_resolution;
if (!ds1820_read_spd(addr)) return 0; 
temp_resolution = (resolution << 5) | 0x1F;

__ds1820_scratch_pad.conf = temp_resolution;

if (ds1820_write_spd(addr)==0) return 0;
ds1820_read_spd(addr);
if (__ds1820_scratch_pad.conf!=temp_resolution) return 0;

ds1820_select(addr);
w1_write(0x48);             
delay_ms(15);
return w1_init();
}

signed char ds1820_get_resolution(unsigned char *addr, unsigned char *resolution) {
if (!ds1820_read_spd(addr)) return -99;         

*resolution = __ds1820_scratch_pad.conf;  

return 1;
}
