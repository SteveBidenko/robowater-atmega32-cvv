#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// DS1307 Real Time Clock functions
#include <ds1307.h>
#include <delay.h>

#include "robowater.h"
#include "menu.h"
#include "spd1820.h"
#include "alarm.h"
#include "at2404.h"

// ���������� ����������������
// #define MENU_PARAMETERS 12

// ��������� �������� ����������
// extern struct st_prim_par prim_par;

// signed char curr_menu_level1 = 0, next_menu_level1 = 1;  // ������� � ��������� ����� ����
struct st_array_pos curr_menu;                // ������� � ����
struct st_array_pos dt_curr_menu;             // ����� ��������� ������� ���� �� ����� ����� � ���� ��������� ���� � �������
// struct st_array_pos menu_level2;  // ������� � ���� ������� ������
//    {1, e_clatsman, "���� "},              // [9] ����� �������� (1 - ����, 0 -����)
struct st_parameter main_menu[NUM_MENU]= {   // ���� ������� ������
    {-9900, e_temperature, 0, 1},     // [0]
    {0, e_isfour, 1, 2},              // [1] �������� ����� (00 - ����, 01 - �������, 10 - ���������, 11 - ����)
    {1, e_winter, 0, 3},              // [2] ����
    {0, e_dt, 1, 4},                  // [3] ��������� ���� � �������
    {0, e_empty, 1, 5},               // [4] ���� � ���� ��������� ����������
    {0, e_alarm, 0, 6},               // [5] ������ ���
    {0, e_empty, 0, 7}                // [6] ���� � ���� ������ �������� (��� ������� ��������� ���������� {0, e_empty, 1, 7})
};

struct st_parameter sdt[NUM_DT]= {   // ���� ��������� ������� � ����
    {20, e_hour, 1, 8},             // [0]
    {05, e_minute, 1, 9},           // [1]
    {19, e_day, 1, 10},             // [2]
    {03, e_month, 1, 11},           // [3]
    {11, e_year, 1, 12},            // [4]
    {0, e_winter, 1, 33},           // [5] ����
    {0, e_password, 1, 34}          // [6]
};
// ������� ���������
struct st_parameter parameters[NUM_PARAMETERS]= {
    {2200, e_room, 1, 15},          // [0] �������� ����������� ������� TA_out_prs
    {2000, e_temperature, 0, 13},   // [1] ����������� ������� � ���������
    {-350, e_temperature, 0, 14},   // [2] ����������� ������� �� �����
    {7200, e_temperature, 0, 16},   // [3] ����������� ���� �� �����
    {6500, e_temperature, 0, 17},   // [4] ����������� ���� �� ������
    {40, e_percent, 0, 18},         // [5] ������� ��������� ����� (���)
    {42, e_percent, 1, 19},         // [6] �������� ��������� ����� (��������� PWM)
    {95, e_percent, 0, 20},         // [7] ������� ��������� ����������� (���)
    {100, e_percent, 1, 21},        // [8] �������� ��������� ����������� (��������� PWM)
    {1, e_clatsman, 0, 22},         // [9] ��������� ��������� �������� (0 - �������, 1 - �������)
    {1, e_clatsman, 1, 23}          // [10] ������ ������ (1 - ���., 0 - ����.)
};
// ���������� ������� (������ ���������)
struct st_parameter settings[NUM_SETTINGS]={
    {-2500, e_world, 1, 24},        // [0] ����������� ������� ��� TA_in_Min
    {3500, e_water, 1, 25},         // [1] ����������� ���� ���� TW_out_Stop
    {1000, e_water, 1, 26},         // [2] ����������� ���� ����� TW_out_Min
    {1500, e_room, 1, 27},          // [3] ����������� ������� ��� TA_out_Min
    {30, e_stime, 1, 28},           // [4] ����� �������������� ���� T_z ?
    {10, e_stime, 1, 29},           // [5] ����� �������������� ���� T_int
    {1, e_coef, 1, 30},             // [6] ����������� �������� Ku
    {0, e_coef, 1, 31},             // [7] ������ ���� Ki
    {0, e_coef, 1, 32},             // [8] ������ ���� Kd
    {0, e_delete, 1, 35},           // [9] ���������� ���������� 1
    {-1, e_address, 1, 36},         // [10] ��������� ���������� 1
    {1, e_delete, 1, 37},           // [11] ���������� ���������� 2
    {-1, e_address, 1, 38},         // [12] ��������� ���������� 2
    {2, e_delete, 1, 39},           // [13] ���������� ���������� 3
    {-1, e_address, 1, 40},         // [14] ��������� ���������� 3
    {3, e_delete, 1, 41},           // [15] ���������� ���������� 4
    {-1, e_address, 1, 42}          // [16] ��������� ���������� 4
};
#define ALERT_POS 61
struct st_parameter alerts[MAX_ALERTS] = {
    {0, e_alert, 0, ALERT_POS},
    {0, e_alert, 0, ALERT_POS + 1},
    {0, e_alert, 0, ALERT_POS + 2},
    {0, e_alert, 0, ALERT_POS + 3},
    {0, e_alert, 0, ALERT_POS + 4},
    {0, e_alert, 0, ALERT_POS + 5},
    {0, e_alert, 0, ALERT_POS + 6},
    {0, e_alert, 0, ALERT_POS + 7},
    {0, e_alert, 0, ALERT_POS + 8},
    {0, e_alert, 0, ALERT_POS + 9},
    {0, e_alert, 0, ALERT_POS + 10},
    {0, e_alert, 0, ALERT_POS + 11}
};
flash lcd_str all_menu_str[] = {
        "��������� ",  // [0]
        "����� ",      // [1]
        "����� ",      // [2]
        "���. ",       // [3]
        "��������� ",  // [4]
        "������ ",      // [5]
        "���������...", // [6]
        "���.���. ",   // [7]
        "��� ��� ",    // [8]
        "���.����. ",  // [9]
        "��� ���. ",   // [10]
        "��� ���. ",   // [11]
        "�����. ",     // [12]
        "��.� ",       // [13]
        "���.� ",      // [14] TA_out_prs
        "��� ��. ",    // [15]
        "��� ���.",    // [16]
        "���� ",       // [17]
        "���.����. ",  // [18]
        "���������� ", // [19]
        "���. ����. ", // [20]
        "����. ",      // [21]
        "����� ",      // [22]
        "TA IN Min ",  // [23] TA_in_Min
        "TW STOP ",    // [24] TW_out_Stop
        "TW Min ",     // [25] TW_out_Min
        "TA Min ",     // [26] TA_out_Min
        "��.����=",    // [27] T_z
        "��.����=",    // [28] T_int
        "��=",         // [29]
        "�t ����=",    // [30]
        "�t ����=",     // [31]
        "�����=",       // [32]
        "������=",      // [33]
        "DEL ���.",   // [34]
        "SET ���.",   // [35]
        "DEL ��.",    // [36]
        "SET ��.",    // [37]
        "DEL WIn",    // [38]
        "SET WIn",    // [39]
        "DEL WOut",   // [40]
        "SET WOut"    // [41]
    };
char linestr[20];           // ������ ��� LCD
bit need_eeprom_write;      // ����, ���� ���������� �������� � EEPROM
// ������� ������������� ��������� �������� ����������
void sync_set_par(byte sync) {
    register byte i, j;
    // ���� ������ � ����
    if (sync == SYNC_TO_MENU) {
        main_menu[2].val_data = prim_par.season;
        settings[0].val_data = prim_par.TA_in_Min;
        settings[1].val_data = prim_par.TW_out_Stop;
        settings[2].val_data = prim_par.TW_out_Min;
        settings[3].val_data = prim_par.TA_out_Min;
        settings[4].val_data = prim_par.T_z;
        settings[5].val_data = prim_par.T_int;
        settings[6].val_data = prim_par.Ku;
        settings[7].val_data = prim_par.Ki;
        settings[8].val_data = prim_par.Kd;
        // ����� ����� ����������/������ �� �������������� � ����
        for (i = 0; i < MAX_DS1820; i++) {
            unsigned char i_set9 = 9 + (int)i * 2;
            unsigned char i_set10 = 10 + (int)i * 2;

            // ���� ������ ���� ������ �������� 'FF' - �� ��������� ���������
            if (*prim_par.addr[i] == 0xFF) {
                // �������� ��������� DEL, � �������� SET
                settings[i_set9].can_edit = 0;
                settings[i_set10].can_edit = (mode.new_terms) ? 1 : 0;
            } else {
                // �������� ��������� DEL, � �������� SET
                settings[i_set9].can_edit = 1;
                settings[i_set10].can_edit = 0;
            }
            settings[i_set9].val_data = i;
            settings[i_set10].val_data = -1;
        }
        if (mode.run == 0) {
            parameters[8].val_data = prim_par.fan_speed  ;
        };
        //parameters[8].val_data = fan_speed;
        parameters[0].val_data = prim_par.TA_out_prs;
        parameters[10].val_data = mode.pomp;
        // printf("����� ������� � ���� \r\n");
        sdt[0].val_data = s_dt.cHH;
        sdt[1].val_data = s_dt.cMM;
        sdt[2].val_data = s_dt.cdd;
        sdt[3].val_data = s_dt.cmo;
        sdt[4].val_data = s_dt.cyy;
        SEASON = prim_par.season;
    } else {
        // ���� ������� �� ����
        if (mode.stop_sync_dt) {
            if ((s_dt.cHH != sdt[0].val_data) || (s_dt.cMM != sdt[1].val_data)) {
                s_dt.cHH = sdt[0].val_data;
                s_dt.cMM = sdt[1].val_data;
                s_dt.cSS = 0;
                set_cur_dt();
                printf("������ ������� %02u:%02u:00\r\n",sdt[0].val_data, sdt[1].val_data);
                printf("���� %02u.%02u.20%02u\r\n", sdt[2].val_data, sdt[3].val_data, sdt[4].val_data);
            };
            if ((s_dt.cdd != sdt[2].val_data) || (s_dt.cmo != sdt[3].val_data) || (s_dt.cyy != sdt[4].val_data)) {
                s_dt.cdd = sdt[2].val_data;
                s_dt.cmo = sdt[3].val_data;
                s_dt.cyy = sdt[4].val_data;
                set_cur_dt();
                printf("������ ���� %02u.%02u.20%02u\r\n", sdt[2].val_data, sdt[3].val_data, sdt[4].val_data);
                printf("����� %02u:%02u:00\r\n",sdt[0].val_data, sdt[1].val_data);
            };
        } else {
            // ��������� ������ �������� � ���� �� ���������� �� ����������, �� ���������� ���� ������������� ������ � EEPROM
            if (prim_par.TA_in_Min != settings[0].val_data) {
                prim_par.TA_in_Min = settings[0].val_data; need_eeprom_write = 1;
            }
            if (prim_par.TW_out_Stop != settings[1].val_data) {
                prim_par.TW_out_Stop = settings[1].val_data; need_eeprom_write = 1;
            }
            if (prim_par.TW_out_Min != settings[2].val_data) {
                prim_par.TW_out_Min = settings[2].val_data; need_eeprom_write = 1;
            }
            if (prim_par.TA_out_Min != settings[3].val_data) {
                prim_par.TA_out_Min = settings[3].val_data; need_eeprom_write = 1;
            }
            if (prim_par.T_z != settings[4].val_data) {
                prim_par.T_z = settings[4].val_data; need_eeprom_write = 1;
            }
            if (prim_par.T_int != settings[5].val_data) {
                prim_par.T_int = settings[5].val_data; need_eeprom_write = 1;
            }
            if (prim_par.Ku != settings[6].val_data) {
                prim_par.Ku = settings[6].val_data; need_eeprom_write = 1;
            }
            if (prim_par.Ki != settings[7].val_data) {
                prim_par.Ki = settings[7].val_data; need_eeprom_write = 1;
            }
            if (prim_par.Kd != settings[8].val_data) {
                prim_par.Kd = settings[8].val_data; need_eeprom_write = 1;
            }
            // ��������� ����� ����, ��� ���� ���������� ������������
            for (i = 0; i < MAX_DS1820; i++) {
                unsigned char i_set9 = 9 + (int)i * 2;
                unsigned char i_set10 = 10 + (int)i * 2;
                int val_data_set_9 = settings[i_set9].val_data,
                    val_data_set_10 = settings[i_set10].val_data;

                // ��������� �� ��������������� DEL
                if (val_data_set_9 < 0) {
                    ds1820_set_THTL(prim_par.addr[i], 0xFF, 0xFF);
                    ds1820_rom_codes[i][1] = ds1820_rom_codes[i][0] = prim_par.addr[i][0] = 0xFF;
                    // ��������� ������ ��� ������� � ��� ����
                    settings[i_set9].can_edit = 0;
                    if (mode.new_terms) settings[i_set10].can_edit = 1;
                    need_eeprom_write = 1;
                    // ��� ����, ����� ������ ��� �� ������ � ���� ����
                    settings[i_set9].val_data = i;
                }
                // �������� ��������������� SET
                if (val_data_set_10 >= 0) {
                    // printf("���������� ����� � ufo: settings[i_set10].val_data = %d\r\n", val_data_set_10);
                    for (j = 0; j < 9; j++) {
                        // ������������ �� ����������� mode.ufo � prim_par.addr � ds1820_rom_codes
                        ds1820_rom_codes[i][j] = prim_par.addr[i][j] = mode.ufo[val_data_set_10][j];
                        mode.ufo[val_data_set_10][j] = mode.ufo[mode.new_terms - 1][j];
                    }
                    mode.new_terms--;
                    // ����������� � TH � TL ����������� ���������
                    printf("����������� � TH � TL ���������� [%s]\r\n", address_to_LCD (prim_par.addr[i]));
                    ds1820_set_THTL(prim_par.addr[i], 0xFE - i, OUR_SIGNATURE);
                    need_eeprom_write = 1;
                    settings[i_set9].can_edit = 1;
                    settings[i_set10].can_edit = 0;
                    // ��� ����, ����� ������ ��� �� ������ � ���� ����
                    settings[i_set10].val_data = -1;
                }
            }
            if (prim_par.TA_out_prs != parameters[0].val_data) {
                prim_par.TA_out_prs = parameters[0].val_data; need_eeprom_write = 1;
            };
            if (mode.run == 0) {
            if (prim_par.fan_speed  != parameters[8].val_data) {
                if (parameters[8].val_data <= FAN_SPEED_MIN) parameters[8].val_data = FAN_SPEED_MIN;
                    prim_par.fan_speed = parameters[8].val_data; need_eeprom_write = 1;
                };
            };
            if (mode.pomp  != parameters[10].val_data) {
                mode.pomp = parameters[10].val_data;
            };
            if (mode.run != main_menu[1].val_data) {
                mode.initrun = (unsigned char)main_menu[1].val_data + 4;
            };
            for (i = 0; i < MAX_ALERTS; i++) {
                if (prim_par.alert_status[i] && (alerts[i].val_data == 0)) {
                 alarm_unreg (i);
                 printf ("������� �������� �������: %s\r\n", get_alert_str(i));
                 read_all_terms(INIT_MODE);
                 printf ("�������������� ��� ����������!\r\n");
                 }
            }
        };
        // ����������� ������ ��������� � ���� ��������� �����. ���������� ������������� ���������� � eeprom
        if (prim_par.season != SEASON) {
            prim_par.season = SEASON; need_eeprom_write = 1;
            SEASON_MAIN = SEASON;
        }
     };
}
// �������, ������� ���������� ������ �� ��������� ��������� ���������, � ��� �������� ���������
// ��������� - ����� �������� ������� � ��������� �� ������
// ������� - ��������� �� ������ linestr
char *param_str(byte num_line, struct st_parameter parameter[]) {
    int pr_data;
    struct st_parameter *st_pointer;
    st_pointer = &parameter[num_line];          // �������� ���������� ��������� �� ��������� �������� �������� �������
    pr_data = st_pointer->val_data;
    /* struct st_parameter *pnt; pnt = parameter + num_line; */
    return (par_str(st_pointer, 0, pr_data));
}
// ������������� ������� ������������ ������ � ��������������� ������� ���������.
// 1) �������� - ��������� �� ��������� ���������
// 2) ���� - ������� ������ ������ ��������. (0 - �������� ��������� ��������, 1 - �� �������� ��������� ��������)
// 3) ������, ������� ����� ����������
// ������� - ��������� �� ������ linestr
char *par_str(struct st_parameter *st_pointer, unsigned char only_val, int pr_data) {
    char *pr_name;
    char prompt[] = "=> ";
    unsigned char sign = (pr_data < 0) ? '-' : '+';

    if (only_val) pr_name = prompt; else pr_name = getmenustr(st_pointer->str_num);
    switch (st_pointer->val_type) {
        case e_empty:
            sprintf(linestr, "%s", pr_name);
            break;
        case e_clatsman:
            // ���� ������ ��� �������, �� �������� �� ������ ���. ��� ����.
            if(pr_data)
                sprintf(linestr, "%s���.", pr_name);
            else
                sprintf(linestr, "%s����.", pr_name);
            break;
        case e_percent:
            // ���� ������ ��� ��������, �� �������� ��� ��������
            if (pr_data <= prim_par.tap_angle ) pr_data = prim_par.tap_angle;
            if (pr_data >= PWM_MAX ) pr_data = PWM_MAX;
            sprintf(linestr, "%s%u%%", pr_name, ((pr_data -  prim_par.tap_angle)*100)/(PWM_MAX -  prim_par.tap_angle));
            //sprintf(linestr, "%s%u%%", pr_name, pr_data);
            break;
         //case e_percent:
            // ���� ������ ��� ��������, �� �������� ��� ��������
            //sprintf(linestr, "%s%u%%", pr_name, (pr_data*100)/0xFF);
            //sprintf(linestr, "%s%u%%", pr_name, pr_data);
         //   break;
        case e_coef:
            // ���� ������ ��� �����������, �� �������� ��� ������ �����
            sprintf(linestr, "%s%u.%-01u", pr_name,  abs(pr_data)/10, abs(pr_data%10));
            //sprintf(linestr, "%s%u", pr_name, pr_data);
            break;
        case e_address:
            if (only_val) {
                // printf("[pr_data = %d]", pr_data);
                if (pr_data < 0)
                    sprintf(linestr, "> ...");
                else
                    sprintf(linestr, ">%s", address_to_LCD (mode.ufo[pr_data]));
            } else {
                sprintf(linestr, "%s(%d)", pr_name, mode.new_terms);
            }
            break;
        case e_delete:
            // �������� ������������������� ����������
            if (only_val) {
                if (pr_data < 0)
                    sprintf(linestr, "> ...");
                else
                    sprintf(linestr, ">%s", address_to_LCD (ds1820_rom_codes[pr_data]));
            } else {
                // �� ��� �������� � ������ ������
                // printf("[e_delete: pr_data = %02i] ", pr_data);
                sprintf(linestr, "%s", pr_name);
            }
            break;
        case e_scale:
            // ���� ������ ��� �����, �� �������� ��� �������� ���� (-128..127)
            sprintf(linestr, "%s%d", pr_name, pr_data);
            break;
        case e_isfour:
            main_menu[1].val_data = mode.run;
            // ���� ������ ��� �����, �� �������� �� ������� (00 - ����, 01 - �������, 10 - ���������, 11 - ����)
            switch (pr_data) {
                case 0: sprintf(linestr, "%s����", pr_name);  break;   //mode.run=0 ;
                case 1: sprintf(linestr, "%s�������", pr_name); break; //mode.run=1;
                case 2: sprintf(linestr, "%s�������", pr_name); break; //mode.run=2 ;
                case 3: sprintf(linestr, "%s����", pr_name);  ; break; //mode.run=3
                default: break;
            };
            break;
        case e_password:
            // ���� ������ ������� ����� ������, �� ������ ��������� ��������� �������� '**'
            if (only_val) sprintf(linestr, "%s%u", pr_name, pr_data);
            else sprintf(linestr, "%s **", pr_name, pr_data);
            break;
        case e_alarm:
            // ���� ������ ������� ������, �� ������� ���� ����� ���, ���� ���������� ������ � '!'
            if (pr_data) sprintf(linestr, "%s%u!", pr_name, pr_data);
            else sprintf(linestr, "%s���", pr_name);
            break;
        case e_alert:
            if (pr_data) sprintf(linestr, "%s%u!", pr_name, pr_data);
            else sprintf(linestr, "%s", pr_name);
            break;
        case e_winter:
            // ���� ������ ������, �� ������� ����/����
            if (pr_data)
                sprintf(linestr, "%s����", pr_name);
            else
                sprintf(linestr, "%s����", pr_name);
            break;
        case e_time:
            // ���� ������ ��� �����, �� �������� �� ��������� ������� � �������� ��� ��.��.��
            sprintf(linestr, "%s%02u:%02u", pr_name, s_dt.cHH, s_dt.cMM);
            break;
        case e_date:
            // ���� ������ ��� ����, �� �������� �� ��������� ���� � �������� ��� ��.��.��
            sprintf(linestr, "%s%02u.%02u.%02u", pr_name, s_dt.cdd, s_dt.cmo, s_dt.cyy);
            break;
        case e_dt:
            sprintf(linestr, "%s ��./���.", pr_name);
            break;
        case e_stime:
            // ���� ������ ��� ����, �� �������� �� ��������� ���� � �������� ��� ��.��.��
            sprintf(linestr, "%s%03u", pr_name, pr_data);
            break;
        case e_temperature:
        case e_room:
        case e_world:
        case e_water:
            // ���� ������ ��� �����������, �� �������� ��� �����������
            sprintf(linestr, "%s%c%u.%-01uC", pr_name, sign, abs(pr_data)/100, abs(pr_data%100)/10);
            break;
        case e_hour:
        case e_minute:
        case e_day:
        case e_month:
            // ���� ������ ��� [����, ������, ����, �����], �� �������� ��� ����
            sprintf(linestr, "%s%u", pr_name, pr_data);
            break;
        case e_year:
            // ���� ������ ��� ���, �� �������� ��� ���� � ������������
            sprintf(linestr, "%s20%u", pr_name, pr_data);
            break;
        default:
            linestr[0] = 0;
    };
    return(linestr);
}
// �������, ���������������� menu_level1
void init_curr_menu(struct st_parameter *lmenu, unsigned char lmax) {
    curr_menu.line0 = 0; curr_menu.line1 = 1;

    // curr_menu.menu = &main_menu[0];         // ������� ��������� ������������� �� ������� ����
    curr_menu.menu = lmenu;         // ������� ��������� ������������� �� ������� ����
    // curr_menu.maximum = MENU_LEVEL1;
    curr_menu.maximum = lmax;
    curr_menu.lcd = 0; // curr_menu.value = lmenu->val_data;
}
/* void set_curr_line(unsigned char line) {
//     curr_menu.lcd = line;
//}
// unsigned char get_curr_line(void) {
//    return curr_menu.lcd;
//}
*/
// �������, �������������� ��������� ���������� ��������� �� ������� � ��������� �������. direction - (-1, 0, 1)
void calc_pos(struct st_array_pos *array_pos, byte maxlines, signed char direction) {
    // ����������� ��� ��������� ������� ������� ������� �����
    // array_pos->line0 = array_pos->line0 + direction;
    array_pos->line0 += direction;      // ������� �������� ��������� �� ������ array_pos
    // �������� �������� �� �������������� � ���������
    if (array_pos->line0 >= maxlines) array_pos->line0 = 0;
    if (array_pos->line0 < 0) { array_pos->line0 = maxlines - 1; array_pos->line1 = 0; return; }
    // ����������� ��, ��� �� ����� ����������
    if ((array_pos->line0 + 1) == maxlines) array_pos->line1 = 0; else array_pos->line1 = array_pos->line0 + 1;
}
// ������� ������ �������� � ���������� ������ ����
void lcd_menu(signed char direction) {
    // printf ("Syncing...\r\n");

    // ���� valcoder �������� ������ � ������ ��� �� 1 �����, �� ������ ��� ������� �������� �� 2 �����
    if ((curr_menu.lcd == 0) && (direction > 0))    // �����������, ��� ��������� ������ � ���� ������ valcoder
        curr_menu.lcd = 1;
    else
        // ���� valcoder �������� ����� � ������ ��� �� 2 �����, �� ������ ��� ������� �������� �� 1 �����
        if ((curr_menu.lcd == 1) && (direction < 0))
           curr_menu.lcd = 0;
        else
            // ���� ����������� ���������� �� ����, �� ������������ ������� ������ ����
            if (direction) calc_pos(&curr_menu, curr_menu.maximum, direction);
    // printf("LCDLine=%u, dir=%u, line0=%u, line1=%u\r\n", curr_menu.lcd, direction, curr_menu.line0, curr_menu.line1);
    lcd_gotoxy(0,0);        // ������������� ������ � ������� 0 ������ 0
    lcd_line_menu(param_str(curr_menu.line0, curr_menu.menu), curr_menu.menu[curr_menu.line0].can_edit);
    lcd_gotoxy(0,1);        // ������������� ������ � ������� 0 ������ 1
    lcd_line_menu(param_str(curr_menu.line1, curr_menu.menu), curr_menu.menu[curr_menu.line1].can_edit);
    // ������������� ������ � �� ������, �� ������� ��������� ����� curr_menu.lcd
    if (curr_menu.lcd) lcd_gotoxy(LCD_DISP_LENGTH-1,1); else lcd_gotoxy(LCD_DISP_LENGTH-1,0);
    // ���� ������� �� �������� ��������
    lcd_command(LCD_DISP_ON_CURSOR_BLINK);
}
// �������, ���������������� ����� ��������������
void lcd_init_edit(void) {
    struct st_parameter *lmenu;     // ��������� ����������, ������� ����� ��������� �� ��������� ������� ����
    byte num_line;                  // ��������� ����������, ���������� ������� ������� ����
    // ������������� ��������� �� ��������� � ����������� �� ������ �������
    if (curr_menu.lcd) num_line = curr_menu.line1; else num_line = curr_menu.line0;
    // ���� ������ ������� ������� ���� � ��������� ������� num_line > 4
    if ((curr_menu.level == 0) && (num_line >= 3)) {
        sync_set_par(SYNC_TO_MENU); // �������������� ���� � ����������� �����������
        switch (num_line) {
            case 3:
                    dt_curr_menu = curr_menu;
                    init_curr_menu(&sdt[0], NUM_DT);
                    curr_menu.level = 1; // printf("���� � ���� ��������� ���� � �������\r\n");
                    mode.stop_sync_dt = 1;
                    // sync_set_taym(SYNC_TO_MENU);
                    break;
            case 4: init_curr_menu(&parameters[0], NUM_PARAMETERS);
                    curr_menu.level = 1; // printf("���� � ���� ���������\r\n");
                    break;
            case 5:
                    if (IS_ALERT) {
                        init_curr_menu(&alerts[0], MAX_ALERTS);
                        curr_menu.level = 1; // printf("���� � ���� ALERTS\r\n");
                    }
                    break;
            case 6:
                    if (SETTINGS_OPEN) {
                        init_curr_menu(&settings[0], NUM_SETTINGS);
                        curr_menu.level = 1 ;//printf ("���� � ���� ���������\r\n");
                    }
                    break;
            default: ;
        }
        mode.menu = 1;              // ����� ����� - ����
        lcd_menu(0);
        return;
    }
    lmenu = &(curr_menu.menu)[num_line];
    if (lmenu->can_edit == 0) {
        lcd_menu(0);
        return;
    } else {
        mode.menu += lmenu->can_edit; // mode.menu = mode.menu + lmenu->can_edit (�������� �������� can_edit ��������� lmenu �� ���������)
    }
    curr_menu.par = lmenu;
    curr_menu.val_data = lmenu->val_data;        // ������������� � ������� ���� �������� ��� ��������������
    lcd_edit(0);
}
// �������, �������������� ����� �� Enter
void lcd_save_edit (void) {
    struct st_parameter *lmenu;     // ��������� ����������, ������� ����� ��������� �� ��������� ������� ����
    byte num_line;                  // ��������� ����������, ���������� ������� ������� ����
    // ������������� ��������� �� ��������� � ����������� �� ������ �������
    if (curr_menu.lcd) num_line = curr_menu.line1; else num_line = curr_menu.line0;
    lmenu = &(curr_menu.menu)[num_line];
    curr_menu.par = lmenu;
    lmenu->val_data = curr_menu.val_data;
    sync_set_par(SYNC_FROM_MENU);
    if (mode.stop_sync_dt) {
        // printf ("���� � ���� main_menu \r\n"); // ������������� �� ������
        // init_curr_menu(&main_menu[0], NUM_MENU);
        curr_menu = dt_curr_menu;
        curr_menu.level = 0;
        mode.stop_sync_dt = 0;      // ������� ���� ������� �������������
    }
    // ����� �������� ���� �������� � EEPROM !!!!
    if (need_eeprom_write) {
        eeprom_write_struct ((char *)&prim_par, sizeof(prim_par));
        // ������� ���� ������������� ������ � EEPROM
        need_eeprom_write = 0;
    }
    lcd_menu(0);
    return;
}
// �������, ����������������/������������������ ����� �������������� (0 - �������������, 1 - ����� �� Enter, -1 - ����� �� Cancel
void lcd_esc_edit(void) {
    struct st_parameter *lmenu;     // ��������� ����������, ������� ����� ��������� �� ��������� ������� ����
    byte num_line;                  // ��������� ����������, ���������� ������� ������� ����
    // ������������� ��������� �� ��������� � ����������� �� ������ �������
    if (curr_menu.lcd) num_line = curr_menu.line1; else num_line = curr_menu.line0;
    // ��������� ���� � lcd_init_edit
    lmenu = &(curr_menu.menu)[num_line];
    // ���� �� ������� ������� � ������ ������ Cancel ��� ev_timer
    // printf("INIT EDIT(%d): %s = %i-->%i [%u]\r\n", initmode, lmenu->val_name, curr_menu.val_data, lmenu->val_data, curr_menu.level);
    if (curr_menu.level || mode.stop_sync_dt) {
        // printf ("���� � ���� main_menu \r\n"); // ������������� �� ������
        if (mode.stop_sync_dt)
            curr_menu = dt_curr_menu;
        else
            init_curr_menu(&main_menu[0], NUM_MENU);
        curr_menu.level = 0;
        mode.stop_sync_dt = 0;
        return;
    }
    // ��������� ���� � lcd_init_edit
    curr_menu.par = lmenu;
    // printf("INIT EDIT(%d): %s = %i [%i]\r\n", initmode, lmenu->val_name, lmenu->val_data, curr_menu.val_data);
    curr_menu.val_data = 0;
    lcd_menu(0);
    return;
}
// ������� ������������� ������� �������� ����. � �������� ��������� ���������� �������� valcoder
void lcd_edit(signed char direction) {
    struct st_parameter *lmenu;
    lmenu = curr_menu.par;
    // printf("EDIT (%d): LCDLine=%u, line0=%u, line1=%u\r\n", direction, curr_menu.lcd, curr_menu.line0, curr_menu.line1);
    // (��-)������������� ������ ���� � ����������� �� ���� ����������.
    switch (lmenu->val_type) {
        case e_empty:
            // curr_menu.val_data - �� ������ ���� ��������
            break;
        case e_address:
            curr_menu.val_data += (int)direction;
            if (curr_menu.val_data >= mode.new_terms - 1) curr_menu.val_data = -1;
            if (curr_menu.val_data < -1) curr_menu.val_data = mode.new_terms - 1;
            break;
        case e_delete:
            if (direction) curr_menu.val_data = ~curr_menu.val_data;
            break;
        case e_clatsman:
        case e_winter:
            // curr_menu.val_data - ��������� �������� 0 ��� 1
            if (direction) if(curr_menu.val_data) curr_menu.val_data = 0; else curr_menu.val_data = 1;
            // curr_menu.val_data = ~curr_menu.val_data;
            break;
        case e_isfour:
            if (direction) if (curr_menu.val_data) curr_menu.val_data = 0; else curr_menu.val_data = 3;
            break;
        case e_temperature:
            // ��������� ����������� ���� � ����� +/-0,1 �������
            curr_menu.val_data += (int)direction * 10;
            if (curr_menu.val_data > 10000) curr_menu.val_data = -3000;
            if (curr_menu.val_data < -3000) curr_menu.val_data = 10000;
            break;
        case e_room:
            // ��������� ����������� ���� � ����� +/-0,1 �������
            curr_menu.val_data += (int)direction * 10;
            if (curr_menu.val_data > 3000) curr_menu.val_data = 500;
            if (curr_menu.val_data < 500) curr_menu.val_data = 3000;
            break;
        case e_world:
            // ��������� ����������� ���� � ����� +/-1 ������
            curr_menu.val_data += (int)direction * 100;
            if (curr_menu.val_data > 3000) curr_menu.val_data = -3000;
            if (curr_menu.val_data < -3000) curr_menu.val_data = 3000;
            break;
        case e_water:
            // ��������� ����������� ���� � ����� +/-1 ������
            curr_menu.val_data += (int)direction * 100;
            if (curr_menu.val_data > 10000) curr_menu.val_data = 500;
            if (curr_menu.val_data < 500) curr_menu.val_data = 10000;
            break;
        case e_percent:
            // ��������� ��������� ���� � ����� +/-1 %
            curr_menu.val_data += direction;
            //  curr_menu.val_data += 3*direction;
            //if (curr_menu.val_data > 0xFF) curr_menu.val_data = 0;
            //if (curr_menu.val_data < 0) curr_menu.val_data = 0xFF;
            if (curr_menu.val_data >= PWM_MAX) curr_menu.val_data = PWM_MAX;
            if (curr_menu.val_data <= prim_par.tap_angle) curr_menu.val_data = prim_par.tap_angle;
            break;
        case e_coef:
            curr_menu.val_data += direction;
            if (curr_menu.val_data < 0 ) curr_menu.val_data = 0xFF;
            if (curr_menu.val_data > 0xFF) curr_menu.val_data = 0;
            break;
        case e_scale:
            curr_menu.val_data += direction;
            if (curr_menu.val_data < -128 ) curr_menu.val_data = 127;
            if (curr_menu.val_data > 127) curr_menu.val_data = -128;
            break;
        case e_stime:
            if (curr_menu.val_data < 100) curr_menu.val_data += direction; else
                if (curr_menu.val_data < 1000) curr_menu.val_data += (int)direction*10; else
                    curr_menu.val_data += (int)direction*100;
            if (curr_menu.val_data < 0) curr_menu.val_data = 3600;
            if (curr_menu.val_data > 3600) curr_menu.val_data = 0;
            break;
        case e_minute:          // ����� � ������� [00..59]
            curr_menu.val_data += direction;
            if (curr_menu.val_data < 0) curr_menu.val_data = 59;
            if (curr_menu.val_data > 59) curr_menu.val_data = 0;
            break;
        case e_hour:            // ����� � ����� [00..23]
            curr_menu.val_data += direction;
            if (curr_menu.val_data < 0) curr_menu.val_data = 23;
            if (curr_menu.val_data > 23) curr_menu.val_data = 0;
            break;
        case e_day:              // ��� [01..31]
            curr_menu.val_data += direction;
            if (curr_menu.val_data < 1) curr_menu.val_data = 31;
            if (curr_menu.val_data > 31) curr_menu.val_data = 1;
            break;
        case e_month:            // ������ [01..12]
            curr_menu.val_data += direction;
            if (curr_menu.val_data < 1) curr_menu.val_data = 12;
            if (curr_menu.val_data > 12) curr_menu.val_data = 1;
            break;
        case e_year:             // ���� [00..99]
            curr_menu.val_data += direction;
            if (curr_menu.val_data < 11) curr_menu.val_data = 99;
            if (curr_menu.val_data > 99) curr_menu.val_data = 11;
            break;
        case e_alert:
            if (direction) curr_menu.val_data = 0;
            break;
        case e_password:
            curr_menu.val_data += direction;
            SETTINGS_OPEN = (curr_menu.val_data == 20);
            if (curr_menu.val_data < 0) curr_menu.val_data = 99;
            if (curr_menu.val_data > 99) curr_menu.val_data = 0;
            break;
        default:
            curr_menu.val_data += direction;
    }
    // ������ ��� ������.
    lcd_gotoxy(0,0);        // ������������� ������ � ������� 0 ������ 1
    lcd_line_menu(par_str(lmenu, 0, lmenu->val_data), 0);
    lcd_gotoxy(0,1);        // ������������� ������ � ������� 0 ������ 1
    lcd_line_menu(par_str(lmenu, 1, curr_menu.val_data), 0);
    lcd_command(LCD_DISP_ON_CURSOR_BLINK);
}
#pragma used+
// ������� ���������� ������� � ��������� ������� ���� � �������������� ����������� (-1, 0, 1)
void print_curr_menu2(signed char direction) {
    calc_pos(&curr_menu, curr_menu.maximum, direction);
    printf("����� ���� [%u, %u]", curr_menu.line0, curr_menu.line1);
    printf(" 1) ""%s"",\t2) ""%s""\r\n", main_menu[curr_menu.line0], main_menu[curr_menu.line1]);
}
// ������ ���� �������� ���� ���������� � ���������� ���������
void print_curr_menu(void) {
    register byte i;
    // ������� �� ���������� ������� ��� ������ �������� (!) ����
    printf ("������� ����: ");
    for (i=0; i<curr_menu.maximum; i++) printf ("%s\t", param_str(i, curr_menu.menu));
    printf ("\r\n");
}
#pragma used-
// �������, ������������ ��������� �� ������ ���� �� ����������� ������
char *getmenustr(unsigned char menu_num_pp) {
    static lcd_str menustr;
    if (menu_num_pp) {
        if (menu_num_pp < ALERT_POS)
            strcpyf (menustr, all_menu_str[menu_num_pp-1]);
        else
            strcpy (menustr, get_alert_str(menu_num_pp - ALERT_POS));
        return menustr;
    } else
        return NULL;
}
