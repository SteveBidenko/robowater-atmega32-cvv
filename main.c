#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
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
#include "keys.h"
#include "dayofweek.h"
// ��������� ����������������
#define MAJOR_VERSION 5
#define MINOR_VERSION 1
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
struct st_eeprom_par prim_par={
    48, 205, 0x7F, 0x00,                                   // �1=50,�2=50,�3=48
    (int)10, (int)20, (int)10,
    (int)180, (int)100,
    (int)1500, (int)5000,
    (int)-2000, (int)1000, (int)2200,
// ������� �� boiler-control 15.05.2013
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
     */
     // ������������ ������� 1
     {0x28,0x6D,0x3F,0x94,0x03,0x00,0x00,0x77,0x01}, // 28  6D  3F  94  03  00  00  77  1	FF	FE   ���������
     {0x28,0x9D,0x41,0x94,0x03,0x00,0x00,0xCA,0x01}, // 28  9D  41  94  03  00  00  CA  1	FF	FD   �����
     {0x28,0xF7,0xFF,0x93,0x03,0x00,0x00,0xAD,0x01}, // 28  F7  FF  93  03  00  00  AD  1	FF	FC   ���� ����
     {0x28,0xA9,0x29,0x94,0x03,0x00,0x00,0xD4,0x01}  // 28  A9  29  94  03  00  00  D4  1   FF	FB   ���� �����
     /*
     // ������������ ������� 2
     {0x28,0xc2,0x14,0x7e,0x03,0x00,0x00,0xd5,0x01}, // 28  C2  14  7E  3   0   0   D5  1	FF	FE   ���������
     {0x28,0xd6,0x3a,0x7e,0x03,0x00,0x00,0x08,0x01}, // 28  D6  3A  7E  3   0   0   8   1	FF	FD   �����
     {0x28,0x4f,0x4c,0x7e,0x03,0x00,0x00,0xde,0x01}, // 28  4F  4C  7E  3   0   0   DE  1	FF	FC   ���� ����
     {0x28,0x0a,0x3e,0x7e,0x03,0x00,0x00,0xae,0x01}  // 28  A   3E  7E  3   0   0   AE  1 FF	FB   ���� �����
     */
    }
};
unsigned char size_prim_par;        // ����� ���������. ����� ��� ������� ������/������ ��/� EEPROM
unsigned int time_integration=0;
unsigned char timer_start_to = 0;
unsigned char timer_stop_to = 0;
int tmp_delta;
byte tap_angle_min = 0;   // ����������� ����� ����� �� �����������
byte fan_speed = 0;  // �������� �����������
enum en_event event;                          // ������� ������� � �������
// �������� �������
void printallterms(unsigned char); void lcd_primary_screen(void);
void printnewterms(void);
void print_prim_par(unsigned char *, unsigned char);
// ������� ��������� �����
void check_serial(void);
void check_peripheral(void);
void event_processing(void);
void mode_processing(void);
void keep_life_in_winter(void); // ��������� ����������������� ������� �����
void winter_fan_speed(void);    // ������������� ����������� �����
void winter_regulator(void);    // ������������ ������������� ���������� �����
void coolant_regulator(void);   // ������������ ������������� ���������� (�� �����)
void update_P(int);
// void delay_run(void);
//void update_PID(int error, int iMin, int iMax);
void start_screen(unsigned char);
char high_time_TO(void);        // �������, ����������� ������������� ���������� ��
void full_reset(void);          // �������, ���������������� ��� (!) � ��������� ����������
void factory_reset(void);       // �������, ���������������� ��������� �� ��������� �� ������ ���������� � �����������
void only_new_term_reset(void); // �������, ����������������� �������� �� EEPROM � ������������ ��������������� ���������� �������
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
        if (!KEY_ENTER) only_new_term_reset();
    }
    // deley_run(); �������� ���������
    // ��������������� �� EEPROM ��������� prim_par �� ����� ������������
    eeprom_read_struct ((char *)&prim_par, size_prim_par);
    // �������������� ��� ���������� � ��������� �� ���������
    read_all_terms(INIT_MODE);
    start_screen(0);
    // print_all_menu();       // ������� �� ���������� ������� ��� ������ ����
    sync_set_par(SYNC_TO_MENU); // �������������� ���� � ����������� �����������
    update_alert_menu();        // ��������� ���� alerts
    //printf("����� ������ �� EEPROM �������� Tw2=%u\r\n", prim_par.Tw2);
    // printf("���� %u, ����� ����� ������ �� EEPROM size=%u\r\n", sizeof(prim_par), size_prim_par);
    #ifndef NODEBUG
    // ���� ���� ����������, �� ������� �� ��������
    printallterms(0);
    #endif
    lcd_primary_screen();       // ������� ��������� �������� �� ��������
    // if (PINC.6) PORTD |= (1<<4); else PORTD &= ~(1<<4);
    // if (PINC.7) PORTD &= ~(1<<5); else PORTD |= (1<<5);
    // if (PINC.5) PORTD &= ~(1<<5); else PORTD |= (1<<5);
    if (prim_par.season) signal_white(ON); else signal_white(OFF);
    FAN_SPEED = prim_par.fan_speed;
    fan_speed = FAN_SPEED;
    if (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
    POM_T = termometers[0].t;
    UL_T = termometers[1].t;
    WIN_T = termometers[2].t;
    WOUT_T = termometers[3].t;
    tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;  // ���������� ��������� ������ ������������ ����� �� �����������
    mode.k_angle_limit = ((TAP_ANGLE_LIMIT * 1000) / tmp_delta); // ���������� ������������ ����������� ����� ��� �������� ��������
    if (UL_T < TA_IN_NOLIMIT) {    // ���������� ���� ����������� (UL_T < TA_IN_NOLIMIT)
        tap_angle_min = prim_par.tap_angle + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;   // ���������� ����������� ����� �� ����������� ������� �� ����� � ������������ mode.k_angle_limit
        printf(" tap_angle_min %u .   K���������� :%d\r\n",  tap_angle_min, mode.k_angle_limit);
    };
    TAP_ANGLE = (TAP_ANGLE < tap_angle_min) ? tap_angle_min : prim_par.tap_angle;
    printf(" ��������� ����� %u .   prim_par.tap_angle :%d\r\n", TAP_ANGLE, prim_par.tap_angle);
    while(1) {
        // ��������! ���� �� ��������� ����� WHILE ��� �� ���������!!!
        check_serial();
        poll_keys();
        // if (event == ev_none) check_peripheral();
        /* if (0 == 1) */ check_peripheral();
        /* if (0 == 1) */ event_processing();
        /* if (0 == 1) */ mode_processing();
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
    if (CHECK_EVENT && (IS_ALERT == 0) && (!KEY_START)) event = ev_start;
    // ��� ����� "�����" ���������� �������, ���� ������� �������������
    // �������� ���������
    if (CHECK_EVENT && (!prim_par.alert_status[0]) && (KEY_ALARM1)) event = ev_alarm1; // �����, �������� �����������, ������ ����������
    if (CHECK_EVENT && (!prim_par.alert_status[1]) && (KEY_ALARM2)) event = ev_alarm2; // ������ ������������� �� �������� �������
    if (CHECK_EVENT && (!prim_par.alert_status[11]) && (!KEY_FILTER)) event = ev_filter; // ����������� �������.
    // �������� �����������
    if (CHECK_EVENT && (!prim_par.alert_status[7]) && (termometers[0].err >= MAX_OFFLINES))
        // printf("��� ���������� �1 (���������): %d, err=%d", prim_par.alert_status[7], termometers[0].err);
        event = ev_term1_nf;
    if (CHECK_EVENT && (!prim_par.alert_status[8]) && (termometers[1].err >= MAX_OFFLINES))
        // ��� ���������� � 2 - �����
        event = ev_term2_nf;
    if (CHECK_EVENT && (!prim_par.alert_status[9]) && (termometers[2].err >= MAX_OFFLINES))
        // ��� ���������� � 3 - ������
        event = ev_term3_nf;
    if (CHECK_EVENT && (!prim_par.alert_status[10]) && (termometers[3].err >= MAX_OFFLINES))
        // ��� ���������� � 4 - �������
        event = ev_term4_nf;
    // ����� ������������ ��������� ��� ��������� �������
    if (UL_T < TA_IN_NOLIMIT) {
        if (CHECK_EVENT && !(prim_par.alert_status[8] || prim_par.alert_status[2]) &&
            (termometers[1].t < (prim_par.TA_in_Min-5))) // ����������� �� ����� ���� �����������  �� 5 ��������.UL_T
            event = ev_freezing1;
        if (CHECK_EVENT && !(prim_par.alert_status[7] || prim_par.alert_status[3]) &&
            (termometers[0].t < prim_par.TA_out_Min) && (termometers[1].t < prim_par.TA_out_Min))  // ����������� � ��������� ���� ����������� POM_T
               // ���� ���������� �� ����� ���� ����������� POM_T
            event = ev_freezing2;
        if (CHECK_EVENT && !(prim_par.alert_status[10] || prim_par.alert_status[5]) &&
            (termometers[3].t < prim_par.TW_out_Min)) // ����������� ���� ������� ���� ����������� WOUT_T
            event = ev_freezing3;
    }
    // ���������, ��� �� ���� ���� ������ ������� ����, ���� ��� ��
    if (CHECK_EVENT && (mode.initrun)) {
        mode.initrun -= INITMODE;
        switch (mode.initrun) {
            case mo_stop:
                event = (mode.run == mo_to) ? ev_end_to : ev_stop;
                break;
            case mo_action:
                if ((IS_ALERT == 0) && mode.run == mo_stop) {
                    event = ev_start;
                }
                break;
            case mo_to:
                if ((IS_ALERT == 0) && mode.run == mo_stop) {
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
            s_dt.dayofweek = dayofweek(s_dt.cdd, s_dt.cmo, s_dt.cyy);
            MAIN_T = read_term(0);      // ������� ���������� � ������� ���������� !!!
            POM_T = read_term(0);
            UL_T = read_term(1);
            WIN_T = read_term(2);
            WOUT_T = read_term(3);
            switch (mode.menu) {
                case 0: lcd_primary_screen(); break;
                // case 2: lcd_edit(0); break;
                case 1: lcd_menu(0); break;
                default: ;
            }
            // ���� ���� ��������� ��, �� ���������� �������
            if (high_time_TO()) {
                printf ("���� ��������� ��\n");
                prim_par.TO.status = 1;
                event = ev_begin_to;
            } else {
                event = ev_none;            // ������� �������
            }
            if (mode.run == mo_to) {
                if (timer_start_to) {
                    timer_start_to--;
                        // ���� �������� ����, �� ������� ev_to_nf
                    if (!timer_start_to && (ADC_VAR1 < prim_par.ADC1_hi)) {
                        printf ("����������� ����� �������� \n");
                        event = ev_to_nf;
                    }
                }
                if (timer_stop_to) {
                    timer_stop_to--;
                    if (ADC_VAR1 <= (prim_par.ADC1_lo + 5)) {
                       printf ("�������� ���������� �� �����. \n");
                       event = ev_end_to;
                    } else {
                    // ���� �������� ����, �� ������� ev_to_nf
                        if (!timer_stop_to && (ADC_VAR1 > (prim_par.ADC1_lo + 10))) {
                            printf ("����������� ����� �������� \n");
                            event = ev_to_nf;
                        }
                    }
                }
            }
            break;
        case ev_left:                   // printf ("������������ ��������� valcoder �����\r\n");
        case ev_right:                  // printf ("������������ ��������� valcoder ������\r\n");
            // ��������� ������ ������������
            //timer1_valcoder = TIMER_INACTIVE;
            timer1_valcoder = TIMER_INACTIVE;
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
            timer1_valcoder = TIMER_INACTIVE;     //��������� ������ ������������
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
        case ev_timer:
            // ��������� ������ ������������
            if (mode.menu) timer1_valcoder = TIMER_INACTIVE; // TIMER_INACTIVE;
        case ev_cancel:
            if (event == ev_cancel) {
                signal_buz(SHORT);
                key_treated[3]=1;
                timer1_valcoder = TIMER_INACTIVE;     //��������� ������ ������������ ()
            }
            event = ev_none;            // ������� �������
            // print_prim_par((unsigned char *)&prim_par, sizeof(prim_par));
            switch (mode.menu) {
                case 0: lcd_primary_screen(); break;
                case 1: --mode.menu; lcd_esc_edit(); break;
                case 2: --mode.menu; lcd_esc_edit(); break;
                case 3: mode.menu = 1; break;
            };
             lcd_clrscr();
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
                        timer_start = TIME_START;
                        //timer_start = prim_par.T_z; // TIMER_INACTIVE  ...��������� ������ STRT
                        TAP_ANGLE = PWM_MAX;
                        printf("������� ����� �������. LIMIT = %d\r\n", mode.k_angle_limit);
                        signal_green(SHORT);
                    } else {
                        signal_white(OFF);
                        mode.pomp = 0;
                        mode.run = mo_action;
                        mode.fan = 1;
                        signal_green(ON);
                        printf("������� ����� ����\r\n");
                    };
                    FAN_SPEED = prim_par.fan_speed;
                    if (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
                    //count_fan = 0;
                    time_integration = 0;
                    //time_cooling = TIME_COOLING_MAX;
                    time_cooling = prim_par.T_z;
                    signal_buz(LONG);
                    break; // mo_stop
                case mo_warming_up:
                    signal_buz(SHORT);
                    if (mode.print) printf("����� �������.\r\n");
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
                case mo_warming_up: mode.run = mo_warming_down;
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
                TAP_ANGLE = prim_par.PWM1_hi;
                printf("�� �����. ���������� LIMIT = %d, ����� �� ���������� = %d\n", TAP_ANGLE, timer_start_to);
            }
            event = ev_none;
            break;
        case ev_to_nf:
            alarm_reg(0, 1, get_alert_str(6), 6);
            printf("���������� �������� �� �����." );
        case ev_end_to:
            timer_start_to = 0;
            timer_stop_to = 0;
            mode.run = mo_stop;
            event = ev_none;
            prim_par.TO.status = 0;
            printf("����� �� �����." );
            break;
        case ev_alarm1:   // �����, �������� �����������, ������ ����������
            signal_red(ON); signal_buz(MEANDR);
            mode.run = mo_stop;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1; // ���������
            TAP_ANGLE = PWM_MAX;
            signal_green(OFF);
            alarm_reg(0, 1, get_alert_str(0), 0);
            printf ("������: %s\r\n", get_alert_str(0));
            event = ev_none;
            break;
        case ev_alarm2:   // ������ ������������� �� �������� �������
            signal_red(ON); signal_buz(MEANDR);
            mode.run = mo_stop;
            mode.fan = 0;
            mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            alarm_reg(0, 1, get_alert_str(1), 1);
            printf ("������: %s\r\n", get_alert_str(1));
            event = ev_none;
            break;
        case ev_freezing1:  // ����������� �� ����� ���� ����������� UL_T (���� �� ����������� � ������� ���� ��������������)
            alarm_reg(0, 1, get_alert_str(2), 2);

            signal_red(SHORT);
            signal_buz(MEANDR);
            //signal_green(SHORT);
            mode.run = mo_stop; // ����� ����������
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(2));
            event = ev_none;
            break;
        case ev_freezing2: // ����������� � ��������� ���� ����������� POM_T
            alarm_reg(0, 1, get_alert_str(3), 3);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = mo_stop;  // ����� ����������
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(3));
            event = ev_none;
            break;
        case ev_freezing3: // ����������� ���� ������� ���� ����������� POM_T
            alarm_reg(0, 1, get_alert_str(5), 5);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = mo_stop;  // ����� ����������
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(5));
            event = ev_none;
            break;
        case ev_term1_nf:   // ��������� �1 ���������
            alarm_reg(MAX_OFFLINES, termometers[0].err, get_alert_str(7), 7);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = mo_stop;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(7));
            event = ev_none;
            break;
        case ev_term2_nf:  // ��������� �2 �����
            alarm_reg(MAX_OFFLINES, termometers[1].err, get_alert_str(8), 8);
            signal_red(LONG); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = mo_stop;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(8));
            event = ev_none;
            break;
        case ev_term3_nf:  // ��������� �3 ������
            // ��� ���, �� � ���� � ���
            alarm_reg(MAX_OFFLINES, termometers[2].err, get_alert_str(9), 9);
            printf ("��������������: %s\r\n", get_alert_str(9));
            event = ev_none;
            break;
        case ev_term4_nf:  // ��������� �4 �������
            alarm_reg(MAX_OFFLINES, termometers[3].err, get_alert_str(10), 10);
            signal_red(LONG); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = mo_stop;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(10));
            event = ev_none;
            break;
        case ev_filter:  // ����������� �������.
            alarm_reg(0, 1, get_alert_str(11), 11);
            event = ev_none;
            printf ("������: %s\r\n", get_alert_str(11));
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
               TAP_ANGLE = prim_par.PWM1_lo;
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
            OCR0 = menu_value;
            break;
        case mo_setup_output2:
            OCR2 = menu_value;
            break;
        default:

    }
    // ����� ������������ ������ � ����� �������
    switch (mode.run) {
        case mo_stop:
            time_integration = 0;
            if (prim_par.season) {
                keep_life_in_winter();
            } else {
                signal_white(OFF);
                mode.pomp = 0;
            }
            signal_green(OFF);
            time_cooling = 0;
            //mode.cooling1 = 0;
            //mode.cooling2 = 0;
            break;
        case mo_action:
            signal_white(prim_par.season);
            if (prim_par.season) {
                winter_regulator();
            } else {
                coolant_regulator();
            }
            // ��� ���������� �������� ���� ������������� ��� ����������
            if (1 == 0) {
                // update_PID(SET_T - POM_T, -5000, 5000); // ������� ����� T ��� � � ���������
                time_integration = prim_par.T_int;
               //�������� ��������� ������������� �������� �����������!!!
                if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
                   winter_fan_speed();
                }
            }
            //signal_green(ON);
            break;
        default:
            break;
    };
}
// ������������ ������������� ���������� (�� �����)
void coolant_regulator (void) {
    if (prim_par.season) return;   // ������ �� ������-������������
    // ��� ��������� � ������� ����������� !
    // ������ ����������
    if (time_cooling == 0) {
        //if (POM_T > (SET_T + (prim_par. Ki*10))) {
        if (UL_T > SET_T) {
            if (POM_T > SET_T) {
                //if ( mode.cooling1 == 0) {
                //      mode.cooling1 = 1;
                //      signal_green(SHORT);
                //} else {
                //      mode.cooling2 = 1;
                //      signal_green(LONG);
                // }
                if (mode.print) printf("������� ����������.  POM_T :%d\r\n",   POM_T);
            }
        }
        //��������� ����������
        if (POM_T < (SET_T - (prim_par. Ki*10)) || (UL_T < SET_T) )  {
            //if ( mode.cooling1 == 1) {
            //     mode.cooling1 = 0;
            //     signal_green(SHORT);
            // } else {
            //     mode.cooling2 = 0;
            //     signal_green(ON);
            // }
            if (mode.print) printf("�������� ����������. �������� ���������� - ������: %d, POM_T :%d\r\n",  (SET_T - (prim_par. Ki*10)), POM_T);
        }
        time_cooling = prim_par.T_z;
        //time_cooling = TIME_COOLING_MAX;
        //mode.cooling1 = 0;
        //mode.cooling2 = 0;
        //count_cooling = 0;
    }
}
// ������������ ������������� �������� ����������� �����
void winter_fan_speed(void) {
    // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
    //tap_angle_min = ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;
    //printf("�������� �����������: %d, ��. � :%d, ����������� :%d \r\n",  (TA_IN_NOLIMIT - UL_T), UL_T, mode.k_angle_limit);
    // ���������� ������� �����������
    //#define FAN_SPEED_T_UP 300
    //#define FAN_SPEED_T_DOWN 100
    if ((TAP_ANGLE == PWM_MAX) && ((POM_T - FAN_SPEED_T_DOWN) < SET_T)) {
        count_fan++;
        if (count_fan == COUNT_FAN_MAX) {
           FAN_SPEED = FAN_SPEED - FAN_SPEED_STEP;
           count_fan = 0;
           if  (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
        //if (!mode.print) printf("��������� �������� ����������� ��������� : %d, ���������� : %d, ������� ������ :%d, POM_T :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, POM_T);
        if (mode.print) printf("��������� �������� ����������� ��������� : %d, ���������� : %d, ������� ������ :%d, POM_T :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, POM_T);
        }
    } else  {// count_fan = 0;
        if ((FAN_SPEED < prim_par.fan_speed) && ((POM_T + FAN_SPEED_T_UP) > SET_T)) {   //&& (TAP_ANGLE == tap_angle_min
            count_fan++;
            if (count_fan == COUNT_FAN_MAX) {
               FAN_SPEED = FAN_SPEED + FAN_SPEED_STEP;
               count_fan = 0;
               //if  (prim_par.fan_speed <= FAN_SPEED) FAN_SPEED = prim_par.fan_speed;
               if  (FAN_SPEED > prim_par.fan_speed ) FAN_SPEED = prim_par.fan_speed;
               //if (!mode.print) printf("���������� �������� ����������� ���������: %d, ���������� : %d, ������� ������ :%d, �������� �������� :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, prim_par.fan_speed);
               if (mode.print) printf("���������� �������� ����������� ���������: %d, ���������� : %d, ������� ������ :%d, �������� �������� :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, prim_par.fan_speed);
            }

        } else count_fan = 0;
    }
    //if (!mode.print) printf("�������� ����������� ���������: %d, ���������� : %d, �������� ��������: %d, POM_T: %d, TAP_ANGLE_MIN = %d \r\n",  FAN_SPEED, ADC_VAR2, prim_par.fan_speed, POM_T, tap_angle_min);
    if (mode.print) printf("�������� ����������� ���������: %d, ���������� : %d, �������� ��������: %d, POM_T: %d, TAP_ANGLE_MIN = %d \r\n",  FAN_SPEED, ADC_VAR2, prim_par.fan_speed, POM_T, tap_angle_min);
}
// ������������ ������������� ���������� �����
void winter_regulator (void) {
    if (!prim_par.season) return;   // ������ �� ������-������������
    //  ������� �������� ���������
    if (time_integration == 0) {
        if (POM_T < (SET_T - (prim_par. Kd*10)))  update_P(SET_T - POM_T);// ������� ����� T ��� � � ���������
        if (POM_T > (SET_T+(prim_par. Kd*10)))  update_P(SET_T - POM_T);// ������� ����� T ��� � � ���������
        time_integration = prim_par.T_int;
        //mode.cooling1 = 0;
        //mode.cooling2 = 0;
        time_cooling = 0;
        signal_green(ON);
    }
}
// ������������ ��������� ����������������� ������� ����� � ������ ��������
void keep_life_in_winter(void) {
    static int error_w_stop = 0;
    int water_out_bound;
    int water_out_bound_1;

    if (!prim_par.season) return;   // ������ �� ������-������������
   // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
    if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
        //int tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;
        //mode.k_angle_limit = (TAP_ANGLE_LIMIT / tmp_delta) * 1000;
        tap_angle_min =prim_par.tap_angle + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;   // ���������� ����������� ����� �� ����������� ������� �� ����� � ������������ mode.k_angle_limit
        if (tap_angle_min < 100) tap_angle_min = 100 ;  // �������������� ������ �������� ����� ������ 40% � ������ ����
    }
    // ������� ����������� ����������� ���������� � ������ ���� �����
    error_w_stop = (prim_par.TW_out_Stop - WOUT_T)/100;
    water_out_bound = prim_par.TW_out_Stop - 200;
    water_out_bound_1 = prim_par.TW_out_Stop - 500;
    // �������� ����������
    if ((WOUT_T <= water_out_bound) && (timer_fan == 0)) {
        timer_fan = TIME_COOL_STOP;
        // forcheck_event = ev_pomp;
        TAP_ANGLE = TAP_ANGLE + error_w_stop;
        if (TAP_ANGLE < tap_angle_min) TAP_ANGLE = tap_angle_min ;   // TAP_ANGLE - ��������� ������ �� PWM
        if (TAP_ANGLE > PWM_MAX) TAP_ANGLE = PWM_MAX;         // TAP_ANGLE - ��������� ������ �� PWM
        if (WOUT_T < water_out_bound_1) {
            //if (!mode.print) printf("�������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
            // if (mode.print) printf("�������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
            signal_white(LONG);
            mode.pomp = 1;
        } else {
            signal_white(ON);
            mode.pomp = 0;
        }
        if (mode.print) printf("�������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
    }
    // ���������� ����������
    water_out_bound = prim_par.TW_out_Stop + 250;
    water_out_bound_1 = prim_par.TW_out_Stop + 500;
    if ((WOUT_T > water_out_bound) && (timer_fan == 0)) {
        timer_fan = TIME_COOL_STOP;
        // printf("���� ����� ��������� :%d  \r\n",   TAP_ANGLE);
        if (TAP_ANGLE < tap_angle_min) TAP_ANGLE = tap_angle_min;         // TAP_ANGLE - ��������� ������ �� PWM
        if (TAP_ANGLE > PWM_MAX) TAP_ANGLE = PWM_MAX;         // TAP_ANGLE - ��������� ������ �� PWM
        if ((WOUT_T >= water_out_bound) && (mode.pomp == 1)) {
            //if (!mode.print) printf("���������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
            //if (mode.print) printf("���������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
            signal_white(ON);
            mode.pomp = 0;
        };
        if (WOUT_T >= water_out_bound_1)  TAP_ANGLE = TAP_ANGLE + error_w_stop;
        if (mode.print) printf("���������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
    }
    // ����������� ����������
    if (timer_fan == 0) {
        timer_fan = TIME_COOL_STOP;
        if (WIN_T >= prim_par.TW_out_Stop)  mode.pomp = 0;
        //if (!mode.print) printf("����������� ����������: %d,  ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
        if (mode.print) printf("����������� ����������: %d,  ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
    }
}
// �������� �� �������������� ���������
void check_range(void) {
    // ���������� ����������� �������� ����� �� ���������� 2 ������
    if  ( TAP_ANGLE < prim_par.tap_angle ) TAP_ANGLE = prim_par.tap_angle;
    // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
    if (UL_T < TA_IN_NOLIMIT)  tap_angle_min = prim_par.tap_angle + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;
    if (!mode.print) printf("�������� �����������: %d, ��. � :%d, ����������� :%i, ����������� ����� :%d, UL_T :%i  \r\n",  (TA_IN_NOLIMIT - UL_T), UL_T, mode.k_angle_limit,prim_par.tap_angle, UL_T);
    if (TAP_ANGLE < tap_angle_min)
        TAP_ANGLE = tap_angle_min;
    else
        if (TAP_ANGLE > PWM_MAX)
            TAP_ANGLE = PWM_MAX;
    // TAP_ANGLE - ��������� ������ �� PWM
}
// ������� ����������������� �������������
void update_P(int error) {
    // TAP_ANGLE = TAP_ANGLE + error/100;          // TAP_ANGLE - ��������� ������ �� PWM
    if ((TAP_ANGLE >=0) && (TAP_ANGLE <= PWM_MAX))
        TAP_ANGLE = TAP_ANGLE + ((error* prim_par.Ku)/1000);
    check_range();
    //if (mode.print == 2) printf("�������� ����������: %d, �������_ANGLE :%d, TAP_ANGLE:%d, ANGLE CALC:%d,KU:%d \r\n",  error, ((TAP_ANGLE*100)/0xFF),TAP_ANGLE,((error / 100) * prim_par.Ku),prim_par.Ku);
    if (!mode.print) printf("�������� ����������: %d, �������_ANGLE :%d, TAP_ANGLE:%d, ANGLE CALC:%d,KU:%d \r\n",  error, (((TAP_ANGLE -  prim_par.tap_angle)*100)/(PWM_MAX -  prim_par.tap_angle)),TAP_ANGLE,((error / 100) * prim_par.Ku),prim_par.Ku);
}
/*
void update_PID(int error, int iMin, int iMax) {
    static int dState = 0, iState = 0;
    int pTerm, dTerm, iTerm;
    int result;

    pTerm = (prim_par.Ku * error) / 10;    // calculate the proportional term
    iState += error;                // calculate the integral state with appropriate limiting
    // ��������� �� �������������� ���������

        if ((iState > iMax) || (iState < iMin)) {
            if (iState > iMax)
                iState = iMax;
            else
                iState = iMin;
        }
        iTerm = (prim_par.Ki * iState) / 100;    // calculate the integral term
        dTerm = prim_par.Kd * (POM_T - dState);
        dState = POM_T;
        result = pTerm + iTerm - dTerm;
        TAP_ANGLE += result / 100;
        check_range();

    if (!mode.print)
    // ����������� ��������, ����������� ����������, Delta, ���� ����� ���������, ���� ���������������, ���� �����������, result, pTerm, iTerm, dTerm, ������� ���������,������� �����������\r\n"
        printf("%02u:%02u:%02u, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n",
            s_dt.cHH, s_dt.cMM, s_dt.cSS,
            SET_T, POM_T, error,
            TAP_ANGLE, ADC_VAR1, tap_angle_min, result,
            pTerm, iTerm, dTerm,
            FAN_SPEED, ADC_VAR2);
    else
        printf("Delta (x100) = %d, result = %d, TAP_ANGLE = %d, pTerm = %d, iTerm = %d, dTerm = %d\r\n",
            error, result, TAP_ANGLE, pTerm, iTerm, dTerm, POM_T);
}
*/
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
            printf("[%s:%d:%d] ", address_line, tl, th);
        } else {
            printf(" t%-u = %i(%-i.%-u)C[%02X%02X:%02x]%d:%d; ", i+1, term, term/100, abs(term%100), spd[1], spd[0], resolution, tl, th);
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

// ��������� ����������� ������� �����
void lcd_primary_screen(void) {
    char run_mod [8];
    register unsigned char i;
    unsigned char c_alerts = 0;
    unsigned char sign = '+';
    int lcd_term = main_menu[0].val_data;
    #ifndef NODEBUG
    // printf ("%02u:%02u ������ ���������� ������...", s_dt.cMM, s_dt.cSS);
    #endif
    for (i = 0; i < MAX_ALERTS; i++) {
        // 9-� ����� (���������� 3-�� ����������) ��� �� ���������
        if (prim_par.alert_status[i] && i != 9) c_alerts++;
    }
    // �������� ��������, ��� �������� ������ �� ������ ���� ������ 5-�� ��������
    switch (mode.run) {
        case mo_stop:           sprintf(run_mod, "���� "); break;
        case mo_warming_up:     sprintf(run_mod, "�����"); break;
        case mo_warming_down:   sprintf(run_mod, "�����"); break;
        case mo_action:         sprintf(run_mod, "���� "); break;
        case mo_to:             sprintf(run_mod, "��   "); break;
        default: break;
    };
    if (c_alerts)
        sprintf(run_mod, "������ ");
    else {
        signal_red(OFF);  signal_buz(OFF); // signal_green(OFF);
    }
    lcd_command(LCD_DISP_ON);       // ������� ������ � LCD
    lcd_gotoxy(0,0);        // ������������� ������ � ������� 0 ������ ������
    sprintf(linestr, "%02u:%02u:%02u %02u.%02u %01u", s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo, s_dt.dayofweek);
    lcd_puts(linestr);
    lcd_gotoxy(0,1);                // ������������� ������ � ������� 0 ������ 2
    // ������� ���������� � �����������
    if(ds1820_devices) {
        sign = (lcd_term < 0) ? '-' : '+';
        sprintf(linestr, "t=%c%02u.%01uC %s", sign, abs(lcd_term)/100, (abs(lcd_term)%100)/10, run_mod);
    } else {
        sprintf(linestr, "��� �����������");
    }
    lcd_puts(linestr);
    #ifndef NODEBUG
    // printf ("����� � %02u:%02u\r\n", s_dt.cMM, s_dt.cSS);
    #endif
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
void ask_turn_off(void) {
    printf("��������� ������� � ���������� ��� ������������\r\n");
    while(1);
}
// ��������� ������ ���������� ������������� �� 1-� ����������.
void init_force_term(signed char number) {
    // �������� ��������� ����������
    byte inbyte = 0;
    // ������
    #asm("cli")
    printf("��������� ��� ����������, ����� [%02x] � ������� ������� SHIFT-'~'\r\n����� �� ������ - ���������� �������\r\n", number);
    do
        if (UCSRA & RX_COMPLETE) inbyte = UDR;
    while (inbyte != 0x7E);
    ds1820_devices = w1_search(0xf0, ds1820_rom_codes);
    delay_ms (DS1820_ALL_DELAY);
    printallterms(0);
    if (!ds1820_devices)
        printf("���������� �� �������\r\n");
    else
        if (ds1820_set_THTL(ds1820_rom_codes[0], number, OUR_SIGNATURE))
            printf("��������� [%02x] ������� ������������������\r\n", number);
        else
            printf("�� ���� ������������������� ����� ��������� [%02x]\r\n", number);
    ask_turn_off();
    #asm("sei")
}
void set_term(signed char number, signed char sign) {
    if (sign == OUR_SIGNATURE) {
        if (ds1820_set_THTL(ds1820_rom_codes[number], -2 - number, sign))
            printf("��������� [%02x] ������� ������������������\r\n", number);
        else
            printf("�� ���� ������������������� ��������� [%02x]\r\n", number);
    } else {
        if (ds1820_set_THTL(ds1820_rom_codes[number], 1, sign))
            printf("��������� [%02x] ������� ������\r\n", number);
        else
            printf("�� ���� �������� ��������� [%02x]\r\n", number);
    }
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
                for (i=0; i < MAX_ALERTS; i++) {
                    if (!(alarm_unreg(i))) {
                       printf (" ������� �������� �������: %s\r\n", get_alert_str(i));
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
            case 0x4a:              // ������ 'j'
                poll_keys();
                break;
            case 0x75:                // ������ 'u' ������ � ���������
                init_force_term(0xFE); break;
            case 0x69:                // ������ 'i' ������ �� �����
                init_force_term(0xFD); break;
            case 0x6F:                // ������ 'o' ����
                init_force_term(0xFC); break;
            case 0x70:                // ������ 'p'
                init_force_term(0xFB); break;
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
            case 0x3f:              /* Shift + '?'*/
                set_term(3, 1); break;
            case 0x3e:              /* Shift + '>' */
                set_term(2, 1); break;
            case 0x3c:              /* Shift + '<' */
                set_term(1, 1); break;
            case 0x4d:              /* Shift + 'M' */
                set_term(0, 1); break;
            case 0x22:              /* Shift + '"'*/
                set_term(3, OUR_SIGNATURE); break;
            case 0x3a:              /* Shift + ':' */
                set_term(2, OUR_SIGNATURE); break;
            case 0x4c:              /* Shift + 'L' */
                set_term(1, OUR_SIGNATURE); break;
            case 0x4b:              /* Shift + 'K' */
                set_term(0, OUR_SIGNATURE); break;
            case 0x39:              // ������ '9'
                signal_printallbytes();
                break;
            default:
                printf("����� ������ 0x%x\r\n", inbyte);
        };
    }
}
// ������� ����������� ������� ���������� ��, � ���� ���� - ���������� ��������� ��������
char high_time_TO(void) {
    if (prim_par.TO.weekday == s_dt.dayofweek) {
        if (prim_par.TO.hour == s_dt.cHH) {
            if (prim_par.TO.minute == s_dt.cMM) {
                if (prim_par.TO.status == 0) {
                    return (1);
                }
            }
        }
    }
    return (0);
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
void only_new_term_reset(void) {
    unsigned char size_addr = size_prim_par - sizeof(prim_par.addr);
    unsigned char term_status;

    start_screen(3);
    eeprom_read_struct ((char *)&prim_par, size_addr);
    ds1820_devices = MAX_DS1820;
    // ������������ ������� ����������� � EEPROM
    term_status = sync_ds1820_eeprom();
    printf("������ � EEPROM %u ����� �����������.\r\n", term_status);
    eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
}