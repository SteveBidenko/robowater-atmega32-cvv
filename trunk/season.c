#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include "robowater.h"
#include "fan.h"
#include "signals.h"
#include "season.h"
#include "menu.h"
#include "regular.h"
// �������� ��������� ����������
//unsigned int time_integration = 0;
unsigned char tap_angle_min = 0;   // ����������� ����� ����� �� �����������
// ������������ ������������� ���������� (�� �����)
void coolant_regulator (void) {
    if (prim_par.season) return;   // ������ �� ������-������������
    // ��� ��������� � ������� ����������� ��������� ����������
    if (time_cooling == 0) {
        //if (POM_T > (SET_T + (prim_par. Ki*10))) {
        if (UL_T > SET_T) {
            if (POM_T > SET_T) {
                /* if ( mode.cooling1 == 0) {
                      mode.cooling1 = 1;
                      signal_green(SHORT);
                } else {
                      mode.cooling2 = 1;
                      signal_green(LONG);
                } */
                if (mode.print) printf("������� ����������.  POM_T :%d\r\n",   POM_T);
            }
        }
        //��������� ����������
        if (POM_T < (SET_T - (prim_par. Ki*10)) || (UL_T < SET_T) )  {
            /* if (mode.cooling1 == 1) {
                 mode.cooling1 = 0;
                 signal_green(SHORT);
            } else {
                 mode.cooling2 = 0;
                 signal_green(ON);
            } */
            if (mode.print) printf("�������� ����������. �������� ���������� - ������: %d, POM_T :%d\r\n",  (SET_T - (prim_par. Ki*10)), POM_T);
        }
        time_cooling = prim_par.T_z;
        /* time_cooling = TIME_COOLING_MAX;
        mode.cooling1 = 0;
        mode.cooling2 = 0;
        count_cooling = 0; */
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
           FAN_SPEED = check_fan_range(FAN_SPEED - FAN_SPEED_STEP);
           count_fan = 0;
        if (mode.print) printf("��������� �������� ����������� ��������� : %d, ���������� : %d, ������� ������ :%d, POM_T :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, POM_T);
        }
    } else  {// count_fan = 0;
        if ((FAN_SPEED < prim_par.fan_speed) && ((POM_T + FAN_SPEED_T_UP) > SET_T)) {   //&& (TAP_ANGLE == tap_angle_min
            count_fan++;
            if (count_fan == COUNT_FAN_MAX) {
               FAN_SPEED = check_fan_range(FAN_SPEED + FAN_SPEED_STEP);
               count_fan = 0;
               if (mode.print) printf("���������� �������� ����������� ���������: %d, ���������� : %d, ������� ������ :%d, �������� �������� :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, prim_par.fan_speed);
            }

        } else count_fan = 0;
    }
    if (mode.print) printf("�������� ����������� ���������: %d, ���������� : %d, �������� ��������: %d, POM_T: %d, TAP_ANGLE_MIN = %d \r\n",  FAN_SPEED, ADC_VAR2, prim_par.fan_speed, POM_T, tap_angle_min);
}
// ������������ ��������� ����������������� ������� ����� � ������ ��������
void keep_life_in_winter(void) {
    static int error_w_stop = 0;
    int water_out_bound;
    int water_out_bound_1;

    if (!prim_par.season) return;   // ������ �� ������-������������
   // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
    if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
        // ���������������� � ����� � ���, ��� ������� PWM �������� �� ����
        // int tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;
        // mode.k_angle_limit = (TAP_ANGLE_LIMIT / tmp_delta) * 1000;
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
