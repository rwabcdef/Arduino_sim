#ifndef TIMER0_H
#define TIMER0_H

extern volatile uint16_t g_timer0_ISR;

#ifdef __cplusplus
 extern "C" {
#endif

void timer0_init();

#ifdef __cplusplus
}
#endif

#endif