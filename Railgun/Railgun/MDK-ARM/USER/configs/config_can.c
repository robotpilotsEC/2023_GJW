#include "config_can.h"
#include "lever_motor.h"
#include "string.h"
#include "launcher_motor.h"
#include "chassis_motor.h"

//200为前4个，1FF为后4个
int16_t can1_0x200_send_buff[4];
int16_t can1_0x1FF_send_buff[4];
int16_t can2_0x200_send_buff[4];
int16_t can2_0x1FF_send_buff[4];

void CAN1_Get_Data(uint32_t id, uint8_t *rxBuf)
{
	switch (id)
	{
		case l_lever_motor_id:
      lever_motor[L_LEVER].rx(&lever_motor[L_LEVER],rxBuf,8);
			break;
		case r_lever_motor_id:
      lever_motor[R_LEVER].rx(&lever_motor[R_LEVER],rxBuf,8);
			break;
		case d_lever_motor_id:
      lever_motor[D_LEVER].rx(&lever_motor[D_LEVER],rxBuf,8);
			break;
		case main_launcher_motor_id:
      launcher_motor[MAIN_LAUNCHER].rx(&launcher_motor[MAIN_LAUNCHER],rxBuf,8);
			break;
		case feeder_motor_id:
			launcher_motor[FEEDER].rx(&launcher_motor[FEEDER],rxBuf,8);
		default:
			break;
	}
}

void CAN2_Get_Data(uint32_t id, uint8_t *rxBuf)
{
	switch (id)
	{
		case lf_motor_id:
      chassis_motor[LF_WHEEL].rx(&chassis_motor[LF_WHEEL],rxBuf,8);
			break;
		case rf_motor_id:
      chassis_motor[RF_WHEEL].rx(&chassis_motor[RF_WHEEL],rxBuf,8);
			break;
		case lb_motor_id:
      chassis_motor[LB_WHEEL].rx(&chassis_motor[LB_WHEEL],rxBuf,8);
			break;
		case rb_motor_id:
      chassis_motor[RB_WHEEL].rx(&chassis_motor[RB_WHEEL],rxBuf,8);
			break;
		default:
			break;
	}
}


