#include "fan.h"

// ������� ������������ �������� �������� ������, ���� �������� ����� � ��������� ����� FAN_SPEED_MIN � FAN_SPEED_MAX.
unsigned char check_fan_range(int value) {
    unsigned char new_value;
    if (value <= FAN_SPEED_MIN) {
        new_value = FAN_SPEED_MIN;
    } else {
        new_value = (value > FAN_SPEED_MAX) ? FAN_SPEED_MAX : value; 
    } 
    return (new_value); 
} 