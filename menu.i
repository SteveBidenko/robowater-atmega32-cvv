
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

char *strcat(char *str1,char *str2);
char *strcatf(char *str1,char flash *str2);
char *strchr(char *str,char c);
signed char strcmp(char *str1,char *str2);
signed char strcmpf(char *str1,char flash *str2);
char *strcpy(char *dest,char *src);
char *strcpyf(char *dest,char flash *src);
unsigned int strlenf(char flash *str);
char *strncat(char *str1,char *str2,unsigned char n);
char *strncatf(char *str1,char flash *str2,unsigned char n);
signed char strncmp(char *str1,char *str2,unsigned char n);
signed char strncmpf(char *str1,char flash *str2,unsigned char n);
char *strncpy(char *dest,char *src,unsigned char n);
char *strncpyf(char *dest,char flash *src,unsigned char n);
char *strpbrk(char *str,char *set);
char *strpbrkf(char *str,char flash *set);
char *strrchr(char *str,char c);
char *strrpbrk(char *str,char *set);
char *strrpbrkf(char *str,char flash *set);
char *strstr(char *str1,char *str2);
char *strstrf(char *str1,char flash *str2);
char *strtok(char *str1,char flash *str2);

unsigned int strlen(char *str);
void *memccpy(void *dest,void *src,char c,unsigned n);
void *memchr(void *buf,unsigned char c,unsigned n);
signed char memcmp(void *buf1,void *buf2,unsigned n);
signed char memcmpf(void *buf1,void flash *buf2,unsigned n);
void *memcpy(void *dest,void *src,unsigned n);
void *memcpyf(void *dest,void flash *src,unsigned n);
void *memmove(void *dest,void *src,unsigned n);
void *memset(void *buf,unsigned char c,unsigned n);
unsigned int strcspn(char *str,char *set);
unsigned int strcspnf(char *str,char flash *set);
int strpos(char *str,char c);
int strrpos(char *str,char c);
unsigned int strspn(char *str,char *set);
unsigned int strspnf(char *str,char flash *set);

#pragma used-
#pragma library string.lib

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

#pragma used+
char eeprom_write(unsigned char eeprom_adresse, unsigned char eeprom_input, unsigned char bank);
char eeprom_read(unsigned char eeprom_adresse, unsigned char bank);

void eeprom_write_struct (unsigned char *struct_data, unsigned char size);

void eeprom_read_struct (unsigned char *struct_data, unsigned char size);

void eeprom_write_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);

void eeprom_read_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);
#pragma used-

struct st_array_pos curr_menu;                
struct st_array_pos dt_curr_menu;             

struct st_parameter main_menu[7]= {   
{-9900, e_temperature, 0, 1},     
{0, e_isfour, 1, 2},              
{1, e_winter, 0, 3},              
{0, e_dt, 1, 4},                  
{0, e_empty, 1, 5},               
{0, e_alarm, 0, 6},               
{0, e_empty, 0, 7}                
};

struct st_parameter sdt[7]= {   
{20, e_hour, 1, 8},             
{05, e_minute, 1, 9},           
{19, e_day, 1, 10},             
{03, e_month, 1, 11},           
{11, e_year, 1, 12},            
{0, e_winter, 1, 41},           
{0, e_password, 1, 42}          
};

struct st_parameter parameters[11]= {
{2200, e_room, 1, 15},          
{2000, e_temperature, 0, 13},   
{-350, e_temperature, 0, 14},   
{7200, e_temperature, 0, 16},   
{6500, e_temperature, 0, 17},   
{40, e_percent, 0, 18},         
{42, e_percent, 1, 19},         
{95, e_percent, 0, 20},         
{100, e_percent, 1, 21},        
{1, e_clatsman, 0, 22},         
{1, e_clatsman, 1, 23}          
};

struct st_parameter settings[17]={
{-2500, e_world, 1, 24},        
{3500, e_water, 1, 25},         
{1000, e_water, 1, 26},         
{1500, e_room, 1, 27},          
{30, e_stime, 1, 28},           
{10, e_stime, 1, 29},           
{1, e_coef, 1, 30},             
{0, e_coef, 1, 31},             
{0, e_coef, 1, 32},             
{0, e_temperature, 1, 33},      
{0, e_scale, 1, 34},            
{0, e_temperature, 1, 35},      
{0, e_scale, 1, 36},            
{0, e_temperature, 1, 37},      
{0, e_scale, 1, 38},            
{0, e_temperature, 1, 39},      
{0, e_scale, 1, 40}             
};
struct st_parameter alerts[12        ] = {
{0, e_alert, 0, 61},
{0, e_alert, 0, 61 + 1},
{0, e_alert, 0, 61 + 2},
{0, e_alert, 0, 61 + 3},
{0, e_alert, 0, 61 + 4},
{0, e_alert, 0, 61 + 5},
{0, e_alert, 0, 61 + 6},
{0, e_alert, 0, 61 + 7},
{0, e_alert, 0, 61 + 8},
{0, e_alert, 0, 61 + 9},
{0, e_alert, 0, 61 + 10},
{0, e_alert, 0, 61 + 11}
};
flash lcd_str all_menu_str[] = {
"Ïîìåùåíèå ",  
"ĞÅÆÈÌ ",      
"ÑÅÇÎÍ ",      
"Óñò. ",       
"Ïàğàìåòğû ",  
"ÀÂÀĞÈÉ ",      
"ÍÀÑÒĞÎÉÊÀ...",    
"Óñò.×àñ. ",   
"Óñò Ìèí ",    
"Óñò.äåíü. ",  
"Óñò Ìåñ. ",   
"Óñò ÃÎÄ. ",   
"ÏÎÌÅÙ. ",     
"ÓË.Ò ",       
"Óñò.Ò ",      
"Âîä ÂÕ. ",    
"Âîä ÂÛÕ.",    
"ÊĞÀÍ ",       
"ÓÑÒ.ÊĞÀÍ. ",  
"ÂÅÍÒÈËßÒÎĞ ", 
"Óñò. ÂÅÍÒ. ", 
"ÇÀÑË. ",      
"ÍÀÑÎÑ ",      
"TA IN Min ",  
"TW STOP ",    
"TW Min ",     
"TA Min ",     
"ÏĞÎÃĞÅÂ ",    
"ÂĞ.ÈÍÒ. ",     
"ÊÓ=",         
"ÊÈ=",         
"ÊÄ=",         
"Ñì.Ï.=",      
"Øê.Ï.=",     
"Ñì.Óë.=",    
"Øê.Óë.=",    
"Ñì.ÂÂõ=",    
"Øê.ÂÂõ=",    
"Ñì.ÂÂûõ=",   
"Øê.ÂÂûõ=",   
"Ñåçîí=",     
"Ïàğîëü="     

};
char linestr[20];           
bit need_eeprom_write;      

void sync_set_par(byte sync) {
register byte i, j;

if (sync == 1) {
main_menu[2].val_data = prim_par.season;
settings[0].val_data = prim_par.TA_in_Min;
settings[1].val_data = prim_par.TW_out_Stop;
settings[2].val_data = prim_par.TW_out_Min;
settings[3].val_data = prim_par.TA_out_Min;
settings[4].val_data = prim_par.T_z;
settings[5].val_data = prim_par.T_int;
settings[6].val_data = prim_par.Ku;
settings[7].val_data = prim_par.Ki;
settings[8].val_data = prim_par.Kd;
if (mode.run == 0) {
parameters[8].val_data = prim_par.fan_speed  ;
};     

parameters[0].val_data = prim_par.TA_out_prs;
parameters[10].val_data = mode.pomp;

sdt[0].val_data = s_dt.cHH;
sdt[1].val_data = s_dt.cMM;
sdt[2].val_data = s_dt.cdd;
sdt[3].val_data = s_dt.cmo;
sdt[4].val_data = s_dt.cyy;
sdt[5].val_data = prim_par.season;
for (i = 0; i < 4; i++) {
j = 2*i + 9;
settings[j++].val_data = prim_par.elims[i].shift;
settings[j].val_data = (int)(prim_par.elims[i].scale);
}
} else {
if (mode.stop_sync_dt) {
if ((s_dt.cHH != sdt[0].val_data) || (s_dt.cMM != sdt[1].val_data)) {
s_dt.cHH = sdt[0].val_data;
s_dt.cMM = sdt[1].val_data;
s_dt.cSS = 0;
set_cur_dt();
printf("Çàïèñü âğåìåíè %02u:%02u:00\r\n",sdt[0].val_data, sdt[1].val_data);
printf("Äàòà %02u.%02u.20%02u\r\n", sdt[2].val_data, sdt[3].val_data, sdt[4].val_data);
};
if ((s_dt.cdd != sdt[2].val_data) || (s_dt.cmo != sdt[3].val_data) || (s_dt.cyy != sdt[4].val_data)) {
s_dt.cdd = sdt[2].val_data;
s_dt.cmo = sdt[3].val_data;
s_dt.cyy = sdt[4].val_data;
set_cur_dt();
printf("Çàïèñü äàòû %02u.%02u.20%02u\r\n", sdt[2].val_data, sdt[3].val_data, sdt[4].val_data);
printf("Âğåìÿ %02u:%02u:00\r\n",sdt[0].val_data, sdt[1].val_data);
};
} else {

for (i = 0; i < 4; i++) {
j = 2*i + 9;
if (prim_par.elims[i].shift != settings[j].val_data) {
prim_par.elims[i].shift = settings[j].val_data; need_eeprom_write = 1;
}
j++;
if (prim_par.elims[i].scale != (signed char)settings[j].val_data) {
prim_par.elims[i].scale = (signed char)settings[j].val_data; need_eeprom_write = 1;
}
}
if (prim_par.TA_in_Min != settings[0].val_data) {
prim_par.TA_in_Min = settings[0].val_data; need_eeprom_write = 1;
}
if (prim_par.TW_out_Stop != settings[1].val_data) {
prim_par.TW_out_Stop = settings[1].val_data; need_eeprom_write = 1;
}
if (prim_par.TW_out_Min != settings[2].val_data) {
prim_par.TW_out_Min = settings[2].val_data; need_eeprom_write = 1;
}
if (prim_par.TA_out_Min != settings[3].val_data) {
prim_par.TA_out_Min = settings[3].val_data; need_eeprom_write = 1;
}
if (prim_par.T_z != settings[4].val_data) {
prim_par.T_z = settings[4].val_data; need_eeprom_write = 1;
}
if (prim_par.T_int != settings[5].val_data) {
prim_par.T_int = settings[5].val_data; need_eeprom_write = 1;
}
if (prim_par.Ku != settings[6].val_data) {
prim_par.Ku = settings[6].val_data; need_eeprom_write = 1;
}
if (prim_par.Ki != settings[7].val_data) {
prim_par.Ki = settings[7].val_data; need_eeprom_write = 1;
}
if (prim_par.Kd != settings[8].val_data) {
prim_par.Kd = settings[8].val_data; need_eeprom_write = 1;
}
if (prim_par.TA_out_prs != parameters[0].val_data) {
prim_par.TA_out_prs = parameters[0].val_data; need_eeprom_write = 1;
};
if (mode.run == 0) {
if (prim_par.fan_speed  != parameters[8].val_data) {
if (parameters[8].val_data <= 102             ) parameters[8].val_data = 102             ;
prim_par.fan_speed = parameters[8].val_data; need_eeprom_write = 1;
};    
};
if (mode.pomp  != parameters[10].val_data) {
mode.pomp = parameters[10].val_data;
};
if (mode.run != main_menu[1].val_data) {
mode.initrun = (unsigned char)main_menu[1].val_data + 4;
};
for (i = 0; i < 12        ; i++) {
if (prim_par.alert_status[i] && (alerts[i].val_data == 0)) {
alarm_unreg (i);
printf ("Óäàëèëè àêòèâíóş òğåâîãó: %s\r\n", get_alert_str(i));
read_all_terms(1);
printf ("Èíèöèàëèçèğóåì âñå òåğìîìåòğû!\r\n");
}
}
};

if (prim_par.season != sdt[5].val_data) {
prim_par.season = sdt[5].val_data; need_eeprom_write = 1;
main_menu[2].val_data = sdt[5].val_data;
}
};
}

char *param_str(byte num_line, struct st_parameter parameter[]) {
int pr_data;
struct st_parameter *st_pointer;
st_pointer = &parameter[num_line];          
pr_data = st_pointer->val_data;

return (par_str(st_pointer, 0, pr_data));
}

char *par_str(struct st_parameter *st_pointer, unsigned char only_val, int pr_data) {
char *pr_name;
char prompt[] = "=> ";
unsigned char sign = (pr_data < 0) ? '-' : '+';
if (only_val) pr_name = prompt; else pr_name = getmenustr(st_pointer->str_num);
switch (st_pointer->val_type) {
case e_empty:
sprintf(linestr, "%s", pr_name);
break;
case e_clatsman:

if(pr_data)
sprintf(linestr, "%sÂÊË.", pr_name);
else
sprintf(linestr, "%sÂûêë.", pr_name);
break;
case e_percent:

sprintf(linestr, "%s%u%%", pr_name, (pr_data*100)/0xFF);

break;
case e_coef:

sprintf(linestr, "%s%u", pr_name, pr_data);
break;
case e_scale:

sprintf(linestr, "%s%d", pr_name, pr_data);
break;
case e_isfour:
main_menu[1].val_data = mode.run;

switch (pr_data) {
case 0: sprintf(linestr, "%sÑÒÎÏ", pr_name);  break;   
case 1: sprintf(linestr, "%sÏĞÎÃĞÅÂ", pr_name); break; 
case 2: sprintf(linestr, "%sÎÑÒÀÍÎÂ", pr_name); break; 
case 3: sprintf(linestr, "%sÏÓÑÊ", pr_name);  ; break; 
default: break;
};
break;
case e_password:

if (only_val) sprintf(linestr, "%s%u", pr_name, pr_data);
else sprintf(linestr, "%s **", pr_name, pr_data);
break;
case e_alarm:

if (pr_data) sprintf(linestr, "%s%u!", pr_name, pr_data);
else sprintf(linestr, "%sÍÅÒ", pr_name);
break;
case e_alert:
if (pr_data) sprintf(linestr, "%s%u!", pr_name, pr_data);
else sprintf(linestr, "%s", pr_name);
break;
case e_winter:

if (pr_data)
sprintf(linestr, "%sÇÈÌÀ", pr_name);
else
sprintf(linestr, "%sËÅÒÎ", pr_name);
break;
case e_time:

sprintf(linestr, "%s%02u:%02u", pr_name, s_dt.cHH, s_dt.cMM);
break;
case e_date:

sprintf(linestr, "%s%02u.%02u.%02u", pr_name, s_dt.cdd, s_dt.cmo, s_dt.cyy);
break;
case e_dt:
sprintf(linestr, "%s Âğ./Äàò.", pr_name);
break;
case e_stime:

sprintf(linestr, "%s%03u", pr_name, pr_data);
break;
case e_temperature:
case e_room:
case e_world:
case e_water:

sprintf(linestr, "%s%c%u.%-01uC", pr_name, sign, abs(pr_data)/100, abs(pr_data%100)/10);
break;
case e_hour:
case e_minute:
case e_day:
case e_month:

sprintf(linestr, "%s%u", pr_name, pr_data);
break;
case e_year:

sprintf(linestr, "%s20%u", pr_name, pr_data);
break;
default:
linestr[0] = 0;
};
return(linestr);
}

void init_curr_menu(struct st_parameter *lmenu, unsigned char lmax) {
curr_menu.line0 = 0; curr_menu.line1 = 1;

curr_menu.menu = lmenu;         

curr_menu.maximum = lmax;
curr_menu.lcd = 0; 
}

void calc_pos(struct st_array_pos *array_pos, byte maxlines, signed char direction) {

array_pos->line0 += direction;      

if (array_pos->line0 >= maxlines) array_pos->line0 = 0;
if (array_pos->line0 < 0) { array_pos->line0 = maxlines - 1; array_pos->line1 = 0; return; }

if ((array_pos->line0 + 1) == maxlines) array_pos->line1 = 0; else array_pos->line1 = array_pos->line0 + 1;
}

void lcd_menu(signed char direction) {

if ((curr_menu.lcd == 0) && (direction > 0))    
curr_menu.lcd = 1;
else

if ((curr_menu.lcd == 1) && (direction < 0))
curr_menu.lcd = 0;
else

if (direction) calc_pos(&curr_menu, curr_menu.maximum, direction);

lcd_gotoxy(0,0);        
lcd_line_menu(param_str(curr_menu.line0, curr_menu.menu), curr_menu.menu[curr_menu.line0].can_edit);
lcd_gotoxy(0,1);        
lcd_line_menu(param_str(curr_menu.line1, curr_menu.menu), curr_menu.menu[curr_menu.line1].can_edit);

if (curr_menu.lcd) lcd_gotoxy(16-1,1); else lcd_gotoxy(16-1,0);

lcd_command(0x0F   );
}

void lcd_init_edit(void) {
struct st_parameter *lmenu;     
byte num_line;                  

if (curr_menu.lcd) num_line = curr_menu.line1; else num_line = curr_menu.line0;

if ((curr_menu.level == 0) && (num_line >= 3)) {
sync_set_par(1); 
switch (num_line) {
case 3:
dt_curr_menu = curr_menu;
init_curr_menu(&sdt[0], 7);
curr_menu.level = 1; 
mode.stop_sync_dt = 1;

break;
case 4: init_curr_menu(&parameters[0], 11);
curr_menu.level = 1; 
break;
case 5:
if (main_menu[5].can_edit) {
init_curr_menu(&alerts[0], 12        );
curr_menu.level = 1; 
}
break;
case 6: 
if (main_menu[6].can_edit) {
init_curr_menu(&settings[0], 17);
curr_menu.level = 1 ;
}
break;
default: ;
}
mode.menu = 1;              
lcd_menu(0);
return;
}
lmenu = &(curr_menu.menu)[num_line];
if (lmenu->can_edit == 0) {
lcd_menu(0);
return;
} else {
mode.menu += lmenu->can_edit; 
}
curr_menu.par = lmenu;
curr_menu.val_data = lmenu->val_data;        
lcd_edit(0);
}

void lcd_save_edit (void) {
struct st_parameter *lmenu;     
byte num_line;                  

if (curr_menu.lcd) num_line = curr_menu.line1; else num_line = curr_menu.line0;
lmenu = &(curr_menu.menu)[num_line];
curr_menu.par = lmenu;
lmenu->val_data = curr_menu.val_data;
sync_set_par(0);
if (mode.stop_sync_dt) {

curr_menu = dt_curr_menu;
curr_menu.level = 0;
mode.stop_sync_dt = 0;      
}

if (need_eeprom_write) {
eeprom_write_struct ((char *)&prim_par, sizeof(prim_par));

need_eeprom_write = 0;
}
lcd_menu(0);
return;
}

void lcd_esc_edit(void) {
struct st_parameter *lmenu;     
byte num_line;                  

if (curr_menu.lcd) num_line = curr_menu.line1; else num_line = curr_menu.line0;

lmenu = &(curr_menu.menu)[num_line];

if (curr_menu.level || mode.stop_sync_dt) {

if (mode.stop_sync_dt)
curr_menu = dt_curr_menu;
else
init_curr_menu(&main_menu[0], 7);
curr_menu.level = 0;
mode.stop_sync_dt = 0;
return;
}

curr_menu.par = lmenu;

curr_menu.val_data = 0;
lcd_menu(0);
return;
}

void lcd_edit(signed char direction) {
struct st_parameter *lmenu;
lmenu = curr_menu.par;

switch (lmenu->val_type) {
case e_empty:

break;
case e_clatsman:
case e_winter:

if (direction) if(curr_menu.val_data) curr_menu.val_data = 0; else curr_menu.val_data = 1;

break;
case e_isfour:
if (direction) if (curr_menu.val_data) curr_menu.val_data = 0; else curr_menu.val_data = 3;
break;
case e_temperature:

curr_menu.val_data += (int)direction * 10;
if (curr_menu.val_data > 10000) curr_menu.val_data = -3000;
if (curr_menu.val_data < -3000) curr_menu.val_data = 10000;
break;
case e_room:

curr_menu.val_data += (int)direction * 10;
if (curr_menu.val_data > 3000) curr_menu.val_data = 500;
if (curr_menu.val_data < 500) curr_menu.val_data = 3000;
break;
case e_world:

curr_menu.val_data += (int)direction * 100;
if (curr_menu.val_data > 3000) curr_menu.val_data = -3000;
if (curr_menu.val_data < -3000) curr_menu.val_data = 3000;
break;
case e_water:

curr_menu.val_data += (int)direction * 100;
if (curr_menu.val_data > 10000) curr_menu.val_data = 500;
if (curr_menu.val_data < 500) curr_menu.val_data = 10000;
break;
case e_percent:

curr_menu.val_data += direction;

if (curr_menu.val_data > 0xFF) curr_menu.val_data = 0;
if (curr_menu.val_data < 0) curr_menu.val_data = 0xFF;
break;
case e_coef:
curr_menu.val_data += direction;
if (curr_menu.val_data < 0 ) curr_menu.val_data = 0xFF;
if (curr_menu.val_data > 0xFF) curr_menu.val_data = 0;
break;
case e_scale:
curr_menu.val_data += direction;
if (curr_menu.val_data < -128 ) curr_menu.val_data = 127;
if (curr_menu.val_data > 127) curr_menu.val_data = -128;
break;
case e_stime:
if (curr_menu.val_data < 100) curr_menu.val_data += direction; else
if (curr_menu.val_data < 1000) curr_menu.val_data += (int)direction*10; else
curr_menu.val_data += (int)direction*100;
if (curr_menu.val_data < 0) curr_menu.val_data = 3600;
if (curr_menu.val_data > 3600) curr_menu.val_data = 0;
break;
case e_minute:          
curr_menu.val_data += direction;
if (curr_menu.val_data < 0) curr_menu.val_data = 59;
if (curr_menu.val_data > 59) curr_menu.val_data = 0;
break;
case e_hour:            
curr_menu.val_data += direction;
if (curr_menu.val_data < 0) curr_menu.val_data = 23;
if (curr_menu.val_data > 23) curr_menu.val_data = 0;
break;
case e_day:              
curr_menu.val_data += direction;
if (curr_menu.val_data < 1) curr_menu.val_data = 31;
if (curr_menu.val_data > 31) curr_menu.val_data = 1;
break;
case e_month:            
curr_menu.val_data += direction;
if (curr_menu.val_data < 1) curr_menu.val_data = 12;
if (curr_menu.val_data > 12) curr_menu.val_data = 1;
break;
case e_year:             
curr_menu.val_data += direction;
if (curr_menu.val_data < 11) curr_menu.val_data = 99;
if (curr_menu.val_data > 99) curr_menu.val_data = 11;
break;
case e_alert:
if (direction) curr_menu.val_data = 0;
break;
case e_password:
curr_menu.val_data += direction;
main_menu[6].can_edit = (curr_menu.val_data == 20); 
if (curr_menu.val_data < 0) curr_menu.val_data = 99;
if (curr_menu.val_data > 99) curr_menu.val_data = 0;
break;
default:
curr_menu.val_data += direction;
}

lcd_gotoxy(0,0);        
lcd_line_menu(par_str(lmenu, 0, lmenu->val_data), 0);
lcd_gotoxy(0,1);        
lcd_line_menu(par_str(lmenu, 1, curr_menu.val_data), 0);
lcd_command(0x0F   );
}
#pragma used+

void print_curr_menu2(signed char direction) {
calc_pos(&curr_menu, curr_menu.maximum, direction);
printf("Âûáîğ ìåíş [%u, %u]", curr_menu.line0, curr_menu.line1);
printf(" 1) ""%s"",\t2) ""%s""\r\n", main_menu[curr_menu.line0], main_menu[curr_menu.line1]);
}

void print_curr_menu(void) {
register byte i;

printf ("Òåêóùåå ìåíş: ");
for (i=0; i<curr_menu.maximum; i++) printf ("%s\t", param_str(i, curr_menu.menu));
printf ("\r\n");
}
#pragma used-

char *getmenustr(unsigned char menu_num_pp) {
static lcd_str menustr;
if (menu_num_pp) {
if (menu_num_pp < 61)
strcpyf (menustr, all_menu_str[menu_num_pp-1]);
else
strcpy (menustr, get_alert_str(menu_num_pp - 61));
return menustr;
} else
return 0;
}
