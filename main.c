#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include <string.h>
#include <delay.h>
#include "lcd_4bit.h"
#include "menu.h"
#include "robowater.h"
// DS1820 ������� �������������� �������������� (�������+�������)
#include "spd1820.h"
#include "valcoder.h"
#include "at2404.h"
#include "alarm.h"
#include "signals.h"
#include "sensors.h"
#include "dayofweek.h"
#include "regular.h"
#include "season.h"
#include "fan.h"
// ��������� ����������������
#define MAJOR_VERSION 5
#define MINOR_VERSION 16
// #define NODEBUG
// enum
// ����������� ������� ��������
// �������� ��������� ����������
// int tw_prs;             // �������� ����������� ����
// TW_in_Min -����������� ���� �� �����( ������ ) ���. ?
// TW_out_Min -����������� ���� �� ������ ( ������� ) ���.
// TW_out_Stop -����������� ���� �� ����� ��� ����������� � ������ ����.
// TA_in_Min -����������� ������� �� ����� ��� -30 �.
// TA_out_Min -����������� ������� �� ������ ��� +15 �.
// TA_out_prs -����������� ������� �� ������ ������������� +20 �.(��������)
unsigned char key_treated[7] = {0,0,0,0,0,0,0} ;
struct st_mode mode = {0, mo_stop, 0, 0, 0, 0, 1, 0.0,0,0,0};  // ������� ����� ������
struct st_datetime s_dt;
// ��������� ��������� ��������� �������� ����������
struct st_eeprom_par prim_par = {
    48, 205, 0x7F, 0x00,                // �1=50,�2=50,�3=48
    100, 100,                           // int dt_winter, dt_summer;
    (int)10, 0, 0,                      // int Ku, Ki, Kd;
    (int)180, (int)100,
    180,                                // byte T_start
    (int)1500, (int)5000,
    (int)-2000, (int)1000, (int)2200,
    48, 250, 48, 250, 48, 250, 48, 250,  // ��������� ������ �������� ������ � �������
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    1,  // ����
    0,  // ������� �������� alarm � EEPROM
    0,   // ������� ����� ���������������� � EEPROM
    {2,9,0,0},  //struct st_TO TO; weekday = 2, hour 9 , minute = 0, status = 0;
    MAX_DS1820, // ������� ������ ���� ����������� � �������
    {
    /*
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE}, // 28	4D	52	7E 	3	0	0	6E	1	FF	FE ������ ���������
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD}, // 28	D3	8	7E	3	0	0	E4	1	FF	FD   ������ �����
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC}, // 28	8C	38	7E	3	0	0	6A	1	FF	FC   ���� ������
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB} // 28	91	9	7E	3	0	0	32	1	FF	FB   ���� �������
     // ������� ������������� �1
     {0x28,0x29,0x32,0x7e,0x03,0x00,0x00,0x60,0x01}, // 28	29	32	7E 	3	0	0	60	1	FF	FE
     {0x28,0x13,0x3b,0x7e,0x03,0x00,0x00,0xb1,0x01}, // 28	13	3B	7E	3	0	0	B1	1	FF	FD
     {0x28,0x1e,0x2f,0x7e,0x03,0x00,0x00,0x98,0x01}, // 28	1E	2F	7E	3	0	0	98	1	FF	FC
     {0x28,0x39,0x46,0x7e,0x03,0x00,0x00,0x49,0x01} // 28	39	46	7E	3	0	0	49	1	FF	FB
     // ������� ������������� �2
     {0x28,0x95,0x0f,0x7e,0x03,0x00,0x00,0x72,0x01}, // 28	95	F	7E 	3	0	0	72	1	FF	FE
     {0x28,0xc1,0x1a,0x7e,0x03,0x00,0x00,0x2e,0x01}, // 28	C1	1A	7E	3	0	0	2E	1	FF	FD
     {0x28,0x1a,0x17,0x7e,0x03,0x00,0x00,0xfe,0x01}, // 28	1A	17	7E	3	0	0	FE	1	FF	FC
     {0x28,0xe8,0x2a,0x7e,0x03,0x00,0x00,0x8a,0x01} // 28	E8	2A	7E	3	0	0	8A	1	FF	FB
     // ������� ������������� �3
     {0x28,0x50,0x32,0x7e,0x03,0x00,0x00,0x6e,0x01}, // 28	50	32	7E 	3	0	0	6E	1	FF	FE
     {0x28,0x3e,0x1c,0x7e,0x03,0x00,0x00,0xe4,0x01}, // 28	3E	1C	7E	3	0	0	E4	1	FF	FD
     {0x28,0xb7,0x0d,0x7e,0x03,0x00,0x00,0x29,0x01}, // 28	B7	D	7E	3	0	0	29	1	1	FF	FC
     {0x28,0x2c,0x0c,0x7e,0x03,0x00,0x00,0xad,0x01} // 28	2C	C	7E	3	0	0	AD	1	FF	FB
     // ������� ������������� �4
     {0x28,0x4d,0x52,0x7e,0x03,0x00,0x00,0x6e,0x01}, // 28	4D	52	7E 	3	0	0	6E	1	FF	FE
     {0x28,0xd3,0x08,0x7e,0x03,0x00,0x00,0xe4,0x01}, // 28	D3	8	7E	3	0	0	E4	1	FF	FD
     {0x28,0x8c,0x38,0x7e,0x03,0x00,0x00,0x6a,0x01}, // 28	8C	38	7E	3	0	0	6A	1	FF	FC
     {0x28,0x91,0x09,0x7e,0x03,0x00,0x00,0x32,0x01}} // 28	91	9	7E	3	0	0	32	1	FF	FB
     // 50 �������� ����������.
     {0x28,0x1b,0xc4,0x24,0x00,0x00,0x00,0xfe,0x01}, // 28	1b	c4	24	0	0	0	fe	1	FF	FE
     {0x28,0xbb,0x0a,0x95,0x03,0x00,0x00,0xd8,0x01}, // 28	bb	a	95	3	0	0	d8	1	FF	FD
     {0x22,0x8e,0xe1,0x03,0x00,0x00,0x00,0xf3,0x01}, // 22	8e	e1	3	0	0	0	f3	1	FF	FC
     {0x22,0x2b,0x08,0x04,0x00,0x00,0x00,0xeb,0x01} // 22	2b	8	4	0	0	0	eb	1	FF	FB
     // ��������� �2.
     {0x28,0xf8,0x28,0x94,0x03,0x00,0x00,0x00,0x01}, // 28  F8  28  94  3   0   0   0   1	FF	FE   ���������
     {0x28,0x7b,0x3b,0x94,0x03,0x00,0x00,0x81,0x01}, // 28  7B  3B  94  3   0   0   81  1	FF	FD   �����
     {0x28,0x54,0x26,0x94,0x03,0x00,0x00,0x83,0x01}, // 28  54  26  94  3   0   0   83  1	FF	FC   ���� ����
     {0x28,0xe7,0x3f,0x94,0x03,0x00,0x00,0x52,0x01}  // 28  E7  3F  94  3   0   0   52  1	FF	FB   ���� �����
     // ��������� �1.
     {0x28,0xba,0x56,0x7e,0x03,0x00,0x00,0x86,0x01}, // 28  BA  56  7E  3   0   0   86  1	FF	FE   ���������
     {0x28,0xea,0x20,0x95,0x03,0x00,0x00,0x84,0x01}, // 28  EA  20  95  3   0   0   84  1	FF	FD   �����
     {0x28,0x1e,0x34,0x7e,0x03,0x00,0x00,0x94,0x01}, // 28  1E  34  7E  3   0   0   94  1	FF	FC   ���� ����
     {0x28,0x1e,0x2b,0x7e,0x03,0x00,0x00,0x87,0x01}  // 28  1E  2B  7E  3   0   0   87  1	FF	FB   ���� �����
     // ������� �1.
     {0x28,0x61,0x23,0x7e,0x03,0x00,0x00,0x05,0x01}, // 28  61  23  7E  3   0   0   5   1	FF	FE   ���������
     {0x28,0x56,0x21,0x7e,0x03,0x00,0x00,0xee,0x01}, // 28  56  21  7E  3   0   0   EE  1	FF	FD   �����
     {0x28,0x13,0x57,0x7e,0x03,0x00,0x00,0x81,0x01}, // 28  13  57  7E  3   0   0   81  1	FF	FC   ���� ����
     {0x28,0xc6,0x1e,0x7e,0x03,0x00,0x00,0xb4,0x01}  // 28  C6  1E  7E  3   0   0   B4 1	FF	FB   ���� �����
     // ������� �2.
     {0x28,0xb8,0x24,0x7e,0x03,0x00,0x00,0x06,0x01}, // 28  B8  24  7E  3   0   0   6   1	FF	FE   ���������
     {0x28,0x2c,0x1d,0x7e,0x03,0x00,0x00,0x1c,0x01}, // 28  2C  1D  7E  3   0   0   1C  1	FF	FD   �����
     {0x28,0x6f,0x42,0x7e,0x03,0x00,0x00,0xca,0x01}, // 28  6F  42  7E  3   0   0   CA  1	FF	FC   ���� ����
     {0x28,0x4f,0x36,0x7e,0x03,0x00,0x00,0x0e,0x01}  // 28  4F  36  7E  3   0   0   E   1	FF	FB   ���� �����
     // ������� �3.
     {0x28,0xc2,0x14,0x7e,0x03,0x00,0x00,0xd5,0x01}, // 28  C2  14  7E  3   0   0   D5  1	FF	FE   ���������
     {0x28,0xd6,0x3a,0x7e,0x03,0x00,0x00,0x08,0x01}, // 28  D6  3A  7E  3   0   0   8   1	FF	FD   �����
     {0x28,0x4f,0x4c,0x7e,0x03,0x00,0x00,0xde,0x01}, // 28  4F  4C  7E  3   0   0   DE  1	FF	FC   ���� ����
     {0x28,0x0a,0x3e,0x7e,0x03,0x00,0x00,0xae,0x01}  // 28  A   3E  7E  3   0   0   AE  1   FF	FB   ���� �����
     // ������������ ������� 1
     {0x28,0x6D,0x3F,0x94,0x03,0x00,0x00,0x77,0x01}, // 28  6D  3F  94  03  00  00  77  1	FF	FE   ���������
     {0x28,0x9D,0x41,0x94,0x03,0x00,0x00,0xCA,0x01}, // 28  9D  41  94  03  00  00  CA  1	FF	FD   �����
     {0x28,0xF7,0xFF,0x93,0x03,0x00,0x00,0xAD,0x01}, // 28  F7  FF  93  03  00  00  AD  1	FF	FC   ���� ����
     {0x28,0xA9,0x29,0x94,0x03,0x00,0x00,0xD4,0x01}  // 28  A9  29  94  03  00  00  D4  1   FF	FB   ���� �����

     // ������������ ������� 2
     {0x28,0xc2,0x14,0x7e,0x03,0x00,0x00,0xd5,0x01}, // 28  C2  14  7E  3   0   0   D5  1	FF	FE   ���������
     {0x28,0xd6,0x3a,0x7e,0x03,0x00,0x00,0x08,0x01}, // 28  D6  3A  7E  3   0   0   8   1	FF	FD   �����
     {0x28,0x4f,0x4c,0x7e,0x03,0x00,0x00,0xde,0x01}, // 28  4F  4C  7E  3   0   0   DE  1	FF	FC   ���� ����
     {0x28,0x0a,0x3e,0x7e,0x03,0x00,0x00,0xae,0x01}  // 28  A   3E  7E  3   0   0   AE  1 FF	FB   ���� �����
     //������
     {0x28,0x6a,0x22,0x94,0x03,0x00,0x00,0x62,0x01}, // 28  6A  22  94  03  00  00  62  1	FF	FE   ���������
     {0x28,0x93,0x10,0x94,0x03,0x00,0x00,0x81,0x01}, // 28  93  10  94  03  00  00  81  1	FF	FD   �����
     {0x28,0x03,0x42,0x94,0x03,0x00,0x00,0x26,0x01}, // 28  03  42  94  03  00  00  26  1	FF	FC   ���� ����
     {0x28,0xe4,0x09,0x94,0x03,0x00,0x00,0x13,0x01}  // 28  E4  09  94  03  00  00  13  1 FF	FB   ���� �����  
     */
     // ����������������� 90 ��1
     {0x28,0x00,0x12,0x94,0x03,0x00,0x00,0xea,0x01}, // 28  00  12  94  03  00  00  EA  1	FF	FE   ���������
     {0x28,0xab,0x42,0x94,0x03,0x00,0x00,0xdb,0x01}, // 28  AB  42  94  03  00  00  DB  1	FF	FD   �����
     {0x28,0xbd,0xf6,0x93,0x03,0x00,0x00,0xe4,0x01}, // 28  BD  F6  93  03  00  00  E4  1	FF	FC   ���� ����
     {0x28,0x02,0xf5,0x93,0x03,0x00,0x00,0x89,0x01}  // 28  02  F5  93  03  00  00  89  1   FF	FB   ���� �����

    }
};
unsigned char size_prim_par;        // ����� ���������. ����� ��� ������� ������/������ ��/� EEPROM

int time_cooling = 0;
enum en_event event;                          // ������� ������� � �������
#define HELP_LINES 14
typedef char help_str[70];
flash help_str all_help_str[HELP_LINES] = {
"z - ����� ��������� �������",                                          // [0]
"s - ������������ �����",                                               // [1]
"w - ������ ������ �������� �������",                                   // [2]
"x - ������������ ������",                                              // [3]
"v - ������ ������� ����������� (ds1820_rom_codes)",                    // [4]
"b - ������ ������� �� ������������ ����������� (ufo)",                 // [5]
"e - ������ ������� ��������� ������",                                  // [6]
"d - �������� ������",                                                  // [7]
"\\ - ������ ����� ����� �����������",                                  // [8]
"� - ������ ��������� prim_par",                                        // [9]
"m - ������ ������� ���������� �������",                                // [10]
"n - ������ ��������� �������",                                         // [11]
"0..3 - ����� ������ ���������� �� prim_par.addr � ds1820_rom_codes",   // [12]
"9 - ����� ���� ��������� ��������",                                    // [13]
};
// �������� �������
void print_help();
void printallterms(unsigned char);
void printnewterms(void);
void print_prim_par(unsigned char *, unsigned char);
// ������� ��������� �����
void check_serial(void);
void check_peripheral(void);
void event_processing(void);
void mode_processing(void);
// void delay_run(void);
void start_screen(unsigned char);
void full_reset(void);          // �������, ���������������� ��� (!) � ��������� ����������
void factory_reset(void);       // �������, ���������������� ��������� �� ��������� �� ������ ���������� � �����������
void factory_term_reset(void);  // �������, ����������������� ��������� �� EEPROM � ������������ ������ ��������� ����������� � EEPROM
// �������� ���������
void main(void) {
    // register byte i;
    init();                  // ������������� ���� ���������
    #asm("sei")             // Global enable interrupts
    init_keys();
    // ��������� � EEPROM ��������� prim_par
    size_prim_par = sizeof(prim_par);
    // ����� ���������� � EEPROM � ��������� ����������, ���� ������������ ������ ������� CANCEL + ENTER
    if (!KEY_CANCEL && !KEY_ENTER) {
        full_reset();
    } else {
        if (!KEY_CANCEL) factory_reset();
        if (!KEY_ENTER) factory_term_reset();
    }
    // delay_run(); �������� ���������
    // ��������������� �� EEPROM ��������� prim_par �� ����� ������������
    eeprom_read_struct ((char *)&prim_par, size_prim_par);
    // �������������� ��� ���������� � ��������� �� ���������
    read_all_terms(INIT_MODE);
    start_screen(0);
    // print_all_menu();       // ������� �� ���������� ������� ��� ������ ����
    sync_set_par(SYNC_TO_MENU); // �������������� ���� � ����������� �����������
    update_warning_menu();        // ��������� ���� warnings
    //printf("����� ������ �� EEPROM �������� Tw2=%u\r\n", prim_par.Tw2);
    // printf("���� %u, ����� ����� ������ �� EEPROM size=%u\r\n", sizeof(prim_par), size_prim_par);
    #ifndef NODEBUG
    // ���� ���� ����������, �� ������� �� ��������
    printallterms(0);
    #endif
    lcd_primary_screen();       // ������� ��������� �������� �� ��������
    if (prim_par.season) signal_white(ON); else signal_white(OFF);
    FAN_SPEED = check_fan_range(prim_par.fan_speed);
    link_terms();
    // ��������� �������� ����� �� �������� �����������
    tap_angle_check_range(INIT_MODE);
    while(1) {
        // ��������! ���� �� ��������� ����� WHILE ��� �� ���������!!!
        check_serial();
        poll_keys();
        /* if (event == ev_none) */ check_peripheral();
        /* if (0 == 1) */ event_processing();
        mode_processing();
    }; // while (1)

} // void main(void)
// ������� ������ ��������� � ��������� ������� �� ����������� ������, � ����� �� ����������� ����������
void check_peripheral(void) {
    // � ���� ������� ��������� (!) ������������� ������
    // ������������ ������� valcoder'�
    if ((abs(valcoder)-VALCODER_SENSITY) >= 0) {        // ���� �������� valcoder
        if (valcoder < 0)
            event = ev_left;
        else
            event = ev_right;
    }
    // ���� �������� �� ����, ���������� ������ �����
    if (CHECK_EVENT && (!KEY_ENTER)) event = ev_enter;
    if (CHECK_EVENT && (!KEY_CANCEL)) event = ev_cancel;
    // ����������� ������ ����
    //if (CHECK_EVENT && (!timer_stop) && (!KEY_STOP)) event = ev_stop;
    if (CHECK_EVENT &&  (!KEY_STOP)) event = ev_stop;
    // ����������� ������ �����, �������� ��� ���� ������� �����-���� ������
    if (CHECK_EVENT && (IS_ALARM == 0) && (!KEY_START)) event = ev_start;
    // �������e� ����������, ������� warnings � alarms
    regular_check_alarm_and_warnings();
    // ���������, ��� �� ���� ���� ������ ������� ����, ���� ��� ��
    if (CHECK_EVENT && (mode.initrun)) {
        mode.initrun -= INITMODE;
        switch (mode.initrun) {
            case mo_stop:
                event = (mode.run == mo_to) ? ev_end_to : ev_stop;
                break;
            case mo_action:
                if ((IS_ALARM == 0) && mode.run == mo_stop) {
                    event = ev_start;
                }
                break;
            case mo_to:
                if ((IS_ALARM == 0) && mode.run == mo_stop) {
                    event = ev_begin_to;
                }
                break;
            default:
        }
        mode.initrun = 0;
    }
    #ifndef NODEBUG
    // if (event) printf ("������������� ����� �������: %u\r\n", (unsigned char)event);
    //if (event) printf ("-");
    #endif
}
// ��������� ������� � �������
void event_processing(void) {
    // ������ ����� ����� ������ ������. �� ����� ������ ���������� ���������, ������ ���������
    #ifndef NODEBUG
    // if (event != ev_none)
        // printf ("%02u:%02u ������� ������� = %u\r\n", s_dt.cMM, s_dt.cSS, (unsigned char)event);
    #endif
    // ����� ����� ����������� ����������������� �������� ��� ���������: ���./���� ���������, ��������� ���������, ���������� ������ ���� � �.�.
    switch (event) {
        case ev_secunda:                // ������������ ������������ �������.
            regular_inspection();       // ����������� ��������� �������� � main.c
            break;
        case ev_left:                   // printf ("������������ ��������� valcoder �����\r\n");
        case ev_right:                  // printf ("������������ ��������� valcoder ������\r\n");
            // ��������� ������ ������������
            menu_timer_inactive = TIMER_INACTIVE;
            valcoder = VALCODER_NO_ROTATE;
            signal_buz(OFF);
            // printf ("������������ ��������� valcoder (%d), � ������ %d - ", event-2, mode.menu);
            switch (mode.menu) {
                case 0:
                    sync_set_par(SYNC_TO_MENU);
                    lcd_menu(mode.menu++); break;   // ������� ���� ��� ��������� ������� printf ("entering...");
                case 1: lcd_menu(event-2); break;       // printf ("navigating...");
                case 2: lcd_edit(event-2); break;       // printf ("editing...");
                default: ;                              // printf ("defaulting...");
            }
            // printf ("\r\n");
            event = ev_none;            // ������� �������
            break;
        case ev_enter:                  // ���� ����� Enter
            // LAMP_ECHO_PORT |= (1<<LAMP_ECHO_PIN); timer1_lamp = ENTER_CANCEL_OVERFLOW; if (mode.menu <= 2)
            key_treated[2]=1;
            event = ev_none;            // ������� �������
            signal_buz(SHORT);
            menu_timer_inactive = TIMER_INACTIVE;     //��������� ������ ������������
            switch (mode.menu) {
                // lcd_primary_screen();
                // ������������ ������� enter c ������ ����, ��� �������� mode.menu ��� ������
                case 0:
                    sync_set_par(SYNC_TO_MENU);
                    lcd_menu(mode.menu++); break;           // ���� ���������� � ������� ������, ��������� ���������� ����
                case 1: lcd_init_edit(); break;             // ���� ���������� � ����, �� �����������
                case 2: lcd_save_edit(); mode.menu = 1; break;
            };
            break;
        case ev_cancel:
            menu_timer_break();
            signal_buz(SHORT);
            key_treated[3]=1;
            event = ev_none;            // ������� �������
            break;
        case ev_start:      // ���� ���������
            key_treated[1]=1;
            printf ("������ ������ ����. ");
            switch (mode.run) {
                case mo_stop: // ���� ������ ����� STOP
                    if (prim_par.season) {
                        signal_white(ON);
                        mode.pomp = 1;
                        mode.run = mo_warming_up;     // ������������� ����� �������
                        timer_start = prim_par.T_start; // ��������� ������ STRT
                        TAP_ANGLE = PWM_MAX;
                        printf("������� ����� �������. ����� �������� = %d\r\n", timer_start);
                        signal_green(SHORT);
                    } else {
                        signal_white(OFF);
                        mode.pomp = 0;
                        mode.run = mo_action;
                        mode.fan = 1;
                        signal_green(ON);
                        printf("������� ����� ����\r\n");
                    };
                    FAN_SPEED = check_fan_range(prim_par.fan_speed);
                    //count_fan = 0;
                    time_integration = 0;
                    //time_cooling = TIME_COOLING_MAX;
                    time_cooling = prim_par.T_z;
                    signal_buz(LONG);
                    break; // mo_stop
                case mo_warming_up:
                    signal_buz(SHORT);
                    if (mode.print) printf("����� �������. ����� �������� = %d\r\n", timer_start);
                    break; // mo_warming_up
                case mo_action: signal_buz(SHORT);
                    if (mode.print) printf("����� ����.\r\n");
                    break; // mo_action
                default: break;
            };
            event = ev_none;
            break;
        case ev_stop:
            key_treated[0]=1;
            printf ("������ ������ ����. ");
            switch (mode.run) {
                case mo_stop: signal_buz(SHORT); time_integration = 0;
                    if (mode.print) printf(" ����� ����.\r\n");
                    break; // mo_stop
                case mo_warming_up: 
                    mode.run = mo_warming_down;
                    timer_start = 0;
                    signal_green(SHORT);  signal_buz(LONG);
                    if (mode.print) printf ("������� ����� ���������\r\n");
                    timer_stop = TIME_STOP;    // ��������� ������ STOP
                    time_integration = 0;
                    break; //mode.run=1;
                case mo_warming_down:
                    signal_buz(SHORT);
                    time_integration = 0;
                    if (mode.print) printf("����� ���������.\r\n");
                    break; // mo_warming_down
                case mo_action:
                    mode.run = mo_warming_down;
                    signal_green(LONG); signal_buz(LONG);
                    if (mode.print) printf ("������� ����� ���������\r\n");
                    time_cooling = 0;
                    timer_stop = TIME_STOP;    // ��������� ������ STOP
                    time_integration = 0;
                    break; // mo_action
                case mo_to:
                    printf ("�������������� ��������� ��\n");
                    timer_start = 0;
                    time_integration = 0;
                    prim_par.TO.status = 0;
                    timer_stop_to = 0;
                    timer_start_to = 0;
                    break; // mo_to
                default: break;
            }
            mode.fan = 0;  // ���������� �����������
            event = ev_none;
            break;
        case ev_begin_to:
            if (mode.run == mo_stop) {
                mode.run = mo_to;
                timer_start_to = T_TO; // ��������� ������ STRT
                timer_stop_to = 0;
                TAP_ANGLE = PWM_MAX;
                printf("�� �����. ���������� LIMIT = %d, ����� �� ���������� = %d\n", TAP_ANGLE, timer_start_to);
            }
            event = ev_none;
            break;
        case ev_to_nf:
            alarm_reg(0, 1, get_warning_str(6), 6);
            printf("���������� �������� �� �����." );
        case ev_end_to:
            timer_start_to = 0;
            timer_stop_to = 0;
            switch_mode_run(mo_stop);
            event = ev_none;
            prim_par.TO.status = 0;
            printf("����� �� �����." );
            break;
        case ev_alarm1:   // �����, �������� �����������, ������ ����������
            signal_red(ON); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1; // ���������
            TAP_ANGLE = PWM_MAX;
            alarm_reg(0, 1, get_warning_str(0), 0);
            printf ("������: %s\r\n", get_warning_str(0));
            event = ev_none;
            break;
        case ev_alarm2:   // ������ ������������� �� �������� �������
            signal_red(ON); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            alarm_reg(0, 1, get_warning_str(1), 1);
            printf ("������: %s\r\n", get_warning_str(1));
            event = ev_none;
            break;
        case ev_freezing1:  // ����������� �� ����� ���� ����������� UL_T (���� �� ����������� � ������� ���� ��������������)
            alarm_reg(0, 1, get_warning_str(2), 2);
            signal_red(ON);
            signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_warning_str(2));
            event = ev_none;
            break;
        case ev_freezing2: // ����������� � ��������� ���� ����������� POM_T
            alarm_reg(0, 1, get_warning_str(3), 3);
            signal_red(ON); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_warning_str(3));
            event = ev_none;
            break;
        case ev_freezing3: // ����������� ���� ������� ���� ����������� POM_T
            alarm_reg(0, 1, get_warning_str(5), 5);
            signal_red(ON); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_warning_str(5));
            event = ev_none;
            break;
        case ev_term1_nf:   // ��������� �1 ���������
            alarm_reg(MAX_OFFLINES, termometers[0].err, get_warning_str(7), 7);
            signal_red(ON); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_warning_str(7));
            event = ev_none;
            break;
        case ev_term2_nf:  // ��������� �2 �����
            alarm_reg(MAX_OFFLINES, termometers[1].err, get_warning_str(8), 8);
            signal_red(LONG); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_warning_str(8));
            event = ev_none;
            break;
        case ev_term3_nf:  // ��������� �3 ������
            // ��� ���, �� � ���� � ���
            alarm_reg(MAX_OFFLINES, termometers[2].err, get_warning_str(9), 9);
            printf ("��������������: %s\r\n", get_warning_str(9));
            event = ev_none;
            break;
        case ev_term4_nf:  // ��������� �4 �������
            alarm_reg(MAX_OFFLINES, termometers[3].err, get_warning_str(10), 10);
            signal_red(LONG); signal_buz(MEANDR);
            switch_mode_run(mo_stop);
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_warning_str(10));
            event = ev_none;
            break;
        case ev_filter:  // ����������� �������.
            alarm_reg(0, 1, get_warning_str(11), 11);
            event = ev_none;
            printf ("������: %s\r\n", get_warning_str(11));
            signal_red(SHORT);
            break;
        default:
            break;
    };
}
// ��������� ������ ������ �������
void mode_processing(void) {
    // � ���� ������� ��������� ��������� ������� � ��������� �������. ������ ���������!
    SET_T = prim_par.TA_out_prs;
    // ����� ������������ �������� �� ����� �����������
    switch (mode.run) {
        case mo_stop:
        case mo_warming_up:
        case mo_warming_down:
        case mo_action:
           OCR0 = (unsigned char)TAP_ANGLE;
           OCR2 = (unsigned char)FAN_SPEED;// �������������� ��� ��������� ����������
           FAN = FAN_MENU = mode.fan;
           POMP = POMP_MENU = mode.pomp;
           // COOLING1 = mode.cooling1;
           // COOLING2 = mode.cooling2;
           break;
        case mo_to:
            if ((timer_stop_to == 0) && (ADC_VAR1 >= (prim_par.ADC1_hi - 10))) {
               TAP_ANGLE = PWM_MIN;
               timer_start_to = 0;
               timer_stop_to = T_TO;
               printf("�� �����. ���������� LIMIT = %d, ����� �� ���������� = %d\n", TAP_ANGLE, timer_stop_to);
            }
            OCR0 = (unsigned char)TAP_ANGLE;
            break;
        case mo_setup_input1:
            break;
        case mo_setup_input2:
            break;
        case mo_setup_output1:
            OCR0 = (unsigned char)menu_value;
            break;
        case mo_setup_output2:
            OCR2 = (unsigned char)menu_value;
            break;
        default:

    }
}
/*         */
void switch_mode_run(enum en_mode newstatus) {
    if ((unsigned char)mode.run <= 4) mode.lastrun = mode.run;
    mode.run = newstatus;
    switch (mode.run) {
        case mo_stop:
            signal_green(OFF);
            mode.fan = 0;           // ���������� �����������
        case mo_action:
            signal_green(ON);
            break;
        default: break;
    } 
} 
// ������ ���� �����������. mode - ����� ������ ������� �����������. ���� �� 0 - �������� ������.
void printallterms(unsigned char print_addr) {
    int term;
    register byte i;
    unsigned char resolution;
    signed char th, tl;
    unsigned char *spd, *address_line;

    if(!ds1820_devices) return;			// ���� ���������� �� ���������� - ������ ������� �� �������
    spd = ds1820_show_spd();
    if (print_addr) printf("������ �����������(ds1820_rom_codes): ");
    for(i=0; i<ds1820_devices; i++) {
        ds1820_get_resolution(&ds1820_rom_codes[i][0], &resolution);
    	term = ds1820_temperature(ds1820_rom_codes[i]);
    	// ������������ term = ds1820_temperature(&ds1820_rom_codes[i][0]);
        tl = th = 0;
        ds1820_get_alarm(ds1820_rom_codes[i], &tl, &th);
        // ������������ ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
        if (print_addr) {
            address_line = address_to_LCD (ds1820_rom_codes[i]);
            printf("%s:%d ", address_line, termometers[i].err);
        } else {
            printf(" t%-u = %i(%-i.%-u)C[%02X%02X:%02x] err:%d; ", i+1, term, term/100, abs(term%100), spd[1], spd[0], resolution, termometers[i].err);
        }
    }
    printf("\r\n");
}
// ������ ���� ����� �����������
void printnewterms(void) {
    if (mode.new_terms) {
        register byte i;
        unsigned char *address_line;

        printf("mode.ufo ����������� ����������� new_terms = %d: ", mode.new_terms);
        for (i = 0; i < mode.new_terms; i++) {
            address_line = address_to_LCD (mode.ufo[i]);
            printf("[ %s ] ", address_line);
        }
        printf("\r\n");
    } else
        printf("����� ����������� �� �������\r\n");
}
// ������� ������������� ������ ��������� �� LCD � �� ���������
void start_screen(unsigned char init) {
    lcd_clrscr();
    printf("����� ������� �3. %u.%02u. ������� %u �����������.\r\n", MAJOR_VERSION, MINOR_VERSION, ds1820_devices);
    lcd_command(LCD_DISP_ON);       // ������� ������ � LCD
    lcd_gotoxy(0,0);        // ������������� ������ � ������� 0 ������ ������
    sprintf(linestr, "MTECH %u.%02u",MAJOR_VERSION, MINOR_VERSION );
    lcd_puts(linestr);
    lcd_gotoxy(0,1);                // ������������� ������ � ������� 0 ������ 2
    switch (init) {
        case 1:
            sprintf(linestr, "FULL RESET"); break;
        case 2:
            sprintf(linestr, "FACTORY"); break;
        case 3:
            sprintf(linestr, "ONLY NEW TERMS"); break;
        case 0:
        default:
            sprintf(linestr, "Robowater ..."); break;
    }
    lcd_puts(linestr);
}
// ������ ��������� prim_par
void print_prim_par(unsigned char *struct_data, unsigned char size) {
    register unsigned char i;
    unsigned char s_byte;
    printf("��������� PRIM_PAR [%d bytes]: ", size);
    for (i=0; i<size; i++) {
        s_byte = *struct_data++;
        if (i <= (size - 37))		// 37 - ��� �������� ������� ����������� � prim_par �� �����
            printf(" %d", s_byte);
        else
            printf("%02X ", s_byte);
        if (i == size - 37) printf("\r\nTERMS: ");
    }
    printf("\r\n");
}
// ������������ �����
void toggle_sound(void) {
    if (mode.sound)
        printf("���� ��������\r\n");
    else
        printf("���� �������\r\n");
    mode.sound = !mode.sound;
}
// ����� ��������� � ���������
// #define DELAY_TIME 300
#define DELAY_TIME 30
/*
void delay_run(void) {
    byte i =0;
    signal_buz(LONG);
    for (i = 0; i < 7; i++){
        delay_ms(DELAY_TIME);
        signal_white(ON);
        delay_ms(DELAY_TIME);
        signal_white(OFF);
        signal_red(ON);
        delay_ms(DELAY_TIME);
        signal_red(OFF);
        signal_green(ON);
        delay_ms(DELAY_TIME);
        signal_green(OFF);
    };

}
*/
// ������������ ������
void toggle_print(void) {
    mode.print = !mode.print;
    if (mode.print)
        printf("������ ��������\r\n");
        //printf("������ ���������\r\n �����, ����������� ��������, ����������� ����������, Delta, ���� ����� ���������, ���� ���������������, ���� �����������, result, pTerm, iTerm, dTerm, ������� ���������, ������� �����������\r\n");
    else
        //printf("������ ���������\r\n �����, ����������� ��������, ����������� ����������, Delta, ���� ����� ���������, ���� ���������������, ���� �����������, result, pTerm, iTerm, dTerm, ������� ���������, ������� �����������\r\n");
        printf("������ ���������\r\n");
}
// ��������� ������� �� ��������� �����
void check_serial(void) {
    byte inbyte;    // �������� ��������� ����������
    register byte i;
    // ������������ ���������������� ����
    if (UCSRA & RX_COMPLETE) { // ������ �� ���-������
        inbyte = UDR;
        switch (inbyte) {
            case 0x68:  /* 'h' */
                print_help(); break;
            case 0x7A:  /* 'z' */
                printf("�����: %02u:%02u:%02u, ����:%02u.%02u.%02u, ������� %u �����������\r\n",
                        s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo, s_dt.cyy, ds1820_devices);
                printallterms(0);
                break;
            case 0x73:  /* 's' */     // ������������ �����
                toggle_sound(); break;
            case 0x77:  /* 'w' */     // ������
                printf ("C������ = %u\r\n", (unsigned char)event);
                break;
            case 0x78:  // ������ 'x'
                toggle_print(); break;
            case 0x76: // ������ 'v'
                printallterms(1); break;
            case 0x62: // ������ 'b'
                printnewterms(); break;
            case 0x65:  // ������ 'e'
                alarm_all_print(); break;
            case 0x64:  // ������ 'd'
                printf ("��������� ������ ������.\r\n");
                for (i=0; i < MAX_WARNINGS; i++) {
                    if (!(alarm_unreg(i))) {
                       printf (" ������� �������� �������: %s\r\n", get_warning_str(i));
                    }
                }
                break;
            case 0x5c:  // ������ '\'
                printf ("�������������� ��� ����������!\r\n");
                read_all_terms(INIT_MODE);
                break;
            case 0x63:                // ������ 'c'
                print_prim_par((unsigned char *)&prim_par, sizeof(prim_par));
                break;
            case 0x6D:                // ������ 'm'
                for(i=0; i<NUM_PARAMETERS; i++) printf("%s\t", param_str(i, parameters));
                printf("\r\n");
                break;
            case 0x6E:                // ������ 'n'
                for(i=0; i<NUM_MENU; i++) printf("%s\t", param_str(i, main_menu));
                printf("\r\n");
                break;
            case 0x30:              // ������ '0'
                i = ds1820_is_exist (prim_par.addr[0], ds1820_rom_codes[0]);
                printf ("%s ��������� ������ � ������� %u\r\n", address_to_LCD(prim_par.addr[0]), i);
                break;
            case 0x31:              // ������ '1'
                i = ds1820_is_exist (prim_par.addr[1], ds1820_rom_codes[0]);
                printf ("%s ��������� ������ � ������� %u\r\n", address_to_LCD(prim_par.addr[1]), i);
                break;
            case 0x32:              // ������ '2'
                i = ds1820_is_exist (prim_par.addr[2], ds1820_rom_codes[0]);
                printf ("%s ��������� ������ � ������� %u\r\n", address_to_LCD(prim_par.addr[2]), i);
                break;
            case 0x33:              // ������ '3'
                i = ds1820_is_exist (prim_par.addr[3], ds1820_rom_codes[0]);
                printf ("%s ��������� ������ � ������� %u\r\n", address_to_LCD(prim_par.addr[3]), i);
                break;
            case 0x39:              // ������ '9'
                signal_printallbytes();
                break;
            default:
                printf("����� ������ 0x%x\r\n", inbyte);
        };
    }
}
// �������, ���������������� ��� (!) � ��������� ����������
void full_reset(void) {
    start_screen(1);
    printf("������ � EEPROM ���� (!) ��������� ���������.\r\n");
    // ������������ ������� ����������� � EEPROM
    eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
}
// �������, ���������������� ��������� �� ��������� �� ������ ���������� � �����������
void factory_reset(void) {
    unsigned char size_addr = size_prim_par - sizeof(prim_par.addr);

    // ��� ������� ������� CANCEL ���������� ��������� �� ��������� �� ������ ���������� � �����������
    start_screen(2);
    ds1820_devices = MAX_DS1820;
    printf("������ � EEPROM ��������� �� ���������.\r\n");
    eeprom_write_struct ((unsigned char *)&prim_par, size_addr);
}
// �������, ����������������� �������� �� EEPROM � ������������ ��������������� ���������� �������
void factory_term_reset(void) {
    unsigned char size_addr = size_prim_par - sizeof(prim_par.addr);

    start_screen(3);
    printf("���������� �� EEPROM ��������� ��� ������� ����������\r\n");
    eeprom_read_struct ((char *)&prim_par, size_addr);
    printf("������ � EEPROM ������� ��������� �����������.\r\n");
    eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
}
// ������� ������ �� ����� HELP ����������
void print_help() {
    static help_str curr_line;
    register unsigned char i;
    for (i = 0; i < HELP_LINES; i++) {
        strcpyf(curr_line, all_help_str[i]);
        printf("%s\r\n", curr_line);
    }
}