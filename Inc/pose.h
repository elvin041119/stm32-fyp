/*
 * pose.h
 *
 *  Created on: Oct 25, 2026
 *      Author: sheungheungding
 */

#ifndef INC_POSE_H_
#define INC_POSE_H_


#include <stdint.h>
#include "config.h"

typedef struct {
    float deg[N_JOINTS];
} Pose;

void pose_init(void);

/* Non-blocking: interpolates toward target over duration_ms. */
void pose_goto(const Pose *target, uint16_t duration_ms);

void pose_hold(void);          /* freeze current angles */
int  pose_busy(void);          /* 1 = still interpolating */

/* Call every POSE_DT_MS from main/SysTick. */
void pose_update(void);

extern const Pose POSE_STAND;  /* fill on the jig */



#endif /* INC_POSE_H_ */
