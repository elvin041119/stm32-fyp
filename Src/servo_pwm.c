/*
 * servo_pwm.c
 *
 *  Created on: Oct 25, 2026
 *      Author: sheungheungding
 */


#include "servo.h"
#include "stm32f1xx_hal.h"

typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;
    float    center_deg;   /* mechanical 90° after horn */
    float    offset_deg;   /* trim */
    float    min_deg;
    float    max_deg;
    int8_t   invert;       /* 1 = left/right mirror */
    uint16_t us_center;    /* 1500 */
    uint16_t us_per_90;    /* ~1000 for 90° → 500–2500 */
} ServoPwm;

/* EDIT pins/timers to match CubeMX */
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

static ServoPwm S[N_JOINTS] = {
    /* L_HIP   */ { &htim2, TIM_CHANNEL_1, 90, 0, 40, 140, 0, 1500, 1000 },
    /* L_KNEE  */ { &htim2, TIM_CHANNEL_2, 90, 0, 40, 140, 0, 1500, 1000 },
    /* L_ANKLE */ { &htim2, TIM_CHANNEL_3, 90, 0, 40, 140, 0, 1500, 1000 },
    /* R_HIP   */ { &htim3, TIM_CHANNEL_1, 90, 0, 40, 140, 1, 1500, 1000 },
    /* R_KNEE  */ { &htim3, TIM_CHANNEL_2, 90, 0, 40, 140, 1, 1500, 1000 },
    /* R_ANKLE */ { &htim3, TIM_CHANNEL_3, 90, 0, 40, 140, 1, 1500, 1000 },
};

static float    shadow_deg[N_JOINTS];
static uint16_t shadow_us[N_JOINTS];

static uint16_t deg_to_us(const ServoPwm *sv, float deg)
{
    if (deg < sv->min_deg) deg = sv->min_deg;
    if (deg > sv->max_deg) deg = sv->max_deg;
    if (sv->invert) deg = 180.0f - deg;

    float d = (deg + sv->offset_deg) - sv->center_deg;
    int us = (int)(sv->us_center + d * (sv->us_per_90 / 90.0f));
    if (us < 500)  us = 500;
    if (us > 2500) us = 2500;
    return (uint16_t)us;
}

void servo_init(void)
{
    for (int i = 0; i < N_JOINTS; i++) {
        shadow_deg[i] = 90.0f;
        shadow_us[i]  = S[i].us_center;
        HAL_TIM_PWM_Start(S[i].htim, S[i].channel);
        __HAL_TIM_SET_COMPARE(S[i].htim, S[i].channel, shadow_us[i]);
    }
}

void servo_set_deg(uint8_t joint, float deg)
{
    if (joint >= N_JOINTS) return;
    shadow_deg[joint] = deg;
    shadow_us[joint]  = deg_to_us(&S[joint], deg);
}

float servo_get_deg(uint8_t joint)
{
    return (joint < N_JOINTS) ? shadow_deg[joint] : 90.0f;
}

void servo_commit(void)
{
    for (int i = 0; i < N_JOINTS; i++)
        __HAL_TIM_SET_COMPARE(S[i].htim, S[i].channel, shadow_us[i]);
}

void servo_estop(void)
{
    /* hold last pulse — still powered. Cut PSU only if you have a MOSFET. */
    servo_commit();
}
