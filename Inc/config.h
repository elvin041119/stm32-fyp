/*
 * config.h
 *
 *  Created on: Oct 25, 2026
 *      Author: sheungheungding
 */

#ifndef INC_CONFIG_H_
#define INC_CONFIG_H_
#define SERVO_USE_STS  0   /* 0 = DS3225 PWM, 1 = Feetech STS3215 */

#define N_JOINTS       6
#define POSE_DT_MS     10  /* 100 Hz */

#define L_HIP    0
#define L_KNEE   1
#define L_ANKLE  2
#define R_HIP    3
#define R_KNEE   4
#define R_ANKLE  5



#endif /* INC_CONFIG_H_ */
