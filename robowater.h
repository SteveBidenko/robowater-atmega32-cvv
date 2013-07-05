#ifndef __ROBOWATER__
#define __ROBOWATER__
#define F_CPU 3686400UL
#define BAUDRATE 115200UL
// Описание битов регистра "А" статуса и управления
#define RXC     7            // Флаг завершения приема USART
#define TXC     6            // Флаг завершения передачи USART
#define UDRE    5            // Флаг "Регистр данных USART пуст"
#define FE      4            // Флаг ошибки кадрирования
#define OVR     3            // Флаг переполнения
#define UPE     2            // Флаг ошибки контроля четности USART
#define MPCM    0            // Режим мультипроцессорного обмена
// Описание битов регистра "B" статуса и управления
#define RXCIE   7            // Разрешение прерывания по завершению приема
#define TXCIE   6            // Разрешение прерывания по завершению передачи
#define RXEN    4            // Разрешение приема
#define TXEN    3            // Разрешение передачи
#define UCSZ2   2            // Установка 9-ти битного режима
#define RXB8    1            // Разряд номер восемь приемного буфера
#define TXB8    0            // Разряд номер восемь буфера передачи.
// Описание статусов последовательных портов
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
#define TIMER_INACTIVE 60      // Задается в кол-ве полсекундных интервалов
#define ENTER_CANCEL_OVERFLOW 2      // Задается в кол-ве полсекундных интервалов
#define TIME_START 180                // Время старта Задается в кол-ве полсекундных интервалов
#define TIME_STOP 10                 // Время остановки Задается в кол-ве полсекундных интервалов
#define TIME_COOL_STOP 60            // Время таймаута для регулировки температуры калорифера в режиме СТОП
#define TIME_BUZ1 1                  // Время Звука 1
#define TIME_BUZ2 3                  // Время Звука 2
#define STRLENGTH 16                // Длина строки параметра
#define TIME_COOLING_MAX 15       //время задержки включения охладителя секунд
#define COUNT_FAN_MAX 5          // Максимальное количество осчетов для изменения оборотов вентилятора
#define FAN_SPEED_MIN 102             // Минимальный предел оборотов вентилятора (40% = 20 Герц )
#define FAN_SPEED_STEP 10            // Шаг изменения оборотов вентилятора
#define FAN_SPEED_T_UP 300
#define FAN_SPEED_T_DOWN 200
#define MOTOR PORTD.4    // Порт.Пин вентилятора
#define POMP PORTD.5     // Порт.Пин насоса
#define COUNT_SIGNAL_MAX 50 // Количество тиков таймера 1 (1мс) для формирования сигналов звука и светодиодов. 0,5 сек / 1мс = 500
//#define COOLING1 PORTD.7     // Порт.Пин Охладителя 1
#define PWM_MIN 0        // Минимальное управляющее воздействие на двигатель
#define PWM_MAX 245      // Максимальное управляющее воздействие на двигатель П1=249 П2=245
#define TA_IN_NOLIMIT 500L      //500L Окончание ограничения температуры воздуха на входе (500 = +5 С)
#define TAP_ANGLE_LIMIT 78   //78.0 Ограничение закрытия трехходового крана при температуре воздуха TA_in_Min (ШИМ для 20% = 51: для 30% = 78))
//#define TAP_PWM_LIMIT 50   // Ограничение закрытия трехходового по напряжению снизу (ШИМ для 20% = 51: для 30% = 78))
#define MAX_ACCURACY 0b10//0b01       // Точность для термометров в бинарном коде: 0b11 = 0.0625, 0b10 = 0.125, 0b01 = 0.25, 0b00 = 0.5
#define MAX_OFFLINES 5
#define ENGINEERING 5
#define CHECK_EVENT (event == ev_none)
#define ds1820_devices prim_par.terms
// Чтение температуры (аргумент - номер термометра начиная с 0)
#define read_term(num) termometers[num].t
#define TIM1_COUNT 52767 // 61845

// Описание типов переменных
typedef unsigned char 	byte;	// byte = unsigned char
typedef unsigned int 	word;	// word = unsigned int
typedef char str_val[STRLENGTH];
// Описание перечисления событий, возникающих в системе
enum en_event {
    ev_none = 0,            // [0]событие отсутствует
    ev_left = 1,            // [1]используется valcoder -
    ev_enter = 2,           // [2]полностью нажат/отжат enter
    ev_right = 3,           // [3]используется valcoder +
    ev_cancel,              // [4] полностью нажат/отжат cancel
    ev_secunda,             // [5] ежесекундное событие
    ev_timer,               // [6] сработал таймер
    ev_start,               // [7] Нажатие кнопки ПУСК
    ev_stop,                // [8] Нажатие события СТОП
    ev_alarm1,              // [9]  \
    ev_alarm2,              // [10]  > События внешних тревог
    ev_filter,              // [11] /
    ev_freezing1,           // [12] Угроза замерзания по воздух вход
    ev_freezing2,           // [13] Угроза замерзания по Воздух выход
    ev_freezing3,           // [14] Угроза замерзания по воде выход
    ev_term1_nf,            // [15] Термометр 1 не найден
    ev_term2_nf,            // [16] Термометр 2 не найден
    ev_term3_nf,            // [17] Термометр 3 не найден
    ev_term4_nf             // [18] Термометр 4 не найден

};
// перенес из boiler-control 15.05.2013
// Описание режимов работы системы
enum en_mode {
    mo_reg = 0,             //   0 - Регулирование,
    mo_to = 1,              //   1 - ТО,
    mo_setup_input = 2,     //   2 - настройка выхода,
    mo_setup_output = 3,    //   3 - настройка входа,
    mo_control_line1,       //   4 - контроль входа1 (внешний вход)
    mo_control_line2        //   5 - контроль входа2 (внешний вход)
};

// Описание cтруктур
struct st_datetime {
    byte cHH, cMM, cSS;         // Текущее время
    byte cyy, cmo, cdd;         // Текущая дата
};
// Структура основных переменных в системе
extern struct st_eeprom_par {
    byte tap_angle, fan_speed, ADC1, ADC2;        // 0x7F, 205, 0x7F, 0x7F, [4]
    int Ku, Ki, Kd;                             // 10, 0, 0 [6]
    int T_z, T_int;                             // 300, 100 [4] Время задержки, Время интегрирования
    int TW_out_Min, TW_out_Stop;                // 1500, 5000,
    int TA_in_Min, TA_out_Min, TA_out_prs;    //-1500, 1000, 2200   [10]
    // перенес из boiler-control 15.05.2013
    byte PWM1_lo, PWM1_hi, ADC1_lo, ADC1_hi, PWM2_lo, PWM2_hi, ADC2_lo, ADC2_hi;    // Установка границ вольтажа входа и выхода
    byte alert_status[ALL_ALERTS];           // 0) Тревога (0 - нет тревоги, > 0 - количество необработанных тревог) [12]
    byte season;          // Сезон года (?)      [1]
    byte alarm;           // Позиция текущего alarm в EEPROM [1]
    byte c_alarm;         // Сколько всего зарегистрировано в EEPROM [1]
    byte terms;           // Сколько должно быть термометров в системе
    byte addr[MAX_DS1820][9];  // [36] Найденные терм. (в адресе - 9 байт). Порядковый номер 0 - Помещение, 1 - Улица, 2 - вода вх., 3 - вода выход
} prim_par;

// Структура основных переключателей в системе
extern struct st_mode {
    byte menu;            // 0) Режим меню (00 - главный экран, 01 - меню, 10 - редактирование, 11 - сохраннение/восстановление параметра
    byte run;             // 1) Текущий режим работы (00 - Стоп, 1 - прогрев,
    // перенес из boiler-control 15.05.2013                          //    2 - остановка, 3 - пуск, 4 - размораживание, 5 - Инженерная наладка)
    // enum en_mode run;    // 1) Текущий режим работы
    byte initrun;         // 2) Запрашиваемы новый режим для запуска/остановки с меню
    byte pomp;            // 3) Режим работы насоса (1 = вкл, 0 выкл)
    byte fan;             // 4) Режим работы вентилятора (1 = вкл, 0 выкл)
    byte stop_sync_dt;    // 5) Флаг запрета синхронизации RTC и глобальной структуры. Если установлен - не синхронизируем
    byte sound;           // 6) Флаг запрета звука (0 = звук выключен, 1 = звук включен
    int k_angle_limit;   // 7) Коеффициент пересчета угла ограничения водяного крана в зимний период
    byte ufo[MAX_DS1820][9]; // 8) Массив Неопознанных термометров
    byte print;           // 9) Печать показаний системы регулирования
    byte cooling1;        // 10) Охладитель 1
    byte cooling2;        // 11) Охладитель 2
    byte new_terms;       // 12) Количество найденных новых термометров
} mode ;
extern enum en_event event;        // Текущее событие в системе
// Описание функций
extern void init(void);
extern void set_cur_dt (void);
extern void get_cur_dt (unsigned char);
// Описание глобальных переменных
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
extern unsigned int count_signal;
//extern byte fan_speed;
// extern unsigned char key_treated[7] = {0, 0, 0, 0, 0, 0, 0};
#endif
