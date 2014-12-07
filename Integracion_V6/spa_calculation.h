/*
 * spa_calculate.h
 *
 *  Created on: Dec 4, 2014
 *      Author: User
 */

#ifndef SPA_CALCULATION_H_
#define SPA_CALCULATION_H_

extern void spa_calculate_M(struct tm *date , spa_data *spa_values , float machine_pos[]);
extern float calculate_motor_position(int motor_id_number ,spa_data *spa_values);
extern float radians(float degrees);
extern float degrees(float radians);



#endif /* SPA_CALCULATION_H_ */
