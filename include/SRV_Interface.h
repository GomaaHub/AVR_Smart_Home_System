/*
 * SRV_Interface.h
 *
 *  Created on: May 17, 2022
 *      Author: Ahmad
 */

#ifndef SRV_INTERFACE_H_
#define SRV_INTERFACE_H_

void SRV_Test(void);
void SRV_SetDegree(u8 degree, u8 max_degree);

#define SERVO_MIN 348
#define SERVO_MAX 2630

#define SERVO_MAX_DEGREE 204

#endif /* SRV_INTERFACE_H_ */
