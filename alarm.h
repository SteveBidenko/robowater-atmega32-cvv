#ifndef __ALARMS__
#define __ALARMS__

// ALARM - ������������������ �������, ������ � �������
// ALERT - �������, ���������� ������������� ��� �������� ����������� ������������
#define MAX_ALERTS 12        // ���������� ���������������� ������� � robowater.h #define ALL_ALERTS
#define DT_LENGTH 12
#define ALARM_LENGTH 16      // ������������ ����� ������ ALARM
#define MAX_ALARMS 8

typedef char alarm_text[ALARM_LENGTH];
// �������� ��������� ALARM
struct st_alarm {
    char datestamp[DT_LENGTH];  // ��������� ����/������� ALARM
    int val_preset;             // �������� ��������
    int val_alarm;              // �������� ALARM
    alarm_text text;            // ��������� ����������� ALARM
};
// �������� ������� �������
extern void alarm_reg (int reg_preset, int reg_alarm, char *text_alarm, unsigned char);
extern signed char alarm_unreg (unsigned char);
extern void alarm_all_print (void);
extern char *get_alert_str(unsigned char);
extern void update_alert_menu (void);

#endif
