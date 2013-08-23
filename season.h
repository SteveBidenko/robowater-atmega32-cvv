#ifndef _SEASON_
#define _SEASON_
// Определение подстановок

// Описание функций
extern void coolant_regulator(void);   // Подпрограмма регулирования охладителя (не зимой)
extern void winter_fan_speed(void);    // регулирование вентилятора зимой
extern void keep_life_in_winter(void); // поддержка работоспособности системы зимой
extern void winter_regulator(void);    // Подпрограмма регулирования калорифера зимой
// Описание глобальных переменных
//extern unsigned int time_integration;
extern unsigned char tap_angle_min;
#endif
