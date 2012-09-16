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
// ��������� ����������������
#define MAJOR_VERSION 4
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
struct st_mode mode = {0, 0, 0, 0, 0, 0, 1, 0.0,0};  // ������� ����� ������
struct st_datetime s_dt;
// ��������� ��������� ��������� �������� ����������
struct st_eeprom_par prim_par={
    {{(int)0, 0}, {(int)0, 0}, {(int)0, 0}, {(int)0, 0}},
    0xFF, 205, 0x7F, 0x7F,
    (int)10, (int)0, (int)0,
    (int)180, (int)100,
    (int)1600, (int)5000, 
    (int)-2000, (int)1500, (int)2200,
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    1,  // ����
    0,  // ������� �������� alarm � EEPROM
    0,   // ������� ����� ���������������� � EEPROM
    MAX_DS1820, // ������� ������ ���� ����������� � �������
   
    /*{{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE}, // 28	4D	52	7E 	3	0	0	6E	1	FF	FE ������ ���������
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD}, // 28	D3	8	7E	3	0	0	E4	1	FF	FD   ������ �����
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC}, // 28	8C	38	7E	3	0	0	6A	1	FF	FC   ���� ������
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB}} // 28	91	9	7E	3	0	0	32	1	FF	FB   ���� �������
    */ 
     /*  ������� ������������� �1
     {{0x28,0x29,0x32,0x7e,0x03,0x00,0x00,0x60,0x01}, // 28	29	32	7E 	3	0	0	60	1	FF	FE
     {0x28,0x13,0x3b,0x7e,0x03,0x00,0x00,0xb1,0x01}, // 28	13	3B	7E	3	0	0	B1	1	FF	FD
     {0x28,0x1e,0x2f,0x7e,0x03,0x00,0x00,0x98,0x01}, // 28	1E	2F	7E	3	0	0	98	1	FF	FC
     {0x28,0x39,0x46,0x7e,0x03,0x00,0x00,0x49,0x01}} // 28	39	46	7E	3	0	0	49	1	FF	FB
    */ 
     //������� ������������� �2
     {{0x28,0x95,0x0f,0x7e,0x03,0x00,0x00,0x72,0x01}, // 28	95	F	7E 	3	0	0	72	1	FF	FE
     {0x28,0xc1,0x1a,0x7e,0x03,0x00,0x00,0x2e,0x01}, // 28	C1	1A	7E	3	0	0	2E	1	FF	FD
     {0x28,0x1a,0x17,0x7e,0x03,0x00,0x00,0xfe,0x01}, // 28	1A	17	7E	3	0	0	FE	1	FF	FC
     {0x28,0xe8,0x2a,0x7e,0x03,0x00,0x00,0x8a,0x01}} // 28	E8	2A	7E	3	0	0	8A	1	FF	FB
    
     /*  ������� ������������� �3
     {{0x28,0x50,0x32,0x7e,0x03,0x00,0x00,0x6e,0x01}, // 28	50	32	7E 	3	0	0	6E	1	FF	FE
     {0x28,0x3e,0x1c,0x7e,0x03,0x00,0x00,0xe4,0x01}, // 28	3E	1C	7E	3	0	0	E4	1	FF	FD
     {0x28,0xb7,0x0d,0x7e,0x03,0x00,0x00,0x29,0x01}, // 28	B7	D	7E	3	0	0	29	1	1	FF	FC
     {0x28,0x2c,0x0c,0x7e,0x03,0x00,0x00,0xad,0x01}} // 28	2C	C	7E	3	0	0	AD	1	FF	FB
     */
    /* ������� ������������� �4
    {{0x28,0x4d,0x52,0x7e,0x03,0x00,0x00,0x6e,0x01}, // 28	4D	52	7E 	3	0	0	6E	1	FF	FE
     {0x28,0xd3,0x08,0x7e,0x03,0x00,0x00,0xe4,0x01}, // 28	D3	8	7E	3	0	0	E4	1	FF	FD
     {0x28,0x8c,0x38,0x7e,0x03,0x00,0x00,0x6a,0x01}, // 28	8C	38	7E	3	0	0	6A	1	FF	FC
     {0x28,0x91,0x09,0x7e,0x03,0x00,0x00,0x32,0x01}} // 28	91	9	7E	3	0	0	32	1	FF	FB
    */
     
    
    /*    50 �������� ����������.
    {{0x28,0x1b,0xc4,0x24,0x00,0x00,0x00,0xfe,0x01}, // 28	1b	c4	24	0	0	0	fe	1	FF	FE
     {0x28,0xbb,0x0a,0x95,0x03,0x00,0x00,0xd8,0x01}, // 28	bb	a	95	3	0	0	d8	1	FF	FD
     {0x22,0x8e,0xe1,0x03,0x00,0x00,0x00,0xf3,0x01}, // 22	8e	e1	3	0	0	0	f3	1	FF	FC
     {0x22,0x2b,0x08,0x04,0x00,0x00,0x00,0xeb,0x01}} // 22	2b	8	4	0	0	0	eb	1	FF	FB
    */
    
};
unsigned int time_integration=0;
int tmp_delta;
byte tap_angle_min = 0;   // ����������� ����� ����� �� �����������
// byte fan_speed = 0;  // �������� �����������
enum en_event event;                          // ������� ������� � �������
// �������� �������
void printallterms(void); void lcd_primary_screen(void);
void print_prim_par(unsigned char *, unsigned char);
// ������� ��������� �����
void check_serial(void);
void check_peripheral(void);
void event_processing(void);
void mode_processing(void);
void update_P(int);
void update_PID(int error, int iMin, int iMax);
void init_new_terms(unsigned char);
// �������� ���������
void main(void) {
    // register byte i;
    byte size_prim_par;
    
    init();                  // ������������� ���� ���������
    #asm("sei")             // Global enable interrupts
    printf ("������������� ������...\r\n"); 
    init_keys();
    printf("����� ������� ������������� �2_1. %u.%02u. ������� %u �����������.\r\n", MAJOR_VERSION, MINOR_VERSION, ds1820_devices);
    // ��������� � EEPROM ��������� prim_par
    // ppr_par = &prim_par;
    size_prim_par = sizeof(prim_par);
    // ����� ���������� � EEPROM � ��������� ����������
    if (!KEY_CANCEL && !KEY_ENTER) {
        unsigned char term_status;
        ds1820_devices = MAX_DS1820;
        // ������������ ������� ����������� � EEPROM
        term_status = sync_ds1820_eeprom();
        eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
        // ��������� ������ ������������
        printf("������ � EEPROM ��������� ���������.\r\n ������������ ����������� = %u\r\n", term_status);
    } else {
        unsigned char size_addr = size_prim_par - sizeof(prim_par.addr);
        if (!KEY_CANCEL) {
            ds1820_devices = MAX_DS1820;
            printf("������ � EEPROM ��������� �� ���������.\r\n");
            eeprom_write_struct ((unsigned char *)&prim_par, size_addr);
        }
        if (!KEY_ENTER) {
            unsigned char term_status;
            eeprom_read_struct ((char *)&prim_par, size_addr);
            ds1820_devices = MAX_DS1820;
            // ������������ ������� ����������� � EEPROM
            term_status = sync_ds1820_eeprom();
            printf("������ � EEPROM %u ����� �����������.\r\n", term_status);
            eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
        }
    }
    // ��������������� �� EEPROM ��������� prim_par
    eeprom_read_struct ((char *)&prim_par, size_prim_par);
    // printf("\r\n");
    read_all_terms(INIT_MODE);
    // print_all_menu();       // ������� �� ���������� ������� ��� ������ ����
    sync_set_par(SYNC_TO_MENU); // �������������� ���� � ����������� �����������
    update_alert_menu();        // ��������� ���� alerts
    //printf("����� ������ �� EEPROM �������� Tw2=%u\r\n", prim_par.Tw2);
    // printf("���� %u, ����� ����� ������ �� EEPROM size=%u\r\n", sizeof(prim_par), size_prim_par);
    #ifndef NODEBUG
    // ���� ���� ����������, �� ������� �� ��������
    printallterms();
    #endif
    lcd_primary_screen();       // ������� ��������� �������� �� ��������
    // if (PINC.6) PORTD |= (1<<4); else PORTD &= ~(1<<4);
    // if (PINC.7) PORTD &= ~(1<<5); else PORTD |= (1<<5);
    // if (PINC.5) PORTD &= ~(1<<5); else PORTD |= (1<<5);
    if (prim_par.season) signal_white(ON); else signal_white(OFF);
    FAN_SPEED = prim_par.fan_speed;
    //fan_speed = FAN_SPEED;
    //if (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
    tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;  // ���������� ��������� ������ ������������ ����� �� �����������
    mode.k_angle_limit = (TAP_ANGLE_LIMIT / tmp_delta) * 1000; // ���������� ������������ ����������� ����� ��� �������� ��������                    
    if (UL_T < TA_IN_NOLIMIT) {    // ���������� ���� ����������� (UL_T < TA_IN_NOLIMIT)
        tap_angle_min = ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;   // ���������� ����������� ����� �� ����������� ������� �� ����� � ������������ mode.k_angle_limit                     
    };     
    if (TAP_ANGLE < tap_angle_min) TAP_ANGLE = tap_angle_min;
       else TAP_ANGLE = prim_par.tap_angle;     
    printf("��������� �����������. %u. ��������� ����� %u .\r\n", FAN_SPEED, TAP_ANGLE);
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
        // printf ("������������� �������� (%i)...\r\n", valcoder);
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
            (termometers[0].t < prim_par.TA_out_Min)) // ����������� � ��������� ���� ����������� POM_T
            event = ev_freezing2;
        if (CHECK_EVENT && !(prim_par.alert_status[10] || prim_par.alert_status[5]) && 
            (termometers[3].t < prim_par.TW_out_Min)) // ����������� ���� ������� ���� ����������� WOUT_T
            event = ev_freezing3;
    }    
    if (CHECK_EVENT && (mode.initrun)) {
        mode.initrun -= 4;
        if ((mode.initrun) && (IS_ALERT == 0)) event = ev_start;         
       else
            event = ev_stop;
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
            MAIN_T = read_term(0);       // ������� ���������� � ������� ���������� !!!
            switch (mode.menu) {
                case 0: lcd_primary_screen(); break;
                case 1: lcd_menu(0); break;
                default: ;
                // case 2: lcd_edit(0); break;
            }
            //if (!mode.print && (mode.run ==1)) printf("-");
            if (mode.print && (mode.run ==1)) printf("-");
            if (mode.print && (mode.run ==3)) printf ("�� ���������� ��������� ����: %u\r\n", time_integration);
            if (mode.print && (mode.run ==0)) printf ("�� ���������� ��������� ����: %u\r\n", timer_fan);
            event = ev_none;            // ������� �������
            break;
        case ev_left:                   // printf ("������������ ��������� valcoder �����\r\n");
        case ev_right:                  // printf ("������������ ��������� valcoder ������\r\n");
            // ��������� ������ ������������
            //timer1_valcoder = TIMER_INACTIVE;
            timer1_valcoder = TIMER_INACTIVE; // prim_par.T_z
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
            timer1_valcoder = TIMER_INACTIVE;     // prim_par.T_z ��������� ������ ������������
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
            if (mode.menu) timer1_valcoder = TIMER_INACTIVE; // prim_par.T_z.....TIMER_INACTIVE;
        case ev_cancel:
            if (event == ev_cancel) {
                signal_buz(SHORT);
                key_treated[3]=1;
                timer1_valcoder = TIMER_INACTIVE;     //prim_par.T_z  ....��������� ������ ������������ ()    
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
                case 0: { // ������� ������ �����������
                    if (prim_par.season) {
                        signal_white(ON);
                        mode.pomp = 1;
                        mode.run = 1;                // ������������� ����� �������
                        timer_start = prim_par.T_z; // TIMER_INACTIVE  ...��������� ������ STRT
                        TAP_ANGLE = PWM_MAX;
                        printf("������� ����� �������. LIMIT = %d, ����� �������� = %d\r\n", mode.k_angle_limit,prim_par.T_z);
                        signal_green(SHORT);
                    } else {
                        signal_white(OFF);
                        mode.pomp = 0;
                        mode.run = 3;
                        mode.fan = 1;
                        signal_green(ON);
                        printf("������� ����� ����\r\n");
                    };   
                    FAN_SPEED = prim_par.fan_speed;
                    if (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
                    count_fan = 0; 
                    time_integration = 0;
                    signal_buz(LONG); 
                    break;
                } //mode.run=0 ;
                case 1: signal_buz(SHORT); signal_green(SHORT); printf("����� �������.\r\n"); 
                    break; // mode.run=1;
                case 2: signal_buz(SHORT); signal_green(LONG); printf("����� ���������.\r\n"); 
                    break; // mode.run=2 ;
                case 3: signal_buz(SHORT); signal_green(ON); printf("����� ����.\r\n");
                    break; // mode.run=3
            default: break;
            };  
            event = ev_none;
            break;
        case ev_stop:
            key_treated[0]=1;
            printf ("������ ������ ����. ");
            switch (mode.run) {
                case 0: signal_buz(SHORT); time_integration = 0; signal_green(OFF); printf(" ����� ����.\r\n"); break; //mode.run=0 ;
                case 1: mode.run = 2;
                        timer_start = 0;
                        signal_green(SHORT);  signal_buz(LONG);
                        printf ("������� ����� ���������\r\n");
                        timer_stop = TIME_STOP;    // ��������� ������ STOP
                        time_integration = 0; 
                    break; //mode.run=1;
                case 2: signal_buz(SHORT); time_integration = 0; printf("����� ���������.\r\n"); break; //mode.run=2 ;
                case 3: mode.run = 2; 
                        signal_green(LONG); signal_buz(LONG);
                        printf ("������� ����� ���������\r\n");
                        timer_stop = TIME_STOP;    // ��������� ������ STOP
                        time_integration = 0; break; //mode.run=3
                default: break;
            }
            mode.fan = 0;  // ���������� �����������                  
            event = ev_none;
            break;
        case ev_alarm1:   // �����, �������� �����������, ������ ����������
            signal_red(ON); signal_buz(MEANDR);  
            mode.run = 0;
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
            mode.run = 0;
            mode.fan = 0;
            mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            alarm_reg(0, 1, get_alert_str(1), 1);
            printf ("������: %s\r\n", get_alert_str(1));
            event = ev_none;
            break;
        case ev_freezing1:  // ����������� �� ����� ���� ����������� UL_T (���� �� ����������� � ������� ���� ��������������)
            alarm_reg(0, 1, get_alert_str(2), 2);
            /* 
            signal_red(SHORT); signal_buz(MEANDR);
            signal_green(SHORT);
            mode.run = 0; // ����� ����������
            mode.fan = 0; 
            */
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("������: %s\r\n", get_alert_str(2));
            event = ev_none;
            break;
        case ev_freezing2: // ����������� � ��������� ���� ����������� POM_T
            alarm_reg(0, 1, get_alert_str(3), 3);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = 0;  // ����� ����������
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
            mode.run = 0;  // ����� ����������
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
            mode.run = 0;
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
            mode.run = 0;
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
            mode.run = 0;
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
    int water_out_bound;
    int water_out_bound_1;
    static int error_w_stop = 0;
    
    SET_T = prim_par.TA_out_prs;
    if (mode.run == ENGINEERING) {
    } else {
        OCR0 = (unsigned char)TAP_ANGLE;
        OCR2 = (unsigned char)FAN_SPEED; 
                    
        MOTOR = mode.fan;
        FAN_VAR = mode.fan;
        POMP = mode.pomp;
        POMP_VAR = mode.pomp;

        switch (mode.run) {
            case 0:
                time_integration = 0;
                if (prim_par.season) {
                // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
                    if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
                        //int tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT; 
                        //mode.k_angle_limit = (TAP_ANGLE_LIMIT / tmp_delta) * 1000;
                        tap_angle_min = ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;   // ���������� ����������� ����� �� ����������� ������� �� ����� � ������������ mode.k_angle_limit
                        if (tap_angle_min < 100) tap_angle_min = 100 ; 
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
                } else {
                    signal_white(OFF);
                    mode.pomp = 0;
                             
                }
                signal_green(OFF);   
                break;
            //case 1:
            //         if (!mode.print) printf("-");
            //    break;
            case 3:
                //if (IS_ALERT == 0) {
                    if (prim_par.season) 
                        signal_white(ON);  
                    else { 
                        signal_white(OFF);
                        FAN_SPEED = prim_par.fan_speed;
                    }
                    //  ������� �������� ���������
                    if (1 == 0) {
                        update_P(SET_T - POM_T); // ������� ����� T ��� � � ���������
                        time_integration = prim_par.T_int;
                    } 
                    if (time_integration == 0) {
                        update_PID(SET_T - POM_T, -5000, 5000); // ������� ����� T ��� � � ���������
                        time_integration = prim_par.T_int;
                        if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
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
                    } 
                    signal_green(ON);
                // } 
                break;
            default:
                break;
        };
    }
}
void check_range(void) {
    // ��������� �� �������������� ���������
    // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
    if (UL_T < TA_IN_NOLIMIT)  tap_angle_min = ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;
    //printf("�������� �����������: %d, ��. � :%d, ����������� :%d \r\n",  (TA_IN_NOLIMIT - UL_T), UL_T, mode.k_angle_limit);
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
        TAP_ANGLE = TAP_ANGLE + (error / 100) * prim_par.Ku;
    check_range();
    printf("�������� ����������: %d, TAP_ANGLE :%d, time_integration :%d \r\n",  error, ((TAP_ANGLE*100)/0xFF),time_integration);
}
void update_PID(int error, int iMin, int iMax) {
    static int dState = 0, iState = 0;
    int pTerm, dTerm, iTerm;
    int result;
    
    pTerm = (prim_par.Ku * error) / 10;    // calculate the proportional term
    iState += error;                // calculate the integral state with appropriate limiting
    // ��������� �� �������������� ���������
    if (FAN_SPEED >= prim_par.fan_speed) {
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
    }
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
// ������ ���� �����������
void printallterms(void) {
    int term;
    register byte i;
    unsigned char resolution;
    signed char th, tl;
    unsigned char *spd;

    if(!ds1820_devices) return;			// ���� ���������� �� ���������� - ������ ������� �� �������
    spd = ds1820_show_spd();         
    // printf("\t");						// �������� ���� ��������� � ���������
    for(i=0; i<ds1820_devices; i++) {
        ds1820_get_resolution(&ds1820_rom_codes[i][0], &resolution);
    	term = ds1820_temperature(&ds1820_rom_codes[i][0]);
        tl = th = 0;
        ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
        printf(" t%-u = %i(%-i.%-u)C[%02X%02X:%02x]%d:%d; ", i+1, term, term/100, abs(term%100), spd[1], spd[0], resolution, tl, th);
    }
    printf("\r\n");
}
// ������ ����������� (�������� - ����� ���������� ������� � 0)
int read_term(byte num) {
    // float lt;
    // lt = termometers[num].scale / 127;
    // lt = (float)prim_par.elims[num].scale / 128 + 1;
    // lt = (termometers[num].t * lt) + prim_par.elims[num].shift;
    return termometers[num].t;
};
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
    switch (mode.run) {
        case 0: sprintf(run_mod, "����   "); break; //mode.run=0 ;
        case 1: sprintf(run_mod, "�������"); break; //mode.run=1;
        case 2: sprintf(run_mod, "�������"); break; //mode.run=2 ;
        case 3: sprintf(run_mod, "����   "); break; //mode.run=3
        default: break;
    };
    if (c_alerts)
        sprintf(run_mod, "������ ");
    else {
        signal_red(OFF);  signal_buz(OFF); // signal_green(OFF);
    }
    lcd_command(LCD_DISP_ON);       // ������� ������ � LCD
    lcd_gotoxy(0,0);        // ������������� ������ � ������� 0 ������ ������
    sprintf(linestr, "%02u:%02u:%02u %02u.%02u  ", s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo);
    lcd_puts(linestr);
    lcd_gotoxy(0,1);                // ������������� ������ � ������� 0 ������ 2
    // ������� ���������� � �����������
    if(ds1820_devices) {
        sign = (lcd_term < 0) ? '-' : '+';
        sprintf(linestr, "t=%c%02u.%01uC %s", sign, abs(lcd_term)/100, (abs(lcd_term)%100)/10, run_mod);
        // linestr += "panarin";
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
    printf("��������� prim_par [%d bytes]: ", size);
    for (i=0; i<size; i++) {
        s_byte = *struct_data++;
        if (i < (size - 36)) 
            printf(" %d", s_byte);
        else 
            printf(" %X", s_byte);
        //
    }
    printf("\r\n");
}
// ������� ���������������� ����� ����������. !!! ���� ����� �����������.
void init_new_terms(unsigned char number) {
    register unsigned char i;
    unsigned char is_found = 0, is_new = 0;
    signed char th, tl, index;
    unsigned char newterms;
    // ���� �� ����� ��� ����������
    newterms = w1_search(0xf0, ds1820_rom_codes);
    delay_ms (DS1820_ALL_DELAY);
    printallterms();
    // ���������� TH, TL. ����� ������� ��������� ������.
    for(i = 0; i < newterms; i++) {
        ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
        index = (th == OUR_SIGNATURE) ? abs(tl) - 2 : -1;
        // ��������� �� ���������� � number
        if (!is_found) is_found = (index == number) ? i + 1 : 0;
        // ��������� �����������
        if (!is_new) is_new = (index == -1) ? 0 : i + 1;
    }
    #ifndef NODEBUG
    printf ("����� N%u (number), ����� N%u (is_found), ��������� N%u (is_new)\r\n", number, is_found, is_new);
    #endif
    // ���� ����, �� ������� �� ������� � �������������� ����������
    if (is_found) {
        printf ("��������� N%u ���������� �� ����������� ������ %u\r\n", number, is_found);
        return;
    }
    // ���� ��� ���������, ������� � �������������� ����������
    if (!is_new) {
        printf ("��������� ����������� ���\r\n");
        return;
    }
    // ���� �����, �� ����������� TH, TL
    tl = -2 - number; i = is_new - 1;
    #ifndef NODEBUG
    printf ("����� TL = %d[%02x] �� ������ %u\r\n", tl, tl, i);
    #endif
    if (ds1820_set_alarm(&ds1820_rom_codes[i][0], tl, OUR_SIGNATURE)) {
        printf ("������������������� ��������� N%u �� ����������� ������ %u\r\n", number, is_new);
        delay_ms (DS1820_ALL_DELAY);
        // ��������� ������������
        #asm
            JMP  __RESET
        #endasm
    }
    printf ("�� ���� ������������������� ����� ��������� N%u\r\n", number);
    return;
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
    printallterms();
    if (!ds1820_devices)
        printf("���������� �� �������\r\n");
    else
        if (ds1820_set_alarm(&ds1820_rom_codes[0][0], number, OUR_SIGNATURE))
            printf("��������� [%02x] ������� ������������������\r\n", number);
        else
            printf("�� ���� ������������������� ����� ��������� [%02x]\r\n", number);
    ask_turn_off();
    #asm("sei")
}
void set_term(signed char number, signed char sign) {
    if (sign == OUR_SIGNATURE) {
        if (ds1820_set_alarm(&ds1820_rom_codes[number][0], -2 - number, sign))
            printf("��������� [%02x] ������� ������������������\r\n", number);
        else
            printf("�� ���� ������������������� ��������� [%02x]\r\n", number);
    } else { 
        if (ds1820_set_alarm(&ds1820_rom_codes[number][0], 1, sign))
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
// ������������ ������
void toggle_print(void) {
    mode.print = !mode.print;
    if (mode.print)
        printf("������ ��������\r\n"); 
        //printf("������ ���������\r\n �����, ����������� ��������, ����������� ����������, Delta, ���� ����� ���������, ���� ���������������, ���� �����������, result, pTerm, iTerm, dTerm, ������� ���������, ������� �����������\r\n");
    else
        printf("������ ���������\r\n �����, ����������� ��������, ����������� ����������, Delta, ���� ����� ���������, ���� ���������������, ���� �����������, result, pTerm, iTerm, dTerm, ������� ���������, ������� �����������\r\n");
        //printf("������ ��������\r\n");
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
                printallterms();
                break;
            case 0x73:  /* 's' */     // ������������ �����
                toggle_sound(); break;   
            case 0x77:  /* 'w' */     // ������ 
                printf ("C������� = %u\r\n", (unsigned char)event);
                break;
            case 0x78:  // ������ 'x' 
                toggle_print(); break;
                // print_curr_menu(); break; // �������� ������� ����                                            
            case 0x65:  // ������ 'e'
                alarm_all_print();
                break;
            case 0x64:  // ������ 'd'
                for (i=0; i < MAX_ALERTS; i++) {
                    if (!(alarm_unreg(i))) {
                       printf ("������� �������� �������: %s\r\n", get_alert_str(i));
                    }
                }
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
                printf("���� �������� ����������� ����������� 0x69 0xFE - ��������� � ���������\r\n");
                init_new_terms(0);   // ������� ����������������������� ���������� � ������� ��������� ����������� 0x69 0xFE
                break;
            case 0x31:              // ������ '1'
                printf("���� �������� ����������� ����������� 0x69 0xFD - ��������� �� �����\r\n");
                init_new_terms(1);
                break;
            case 0x32:              // ������ '2'
                printf("���� �������� ����������� ����������� 0x69 0xFC - ��������� �� ������ ����\r\n");
                init_new_terms(2);
                break;
            case 0x33:              // ������ '3'
                printf("���� �������� ����������� ����������� 0x69 0xFB - ��������� �� ������� ����\r\n");
                init_new_terms(3);
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