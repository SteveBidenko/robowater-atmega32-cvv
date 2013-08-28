#include <mega32.h>
#include <stdlib.h>  // for abs
#include <stdio.h>
#include "robowater.h"
#include "signals.h"
#include "regular.h"
#include "menu.h"
#include "spd1820.h"
#include "dayofweek.h"
// Описание модульных переменных
unsigned int time_integration = 0;
unsigned char timer_start_to = 0;
unsigned char timer_stop_to = 0;
unsigned int timer_start = 0;
unsigned char timer_stop = 0;
unsigned char timer_fan = 0;
unsigned char count_fan = 0;
// Функция, вызываемая раз в секунду по событию ev_secunda
void regular_inspection(void) {
    #ifndef NODEBUG
    // printf ("%02u:%02u Начало прерывания Секунда...", s_dt.cMM, s_dt.cSS);
    // printf (".");
    #endif
    if (timer_fan) timer_fan--;
    if (timer_start) {
        timer_start--;
        // Если достигли нуля, то включаем вентилятор
        if (!timer_start && (mode.run == mo_warming_up)) {         // event = ev_timer_Start;
             mode.fan = 1;
             #ifndef NODEBUG
             printf ("ПУСК \r\n");
             #endif
             mode.run = mo_action;  // Включаем режим Пуск
             count_fan = 0;
             signal_green(ON);
        }
    }
    if (timer_stop) {
        timer_stop--;
        // Если достигли нуля, то выключаем вентилятор
        if (!timer_stop && (mode.run == mo_warming_down)) {          // event = ev_timer_Stop;
            mode.run = mo_stop;
            mode.fan = 0;           // Выключение насоса
            // printf ("Остановили Вентилятор Режим СТОП \r\n");
            signal_green(OFF);
        }
    }
     // Пока timer1_counter > 0, уменьшаем его значение
    if (menu_timer_inactive) {
        menu_timer_inactive--;
        // Если достигли нуля, то останавливаем обслуживание valcoder
        if (!menu_timer_inactive) {
            if (event)
                menu_timer_inactive++;
            else
                event = ev_timer;
            // clatsman.valcoder_mode = 0; lcd_clrscr();
        }
    }
    if (!mode.stop_sync_dt) get_cur_dt (0);
    read_all_terms(DUTY_MODE);
    // Вычитаваем АЦП
    ADC_VAR2 = read_adc(0)/4;
    ADC_VAR1 = read_adc(1)/4;
    if (time_integration) time_integration--;
    if (time_cooling ) time_cooling-- ;
    #ifndef NODEBUG
    // printf ("конец в %02u:%02u\r\n", s_dt.cMM, s_dt.cSS);
    #endif
    s_dt.dayofweek = dayofweek(s_dt.cdd, s_dt.cmo, s_dt.cyy);
    MAIN_T = read_term(0);      // Выводим информацию о главном термометре !!!
    POM_T = read_term(0);
    UL_T = read_term(1);
    WIN_T = read_term(2);
    WOUT_T = read_term(3);
    switch (mode.menu) {
        case 0: lcd_primary_screen(); break;
        // case 2: lcd_edit(0); break;
        case 1: lcd_menu(0); break;
        default: ;
    }
    // Если пора проводить ТО, то генерируем событие ev_begin_to
    if (high_time_TO()) {
        printf ("Пора проводить ТО\n");
        prim_par.TO.status = 1;
        event = ev_begin_to;
    } else {
        event = ev_none;            // Очищаем событие
        if (mode.run == mo_to) {
            if (timer_start_to) {
                timer_start_to--;
                    // Если достигли нуля, то Событие ev_to_nf
                if (!timer_start_to && (ADC_VAR1 < prim_par.ADC1_hi)) {
                    printf ("Закончилось время открытия \n");
                    event = ev_to_nf;
                }
            }
            if (timer_stop_to) {
                timer_stop_to--;
                if (ADC_VAR1 <= (prim_par.ADC1_lo + 5)) {
                   printf ("Успешное завершение ТО Крана. \n");
                   event = ev_end_to;
                } else {
                // Если достигли нуля, то Событие ev_to_nf
                    if (!timer_stop_to && (ADC_VAR1 > (prim_par.ADC1_lo + 10))) {
                        printf ("Закончилось время закрытия \n");
                        event = ev_to_nf;
                    }
                }
            }
        }
    }
}
// Функция проверяющая условия проведения ТО, и если пора - возвращает ненулевое значение
char high_time_TO(void) {
    if (prim_par.TO.weekday == s_dt.dayofweek) {
        if (prim_par.TO.hour == s_dt.cHH) {
            if (prim_par.TO.minute == s_dt.cMM) {
                if (prim_par.TO.status == 0) {
                    return (1);
                }
            }
        }
    }
    return (0);
}
// Полностью прорисовать главный экран
void lcd_primary_screen(void) {
    char run_mod [8];
    register unsigned char i;
    unsigned char c_warnings = 0;
    unsigned char sign = '+';
    int lcd_term = main_menu[0].val_data;
    #ifndef NODEBUG
    // printf ("%02u:%02u Начало прорисовки экрана...", s_dt.cMM, s_dt.cSS);
    #endif
    for (i = 0; i < MAX_WARNINGS; i++) {
        // 9-й алерт (отсутствие 3-го термометра) нам не интересен
        if (prim_par.warning_status[i] && i != 9) c_warnings++;
    }
    // Обратите внимание, что значение режима не должно быть длинее 5-ти символов
    switch (mode.run) {
        case mo_stop:           sprintf(run_mod, "СТОП "); break;
        case mo_warming_up:     sprintf(run_mod, "ПРОГР"); break;
        case mo_warming_down:   sprintf(run_mod, "ОСТАН"); break;
        case mo_action:         sprintf(run_mod, "ПУСК "); break;
        case mo_to:             sprintf(run_mod, "ТО   "); break;
        default: break;
    };
    if (c_warnings)
        sprintf(run_mod, "Авария ");
    else {
        signal_red(OFF);  signal_buz(OFF); // signal_green(OFF);
    }
    lcd_command(LCD_DISP_ON);       // Убираем курсор с LCD
    lcd_gotoxy(0,0);        // Устанавливаем курсор в позицию 0 первой строки
    sprintf(linestr, "%02u:%02u:%02u %02u.%02u %01u", s_dt.cHH, s_dt.cMM, s_dt.cSS, s_dt.cdd, s_dt.cmo, s_dt.dayofweek);
    lcd_puts(linestr);
    lcd_gotoxy(0,1);                // Устанавливаем курсор в позицию 0 строки 2
    // Выводим информацию о термометрах
    if(ds1820_devices) {
        sign = (lcd_term < 0) ? '-' : '+';
        sprintf(linestr, "t=%c%02u.%01uC %s", sign, abs(lcd_term)/100, (abs(lcd_term)%100)/10, run_mod);
    } else {
        sprintf(linestr, "Нет термометров");
    }
    lcd_puts(linestr);
    #ifndef NODEBUG
    // printf ("конец в %02u:%02u\r\n", s_dt.cMM, s_dt.cSS);
    #endif
}
