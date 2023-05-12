#include "config_can.h"
#include "cap_protocol.h"
#include "cap.h"
#include "my_judge.h"

void CAN1_Get_Data(uint32_t id, uint8_t *data)
{
	switch(id)
	{
		case 0x0FF:
			if(data[0] == 1)
			{
				cap.normal_switch = 1;
			}
			else
			{
				cap.normal_switch = 0;
			}
			
			global_receive.top_mode = data[1];
			global_receive.vision_on = data[2];
			global_receive.detect_top = data[3];
			global_receive.friction_status = data[4];
			global_receive.vision_detect_num = data[5];
			break;
		case 0x300:
			break;
		default:
			break;
	}
}

void CAN2_Get_Data(uint32_t id, uint8_t *data)
{
	switch(id)
	{
		case 0x030:
			cap_receive(data);
			break;
		default:
			break;
	}
}

