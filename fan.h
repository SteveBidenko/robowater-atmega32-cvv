#ifndef _FAN_
#define _FAN_
#include "robowater.h"
// ����������� �����������
#define FAN_SPEED_MIN prim_par.PWM2_lo     // ����������� ������ �������� ����������� 
#define FAN_SPEED_MAX prim_par.PWM2_hi     // �a���������� ������ �������� ����������� 
#define FAN_SPEED_STEP 10            // ��� ��������� �������� �����������
#define FAN_SPEED_T_UP 300
#define FAN_SPEED_T_DOWN 200
#define FAN PORTD.4    // ����.��� �����������
// �������� �������
extern unsigned char check_fan_range(int value);  
// �������� ���������� ����������
#endif

