#ifndef _SEASON_
#define _SEASON_
// ����������� �����������

// �������� �������
extern void coolant_regulator(void);   // ������������ ������������� ���������� (�� �����)
extern void winter_fan_speed(void);    // ������������� ����������� �����
extern void keep_life_in_winter(void); // ��������� ����������������� ������� �����
extern void winter_regulator(void);    // ������������ ������������� ���������� �����
// �������� ���������� ����������
//extern unsigned int time_integration;
extern unsigned char tap_angle_min;
#endif
