#ifndef __ROBOWATERMENU__
#define __ROBOWATERMENU__
#include "lcd_4bit.h"
#include "alarm.h"
// ����������� ����������������
#define NUM_MENU        7
#define NUM_PARAMETERS  11
#define NUM_SETTINGS    25
#define NUM_DT 7

#define SYNC_TO_MENU 1
#define SYNC_FROM_MENU 0

#define MAIN_T main_menu[0].val_data
#define IS_ALERT main_menu[5].can_edit
#define KOL_ALERT main_menu[5].val_data
#define SET_T parameters[0].val_data
#define POM_T parameters[1].val_data
#define UL_T parameters[2].val_data
#define WIN_T parameters[3].val_data
#define WOUT_T parameters[4].val_data
#define ADC_VAR1 parameters[5].val_data
#define TAP_ANGLE parameters[6].val_data
#define ADC_VAR2 parameters[7].val_data
#define FAN_SPEED parameters[8].val_data
#define FAN_VAR parameters[9].val_data
#define POMP_VAR parameters[10].val_data
#define SEASON sdt[5].val_data
#define SEASON_MAIN main_menu[2].val_data
#define SETTINGS_OPEN main_menu[6].can_edit
// #define mode.run main_menu[1].val_data

// �������� �����
typedef char lcd_str[LCD_DISP_LENGTH];        // ������ �� 16 �������� ��� ������ LCD
// �������� ������������
enum en_type {
    e_empty = 0,              // ������ ���. ������ �� ��������� ��� ������
    e_clatsman,               // ������������� (bit) ���./����.
    e_winter,                 // ������������� ����/����
    e_isfour,                 // ������� ����, ��� �������� ��������� ���� �� 4-� ���������
    e_percent,                // ������� ��� PWM ��� ADC (byte)
    e_temperature,            // ������������� ��� ������������� �������� ����������� ���������� �� 100 (int)
    e_room,                   // ���������� + ����������� � ��������� ���������� �� 100 (int) ����� 0.1
    e_world,                  // ���������� ���. �������� ����������� �� ����� � ����� 1 ������
    e_water,                  // ����������� �������� ����������� ���� � ����� 1 ������.
    e_coef,                   // ������ ������������� �������� (0..255)
    e_scale,                  // ��������� ����� (-126..127)
    e_stime,                  // ����� � ��������
    e_mtime,                  // ����� � ������� ��:��
    e_minute,                 // ����� � ������� [00..59]
    e_hour,                   // ����� � ����� [00..23]
    e_day,                    // ��� [01..31]
    e_month,                  // ������ [01..12]
    e_year,                   // ���� [00..99]
    e_dt,                     // ��������� ��./���.
    e_time,                   // ����� � ������� ��:��:��
    e_date,                   // ����� � ������� dd.mm.yy
    e_sync,                   // ������� ��������� ����-������� � ���������� �����
    e_alarm,                  // ������� ������� ������
    e_alert,                  // ���������� �������
    e_password,               // �������� ������ -32766..32767
    e_delete,                 // ������������� �������/���\
    e_address,                // MAC ����� ����������
    // ������� �� boiler-control 15.05.2013
    e_PWM1,                    // ���  ������ ������������� �������� (0..255)
    e_ADC1,                    // ���  ������ ������������� �������� (0..255)
    e_PWM2,                    // ���  ������ ������������� �������� (0..255)
    e_ADC2                     // ���  ������ ������������� �������� (0..255)
};
// ����������� ��������
// �������� ������ ��������� �������. ����� ��� ����������� ������� ���������� �������
    // lcd_str *val_name;                   // ��������� �������� ��������� ��� LCD
struct st_parameter {
    // byte id;                         // ���������� ����� � ������� ��� ��������� ������ ����������
    signed int val_data;                // ���� �������� ��������� (-32768..32767
    enum en_type val_type;              // ��� �������� ���������
    unsigned char can_edit;             // ������� ����, ��� val_data ����� �������������
                                        // 0 - ��������� ���������/������� �����������
                                        // 1 - �������������� ���������/ ���� �������
    unsigned char str_num;              // ���������� ����� ������ ���� � ������� getmenustr
};
struct st_array_pos {
    signed char line0;          // ������� ������� � ������� (����)
    signed char line1;          // ��������� ������� � ������� (����)
    unsigned char lcd;          // ������� �������� ������ ������� (0 - 1 ������, 1 - 2 ������)
    struct st_parameter *menu;  // ��������� �� ������� ������� �������� ����
    unsigned char maximum;      // ������������ ���-�� ��������� � ����
    // ��� �������������� ���������� ���������� ������� ��� ���� ��������� ���������
    struct st_parameter *par;   // ��������� �� ������� ��������
    int val_data;               // ������������� �������� ���������
    unsigned char level;        // ������� ������� ���� (0 - ������� ����, 1 - �������������� ����)
};
// �������� ������� ����������
// extern lcd_str linestr;             // ������ ��� LCD
extern char linestr[];                // ������ ��� LCD
// extern signed char curr_menu_level1, next_menu_level1;  // ������� � ��������� ����� ����
extern struct st_parameter main_menu[NUM_MENU];
extern struct st_parameter parameters[NUM_PARAMETERS];
extern struct st_parameter alerts[MAX_ALERTS];
extern struct st_array_pos curr_menu;                // ������� � ����
// �������� ������� �������
extern void sync_set_par(unsigned char);
extern void init_curr_menu(struct st_parameter *lmenu, unsigned char lmax);
extern void lcd_init_edit(void);
extern void lcd_save_edit(void);
extern void lcd_esc_edit(void);
extern void lcd_menu(signed char);
extern void lcd_edit(signed char);
// extern void print_curr_menu2(signed char);
// extern void print_curr_menu(void);
extern char *param_str(unsigned char, struct st_parameter parameter[]);
extern char *par_str(struct st_parameter *, unsigned char, int);
extern char *getmenustr(unsigned char menu_num_pp);
// ������� �� boiler-control 15.05.2013
extern int calc_percent(unsigned char, unsigned char, unsigned char);
// extern unsigned char next_menu(signed char *menu_level, signed char menu_choice);
#endif
