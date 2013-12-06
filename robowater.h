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
#define ALL_WARNINGS 12
#define ENTER_CANCEL_OVERFLOW 2      // �������� � ���-�� ������������ ����������
#define TIME_STOP 10                 // ����� ��������� �������� � ���-�� ��������� ����������
#define TIME_COOL_STOP 60            // ����� �������� ��� ����������� ����������� ���������� � ������ ����
#define TIME_BUZ1 1                  // ����� ����� 1
#define TIME_BUZ2 3                  // ����� ����� 2
#define STRLENGTH 16                // ����� ������ ���������
#define TIME_COOLING_MAX 15       //����� �������� ��������� ���������� ������
#define COUNT_FAN_MAX 5          // ������������ ���������� ������� ��� ��������� �������� �����������
#define POMP PORTD.5     // ����.��� ������
#define COUNT_SIGNAL_MAX 50 // ���������� ����� ������� 1 (1��) ��� ������������ �������� ����� � �����������. 0,5 ��� / 1�� = 500
//#define COOLING1 PORTD.7     // ����.��� ���������� 1
#define PWM_MIN prim_par.PWM1_lo     // ����������� ����������� ����������� �� ���������
#define PWM_MAX prim_par.PWM1_hi     // ������������ ����������� ����������� �� ��������� �1=249 �2=245
#define TA_IN_NOLIMIT 500L      //500L ��������� ����������� ����������� ������� �� ����� (500 = +5 �)
#define TAP_ANGLE_LIMIT 78   //78.0 ����������� �������� ������������ ����� ��� ����������� ������� TA_in_Min (��� ��� 20% = 51: ��� 30% = 78))
//#define TAP_PWM_LIMIT 50   // ����������� �������� ������������ �� ���������� ����� (��� ��� 20% = 51: ��� 30% = 78))
#define MAX_ACCURACY 0b10//0b01       // �������� ��� ����������� � �������� ����: 0b11 = 0.0625, 0b10 = 0.125, 0b01 = 0.25, 0b00 = 0.5
#define MAX_OFFLINES 5
#define ENGINEERING 5
#define CHECK_EVENT (event == ev_none)
#define ds1820_devices prim_par.terms
// ������ ����������� (�������� - ����� ���������� ������� � 0)
#define read_term(num) termometers[num].t
#define TIM1_COUNT 52767 // 61845
#define INITMODE 8                // ��� ������� ������� �� ���� 
#define T_TO 180                  // ����� �������� - �������� ����� ��
#define INIT_MODE 1
#define DUTY_MODE 0

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
    ev_start,               // [6] ������� ������ ����
    ev_stop,                // [7] ������� ������� ����
    ev_alarm1,              // [8]  \
    ev_alarm2,              // [9]  > ������� ������� ������
    ev_filter,              // [10] /
    ev_freezing1,           // [11] ������ ���������� �� ������ ����
    ev_freezing2,           // [12] ������ ���������� �� ������ �����
    ev_freezing3,           // [13] ������ ���������� �� ���� �����
    ev_term1_nf,            // [14] ��������� 1 �� ������
    ev_term2_nf,            // [15] ��������� 2 �� ������
    ev_term3_nf,            // [16] ��������� 3 �� ������
    ev_term4_nf,            // [17] ��������� 4 �� ������
    ev_begin_to,            // [18] ���� ��������� ��
    ev_end_to,              // [19] ��������� ��
    ev_to_nf                // [20] ���������� �������� �� �����
};
// ������� �� boiler-control 15.05.2013
// �������� ������� ������ �������
enum en_mode {
    mo_stop = 0,            //   0 - ��������� � ���������� ������,
    mo_action = 1,          //   1 - ������� ����� � ��������������
    mo_to = 2,              //   2 - �� ����� (ADC1 + PWM1)
    mo_warming_up = 3,      //   3 - ������� ����� ���������� �����������,
    mo_warming_down = 4,    //   4 - ���������� ����� ���������� �����������,
    mo_setup_input1,        //   5 - ��������� ADC1
    mo_setup_input2,        //   6 - ��������� ADC2
    mo_setup_output1,       //   7 - ��������� PWM1
    mo_setup_output2        //   8 - ��������� PWM2,
};
// �������� c�������
struct st_datetime {
    byte cHH, cMM, cSS;         // ������� �����
    byte cyy, cmo, cdd;         // ������� ����
    byte dayofweek;             // ������� ���� ������ (0..6), 0 - �����������, 1 - ����������� ...
};
// ��������� ���������� ��
struct st_TO {
    byte weekday, hour, minute;
    byte status;                // ���� ��������� ��������, �� �� ������ ����������
};
// ��������� �������� ���������� � �������
extern struct st_eeprom_par {
    byte tap_angle, fan_speed, ADC1, ADC2;        // 0x7F, 205, 0x7F, 0x7F, [4]
    int dt_winter, dt_summer;  
    int Ku, Ki, Kd;                             // 10, 0, 0 [6]
    int T_z, T_int;                             // ����� �������������� �����, ����� �������������� �����
    byte T_start;                               // ����� �������� ��� ������ �������. �� ���������: 180 
    int TW_out_Min, TW_out_Stop;                // 1500, 5000,
    int TA_in_Min, TA_out_Min, TA_out_prs;    //-1500, 1000, 2200   [10]
    // ������� �� boiler-control 15.05.2013
    byte PWM1_lo, PWM1_hi, ADC1_lo, ADC1_hi, PWM2_lo, PWM2_hi, ADC2_lo, ADC2_hi;    // ��������� ������ �������� ����� � ������
    byte warning_status[ALL_WARNINGS];           // 0) ������� (0 - ��� �������, > 0 - ���������� �������������� ������) [12]
    byte season;          // ����� ���� (?)      [1]
    byte alarm;           // ������� �������� alarm � EEPROM [1]
    byte c_alarm;         // ������� ����� ���������������� � EEPROM [1]
    struct st_TO TO;     // ��������� ���������� ��
    byte terms;           // ������� ������ ���� ����������� � �������
    byte addr[MAX_DS1820][9];  // [36] ��������� ����. (� ������ - 9 ����). ���������� ����� 0 - ���������, 1 - �����, 2 - ���� ��., 3 - ���� �����
} prim_par;

// ��������� �������� �������������� � �������
extern struct st_mode {
    byte menu;            // 0) ����� ���� (00 - ������� �����, 01 - ����, 10 - ��������������, 11 - �����������/�������������� ���������
    enum en_mode run;     // 1) ������� ����� ������ (��. ��� en_mode)
    byte initrun;         // 2) ������������ ����� ����� ��� �������/��������� � ����
    byte pomp;            // 3) ����� ������ ������ (1 = ���, 0 ����)
    byte fan;             // 4) ����� ������ ����������� (1 = ���, 0 ����)
    byte stop_sync_dt;    // 5) ���� ������� ������������� RTC � ���������� ���������. ���� ���������� - �� ��������������
    byte sound;           // 6) ���� ������� ����� (0 = ���� ��������, 1 = ���� �������
    int k_angle_limit;   // 7) ����������� ��������� ���� ����������� �������� ����� � ������ ������
    byte ufo[MAX_DS1820][9]; // 8) ������ ������������ �����������
    byte print;           // 9) ������ ��������� ������� �������������
    byte cooling1;        // 10) ���������� 1
    byte cooling2;        // 11) ���������� 2
    byte new_terms;       // 12) ���������� ��������� ����� �����������
    enum en_mode lastrun; // 13) ��������� ����������� ����� ����� ������������� � �����       
} mode;
extern enum en_event event;        // ������� ������� � �������
// �������� �������
extern void init(void);
extern void set_cur_dt (void);
extern void get_cur_dt (unsigned char);
extern unsigned int read_adc(unsigned char adc_input);  // Read the AD conversion result
void switch_mode_run(enum en_mode newstatus);   // ������������ ������� � �������. ���������� ������������ ���� ��������
// �������� ���������� ����������
extern struct st_datetime s_dt;
extern struct st_mode mode;
extern unsigned int count_signal;
#endif
