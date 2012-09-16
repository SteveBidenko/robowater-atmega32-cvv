
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
char eeprom_write(unsigned char eeprom_adresse, unsigned char eeprom_input, unsigned char bank);
char eeprom_read(unsigned char eeprom_adresse, unsigned char bank);

void eeprom_write_struct (unsigned char *struct_data, unsigned char size);

void eeprom_read_struct (unsigned char *struct_data, unsigned char size);

void eeprom_write_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);

void eeprom_read_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);
#pragma used-

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

struct st_alarm alarm;      

flash alarm_text all_alerts[12        ] = {
"Термозащита ",  
"Замерзание ",   
"Темп.возд.вх ", 
"Т.возд.вых. ",  
"Темп.воды вх ", 
"Т.воды вых.",   
"Нет связи ",    
"Нет терм.B1 ",  
"Нет терм.B2 ",  
"Нет терм.B3 ",  
"Нет терм.B4 ",  
"Фильтр "        
};

void update_alert_menu (void) {
register unsigned char i;
main_menu[5].val_data = 0;
for (i = 0; i < 12        ; i++) {
alerts[i].can_edit = prim_par.alert_status[i];
if (prim_par.alert_status[i]) {
main_menu[5].val_data++;
if (alerts[i].val_data == 0) alerts[i].val_data = 1;
}
}
main_menu[5].can_edit = main_menu[5].val_data;
}

void alarm_reg (int reg_preset, int reg_alarm, char *text_alarm, unsigned char code) {
sprintf (alarm.datestamp, "%02u:%02u %02u.%02u", s_dt.cHH, s_dt.cMM, s_dt.cdd, s_dt.cmo);
sprintf (alarm.text, "%s", text_alarm);
alarm.val_preset = reg_preset;
alarm.val_alarm = reg_alarm;

prim_par.alert_status[code] = 1;
alerts[code].val_data = reg_alarm;

if (prim_par.c_alarm < 8) prim_par.c_alarm++;
prim_par.alarm++;
if (prim_par.alarm > prim_par.c_alarm) prim_par.alarm = 1;

eeprom_write_alarm ((char *)&alarm, sizeof(alarm), prim_par.alarm);

eeprom_write_struct ((char *)&prim_par, sizeof(prim_par));
update_alert_menu ();
return;
}

signed char alarm_unreg (unsigned char code) {

if (prim_par.alert_status[code] == 0) return -1;

prim_par.alert_status[code] = 0;

eeprom_write_struct ((char *)&prim_par, sizeof(prim_par));
update_alert_menu ();
if (code >= 7 && code <= 10) read_all_terms(1); 
return;
}

signed char alarm_read (unsigned char num) {

if (num == 0) return -1;
eeprom_read_alarm ((char *)&alarm, sizeof(alarm), num);
}
void alarm_c_print (void) {
printf("> %s %i %i %s\r\n", alarm.datestamp, alarm.val_preset, alarm.val_alarm, alarm.text);
}

void alarm_all_print (void) {
register unsigned char i;
unsigned char c;
c = prim_par.alarm;

printf("Все аварии:\r\n");
for (i = 1; i <= 8; i++) {
alarm_read(c--);
alarm_c_print();
if (c == 0) c = prim_par.c_alarm;
}
}

char *get_alert_str(unsigned char code) {
static alarm_text alert_string;
if (code < 12        ) {
strcpyf (alert_string, all_alerts[code]);
return alert_string;
} else {
return 0;
}
}
