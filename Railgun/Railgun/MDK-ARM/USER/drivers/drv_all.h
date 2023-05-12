#ifndef __DRV_ALL_H
#define __DRV_ALL_H

#include "stm32f4xx_hal.h"
#include "drv_can.h"
#include "drv_usart.h"
#include "lever.h"
#include "remote.h"
#include "launcher.h"
#include "chassis.h"
#include "drv_timer.h"

void driver_init(void);
void device_init(void);

#endif




