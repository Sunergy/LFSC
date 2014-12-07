/*
 * spa_calculation.c
 *
 *  Created on: Dec 4, 2014
 *      Author: User
 */

#include <time.h>
#include <math.h>
#include "spa.h"
#include "spa_calculation.h"


# define M_PI 3.14159265358979323846

float radians(float degrees){
    float result = degrees*(M_PI / 180.0 );
    return result;
}

float degrees( float radians ){
    float result = radians * (180.0 / M_PI);
    return result;
}

void spa_calculate_M(struct tm *date , spa_data *spa , float machine_pos[]){

	spa->year = (1900 + date->tm_year);
	spa->month = (1 + date->tm_mon );
	spa->day = date->tm_mday;
	spa->hour = date->tm_hour;
	spa->minute = date->tm_min;
	spa->second = date->tm_sec;

	spa->timezone = -4;
	spa->latitude = machine_pos[0];
	spa->longitude = machine_pos[1];
	spa->elevation = machine_pos[2];


// 18.21 -67.14 12

	spa_calculate(spa);


}



float calculate_motor_position(int motor_id_number ,spa_data *spa_values ){




    float zenith = spa_values->zenith;
    float azimuth = (spa_values->azimuth - 180.0 ) ;


    float yc = 1.58;
    float xc[] = {-0.28,-0.14,0,0.14,0.28};

    float angulo_incidente  = atan(tan(radians(zenith))*sin(radians(azimuth)));

    float angle_position;

    if ( motor_id_number  < 2 ) {
        angle_position = degrees(angulo_incidente - atan(yc / -1*xc[motor_id_number]) + (M_PI/2))/2.0;
    }
    else if ( motor_id_number > 2 ) {
         angle_position = degrees(angulo_incidente + atan(yc / xc[motor_id_number]) - (M_PI/2))/2.0;

    }

    else {
        angle_position = degrees(angulo_incidente/2.0);
    }



    return angle_position;
}


