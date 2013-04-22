#ifndef __ROBOWATER__
#define __ROBOWATER__
#define F_CPU 3686400UL
#define BAUDRATE 115200UL
// �������� ����� �������� "�" ������� � ����������
#define RXC     7            // ���� ���������� ������ USART
#define TXC     6            // ���� ���������� �������� USART
#define UDRE    5            // ���� "������� ������ USART ����"
#define FE      4            // ���� ������ ������������
#define OVR     3            // ���� ������������
#define UPE     2            // ���� ������ �������� �������� USART
#define MPCM    0            // ����� ������������������� ������
// �������� ����� �������� "B" ������� � ����������
#define RXCIE   7            // ���������� ���������� �� ���������� ������
#define TXCIE   6            // ���������� ���������� �� ���������� ��������
#define RXEN    4            // ���������� ������
#define TXEN    3            // ���������� ��������
#define UCSZ2   2            // ��������� 9-�� ������� ������
#define RXB8    1            // ������ ����� ������ ��������� ������
#define TXB8    0            // ������ ����� ������ ������ ��������.
// �������� �������� ���������������� ������
#define FRAMING_ERROR	(1<<FE)
#define PARITY_ERROR    (1<<UPE)
#define DATA_OVERRUN    (1<<OVR)
#define DATA_REGISTER_EMPTY (1<<UDRE)
#define TX_COMPLETE 	(1<<UDRE)
#define RX_COMPLETE 	(1<<RXC)
// maximum number of DS1820 devices connected to the 1 Wire bus
#define MAX_DS1820 4
#define FORCE_INIT 1
#define ALL_ALERTS 12
#define TIMER_INACTIVE 60      // �������� � ���-�� ������������ ����������
#define ENTER_CANCEL_OVERFLOW 2      // �������� � ���-�� ������������ ����������
#define TIME_START 180                // ����� ������ �������� � ���-�� ������������ ����������
#define TIME_STOP 10                 // ����� ��������� �������� � ���-�� ������������ ����������
#define TIME_COOL_STOP 60            // ����� �������� ��� ����������� ����������� ���������� � ������ ����
#define TIME_BUZ1 1                  // ����� ����� 1
#define TIME_BUZ2 3                  // ����� ����� 2
#define STRLENGTH 16                // ����� ������ ���������
#define TIME_COOLING_MAX 15       //����� �������� ��������� ���������� ������  
#define COUNT_FAN_MAX 5          // ������������ ���������� ������� ��� ��������� �������� ����������� 
#define FAN_SPEED_MIN 102             // ����������� ������ �������� ����������� (40% = 20 ���� )
#define FAN_SPEED_STEP 10            // ��� ��������� �������� �����������
#define FAN_SPEED_T_UP 300
#define FAN_SPEED_T_DOWN 200
#define MOTOR PORTD.4    // ����.��� �����������
#define POMP PORTD.5     // ����.��� ������
//#define COOLING1 PORTD.7     // ����.��� ���������� 1
#define PWM_MIN 0        // ����������� ����������� ����������� �� ���������
#define PWM_MAX 245      // ������������ ����������� ����������� �� ��������� �1=249 �2=245
#define TA_IN_NOLIMIT 500L      //500L ��������� ����������� ����������� ������� �� ����� (500 = +5 �)
#define TAP_ANGLE_LIMIT 78   //78.0 ����������� �������� ������������ ����� ��� ����������� ������� TA_in_Min (��� ��� 20% = 51: ��� 30% = 78))
//#define TAP_PWM_LIMIT 50   // ����������� �������� ������������ �� ���������� ����� (��� ��� 20% = 51: ��� 30% = 78)) 
#define MAX_ACCURACY 0b10//0b01       // �������� ��� ����������� � �������� ����: 0b11 = 0.0625, 0b10 = 0.125, 0b01 = 0.25, 0b00 = 0.5 
#define MAX_OFFLINES 5
#define ENGINEERING 5
#define CHECK_EVENT (event == ev_none)
#define ds1820_devices prim_par.terms

// �������� ����� ����������
typedef unsigned char 	byte;	// byte = unsigned char
typedef unsigned int 	word;	// word = unsigned int
typedef char str_val[STRLENGTH];
// �������� ������������ �������, ����������� � �������
enum en_event {
    ev_none = 0,            // [0]������� �����������
    ev_left = 1,            // [1]������������ valcoder -
    ev_enter = 2,           // [2]��������� �����/����� enter
    ev_right = 3,           // [3]������������ valcoder +
    ev_cancel,              // [4] ��������� �����/����� cancel
    ev_secunda,             // [5] ������������ �������
    ev_timer,               // [6] �������� ������
    ev_start,               // [7] ������� ������ ����
    ev_stop,                // [8] ������� ������� ����
    ev_alarm1,              // [9]  \
    ev_alarm2,              // [10]  > ������� ������� ������
    ev_filter,              // [11] /
    ev_freezing1,           // [12] ������ ���������� �� ������ ����
    ev_freezing2,           // [13] ������ ���������� �� ������ �����
    ev_freezing3,           // [14] ������ ���������� �� ���� �����
    ev_term1_nf,            // [15] ��������� 1 �� ������
    ev_term2_nf,            // [16] ��������� 2 �� ������
    ev_term3_nf,            // [17] ��������� 3 �� ������
    ev_term4_nf             // [18] ��������� 4 �� ������
    
};
// �������� c�������
struct st_datetime {
    byte cHH, cMM, cSS;         // ������� �����
    byte cyy, cmo, cdd;         // ������� ����
};
// ��������� ������������ ��� ��������� ����������� t = �(��������) * 100
struct st_eliminate {
    signed int shift;       // �������� ������� ����� (0 - ��� ���������) [2]
    signed char scale;      // ��������� (-127..0..126), ������� ����� ����������� �� ������� 1+(scale/127) [1]
};
// typedef st_eliminates t_eliminates[MAX_DS1820];
// ��������� �������� ���������� � �������
extern struct st_eeprom_par {
    struct st_eliminate elims[MAX_DS1820];            // [3] * 4 = [12]
    byte tap_angle, fan_speed, ADC1, ADC2;        // 0x7F, 205, 0x7F, 0x7F, [4]
    int Ku, Ki, Kd;                             // 10, 0, 0 [6]
    int T_z, T_int;                             // 300, 100 [4] ����� ��������, ����� ��������������
    int TW_out_Min, TW_out_Stop;                // 1500, 5000,
    int TA_in_Min, TA_out_Min, TA_out_prs;    //-1500, 1000, 2200   [10]
    byte alert_status[ALL_ALERTS];           // 0) ������� (0 - ��� �������, > 0 - ���������� �������������� ������) [12]
    byte season;          // ����� ���� (?)      [1]
    byte alarm;           // ������� �������� alarm � EEPROM [1]
    byte c_alarm;         // ������� ����� ���������������� � EEPROM [1]
    byte terms;           // ������� ������ ���� ����������� � �������
    byte addr[MAX_DS1820][9];  // [36] ��������� ����. (� ������ - 9 ����). ���������� ����� 0 - ���������, 1 - �����, 2 - ���� ��., 3 - ���� �����
} prim_par;

// ��������� �������� �������������� � �������
extern struct st_mode {
    byte menu;            // 0) ����� ���� (00 - ������� �����, 01 - ����, 10 - ��������������, 11 - �����������/�������������� ���������
    byte run;             // 1) ������� ����� ������ (00 - ����, 1 - �������,
                          //    2 - ���������, 3 - ����, 4 - ��������������, 5 - ���������� �������)
    byte initrun;         // 2) ������������ ����� ����� ��� �������/��������� � ����
    byte pomp;            // 3) ����� ������ ������ (1 = ���, 0 ����)
    byte fan;             // 4) ����� ������ ����������� (1 = ���, 0 ����)
    byte stop_sync_dt;    // 5) ���� ������� ������������� RTC � ���������� ���������. ���� ���������� - �� ��������������
    byte sound;           // 6) ���� ������� ����� (0 = ���� ��������, 1 = ���� �������
    int k_angle_limit;   // 7) ����������� ��������� ���� ����������� �������� ����� � ������ ������
    byte ufo[9];          // 8) ������������ ��������� (-�)
    byte print;           // 9) ������ ��������� ������� �������������
    byte cooling1;        // 10) ���������� 1
    byte cooling2;        // 11) ���������� 2
} mode ;
extern enum en_event event;        // ������� ������� � �������
// �������� �������
extern void init(void);
extern int read_term(byte);
extern void set_cur_dt (void);
extern void get_cur_dt (unsigned char);
// �������� ���������� ����������
extern struct st_datetime s_dt;
// extern struct st_menupos menupos;
extern struct st_mode mode;
extern unsigned int time_integration;
extern byte timer1_valcoder;
extern unsigned int timer_start;
extern byte timer_stop;
extern byte timer_fan;
extern byte count_fan;
extern int time_cooling;
//extern byte fan_speed;
// extern unsigned char key_treated[7] = {0, 0, 0, 0, 0, 0, 0};
#endif
