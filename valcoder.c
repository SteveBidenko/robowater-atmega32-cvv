#include <mega32.h>
#include "robowater.h"
#include "valcoder.h"

// byte valcoder0, valcoder1;      // ���� ����������
// word counter0, counter1;        // ������� ���-�� ���������� �� valcoder
signed char valcoder;              // ����������� ��������
unsigned char EncState; // ���� ���������� �������� INT0
unsigned char EncReady; // ������� ���������
// ������� ���������� Valcoder'�
interrupt [EXT_INT0] void ext_int0_isr(void) {
    valcoder = 0;
    // ��������� ������� ����������
    GIFR |= (1<<6);
    // ��������� ���������� INT0 (���������� 0 � 6 ����)
    GICR &= ~(1<<6);
    EncState = 1;
}
interrupt [EXT_INT1] void ext_int1_isr(void) {
    GIFR |= (1<<7);
    // ��������� ���������� INT0 (���������� 1 � 6 ����)
    if ( EncReady == 1 ) {
        EncReady = 0;
        GICR |= (1<<6);
    };
}
