#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include "robowater.h"
#include "fan.h"
#include "signals.h"
#include "season.h"
#include "menu.h"
#include "regular.h"
//#include "pid.h"            // ���� ����� ����������������, �� ����� ������������ ���������������� �������������
// �������� ����������������
// �������� ��������� ����������
//unsigned int time_integration = 0;
#ifdef PID_H
struct PID_DATA pidData;
#else
// ������� ����������������� �������������
void update_P(signed short setPoint, signed short processValue) {
    int error;
    long int coefficient;
    long int effect;
    error = setPoint - processValue;
    if ((TAP_ANGLE >= prim_par.PWM1_lo) && (TAP_ANGLE <= prim_par.PWM1_hi))
        coefficient =(long int) prim_par.Ku * (long int) (prim_par.PWM1_hi - prim_par.PWM1_lo) / 1000;
        effect = (error * coefficient) / 100;
        TAP_ANGLE = TAP_ANGLE + effect;
    //if (!mode.print) printf("�������� ����������: %d, �������_ANGLE :%d, TAP_ANGLE:%d, effect:%d, coefficient:%d \r\n",  error, (((TAP_ANGLE -  prim_par.PWM1_lo)*100)/(prim_par.PWM1_hi -  prim_par.PWM1_lo)),TAP_ANGLE,effect,coefficient);
}
#endif
// ������������ ������������� ���������� (�� �����)
void coolant_regulator (void) {
    if (prim_par.season) return;   // ������ �� ������-������������
    // ��� ��������� � ������� ����������� ��������� ����������
    if (time_cooling == 0) {
        // if (POM_T > SET_T + prim_par.dt_summer) {
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
        if (POM_T < (SET_T - prim_par.dt_summer) || UL_T < SET_T) {
            /* if (mode.cooling1 == 1) {
                 mode.cooling1 = 0;
                 signal_green(SHORT);
            } else {
                 mode.cooling2 = 0;
                 signal_green(ON);
            } */
            if (mode.print) printf("�������� ����������. �������� ���������� - ������: %d, POM_T :%d\r\n",  (SET_T - prim_par.dt_summer), POM_T);
        }
        time_cooling = prim_par.T_z;
        /* time_cooling = TIME_COOLING_MAX;
        mode.cooling1 = 0;
        mode.cooling2 = 0;
        count_cooling = 0; */
    }
}
// ������������ ��������� ����������������� ������� ����� � ������ ��������
void keep_life_in_winter(void) {
    static int error_w_stop = 0;
    int water_out_bound;
    int water_out_bound_1;

    if (prim_par.season) tap_angle_check_range(DUTY_MODE); else return; // ������ �� ������-������������
    // ������� ����������� ����������� ���������� � ������ ���� �����
    error_w_stop = (prim_par.TW_out_Stop - WOUT_T)/100;
    water_out_bound = prim_par.TW_out_Stop - 200;
    water_out_bound_1 = prim_par.TW_out_Stop - 500;
    // �������� ����������
    if ((WOUT_T <= water_out_bound) && (timer_fan == 0)) {
        timer_fan = TIME_COOL_STOP;
        // forcheck_event = ev_pomp;
        TAP_ANGLE = TAP_ANGLE + error_w_stop;
        tap_angle_check_range(DUTY_MODE);
        if (WOUT_T < water_out_bound_1) {
            // if (mode.print) printf("�������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
            signal_white(LONG);
            mode.pomp = 1;
        } else {
            signal_white(ON);
            mode.pomp = 0;
        }
        if (mode.print) printf("�������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, WOUT_T/10);
    }
    // ���������� ����������
    water_out_bound = prim_par.TW_out_Stop + 250;
    water_out_bound_1 = prim_par.TW_out_Stop + 500;
    if ((WOUT_T > water_out_bound) && (timer_fan == 0)) {
        timer_fan = TIME_COOL_STOP;
        tap_angle_check_range(DUTY_MODE);
        if ((WOUT_T >= water_out_bound) && (mode.pomp == 1)) {
            //if (mode.print) printf("���������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, WOUT_T/10);
            signal_white(ON);
            mode.pomp = 0;
        };
        if (WOUT_T >= water_out_bound_1)  TAP_ANGLE = TAP_ANGLE + error_w_stop;
        if (mode.print) printf("���������� ����������: %d, ���� ����� ��������� :%d, ���� ����� ���������� :%d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, WOUT_T/10);
    }
    // ����������� ����������
    if (timer_fan == 0) {
        timer_fan = TIME_COOL_STOP;
        if (WIN_T >= prim_par.TW_out_Stop)  mode.pomp = 0;
        if (mode.print) printf("����������� ����������: %d,  ���� ����� ��������� :%d, ���� ����� ���������� :%d, ���� �����������: %d, t ������� :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, WOUT_T/10);
    }
}
// ������������ ������������� ���������� �����
void winter_regulator (void) {
    if (!prim_par.season) return;   // ������ �� ������-������������
    //  ������� �������� ���������
    if (time_integration == 0) {
        #ifdef PID_H
        TAP_ANGLE = pid_Controller(SET_T, POM_T, &pidData);
        #else
        if (abs(SET_T - POM_T) > prim_par.dt_winter) update_P(SET_T, POM_T);
        #endif  
        tap_angle_check_range(DUTY_MODE);
        time_integration = prim_par.T_int;
        // mode.cooling2 = mode.cooling1 = 0;
        time_cooling = 0;
        signal_green(ON);
    }
}
// �������� TAP_ANGLE �� �������������� ���������
void tap_angle_check_range(unsigned char check_mode) {
    int tmp_delta;
    unsigned char tap_angle_min = 0;   // ����������� ����� ����� �� �����������
    tmp_delta = check_mode;             // ������� �������������� �����������, ���� ���������� �-������������� 
    #ifdef PID_H
    if (check_mode == INIT_MODE) pid_Init(prim_par.Ku * SCALING_FACTOR, prim_par.Ki * SCALING_FACTOR , prim_par.Kd * SCALING_FACTOR , &pidData); 
    #endif
    tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;  // ���������� ��������� ������ ������������ ����� �� �����������
    mode.k_angle_limit = ((TAP_ANGLE_LIMIT * 1000) / tmp_delta); // ���������� ������������ ����������� ����� ��� �������� ��������   
    // ���������� ����������� �������� ����� TAP_ANGLE = tap_angle_min
    if (UL_T < TA_IN_NOLIMIT) {
        // ���������� ����������� ����� �� ����������� ������� �� ����� � ������������ mode.k_angle_limit
        tap_angle_min = prim_par.PWM1_lo + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit)) / 1000;
        if (mode.print) printf(" tap_angle_min %u .   K���������� :%d\r\n",  tap_angle_min, mode.k_angle_limit);
    }
    // ���������� ����������� �������� ����� �� ���������� 2 ������
    if (TAP_ANGLE <= prim_par.PWM1_lo) TAP_ANGLE = prim_par.PWM1_lo;
    if (mode.print) printf("�������� �����������: %d, ��. � :%d, ����������� :%i, ����������� ����� :%d, UL_T :%i  \r\n",  (TA_IN_NOLIMIT - UL_T), UL_T, mode.k_angle_limit, tap_angle_min, UL_T);
    if (TAP_ANGLE < tap_angle_min)
        TAP_ANGLE = tap_angle_min;
    else
        if (TAP_ANGLE >= prim_par.PWM1_hi) TAP_ANGLE = prim_par.PWM1_hi;
    if (mode.print) printf(" ��������� ����� %u .prim_par.PWM1_lo :%d\r\n", TAP_ANGLE,  prim_par.PWM1_lo);
}
