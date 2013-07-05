#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include <delay.h>
#include "lcd_4bit.h"
#include "menu.h"
#include "robowater.h"
// DS1820 функции температурного преобразования (Биденко+Панарин)
#include "spd1820.h"
#include "valcoder.h"
#include "at2404.h"
#include "alarm.h"
#include "signals.h"
#include "keys.h"
// Локальные макроподстановки
#define MAJOR_VERSION 4
#define MINOR_VERSION 4
// #define NODEBUG
// enum
// Определение главных структур
// Описание глобальны переменных
// int tw_prs;             // Заданная температура воды
// TW_in_Min -Температура Воды на входе( Подаче ) мин. ?
// TW_out_Min -Температура Воды на выходе ( Обратка ) мин.
// TW_out_Stop -Температура Воды на входе для поддержания в режиме стоп.
// TA_in_Min -Температура Воздуха на входе мин -30 С.
// TA_out_Min -Температура Воздуха на выходе мин +15 С.
// TA_out_prs -Температура Воздуха на выходе установленная +20 С.(Заданная)
unsigned char key_treated[7] = {0,0,0,0,0,0,0} ;
struct st_mode mode = {0, 0, 0, 0, 0, 0, 1, 0.0,0,0,0};  // Текущий режим работы
struct st_datetime s_dt;
// Начальные установки структуры основных переменных
struct st_eeprom_par prim_par={
    48, 205, 0x7F, 0x00,                                   // П1=50,П2=50,П3=48
    (int)10, (int)20, (int)10,
    (int)180, (int)100,
    (int)1500, (int)5000,
    (int)-2000, (int)1000, (int)2200,
// перенес из boiler-control 15.05.2013
    48, 250, 48, 250, 48, 250, 48, 250,  // Установка границ вольтажа входов и выходов

    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    1,  // Зима
    0,  // Позиция текущего alarm в EEPROM
    0,   // Сколько всего зарегистрировано в EEPROM
    MAX_DS1820, // Сколько должно быть термометров в системе

     /*{{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFE}, // 28	4D	52	7E 	3	0	0	6E	1	FF	FE воздух помещение
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFD}, // 28	D3	8	7E	3	0	0	E4	1	FF	FD   воздух улица
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC}, // 28	8C	38	7E	3	0	0	6A	1	FF	FC   вода подача
     {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFB}} // 28	91	9	7E	3	0	0	32	1	FF	FB   вода обратка
    */
     /*  Ньютона металоизделия П1
     {{0x28,0x29,0x32,0x7e,0x03,0x00,0x00,0x60,0x01}, // 28	29	32	7E 	3	0	0	60	1	FF	FE
     {0x28,0x13,0x3b,0x7e,0x03,0x00,0x00,0xb1,0x01}, // 28	13	3B	7E	3	0	0	B1	1	FF	FD
     {0x28,0x1e,0x2f,0x7e,0x03,0x00,0x00,0x98,0x01}, // 28	1E	2F	7E	3	0	0	98	1	FF	FC
     {0x28,0x39,0x46,0x7e,0x03,0x00,0x00,0x49,0x01}} // 28	39	46	7E	3	0	0	49	1	FF	FB
    */
     /*Ньютона металоизделия П2
     {{0x28,0x95,0x0f,0x7e,0x03,0x00,0x00,0x72,0x01}, // 28	95	F	7E 	3	0	0	72	1	FF	FE
     {0x28,0xc1,0x1a,0x7e,0x03,0x00,0x00,0x2e,0x01}, // 28	C1	1A	7E	3	0	0	2E	1	FF	FD
     {0x28,0x1a,0x17,0x7e,0x03,0x00,0x00,0xfe,0x01}, // 28	1A	17	7E	3	0	0	FE	1	FF	FC
     {0x28,0xe8,0x2a,0x7e,0x03,0x00,0x00,0x8a,0x01}} // 28	E8	2A	7E	3	0	0	8A	1	FF	FB
     */
     /*  Ньютона металоизделия П3
     {{0x28,0x50,0x32,0x7e,0x03,0x00,0x00,0x6e,0x01}, // 28	50	32	7E 	3	0	0	6E	1	FF	FE
     {0x28,0x3e,0x1c,0x7e,0x03,0x00,0x00,0xe4,0x01}, // 28	3E	1C	7E	3	0	0	E4	1	FF	FD
     {0x28,0xb7,0x0d,0x7e,0x03,0x00,0x00,0x29,0x01}, // 28	B7	D	7E	3	0	0	29	1	1	FF	FC
     {0x28,0x2c,0x0c,0x7e,0x03,0x00,0x00,0xad,0x01}} // 28	2C	C	7E	3	0	0	AD	1	FF	FB
     */
     /* Ньютона металоизделия П4
    {{0x28,0x4d,0x52,0x7e,0x03,0x00,0x00,0x6e,0x01}, // 28	4D	52	7E 	3	0	0	6E	1	FF	FE
     {0x28,0xd3,0x08,0x7e,0x03,0x00,0x00,0xe4,0x01}, // 28	D3	8	7E	3	0	0	E4	1	FF	FD
     {0x28,0x8c,0x38,0x7e,0x03,0x00,0x00,0x6a,0x01}, // 28	8C	38	7E	3	0	0	6A	1	FF	FC
     {0x28,0x91,0x09,0x7e,0x03,0x00,0x00,0x32,0x01}} // 28	91	9	7E	3	0	0	32	1	FF	FB
    */
     /*    50 ЛетВЛКСМ бильярдная.
    {{0x28,0x1b,0xc4,0x24,0x00,0x00,0x00,0xfe,0x01}, // 28	1b	c4	24	0	0	0	fe	1	FF	FE
     {0x28,0xbb,0x0a,0x95,0x03,0x00,0x00,0xd8,0x01}, // 28	bb	a	95	3	0	0	d8	1	FF	FD
     {0x22,0x8e,0xe1,0x03,0x00,0x00,0x00,0xf3,0x01}, // 22	8e	e1	3	0	0	0	f3	1	FF	FC
     {0x22,0x2b,0x08,0x04,0x00,0x00,0x00,0xeb,0x01}} // 22	2b	8	4	0	0	0	eb	1	FF	FB
    */
     /*    Микрофарм П2.
    {{0x28,0xf8,0x28,0x94,0x03,0x00,0x00,0x00,0x01}, // 28  F8  28  94  3   0   0   0   1	FF	FE   Помещение
     {0x28,0x7b,0x3b,0x94,0x03,0x00,0x00,0x81,0x01}, // 28  7B  3B  94  3   0   0   81  1	FF	FD   Улица
     {0x28,0x54,0x26,0x94,0x03,0x00,0x00,0x83,0x01}, // 28  54  26  94  3   0   0   83  1	FF	FC   Вода ВХОД
     {0x28,0xe7,0x3f,0x94,0x03,0x00,0x00,0x52,0x01}} // 28  E7  3F  94  3   0   0   52  1	FF	FB   Вода выход
    */
     /*    Микрофарм П1.
    {{0x28,0xba,0x56,0x7e,0x03,0x00,0x00,0x86,0x01}, // 28  BA  56  7E  3   0   0   86  1	FF	FE   Помещение
     {0x28,0xea,0x20,0x95,0x03,0x00,0x00,0x84,0x01}, // 28  EA  20  95  3   0   0   84  1	FF	FD   Улица
     {0x28,0x1e,0x34,0x7e,0x03,0x00,0x00,0x94,0x01}, // 28  1E  34  7E  3   0   0   94  1	FF	FC   Вода ВХОД
     {0x28,0x1e,0x2b,0x7e,0x03,0x00,0x00,0x87,0x01}} // 28  1E  2B  7E  3   0   0   87  1	FF	FB   Вода выход
     */
     /*    Зеньков П1.
    {{0x28,0x61,0x23,0x7e,0x03,0x00,0x00,0x05,0x01}, // 28  61  23  7E  3   0   0   5   1	FF	FE   Помещение
     {0x28,0x56,0x21,0x7e,0x03,0x00,0x00,0xee,0x01}, // 28  56  21  7E  3   0   0   EE  1	FF	FD   Улица
     {0x28,0x13,0x57,0x7e,0x03,0x00,0x00,0x81,0x01}, // 28  13  57  7E  3   0   0   81  1	FF	FC   Вода ВХОД
     {0x28,0xc6,0x1e,0x7e,0x03,0x00,0x00,0xb4,0x01}} // 28  C6  1E  7E  3   0   0   B4 1	FF	FB   Вода выход
     */
     /*    Зеньков П2.
    {{0x28,0xb8,0x24,0x7e,0x03,0x00,0x00,0x06,0x01}, // 28  B8  24  7E  3   0   0   6   1	FF	FE   Помещение
     {0x28,0x2c,0x1d,0x7e,0x03,0x00,0x00,0x1c,0x01}, // 28  2C  1D  7E  3   0   0   1C  1	FF	FD   Улица
     {0x28,0x6f,0x42,0x7e,0x03,0x00,0x00,0xca,0x01}, // 28  6F  42  7E  3   0   0   CA  1	FF	FC   Вода ВХОД
     {0x28,0x4f,0x36,0x7e,0x03,0x00,0x00,0x0e,0x01}} // 28  4F  36  7E  3   0   0   E   1	FF	FB   Вода выход
    */
      /*    Зеньков П3.
    {{0x28,0xc2,0x14,0x7e,0x03,0x00,0x00,0xd5,0x01}, // 28  C2  14  7E  3   0   0   D5  1	FF	FE   Помещение
     {0x28,0xd6,0x3a,0x7e,0x03,0x00,0x00,0x08,0x01}, // 28  D6  3A  7E  3   0   0   8   1	FF	FD   Улица
     {0x28,0x4f,0x4c,0x7e,0x03,0x00,0x00,0xde,0x01}, // 28  4F  4C  7E  3   0   0   DE  1	FF	FC   Вода ВХОД
     {0x28,0x0a,0x3e,0x7e,0x03,0x00,0x00,0xae,0x01}} // 28  A   3E  7E  3   0   0   AE  1   FF	FB   Вода выход
     */
     {{0xff,0xc2,0x14,0x7e,0x03,0x00,0x00,0xd5,0x01}, // 28  C2  14  7E  3   0   0   D5  1	FF	FE   Помещение
     {0xff,0xd6,0x3a,0x7e,0x03,0x00,0x00,0x08,0x01}, // 28  D6  3A  7E  3   0   0   8   1	FF	FD   Улица
     {0xff,0x4f,0x4c,0x7e,0x03,0x00,0x00,0xde,0x01}, // 28  4F  4C  7E  3   0   0   DE  1	FF	FC   Вода ВХОД
     {0xff,0x0a,0x3e,0x7e,0x03,0x00,0x00,0xae,0x01}} // 28  A   3E  7E  3   0   0   AE  1   FF	FB   Вода выход


};
unsigned int time_integration=0;
int tmp_delta;
byte tap_angle_min = 0;   // Ограничение крана снизу от температуры
byte fan_speed = 0;  // Скорость вентилятора
enum en_event event;                          // Текущее событие в системе
// Описание функций
void printallterms(unsigned char); void lcd_primary_screen(void);
void printnewterms(void);
void print_prim_par(unsigned char *, unsigned char);
// Функции основного цикла
void check_serial(void);
void check_peripheral(void);
void event_processing(void);
void mode_processing(void);
void update_P(int);
void deley_run(void);
//void update_PID(int error, int iMin, int iMax);
void init_new_terms(unsigned char);
void start_screen(unsigned char);
// Основная программа
void main(void) {
    // register byte i;
    byte size_prim_par;
    init();                  // Инициализация всей периферии
    #asm("sei")             // Global enable interrupts
    start_screen(0);
    init_keys();
    // Сохраняем в EEPROM структуру prim_par
    // ppr_par = &prim_par;
    size_prim_par = sizeof(prim_par);
    // СБРОС параметров в EEPROM к заводским установкам, если одновременно нажаты клавиши CANCEL + ENTER
    if (!KEY_CANCEL && !KEY_ENTER) {
        unsigned char term_status;

        start_screen(1);
        ds1820_devices = MAX_DS1820;
        // Сихронизация массива термометров с EEPROM
        term_status = sync_ds1820_eeprom();
        eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
        // Запускаем таймер инактивности
        printf("Запись в EEPROM заводских установок.\r\n Неопознанных термометров = %u\r\n", term_status);
    } else {
        unsigned char size_addr = size_prim_par - sizeof(prim_par.addr);

        if (!KEY_CANCEL) {
            // При нажатой клавише CANCEL записываем установки по умолчанию не трогая информацию о термометрах
            start_screen(2);
            ds1820_devices = MAX_DS1820;
            printf("Запись в EEPROM установок по умолчанию.\r\n");
            eeprom_write_struct ((unsigned char *)&prim_par, size_addr);
        }
        if (!KEY_ENTER) {
            // При нажатой клавише ENTER записываем массив новых термометров в EEPROM
            unsigned char term_status;

            start_screen(3);
            eeprom_read_struct ((char *)&prim_par, size_addr);
            ds1820_devices = MAX_DS1820;
            // Сихронизация массива термометров с EEPROM
            term_status = sync_ds1820_eeprom();
            printf("Запись в EEPROM %u новых термометров.\r\n", term_status);
            eeprom_write_struct ((unsigned char *)&prim_par, size_prim_par);
        }
    }
    deley_run();
    // Восстанавливаем из EEPROM структуру prim_par
    eeprom_read_struct ((char *)&prim_par, size_prim_par);
    // Выитываем из EEPROM все термометры
    read_all_terms(INIT_MODE);
    // print_all_menu();       // Выведем на отладочную консоль все пункты меню
    sync_set_par(SYNC_TO_MENU); // Синхронизируем меню с глобальными структурами
    update_alert_menu();        // Обновляем меню alerts
    //printf("После чтение из EEPROM значение Tw2=%u\r\n", prim_par.Tw2);
    // printf("Было %u, стало после чтение из EEPROM size=%u\r\n", sizeof(prim_par), size_prim_par);
    #ifndef NODEBUG
    // Если есть термометры, то выводим их значение
    printallterms(0);
    #endif
    lcd_primary_screen();       // выводим стартовую картинку на экранчик
    // if (PINC.6) PORTD |= (1<<4); else PORTD &= ~(1<<4);
    // if (PINC.7) PORTD &= ~(1<<5); else PORTD |= (1<<5);
    // if (PINC.5) PORTD &= ~(1<<5); else PORTD |= (1<<5);
    if (prim_par.season) signal_white(ON); else signal_white(OFF);
    FAN_SPEED = prim_par.fan_speed;
    fan_speed = FAN_SPEED;
    if (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
    POM_T = termometers[0].t;
    UL_T = termometers[1].t;
    WIN_T = termometers[2].t;
    WOUT_T = termometers[3].t;
    tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;  // вычисление диапазона работы ограничителя крана по температуре
    mode.k_angle_limit = ((TAP_ANGLE_LIMIT * 1000) / tmp_delta); // вычисление коэффициента ограничения крана для заданных настроек
     if (UL_T < TA_IN_NOLIMIT) {    // Вычисление угла ограничения (UL_T < TA_IN_NOLIMIT)
        tap_angle_min = prim_par.tap_angle + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;   // вычисление ограничения крана по температуре воздуха на входе и коэффициенту mode.k_angle_limit
        printf(" tap_angle_min %u .   Kоэффициент :%d\r\n",  tap_angle_min, mode.k_angle_limit);
    };
    TAP_ANGLE = (TAP_ANGLE < tap_angle_min) ? tap_angle_min : prim_par.tap_angle;
    printf(" Установка крана %u .   prim_par.tap_angle :%d\r\n",  TAP_ANGLE, prim_par.tap_angle );
    while(1) {
        // ВНИМАНИЕ! НИЖЕ ДО ОКОНЧАНИЯ ЦИКЛА WHILE КОД НЕ ДОБАВЛЯТЬ!!!
        check_serial();
        poll_keys();
        // if (event == ev_none) check_peripheral();
        /* if (0 == 1) */ check_peripheral();
        /* if (0 == 1) */ event_processing();
        /* if (0 == 1) */ mode_processing();
    }; // while (1)

} // void main(void)
// Функция опроса перефирии и генерации событий по результатом опроса, а также по результатам МАТАнализа
void check_peripheral(void) {
    // В этой функции запрещено (!) устанавливать режимы
    // Обрабатываем поворот valcoder'а
    if ((abs(valcoder)-VALCODER_SENSITY) >= 0) {        // Если сработал valcoder
        if (valcoder < 0)
            event = ev_left;
        else
            event = ev_right;
    }
    // Если поворота не было, опрашиваем другие порты
    if (CHECK_EVENT && (!KEY_ENTER)) event = ev_enter;
    if (CHECK_EVENT && (!KEY_CANCEL)) event = ev_cancel;
    // Обслуживаем кнопку стоп
    //if (CHECK_EVENT && (!timer_stop) && (!KEY_STOP)) event = ev_stop;
    if (CHECK_EVENT &&  (!KEY_STOP)) event = ev_stop;
    // Обслуживаем кнопку старт, Проверяя при этом наличие каких-либо аварий
    if (CHECK_EVENT && (IS_ALERT == 0) && (!KEY_START)) event = ev_start;
    // Для более "живой" клавиатуры выходим, если событие сгенерировано
    // Проверка свитчиков
    if (CHECK_EVENT && (!prim_par.alert_status[0]) && (KEY_ALARM1)) event = ev_alarm1; // Пожар, перегрев вентилятора, авария частотника
    if (CHECK_EVENT && (!prim_par.alert_status[1]) && (KEY_ALARM2)) event = ev_alarm2; // Угроза замораживания от внешнего датчика
    if (CHECK_EVENT && (!prim_par.alert_status[11]) && (!KEY_FILTER)) event = ev_filter; // Загрязнение фильтра.
    // Проверка термометров
    if (CHECK_EVENT && (!prim_par.alert_status[7]) && (termometers[0].err >= MAX_OFFLINES))
        // printf("Нет термометра В1 (Помещение): %d, err=%d", prim_par.alert_status[7], termometers[0].err);
        event = ev_term1_nf;
    if (CHECK_EVENT && (!prim_par.alert_status[8]) && (termometers[1].err >= MAX_OFFLINES))
        // Нет термометра В 2 - Улица
        event = ev_term2_nf;
    if (CHECK_EVENT && (!prim_par.alert_status[9]) && (termometers[2].err >= MAX_OFFLINES))
        // Нет термометра В 3 - Подача
        event = ev_term3_nf;
    if (CHECK_EVENT && (!prim_par.alert_status[10]) && (termometers[3].err >= MAX_OFFLINES))
        // Нет термометра В 4 - Обратка
        event = ev_term4_nf;
    // Здесь осуществляет матанализ для генерации событий
    if (UL_T < TA_IN_NOLIMIT) {
        if (CHECK_EVENT && !(prim_par.alert_status[8] || prim_par.alert_status[2]) &&
            (termometers[1].t < (prim_par.TA_in_Min-5))) // Температура на улице ниже критической  на 5 градусов.UL_T
            event = ev_freezing1;
        if (CHECK_EVENT && !(prim_par.alert_status[7] || prim_par.alert_status[3]) &&
            (termometers[0].t < prim_par.TA_out_Min) && (termometers[1].t < prim_par.TA_out_Min))  // Температура в помещения ниже критической POM_T
               // Если тепература на улице выше критической POM_T
            event = ev_freezing2;
        if (CHECK_EVENT && !(prim_par.alert_status[10] || prim_par.alert_status[5]) &&
            (termometers[3].t < prim_par.TW_out_Min)) // Температура воды обратки ниже критической WOUT_T
            event = ev_freezing3;
    }
    if (CHECK_EVENT && (mode.initrun)) {
        mode.initrun -= 4;
        if ((mode.initrun) && (IS_ALERT == 0)) event = ev_start;
       else
            event = ev_stop;
        mode.initrun = 0;
    }
    #ifndef NODEBUG
    // if (event) printf ("Сгенерировано новое событие: %u\r\n", (unsigned char)event);
    //if (event) printf ("-");
    #endif
}
// Обработка событий в системе
void event_processing(void) {
    // Только здесь можно менять режимы. Но здесь нельзя опрашивать периферию, делать матанализ
    #ifndef NODEBUG
    // if (event != ev_none)
        // printf ("%02u:%02u Текущее событие = %u\r\n", s_dt.cMM, s_dt.cSS, (unsigned char)event);
    #endif
    // Также здесь выполняются инициализационные действия для процессов: Вкл./выкл индикатор, запустить бибикалку, нарисовать строку меню и т.п.
    switch (event) {
        case ev_secunda:                // Обрабатываем ежесекундное событие.
            MAIN_T = read_term(0);      // Выводим информацию о главном термометре !!!
            POM_T = read_term(0);
            UL_T = read_term(1);
            WIN_T = read_term(2);
            WOUT_T = read_term(3);
            switch (mode.menu) {
                case 0: lcd_primary_screen(); break;
                case 1: lcd_menu(0); break;
                default: ;
                // case 2: lcd_edit(0); break;
            }
            //if (!mode.print && (mode.run ==1)) printf("-");
            if (mode.print && (mode.run ==1)) printf("-");
            //if (mode.print && (mode.run ==3) && (prim_par.season)) printf (" ПУСК Зима: %u\r\n", time_integration);
            //if (mode.print && (mode.run ==3) && !(prim_par.season)) printf (" ПУСК Лето: %u\r\n", time_cooling);
            //if (mode.print && (mode.run ==0)) printf ("До следующего измерения СТОП: %u\r\n", timer_fan);
            event = ev_none;            // Очищаем событие
            break;
        case ev_left:                   // printf ("Обрабатываем прокрутку valcoder влево\r\n");
        case ev_right:                  // printf ("Обрабатываем прокрутку valcoder вправо\r\n");
            // Запускаем таймер инактивности
            //timer1_valcoder = TIMER_INACTIVE;
            timer1_valcoder = TIMER_INACTIVE;
            valcoder = VALCODER_NO_ROTATE;
            signal_buz(OFF);
            // printf ("Обрабатываем прокрутку valcoder (%d), в режиме %d - ", event-2, mode.menu);
            switch (mode.menu) {
                case 0:
                    sync_set_par(SYNC_TO_MENU);
                    lcd_menu(mode.menu++); break;   // Выводим меню без изменения позиции printf ("entering...");
                case 1: lcd_menu(event-2); break;       // printf ("navigating...");
                case 2: lcd_edit(event-2); break;       // printf ("editing...");
                default: ;                              // printf ("defaulting...");
            }
            // printf ("\r\n");
            event = ev_none;            // Очищаем событие
            break;
        case ev_enter:                  // Если нажат Enter
            // LAMP_ECHO_PORT |= (1<<LAMP_ECHO_PIN); timer1_lamp = ENTER_CANCEL_OVERFLOW; if (mode.menu <= 2)
            key_treated[2]=1;
            event = ev_none;            // Очищаем событие
            signal_buz(SHORT);
            timer1_valcoder = TIMER_INACTIVE;     //Запускаем таймер инактивности
            switch (mode.menu) {
                // lcd_primary_screen();
                // Обрабатываем нажатие enter c учетом того, что значение mode.menu еще старое
                case 0:
                    sync_set_par(SYNC_TO_MENU);
                    lcd_menu(mode.menu++); break;           // Если находились в главном экране, запускаем прорисовку меню
                case 1: lcd_init_edit(); break;             // Если находились в меню, то анализируем
                case 2: lcd_save_edit(); mode.menu = 1; break;
            };
            break;
        case ev_timer:
            // Запускаем таймер инактивности
            if (mode.menu) timer1_valcoder = TIMER_INACTIVE; // TIMER_INACTIVE;
        case ev_cancel:
            if (event == ev_cancel) {
                signal_buz(SHORT);
                key_treated[3]=1;
                timer1_valcoder = TIMER_INACTIVE;     //Запускаем таймер инактивности ()
            }
            event = ev_none;            // Очищаем событие
            // print_prim_par((unsigned char *)&prim_par, sizeof(prim_par));
            switch (mode.menu) {
                case 0: lcd_primary_screen(); break;
                case 1: --mode.menu; lcd_esc_edit(); break;
                case 2: --mode.menu; lcd_esc_edit(); break;
                case 3: mode.menu = 1; break;
            };
             lcd_clrscr();
            break;
        case ev_start:      // пуск установки
            key_treated[1]=1;
            printf ("Нажата кнопка ПУСК. ");
            switch (mode.run) {
                case 0: { // Процесс старта вентилятора
                    if (prim_par.season) {
                        signal_white(ON);
                        mode.pomp = 1;
                        mode.run = 1;                // Устанавливаем режим Прогрев
                        timer_start = TIME_START;
                        //timer_start = prim_par.T_z; // TIMER_INACTIVE  ...Запускаем таймер STRT
                        TAP_ANGLE = PWM_MAX;
                        printf("Включен режим Прогрев. LIMIT = %d\r\n", mode.k_angle_limit);
                        signal_green(SHORT);
                    } else {
                        signal_white(OFF);
                        mode.pomp = 0;
                        mode.run = 3;
                        mode.fan = 1;
                        signal_green(ON);
                        printf("Включен режим Пуск\r\n");
                    };
                    FAN_SPEED = prim_par.fan_speed;
                    if (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
                    //count_fan = 0;
                    time_integration = 0;
                    //time_cooling = TIME_COOLING_MAX;
                    time_cooling = prim_par.T_z;
                    signal_buz(LONG);
                    break;
                } //mode.run=0 ;
                case 1: signal_buz(SHORT); signal_green(SHORT); printf("Режим Прогрев.\r\n");
                    break; // mode.run=1;
                case 2: signal_buz(SHORT); signal_green(LONG); printf("Режим Остановки.\r\n");
                    break; // mode.run=2 ;
                case 3: signal_buz(SHORT); signal_green(ON); printf("Режим Пуск.\r\n");
                    break; // mode.run=3
            default: break;
            };
            event = ev_none;
            break;
        case ev_stop:
            key_treated[0]=1;
            printf ("Нажата кнопка СТОП. ");
            switch (mode.run) {
                case 0: signal_buz(SHORT); time_integration = 0; signal_green(OFF); printf(" Режим СТОП.\r\n"); break; //mode.run=0 ;
                case 1: mode.run = 2;
                        timer_start = 0;
                        signal_green(SHORT);  signal_buz(LONG);
                        printf ("Включен режим Остановки\r\n");
                        timer_stop = TIME_STOP;    // Запускаем таймер STOP
                        time_integration = 0;
                    break; //mode.run=1;
                case 2: signal_buz(SHORT); time_integration = 0; printf("Режим Остановки.\r\n"); break; //mode.run=2 ;
                case 3: mode.run = 2;
                        signal_green(LONG); signal_buz(LONG);
                        printf ("Включен режим Остановки\r\n");
                        time_cooling = 0;
                        timer_stop = TIME_STOP;    // Запускаем таймер STOP
                        time_integration = 0; break; //mode.run=3
                default: break;
            }
            mode.fan = 0;  // Выключение Вентилятора
            event = ev_none;
            break;
        case ev_alarm1:   // Пожар, перегрев вентилятора, авария частотника
            signal_red(ON); signal_buz(MEANDR);
            mode.run = 0;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1; // Проверить
            TAP_ANGLE = PWM_MAX;
            signal_green(OFF);
            alarm_reg(0, 1, get_alert_str(0), 0);
            printf ("Авария: %s\r\n", get_alert_str(0));
            event = ev_none;
            break;
        case ev_alarm2:   // Угроза замораживания от внешнего датчика
            signal_red(ON); signal_buz(MEANDR);
            mode.run = 0;
            mode.fan = 0;
            mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            alarm_reg(0, 1, get_alert_str(1), 1);
            printf ("АВАРИЯ: %s\r\n", get_alert_str(1));
            event = ev_none;
            break;
        case ev_freezing1:  // Температура на улице ниже критической UL_T (Если не повторяется в течении часа восстановление)
            alarm_reg(0, 1, get_alert_str(2), 2);

            signal_red(SHORT);
            signal_buz(MEANDR);
            //signal_green(SHORT);
            mode.run = 0; // Режим оттаивания
            mode.fan = 0;

            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(2));
            event = ev_none;
            break;
        case ev_freezing2: // Температура в помещения ниже критической POM_T
            alarm_reg(0, 1, get_alert_str(3), 3);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = 0;  // Режим оттаивания
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(3));
            event = ev_none;
            break;
        case ev_freezing3: // Температура воды обратки ниже критической POM_T
            alarm_reg(0, 1, get_alert_str(5), 5);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = 0;  // Режим оттаивания
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(5));
            event = ev_none;
            break;
        case ev_term1_nf:   // Термометр В1 помещение
            alarm_reg(MAX_OFFLINES, termometers[0].err, get_alert_str(7), 7);
            signal_red(ON); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = 0;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(7));
            event = ev_none;
            break;
        case ev_term2_nf:  // Термометр В2 Улица
            alarm_reg(MAX_OFFLINES, termometers[1].err, get_alert_str(8), 8);
            signal_red(LONG); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = 0;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(8));
            event = ev_none;
            break;
        case ev_term3_nf:  // Термометр В3 Подача
            // Нет его, ну и хрен с ним
            alarm_reg(MAX_OFFLINES, termometers[2].err, get_alert_str(9), 9);
            printf ("ПРЕДУПРЕЖДЕНИЕ: %s\r\n", get_alert_str(9));
            event = ev_none;
            break;
        case ev_term4_nf:  // Термометр В4 Обратка
            alarm_reg(MAX_OFFLINES, termometers[3].err, get_alert_str(10), 10);
            signal_red(LONG); signal_buz(MEANDR);
            signal_green(OFF);
            mode.run = 0;
            mode.fan = 0;
            if (prim_par.season) mode.pomp = 1;
            TAP_ANGLE = PWM_MAX;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(10));
            event = ev_none;
            break;
        case ev_filter:  // Загрязнение фильтра.
            alarm_reg(0, 1, get_alert_str(11), 11);
            event = ev_none;
            printf ("АВАРИЯ: %s\r\n", get_alert_str(11));
            signal_red(SHORT);
            break;
        default:
            break;
    };
}
// Обработка режима работы системы
void mode_processing(void) {
    // В ЭТОЙ ФУНКЦИИ ЗАПРЕЩЕНО ИЗМЕНЕНИЕ РЕЖИМОВ И ГЕНЕРАЦИЯ СОБЫТИЙ. ТОЛЬКО ПОДДЕРЖКА!
    int water_out_bound;
    int water_out_bound_1;
    static int error_w_stop = 0;

    SET_T = prim_par.TA_out_prs;
    if (mode.run == ENGINEERING) {
    } else {
        OCR0 = (unsigned char)TAP_ANGLE;
        OCR2 = (unsigned char)FAN_SPEED;// Задействованно для включения охладителя
        //COOLING1 = mode.cooling1;
        //COOLING2 = mode.cooling2;
        MOTOR = mode.fan;
        FAN_VAR = mode.fan;
        POMP = mode.pomp;
        POMP_VAR = mode.pomp;

        switch (mode.run) {
            case 0:
                time_integration = 0;
                if (prim_par.season) {
                // Вычисление ограничения закрытия крана TAP_ANGLE = tap_angle_min
                    if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
                        //int tmp_delta = abs(prim_par.TA_in_Min) + TA_IN_NOLIMIT;
                        //mode.k_angle_limit = (TAP_ANGLE_LIMIT / tmp_delta) * 1000;
                        tap_angle_min =prim_par.tap_angle + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;   // вычисление ограничения крана по температуре воздуха на входе и коэффициенту mode.k_angle_limit
                        if (tap_angle_min < 100) tap_angle_min = 100 ;  // Принудительный запрет закрытия крана меньше 40% в режиме стоп
                    }
                    // Процесс поддержания температуры калорифера в режиме СТОП Зимой
                    error_w_stop = (prim_par.TW_out_Stop - WOUT_T)/100;
                    water_out_bound = prim_par.TW_out_Stop - 200;
                    water_out_bound_1 = prim_par.TW_out_Stop - 500;
                    // Разогрев калорифера
                    if ((WOUT_T <= water_out_bound) && (timer_fan == 0)) {
                        timer_fan = TIME_COOL_STOP;
                        // forcheck_event = ev_pomp;
                        TAP_ANGLE = TAP_ANGLE + error_w_stop;
                        if (TAP_ANGLE < tap_angle_min) TAP_ANGLE = tap_angle_min ;   // TAP_ANGLE - Состояние выхода на PWM
                        if (TAP_ANGLE > PWM_MAX) TAP_ANGLE = PWM_MAX;         // TAP_ANGLE - Состояние выхода на PWM
                        if (WOUT_T < water_out_bound_1) {
                            //if (!mode.print) printf("Разогрев калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                            // if (mode.print) printf("Разогрев калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                            signal_white(LONG);
                            mode.pomp = 1;
                        } else {
                            signal_white(ON);
                            mode.pomp = 0;
                        }
                        if (mode.print) printf("Разогрев калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                    }
                    // Охлаждение калорифера
                    water_out_bound = prim_par.TW_out_Stop + 250;
                    water_out_bound_1 = prim_par.TW_out_Stop + 500;
                    if ((WOUT_T > water_out_bound) && (timer_fan == 0)) {
                        timer_fan = TIME_COOL_STOP;
                        // printf("Угол крана расчетный :%d  \r\n",   TAP_ANGLE);
                        if (TAP_ANGLE < tap_angle_min) TAP_ANGLE = tap_angle_min;         // TAP_ANGLE - Состояние выхода на PWM
                        if (TAP_ANGLE > PWM_MAX) TAP_ANGLE = PWM_MAX;         // TAP_ANGLE - Состояние выхода на PWM
                        if ((WOUT_T >= water_out_bound) && (mode.pomp == 1)) {
                            //if (!mode.print) printf("Охлаждение калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                            //if (mode.print) printf("Охлаждение калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                            signal_white(ON);
                            mode.pomp = 0;
                        };
                        if (WOUT_T >= water_out_bound_1)  TAP_ANGLE = TAP_ANGLE + error_w_stop;
                        if (mode.print) printf("Охлаждение калорифера: %d, Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                    }
                    // Поддержание калорифера
                    if (timer_fan == 0) {
                        timer_fan = TIME_COOL_STOP;
                        if (WIN_T >= prim_par.TW_out_Stop)  mode.pomp = 0;
                        //if (!mode.print) printf("Поддержание калорифера: %d,  Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                        if (mode.print) printf("Поддержание калорифера: %d,  Угол крана расчетный :%d, Угол крана измеренный :%d, угол ограничения: %d, t обратки :%d  \r\n",  error_w_stop, TAP_ANGLE, ADC_VAR1, tap_angle_min, WOUT_T/10);
                    }
                } else {
                    signal_white(OFF);
                    mode.pomp = 0;
                }
                signal_green(OFF);
                time_cooling = 0;
                //mode.cooling1 = 0;
                //mode.cooling2 = 0;
                break;
            //case 1:
            //         if (!mode.print) printf("-");
            //    break;
            case 3:
                //if (IS_ALERT == 0) {
                    if (prim_par.season) {
                        signal_white(ON);
                        //  Простой алгоритм обработки
                        if (time_integration == 0) {
                            if (POM_T < (SET_T - (prim_par. Kd*10)))  update_P(SET_T - POM_T);// Разница между T Уст и Т помещения
                            if (POM_T > (SET_T+(prim_par. Kd*10)))  update_P(SET_T - POM_T);// Разница между T Уст и Т помещения
                            time_integration = prim_par.T_int;
                            //mode.cooling1 = 0;
                            //mode.cooling2 = 0;
                            time_cooling = 0;
                            signal_green(ON);
                        }
                      }
                    else {
                        signal_white(OFF);
                        // Для установок с водяным охладителем !
                        //Запуск охладителя
                        if (time_cooling == 0) {
                            //if (POM_T > (SET_T + (prim_par. Ki*10))) {
                            if (UL_T > SET_T) {
                                if (POM_T > SET_T) {
                                    //if ( mode.cooling1 == 0) {
                                        //mode.cooling1 = 1;
                                    //    signal_green(SHORT);
                                    //} else {
                                    //     mode.cooling2 = 1;
                                    //    signal_green(LONG);
                                   // }
                                if (mode.print) printf("Включен охладитель.  POM_T :%d\r\n",   POM_T);
                                }
                            }
                        //Остановка охладителя
                            if (POM_T < (SET_T - (prim_par. Ki*10)) || (UL_T < SET_T) )  {
                               //if ( mode.cooling1 == 1) {
                               //     mode.cooling1 = 0;
                               //     signal_green(SHORT);
                               // } else {
                               //     mode.cooling2 = 0;
                               //     signal_green(ON);
                               // }
                            if (mode.print) printf("Отключен охладитель. Разность температур - дельта: %d, POM_T :%d\r\n",  (SET_T - (prim_par. Ki*10)), POM_T);
                            }
                            time_cooling = prim_par.T_z;
                            //time_cooling = TIME_COOLING_MAX;
                            //mode.cooling1 = 0;
                            //mode.cooling2 = 0;
                            //count_cooling = 0;
                        }

                    }

                    /*
                    if (1 == 0) {
                        update_PID(SET_T - POM_T, -5000, 5000); // Разница между T Уст и Т помещения
                        time_integration = prim_par.T_int;

                        if (!prim_par.season && ){
                           if ((SET_T+4+ - POM_T

                        }
                       //Внимание ОТКЛЮЧЕНО РЕГУЛИРОВАНИЕ СКОРОСТИ ВЕНТИЛЯТОРА!!!
                        if (prim_par.season && (UL_T < TA_IN_NOLIMIT)) {
                            // Вычисление ограничения закрытия крана TAP_ANGLE = tap_angle_min
                            //tap_angle_min = ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;
                            //printf("Пересчет ограничения: %d, Ул. т :%d, Коэффициент :%d \r\n",  (TA_IN_NOLIMIT - UL_T), UL_T, mode.k_angle_limit);
                            // Вычисление скорсти вентилятора
                            //#define FAN_SPEED_T_UP 300
                            //#define FAN_SPEED_T_DOWN 100
                            if ((TAP_ANGLE == PWM_MAX) && ((POM_T - FAN_SPEED_T_DOWN) < SET_T)) {
                                count_fan++;
                                if (count_fan == COUNT_FAN_MAX) {
                                   FAN_SPEED = FAN_SPEED - FAN_SPEED_STEP;
                                   count_fan = 0;
                                   if  (FAN_SPEED <= FAN_SPEED_MIN) FAN_SPEED = FAN_SPEED_MIN;
                                //if (!mode.print) printf("Понижение скорости вентилятора расчетное : %d, измеренное : %d, Счетчик циклов :%d, POM_T :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, POM_T);
                                if (mode.print) printf("Понижение скорости вентилятора расчетное : %d, измеренное : %d, Счетчик циклов :%d, POM_T :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, POM_T);
                                }
                            } else  {// count_fan = 0;
                                if ((FAN_SPEED < prim_par.fan_speed) && ((POM_T + FAN_SPEED_T_UP) > SET_T)) {   //&& (TAP_ANGLE == tap_angle_min
                                    count_fan++;
                                    if (count_fan == COUNT_FAN_MAX) {
                                       FAN_SPEED = FAN_SPEED + FAN_SPEED_STEP;
                                       count_fan = 0;
                                       //if  (prim_par.fan_speed <= FAN_SPEED) FAN_SPEED = prim_par.fan_speed;
                                       if  (FAN_SPEED > prim_par.fan_speed ) FAN_SPEED = prim_par.fan_speed;
                                       //if (!mode.print) printf("Увеличение скорости вентилятора расчетная: %d, измеренная : %d, Счетчик циклов :%d, Заданная скорость :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, prim_par.fan_speed);
                                       if (mode.print) printf("Увеличение скорости вентилятора расчетная: %d, измеренная : %d, Счетчик циклов :%d, Заданная скорость :%d \r\n",  FAN_SPEED, ADC_VAR2, count_fan, prim_par.fan_speed);
                                    }

                                } else count_fan = 0;
                            }
                            //if (!mode.print) printf("Скорость вентилятора расчетная: %d, измеренная : %d, Заданная скорость: %d, POM_T: %d, TAP_ANGLE_MIN = %d \r\n",  FAN_SPEED, ADC_VAR2, prim_par.fan_speed, POM_T, tap_angle_min);
                            if (mode.print) printf("Скорость вентилятора расчетная: %d, измеренная : %d, Заданная скорость: %d, POM_T: %d, TAP_ANGLE_MIN = %d \r\n",  FAN_SPEED, ADC_VAR2, prim_par.fan_speed, POM_T, tap_angle_min);
                        }

                    }
                    */
                    //signal_green(ON);
                // }
                break;
            default:
                break;
        };
    }
}
void check_range(void) {
    // Проверяем на принадлежность диапазону
    // Аппаратное ограничение закрытия крана по напряжению 2 вольта
    if  ( TAP_ANGLE < prim_par.tap_angle ) TAP_ANGLE = prim_par.tap_angle;
    // Вычисление ограничения закрытия крана TAP_ANGLE = tap_angle_min
    if (UL_T < TA_IN_NOLIMIT)  tap_angle_min = prim_par.tap_angle + ((long int)((TA_IN_NOLIMIT - UL_T) * mode.k_angle_limit))/1000;
    if (!mode.print) printf("Пересчет ограничения: %d, Ул. т :%d, Коэффициент :%i, ограничение снизу :%d, UL_T :%i  \r\n",  (TA_IN_NOLIMIT - UL_T), UL_T, mode.k_angle_limit,prim_par.tap_angle, UL_T);
    if (TAP_ANGLE < tap_angle_min)
        TAP_ANGLE = tap_angle_min;
    else
        if (TAP_ANGLE > PWM_MAX)
            TAP_ANGLE = PWM_MAX;
    // TAP_ANGLE - Состояние выхода на PWM
}
// Функция пропорционального регулирования
void update_P(int error) {
    // TAP_ANGLE = TAP_ANGLE + error/100;          // TAP_ANGLE - Состояние выхода на PWM
    if ((TAP_ANGLE >=0) && (TAP_ANGLE <= PWM_MAX))
        TAP_ANGLE = TAP_ANGLE + ((error* prim_par.Ku)/1000);
    check_range();
    //if (mode.print == 2) printf("Разность температур: %d, Процент_ANGLE :%d, TAP_ANGLE:%d, ANGLE CALC:%d,KU:%d \r\n",  error, ((TAP_ANGLE*100)/0xFF),TAP_ANGLE,((error / 100) * prim_par.Ku),prim_par.Ku);
    if (!mode.print) printf("Разность температур: %d, Процент_ANGLE :%d, TAP_ANGLE:%d, ANGLE CALC:%d,KU:%d \r\n",  error, (((TAP_ANGLE -  prim_par.tap_angle)*100)/(PWM_MAX -  prim_par.tap_angle)),TAP_ANGLE,((error / 100) * prim_par.Ku),prim_par.Ku);
}
/*
void update_PID(int error, int iMin, int iMax) {
    static int dState = 0, iState = 0;
    int pTerm, dTerm, iTerm;
    int result;

    pTerm = (prim_par.Ku * error) / 10;    // calculate the proportional term
    iState += error;                // calculate the integral state with appropriate limiting
    // Проверяем на принадлежность диапазону

        if ((iState > iMax) || (iState < iMin)) {
            if (iState > iMax)
                iState = iMax;
            else
                iState = iMin;
        }
        iTerm = (prim_par.Ki * iState) / 100;    // calculate the integral term
        dTerm = prim_par.Kd * (POM_T - dState);
        dState = POM_T;
        result = pTerm + iTerm - dTerm;
        TAP_ANGLE += result / 100;
        check_range();

    if (!mode.print)
    // Температура заданная, Температура измеренная, Delta, Угол крана расчетный, Угол кранаизмеренный, Угол ограничения, result, pTerm, iTerm, dTerm, Обороты расчетные,Обороты измеренныее\r\n"
        printf("%02u:%02u:%02u, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d\r\n",
            s_dt.cHH, s_dt.cMM, s_dt.cSS,
            SET_T, POM_T, error,
            TAP_ANGLE, ADC_VAR1, tap_angle_min, result,
            pTerm, iTerm, dTerm,
            FAN_SPEED, ADC_VAR2);
    else
        printf("Delta (x100) = %d, result = %d, TAP_ANGLE = %d, pTerm = %d, iTerm = %d, dTerm = %d\r\n",
            error, result, TAP_ANGLE, pTerm, iTerm, dTerm, POM_T);
}
*/
// Печать всех термометров. mode - режим печати адресов термометров. Если не 0 - печатать адреса.
void printallterms(unsigned char print_addr) {
    int term;
    register byte i;
    unsigned char resolution;
    signed char th, tl;
    unsigned char *spd, *address_line;

    if(!ds1820_devices) return;			// если термометры не обнаружены - просто выходим из функции
    spd = ds1820_show_spd();
    if (print_addr) printf("Адреса термометров(ds1820_rom_codes): ");
    for(i=0; i<ds1820_devices; i++) {
        ds1820_get_resolution(&ds1820_rom_codes[i][0], &resolution);
    	term = ds1820_temperature(ds1820_rom_codes[i]);
    	// эквивалентно term = ds1820_temperature(&ds1820_rom_codes[i][0]);
        tl = th = 0;
        ds1820_get_alarm(ds1820_rom_codes[i], &tl, &th);
        // эквивалентно ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
        if (print_addr) {
            address_line = address_to_LCD (ds1820_rom_codes[i]);
            printf("[%s:%d:%d] ", address_line, tl, th);
        } else {
            printf(" t%-u = %i(%-i.%-u)C[%02X%02X:%02x]%d:%d; ", i+1, term, term/100, abs(term%100), spd[1], spd[0], resolution, tl, th);
        }
    }
    printf("\r\n");
}
// Печать всех новых термометров
void printnewterms(void) {
    if (mode.new_terms) {
        register byte i;
        unsigned char *address_line;

        printf("Неизвестных термометров(mode.ufo) - %d: ", mode.new_terms);
        for (i = 0; i < mode.new_terms; i++) {
            address_line = address_to_LCD (mode.ufo[i]);
            printf("[ %s ] ", address_line);
        }
        printf("\r\n");
    } else
        printf("Новых термометров не найдено\r\n");
}
// Функция высвечивающая версию программы на LCD и на терминале
void start_screen(unsigned char init) {
    lcd_clrscr();
    printf("Старт Зеньков П3. %u.%02u. Найдено %u термометров.\r\n", MAJOR_VERSION, MINOR_VERSION, ds1820_devices);
    lcd_command(LCD_DISP_ON);       // Убираем курсор с LCD
    lcd_gotoxy(0,0);        // Устанавливаем курсор в позицию 0 первой строки
    sprintf(linestr, "MTECH %u.%02u",MAJOR_VERSION, MINOR_VERSION );
    lcd_puts(linestr);
    lcd_gotoxy(0,1);                // Устанавливаем курсор в позицию 0 строки 2
    switch (init) {
        case 1:
            sprintf(linestr, "FULL RESET"); break;
        case 2:
            sprintf(linestr, "FACTORY"); break;
        case 3:
            sprintf(linestr, "ONLY NEW TERMS"); break;
        case 0:
        default:
            sprintf(linestr, "Robowater ..."); break;
    }
    lcd_puts(linestr);
}

// Полностью прорисовать главный экран
void lcd_primary_screen(void) {
    char run_mod [8];
    register unsigned char i;
    unsigned char c_alerts = 0;
    unsigned char sign = '+';
    int lcd_term = main_menu[0].val_data;
    #ifndef NODEBUG
    // printf ("%02u:%02u Начало прорисовки экрана...", s_dt.cMM, s_dt.cSS);
    #endif
    for (i = 0; i < MAX_ALERTS; i++) {
        // 9-й алерт (отсутствие 3-го термометра) нам не интересен
        if (prim_par.alert_status[i] && i != 9) c_alerts++;
    }
    switch (mode.run) {
        case 0: sprintf(run_mod, "СТОП   "); break; //mode.run=0 ;
        case 1: sprintf(run_mod, "ПРОГРЕВ"); break; //mode.run=1;
        case 2: sprintf(run_mod, "ОСТАНОВ"); break; //mode.run=2 ;
        case 3: sprintf(run_mod, "ПУСК   "); break; //mode.run=3
        default: break;
    };
    if (c_alerts)
        sprintf(run_mod, "Авария ");
    else {
        signal_red(OFF);  signal_buz(OFF); // signal_green(OFF);
    }
    lcd_command(LCD_DISP_ON);       // Убираем курсор с LCD
    lcd_gotoxy(0,0);        // Устанавливаем курсор в позицию 0 первой строки
    sprintf(linestr, "%02u:%02u:%02u %02u.%02u  ", s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo);
    lcd_puts(linestr);
    lcd_gotoxy(0,1);                // Устанавливаем курсор в позицию 0 строки 2
    // Выводим информацию о термометрах
    if(ds1820_devices) {
        sign = (lcd_term < 0) ? '-' : '+';
        sprintf(linestr, "t=%c%02u.%01uC %s", sign, abs(lcd_term)/100, (abs(lcd_term)%100)/10, run_mod);
        // linestr += "panarin";
    } else {
        sprintf(linestr, "Нет термометров");
    }
    lcd_puts(linestr);
    #ifndef NODEBUG
    // printf ("конец в %02u:%02u\r\n", s_dt.cMM, s_dt.cSS);
    #endif
}
// Печать структуры prim_par
void print_prim_par(unsigned char *struct_data, unsigned char size) {
    register unsigned char i;
    unsigned char s_byte;
    printf("Состояние PRIM_PAR [%d bytes]: ", size);
    for (i=0; i<size; i++) {
        s_byte = *struct_data++;
        if (i < (size - 36))
            printf(" %d", s_byte);
        else
            printf("%02X ", s_byte);
        if (i == size - 37) printf("\r\nTERMS: ");
        //
    }
    printf("\r\n");
}
// Функция инициализирующая новые термометры. !!! Надо здесь разбираться.
void init_new_terms(unsigned char number) {
    register unsigned char i;
    unsigned char is_found = 0, is_new = 0;
    signed char th, tl, index;
    unsigned char newterms;
    // Ищем по новой все термометры
    newterms = w1_search(0xf0, ds1820_rom_codes);
    delay_ms (DS1820_ALL_DELAY);
    printallterms(0);
    // Вычитываем TH, TL. Иными словами вычисляем индекс.
    for(i = 0; i < newterms; i++) {
        ds1820_get_alarm(&ds1820_rom_codes[i][0], &tl, &th);
        index = (th == OUR_SIGNATURE) ? abs(tl) - 2 : -1;
        // Проверяем на совпадение с number
        if (!is_found) is_found = (index == number) ? i + 1 : 0;
        // Проверяем свободность
        if (!is_new) is_new = (index == -1) ? 0 : i + 1;
    }
    #ifndef NODEBUG
    printf ("Задан N%u (number), нашли N%u (is_found), свободный N%u (is_new)\r\n", number, is_found, is_new);
    #endif
    // Если есть, то выходим из функции с соотвествующим сообщением
    if (is_found) {
        printf ("Термометр N%u существует по порядковому номеру %u\r\n", number, is_found);
        return;
    }
    // Если нет свободных, выходим с соотвествующим сообщением
    if (!is_new) {
        printf ("Свободных термометров нет\r\n");
        return;
    }
    // Если нашли, то прописываем TH, TL
    tl = -2 - number; i = is_new - 1;
    #ifndef NODEBUG
    printf ("Новый TL = %d[%02x] по номеру %u\r\n", tl, tl, i);
    #endif
    if (ds1820_set_THTL(ds1820_rom_codes[i], tl, OUR_SIGNATURE)) {
        printf ("Проинициализировали термометр N%u по порядковому номеру %u\r\n", number, is_new);
        delay_ms (DS1820_ALL_DELAY);
        // Запускаем перезагрузку
        #asm
            JMP  __RESET
        #endasm
    }
    printf ("Не могу проинициализировать новый термометр N%u\r\n", number);
    return;
}
void ask_turn_off(void) {
    printf("Выключите питание и подключите все оборудование\r\n");
    while(1);
}
// Установка номера термометра принудительно на 1-й попавшийся.
void init_force_term(signed char number) {
    // Описание локальных переменных
    byte inbyte = 0;
    // Начало
    #asm("cli")
    printf("Отключите все термометры, кроме [%02x] и нажмите клавишу SHIFT-'~'\r\nВыход из режима - выключение питания\r\n", number);
    do
        if (UCSRA & RX_COMPLETE) inbyte = UDR;
    while (inbyte != 0x7E);
    ds1820_devices = w1_search(0xf0, ds1820_rom_codes);
    delay_ms (DS1820_ALL_DELAY);
    printallterms(0);
    if (!ds1820_devices)
        printf("Термометры не найдены\r\n");
    else
        if (ds1820_set_THTL(ds1820_rom_codes[0], number, OUR_SIGNATURE))
            printf("Термометр [%02x] успешно проинициализирован\r\n", number);
        else
            printf("Не могу проинициализировать новый термометр [%02x]\r\n", number);
    ask_turn_off();
    #asm("sei")
}
void set_term(signed char number, signed char sign) {
    if (sign == OUR_SIGNATURE) {
        if (ds1820_set_THTL(ds1820_rom_codes[number], -2 - number, sign))
            printf("Термометр [%02x] успешно проинициализирован\r\n", number);
        else
            printf("Не могу проинициализировать термометр [%02x]\r\n", number);
    } else {
        if (ds1820_set_THTL(ds1820_rom_codes[number], 1, sign))
            printf("Термометр [%02x] успешно очищен\r\n", number);
        else
            printf("Не могу очистить термометр [%02x]\r\n", number);
    }
}
// Переключение звука
void toggle_sound(void) {
    if (mode.sound)
        printf("Звук выключен\r\n");
    else
        printf("Звук включен\r\n");
    mode.sound = !mode.sound;
}
// Старт программы с задержкой
// #define DELAY_TIME 300
#define DELAY_TIME 30
void deley_run(void) {
    byte i =0;
    signal_buz(LONG);
    for (i = 0; i < 7; i++){
        delay_ms(DELAY_TIME);
        signal_white(ON);
        delay_ms(DELAY_TIME);
        signal_white(OFF);
        signal_red(ON);
        delay_ms(DELAY_TIME);
        signal_red(OFF);
        signal_green(ON);
        delay_ms(DELAY_TIME);
        signal_green(OFF);
    };

}
// Переключение печати
void toggle_print(void) {
    mode.print = !mode.print;
    if (mode.print)
        printf("Печать включена\r\n");
        //printf("Печать выключена\r\n Время, Температура заданная, Температура измеренная, Delta, Угол крана расчетный, Угол кранаизмеренный, Угол ограничения, result, pTerm, iTerm, dTerm, Обороты расчетные, Обороты измеренныее\r\n");
    else
        //printf("Печать выключена\r\n Время, Температура заданная, Температура измеренная, Delta, Угол крана расчетный, Угол кранаизмеренный, Угол ограничения, result, pTerm, iTerm, dTerm, Обороты расчетные, Обороты измеренныее\r\n");
        printf("Печать выключена\r\n");
}
// Обработка событий от серийного порта
void check_serial(void) {
    byte inbyte;    // Описание локальных переменных
    register byte i;
    // Обрабатываем последовательный порт
    if (UCSRA & RX_COMPLETE) { // Пришло ли что-нибудь
        inbyte = UDR;
        switch (inbyte) {
            case 0x7A:  /* 'z' */
                printf("Время: %02u:%02u:%02u, дата:%02u.%02u.%02u, найдено %u термометров\r\n",
                        s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo, s_dt.cyy, ds1820_devices);
                printallterms(0);
                break;
            case 0x73:  /* 's' */     // переключение звука
                toggle_sound(); break;
            case 0x77:  /* 'w' */     // символ
                printf ("Cобытие = %u\r\n", (unsigned char)event);
                break;
            case 0x78:  // символ 'x'
                toggle_print(); break;
                // print_curr_menu(); break; // Печатаем текущее меню
            case 0x76: // символ 'v'
                printallterms(1); break;
            case 0x62: // символ 'b'
                printnewterms(); break;
            case 0x65:  // символ 'e'
                alarm_all_print(); break;
            case 0x64:  // символ 'd'
                for (i=0; i < MAX_ALERTS; i++) {
                    if (!(alarm_unreg(i))) {
                       printf ("Удалили активную тревогу: %s\r\n", get_alert_str(i));
                    }
                }
                printf ("Инициализируем все термометры!\r\n");
                read_all_terms(INIT_MODE);
                break;
            case 0x63:                // символ 'c'
                print_prim_par((unsigned char *)&prim_par, sizeof(prim_par));
                break;
            case 0x6D:                // символ 'm'
                for(i=0; i<NUM_PARAMETERS; i++) printf("%s\t", param_str(i, parameters));
                printf("\r\n");
                break;
            case 0x6E:                // символ 'n'
                for(i=0; i<NUM_MENU; i++) printf("%s\t", param_str(i, main_menu));
                printf("\r\n");
                break;
            case 0x4a:              // символ 'j'
                poll_keys();
                break;
            case 0x75:                // символ 'u' Воздух в помещении
                init_force_term(0xFE); break;
            case 0x69:                // символ 'i' Воздух на улице
                init_force_term(0xFD); break;
            case 0x6F:                // символ 'o' Вода
                init_force_term(0xFC); break;
            case 0x70:                // символ 'p'
                init_force_term(0xFB); break;
            case 0x30:              // символ '0'
                printf("Всем найденым термометрам прописываем 0x69 0xFE - Термометр в помещении\r\n");
                init_new_terms(0);   // Находим непроинициализированные термометры и первому попавшему прописываем 0x69 0xFE
                break;
            case 0x31:              // символ '1'
                printf("Всем найденым термометрам прописываем 0x69 0xFD - Термометр на улице\r\n");
                init_new_terms(1);
                break;
            case 0x32:              // символ '2'
                printf("Всем найденым термометрам прописываем 0x69 0xFC - Термометр на подаче воды\r\n");
                init_new_terms(2);
                break;
            case 0x33:              // символ '3'
                printf("Всем найденым термометрам прописываем 0x69 0xFB - Термометр на обратке воды\r\n");
                init_new_terms(3);
                break;
            case 0x3f:              /* Shift + '?'*/
                set_term(3, 1); break;
            case 0x3e:              /* Shift + '>' */
                set_term(2, 1); break;
            case 0x3c:              /* Shift + '<' */
                set_term(1, 1); break;
            case 0x4d:              /* Shift + 'M' */
                set_term(0, 1); break;
            case 0x22:              /* Shift + '"'*/
                set_term(3, OUR_SIGNATURE); break;
            case 0x3a:              /* Shift + ':' */
                set_term(2, OUR_SIGNATURE); break;
            case 0x4c:              /* Shift + 'L' */
                set_term(1, OUR_SIGNATURE); break;
            case 0x4b:              /* Shift + 'K' */
                set_term(0, OUR_SIGNATURE); break;
            case 0x39:              // символ '9'
                signal_printallbytes();
                break;
            default:
                printf("Нажат символ 0x%x\r\n", inbyte);
        };
    }
}