/*
 * UI.h
 *
 *  Created on: Aug 16, 2020
 *      Author: dybb8
 */

#ifndef UI_H_
#define UI_H_

#include "../ImageCreator/ImageCreator.h"

void UIForAirQuality(PHDC hdc);

/*
 * procgress : 0~100
 */
void UIForStartUp(PHDC hdc, int progress);

#endif /* UI_H_ */
