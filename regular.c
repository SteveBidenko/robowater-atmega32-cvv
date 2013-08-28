#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include "robowater.h"
#include "signals.h"
#include "regular.h"
#include "menu.h"
#include "spd1820.h"
#include "dayofweek.h"
// �������� ��������� ����������
unsigned int time_integration = 0;
unsigned char timer_start_to = 0;
unsigned char timer_stop_to = 0;
unsigned int timer_start = 0;
unsigned char timer_stop = 0;
unsigned char timer_fan = 0;
unsigned char count_fan = 0;
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
        if (!menu_timer_inactive) {
            if (event)
                menu_timer_inactive++;
            else
                event = ev_timer;
            // clatsman.valcoder_mode = 0; lcd_clrscr();
        }
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
