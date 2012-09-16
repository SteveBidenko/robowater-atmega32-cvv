
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
void i2c_init(void);
unsigned char i2c_start(void);
void i2c_stop(void);
unsigned char i2c_read(unsigned char ack);
unsigned char i2c_write(unsigned char data);
#pragma used-

#pragma used+

void delay_us(unsigned int n);
void delay_ms(unsigned int n);

#pragma used-

#pragma used+
char eeprom_write(unsigned char eeprom_adresse, unsigned char eeprom_input, unsigned char bank);
char eeprom_read(unsigned char eeprom_adresse, unsigned char bank);

void eeprom_write_struct (unsigned char *struct_data, unsigned char size);

void eeprom_read_struct (unsigned char *struct_data, unsigned char size);

void eeprom_write_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);

void eeprom_read_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm);
#pragma used-

char eeprom_write(unsigned char eeprom_adresse, unsigned char eeprom_input, unsigned char bank)
{
i2c_start();
if (bank)
i2c_write(0xA2    +0);
else
i2c_write(0xA0    +0);
i2c_write(eeprom_adresse);
i2c_write(eeprom_input);
i2c_stop();
delay_ms(10);
return 1;
}

char eeprom_read(unsigned char eeprom_adresse, unsigned char bank)
{
unsigned char data;
i2c_start();
if (bank)
i2c_write(0xA2    +0);
else
i2c_write(0xA0    +0);
i2c_write(eeprom_adresse);
i2c_start();
if (bank)
i2c_write(0xA2    +1);
else
i2c_write(0xA0    +1);
data = i2c_read(0);
i2c_stop();
return data;
}

void eeprom_write_struct (unsigned char *struct_data, unsigned char size)
{
register unsigned char i;
unsigned char s_byte;
printf("Записываем SETTINGS в EEPROM [0x%x]: ", size);
printf("\r\n");
for (i=0; i<size; i++) {
s_byte = *struct_data;
eeprom_write(i + 0x0, s_byte, 0);
struct_data++;
}
}

void eeprom_write_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm)
{
register unsigned char i;
unsigned char s_byte;
unsigned char eeprom_offset;
eeprom_offset = 0x0 + size * (num_alarm - 1);
printf("Записываем ALARM #%u в EEPROM [0x%x] по адресу 0x%x:\r\n", num_alarm, size, eeprom_offset);
for (i=0; i<size; i++) {
s_byte = *struct_data;
eeprom_write(i + eeprom_offset, s_byte, 1); 
struct_data++;
}
}

void eeprom_read_struct (unsigned char *struct_data, unsigned char size)
{
register unsigned char i;
unsigned char s_byte;
printf("Считываем из EEPROM [0x%x]: ", size);
printf("\r\n");
for (i=0; i<size; i++) {
s_byte = eeprom_read(i + 0x0, 0);
*struct_data = s_byte;
struct_data++;
}
}

void eeprom_read_alarm (unsigned char *struct_data, unsigned char size, unsigned char num_alarm)
{
register unsigned char i;
unsigned char s_byte;
unsigned char eeprom_offset;
eeprom_offset = 0x0 + (num_alarm - 1) * size;
for (i=0; i<size; i++) {
s_byte = eeprom_read(i + eeprom_offset, 1);
*struct_data = s_byte;
struct_data++;
}
}
