#include "judge_protocol.h"
#include "config_judge.h"
#include "string.h"
#include "CRC8.h"
#include "CRC16.h"

judge_frame_header_t judge_frame_header;
drv_judge_info_t drv_judge_info = 
{
	.frame_header = &judge_frame_header,
};

void judge_receive(uint8_t *rxbuf)
{
	uint16_t frame_length = 0;
	if(rxbuf == NULL)
	{
		return;
	}
	drv_judge_info.frame_header->SOF = rxbuf[0];
	if(drv_judge_info.frame_header->SOF == 0xA5)
	{
		memcpy(&drv_judge_info.frame_header->data_length,rxbuf + 1, 4);
		if(Verify_CRC8_Check_Sum(rxbuf,5) == 1)
		{
			frame_length = 5 + 2 + drv_judge_info.frame_header->data_length + 2;
			if(Verify_CRC16_Check_Sum(rxbuf, frame_length) == 1)
			{
				memcpy(&drv_judge_info.cmd_id, rxbuf + 5, 2);
				judge_update(drv_judge_info.cmd_id, rxbuf + 7);
			}
			memcpy(&drv_judge_info.frame_tail, rxbuf + 5 + 2 + drv_judge_info.frame_header->data_length, 2);
		}
	}
	/* ���һ�����ݰ������˶�֡���ݾ��ٴζ�ȡ */
	if(rxbuf[frame_length] == 0xA5)
	{
		judge_receive( &rxbuf[frame_length] );
	}
}

