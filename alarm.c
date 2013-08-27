#include <stdio.h>
#include <string.h>
#include "robowater.h"
#include "at2404.h"
#include "spd1820.h"
#include "alarm.h"
#include "bits.h"
#include "menu.h"

struct st_alarm alarm;      // ������� alarm

flash alarm_text all_warnings[MAX_WARNINGS] = {
        "����������� ",  // [0]
        "���������� ",   // [1]
        "����.����.�� ", // [2]
        "�.����.���. ",  // [3]
        "����.���� �� ", // [4]
        "�.���� ���.",   // [5]
        "��� �� ",       // [6]
        "��� ����.B1 ",  // [7]
        "��� ����.B2 ",  // [8]
        "��� ����.B3 ",  // [9]
        "��� ����.B4 ",  // [10]
        "������ "        // [11]
};
// ������� ����������� ���������� �������� warnings � ������� ��� ����
void update_warning_menu (void) {
    register unsigned char i;
    SUM_ALARMS = 0;
    for (i = 0; i < MAX_WARNINGS; i++) {
        warnings[i].can_edit = prim_par.warning_status[i];
        if (prim_par.warning_status[i]) {
            SUM_ALARMS++;
            if (warnings[i].val_data == 0) warnings[i].val_data = 1;
        }
    }
    IS_ALARM = SUM_ALARMS;
}
// ������� ����������� ALARM
void alarm_reg (int reg_preset, int reg_alarm, char *text_alarm, unsigned char code) {
    sprintf (alarm.datestamp, "%02u:%02u %02u.%02u", s_dt.cHH, s_dt.cMM, s_dt.cdd, s_dt.cmo);
    sprintf (alarm.text, "%s", text_alarm);
    alarm.val_preset = reg_preset;
    alarm.val_alarm = reg_alarm;
    // ������������ ������� � ����� ��������� prim_par
    prim_par.warning_status[code] = 1;
    warnings[code].val_data = reg_alarm;
    // ���������� ������� ��������������� alarm
    if (prim_par.c_alarm < MAX_ALARMS) prim_par.c_alarm++;
    prim_par.alarm++;
    if (prim_par.alarm > prim_par.c_alarm) prim_par.alarm = 1;
    // ������ ALARM
    eeprom_write_alarm ((char *)&alarm, sizeof(alarm), prim_par.alarm);
    // ������ ��������� ��������� (��� �������� ������� � ����� ALARM)
    eeprom_write_struct ((char *)&prim_par, sizeof(prim_par));
    update_warning_menu ();
    return;
}
// ������� ��������� ALARM
signed char alarm_unreg (unsigned char code) {
    // ���� ������� ���, ���������� ������� -1
    if (prim_par.warning_status[code] == 0) return -1;
    // ��������������� ������� � ����� ��������� prim_par
    prim_par.warning_status[code] = 0;
    // ������ ��������� ��������� (��� �������� ������� � ����� ALARM)
    eeprom_write_struct ((char *)&prim_par, sizeof(prim_par));
    update_warning_menu ();
    if (code >= 7 && code <= 10) read_all_terms(INIT_MODE);
    return;
}
// ������� ������ ALARM �� ������ �� EEPROM
signed char alarm_read (unsigned char num) {
    // ���� ������� ���, ���������� ������� -1
    if (num == 0) return -1;
    eeprom_read_alarm ((char *)&alarm, sizeof(alarm), num);
}
void alarm_c_print (void) {
    printf("> %s %i %i %s\r\n", alarm.datestamp, alarm.val_preset, alarm.val_alarm, alarm.text);
}
// ������� ���������� ��� ALARMs
void alarm_all_print (void) {
    register unsigned char i;
    unsigned char c;
    c = prim_par.alarm;
    // �������� ��� ������������������ alarms ������� � ������� � � �������� �������
    printf("��� ������:\r\n");
    for (i = 1; i <= MAX_ALARMS; i++) {
        alarm_read(c--);
        alarm_c_print();
        if (c == 0) c = prim_par.c_alarm;
    }
}
// �������, ������������ ������ �������� ������� �� ����
char *get_warning_str(unsigned char code) {
    static alarm_text warning_string;
    if (code < MAX_WARNINGS) {
        strcpyf (warning_string, all_warnings[code]);
        return warning_string;
    } else {
        return NULL;
    }
}