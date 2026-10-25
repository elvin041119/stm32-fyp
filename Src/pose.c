/*
 * pose.c
 *
 *  Created on: Oct 25, 2026
 *      Author: sheungheungding
 */
#include "pose.h"
#include "servo.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

/* Dummy stand — replace with jig numbers. Degrees, 90 = mechanical middle. */
const Pose POSE_STAND = {{
    90, 100, 80,   /* L hip, knee, ankle */
    90, 100, 80    /* R — mirror inverts live in servo.c */
}};

static Pose  g_start;
static Pose  g_target;
static Pose  g_now;
static uint32_t g_elapsed_ms;
static uint32_t g_duration_ms;
static int g_busy;

void pose_init(void)
{
    g_now = POSE_STAND;
    g_target = POSE_STAND;
    g_busy = 0;
    for (int i = 0; i < N_JOINTS; i++)
        servo_set_deg((uint8_t)i, g_now.deg[i]);
    servo_commit();
}

void pose_goto(const Pose *target, uint16_t duration_ms)
{
    g_start = g_now;
    g_target = *target;
    g_elapsed_ms = 0;
    g_duration_ms = (duration_ms == 0) ? 1 : duration_ms;
    g_busy = 1;
}

void pose_hold(void)
{
    g_busy = 0;
    g_target = g_now;
}

int pose_busy(void)
{
    return g_busy;
}

void pose_update(void)
{
    if (!g_busy) {
        servo_commit();   /* keep PWM refreshed / optional STS repeat */
        return;
    }

    g_elapsed_ms += POSE_DT_MS;
    float t = (float)g_elapsed_ms / (float)g_duration_ms;
    if (t >= 1.0f) {
        t = 1.0f;
        g_busy = 0;
    }

    /* cosine ease — same for DS3225 and STS3215 */
    float s = 0.5f * (1.0f - cosf((float)M_PI * t));

    for (int i = 0; i < N_JOINTS; i++) {
        g_now.deg[i] = g_start.deg[i] + s * (g_target.deg[i] - g_start.deg[i]);
        servo_set_deg((uint8_t)i, g_now.deg[i]);
    }
    servo_commit();
}

