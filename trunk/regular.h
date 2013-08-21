#ifndef _REGULAR_
#define _REGULAR_
// Определение подстановок

// Описание функций
extern void regular_inspection(void);   // Функция, регулярно запускающая по событию Секунда. Вызов функции - не гарантирован.
extern void lcd_primary_screen(void);
extern char high_time_TO(void);        // Функция, проверяющая необходимость проведения ТО
// Описание глобальных переменных
extern unsigned int time_integration;
extern unsigned int timer_start;
extern byte timer_stop;
extern byte timer_fan;
extern byte count_fan;
extern int time_cooling;
extern unsigned char timer_start_to;
extern unsigned char timer_stop_to;
#endif
