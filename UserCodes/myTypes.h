/*
 * myTypes.h
 *
 *  Created on: Jun 3, 2022
 *      Author: me
 */

#ifndef MYTYPES_H_
#define MYTYPES_H_

typedef enum{
	 FALSE = (1!=1)
	,TRUE = (1==1)
}bool_t;

#ifndef fromMX
#define fromMX(xx) xx##_GPIO_Port, xx##_Pin
/*example use:  HAL_GPIO_TogglePin(fromMX(BLINK));  */
#endif

#endif /* MYTYPES_H_ */
