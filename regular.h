#ifndef _REGULAR_
#define _REGULAR_
// ����������� �����������

// �������� �������
extern void regular_inspection(void);   // �������, ��������� ����������� �� ������� �������. ����� ������� - �� ������������.
extern void lcd_primary_screen(void);
extern char high_time_TO(void);        // �������, ����������� ������������� ���������� ��
// �������� ���������� ����������
extern unsigned int time_integration;
extern unsigned int timer_start;
extern byte timer_stop;
extern byte timer_fan;
extern byte count_fan;
extern int time_cooling;
extern unsigned char timer_start_to;
extern unsigned char timer_stop_to;
#endif
