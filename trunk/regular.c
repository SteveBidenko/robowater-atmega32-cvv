#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include "robowater.h"
#include "signals.h"
#include "regular.h"
#include "menu.h"
#include "spd1820.h"
#include "dayofweek.h"
#include "sensors.h"
#include "fan.h"
#define NODEBUG
// �������� ��������� ����������
unsigned int time_integration = 0;
unsigned char timer_start_to = 0;
unsigned char timer_stop_to = 0;
unsigned int timer_start = 0;
unsigned char timer_stop = 0;
// �������, �������� ���������� ������ �������� �����������
void regular_check_alarm_and_warnings(void) {
    // �������� ���������
    if (CHECK_EVENT && (!prim_par.warning_status[0]) && (SENSOR_ALARM1)) event = ev_alarm1; // �����, �������� �����������, ������ ����������
    if (CHECK_EVENT && (!prim_par.warning_status[1]) && (SENSOR_ALARM2)) event = ev_alarm2; // ������ ������������� �� �������� �������
    if (CHECK_EVENT && (!prim_par.warning_status[11]) && (!SENSOR_FILTER)) event = ev_filter; // ����������� �������.
    // �������� �����������
    if (CHECK_EVENT && (!prim_par.warning_status[7]) && (termometers[0].err >= MAX_OFFLINES))
        // printf("��� ���������� �1 (���������): %d, err=%d", prim_par.warning_status[7], termometers[0].err);
        event = ev_term1_nf;
    if (CHECK_EVENT && (!prim_par.warning_status[8]) && (termometers[1].err >= MAX_OFFLINES))
        // ��� ���������� � 2 - �����
        event = ev_term2_nf;
    if (CHECK_EVENT && (!prim_par.warning_status[9]) && (termometers[2].err >= MAX_OFFLINES))
        // ��� ���������� � 3 - ������
        event = ev_term3_nf;
    if (CHECK_EVENT && (!prim_par.warning_status[10]) && (termometers[3].err >= MAX_OFFLINES))
        // ��� ���������� � 4 - �������
        event = ev_term4_nf;
    // ����� ������������ ��������� ��� ��������� �������
    if ((UL_T < TA_IN_NOLIMIT) && (termometers[1].err >= MAX_OFFLINES)) {
        #ifndef NODEBUG
        if (!(prim_par.warning_status[2] || prim_par.warning_status[3] || prim_par.warning_status[7] || prim_par.warning_status[10])) {
            printf ("UL_T ����� ������ TA_IN_NOLIMIT\r\n");
        }
        #endif
        if (CHECK_EVENT &&
          !(prim_par.warning_status[8] || prim_par.warning_status[2]) &&
          (termometers[1].t < (prim_par.TA_in_Min - 5))) // ����������� �� ����� ���� �����������  �� 5 ��������.UL_T
            event = ev_freezing1;
        if (CHECK_EVENT && !(prim_par.warning_status[7] || prim_par.warning_status[3]) &&
            (termometers[0].t < prim_par.TA_out_Min) && (termometers[1].t < prim_par.TA_out_Min)) // ����������� � ��������� ���� ����������� POM_T
               // ���� ���������� �� ����� ���� ����������� POM_T
            event = ev_freezing2;
        if (CHECK_EVENT && !(prim_par.warning_status[10] || prim_par.warning_status[5]) &&
            (termometers[3].t < prim_par.TW_out_Min)) // ����������� ���� ������� ���� ����������� WOUT_T
            event = ev_freezing3;
    }

}
// �������, ������������� �������� ����������� �� ���������� ���������� 
void link_terms(void) {
    MAIN_T = read_term(0);      // ������� ���������� � ������� ���������� !!!
    POM_T = read_term(0);
    UL_T = read_term(1);
    WIN_T = read_term(2);
    WOUT_T = read_term(3); 
} 
// �������, ���������� ��� � ������� �� ������� ev_secunda
void regular_inspection(void) {
    #ifndef NODEBUG
    // printf ("%02u:%02u ������ ���������� �������...", s_dt.cMM, s_dt.cSS);
    // printf (".");
    #endif
    if (timer_fan) timer_fan--;
    if (timer_start) {
        timer_start--;
        // ���� �������� ����, �� �������� ����������
        if (!timer_start && (mode.run == mo_warming_up)) {         // event = ev_timer_Start;
             mode.fan = 1;
             #ifndef NODEBUG
             printf ("���� \r\n");
             #endif
             mode.run = mo_action;  // �������� ����� ����
             count_fan = 0;
             signal_green(ON);
        }
    }
    if (timer_stop) {
        timer_stop--;
        // ���� �������� ����, �� ��������� ����������
        if (!timer_stop && (mode.run == mo_warming_down)) {          // event = ev_timer_Stop;
            mode.run = mo_stop;
            mode.fan = 0;           // ���������� ������
            // printf ("���������� ���������� ����� ���� \r\n");
            signal_green(OFF);
        }
    }
     // ���� timer1_counter > 0, ��������� ��� ��������
    if (menu_timer_inactive) {
        menu_timer_inactive--;
        // ���� �������� ����, �� ������������� ������������ valcoder
        if (!menu_timer_inactive) menu_timer_break();
    }
    if (!mode.stop_sync_dt) get_cur_dt (0);
    read_all_terms(DUTY_MODE);
    // ���������� ���
    ADC_VAR2 = read_adc(0)/4;
    ADC_VAR1 = read_adc(1)/4;
    if (time_integration) time_integration--;
    if (time_cooling ) time_cooling-- ;
    #ifndef NODEBUG
    // printf ("����� � %02u:%02u\r\n", s_dt.cMM, s_dt.cSS);
    #endif
    s_dt.dayofweek = dayofweek(s_dt.cdd, s_dt.cmo, s_dt.cyy);
    link_terms();
    switch (mode.menu) {
        case 0: lcd_primary_screen(); break;
        // case 2: lcd_edit(0); break;
        case 1: lcd_menu(0); break;
        default: ;
    }
    // ���� ���� ��������� ��, �� ���������� ������� ev_begin_to
    if (high_time_TO()) {
        printf ("���� ��������� ��\n");
        prim_par.TO.status = 1;
        event = ev_begin_to;
    } else {
        event = ev_none;            // ������� �������
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
// ��������� ����������� ������� �����
void lcd_primary_screen(void) {
    char run_mod [8];
    register unsigned char i;
    unsigned char c_warnings = 0;
    unsigned char sign = '+';
    int lcd_term = main_menu[0].val_data;
    #ifndef NODEBUG
    // printf ("%02u:%02u ������ ���������� ������...", s_dt.cMM, s_dt.cSS);
    #endif
    for (i = 0; i < MAX_WARNINGS; i++) {
        // 9-� ����� (���������� 3-�� ����������) ��� �� ���������
        if (prim_par.warning_status[i] && i != 9) c_warnings++;
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
    if (c_warnings)
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
