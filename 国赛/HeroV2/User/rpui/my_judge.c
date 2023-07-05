/**
  ******************************************************************************
  * @file           : my_judge.c\h
	* @author         : czf
	* @date           : 
  * @brief          : ���ݡ�RoboMaster_����ϵͳ����Э�鸽¼ V1.3����д
	                    ��Ի����˼佻������
	* @history        : 
  ******************************************************************************
  */

/* Ŀ¼begin */

//���Ĳ�
//**********ʵʱ����
//������
//**********��������
//�ڶ���
//**********��ȡͼ������֡
//********************��ȡֱ������֡
//********************��ȡ��������֡
//********************��ȡ��Բ����֡
//********************��ȡ��Բ����֡
//********************��ȡԲ������֡
//********************��ȡ����������֡
//********************��ȡ����������֡
//********************��ȡ�ַ�����֡
//**********����֡����
//********************���ͻ���һ��ͼ��֡����
//********************���ͻ��ƶ���ͼ��֡����
//********************���ͻ������ͼ��֡����
//********************���ͻ����߸�ͼ��֡����
//********************���ͻ����ַ�֡����
//��һ��
//**********���ڷ�������

/* Ŀ¼end */

#include "my_judge.h"
#include "crc8.h"
#include "crc16.h"
#include "string.h"
#include "stdbool.h"
#include "stdio.h"
#include "judge_protocol.h"
#include "cap_protocol.h"
#include "judge.h"

/* ������ begin */
#define UI_huart huart5  //���ڽӿ�
#define send_frequency 30  //����Ƶ��
/* ������ end */

extern UART_HandleTypeDef UI_huart;

client_info_t client_info = 
{
	.robot_id = 1,
	.client_id = 0x0101,
};
uint8_t client_tx_buf[128];

/**************************************************���Ĳ�begin**************************************************/

/**
  * @brief  ʵʱ����
  * @param  �������ڣ�ms��
  * @retval 
  */
void tick_task(uint16_t time)
{
	uint16_t cnt_max = 1000 / time / send_frequency;
	uint8_t task_num_max = 7;
	static uint16_t cnt = 0;
	static uint8_t task_num = 0;
	
	global_vary_system();
	client_info_update();
	
	if(cnt >= cnt_max)
	{
		switch(task_num)
		{
			case 0:
				UI_send_char_1();
				break;
			case 1:
        UI_send_Group1();
				break;
			case 2:
				UI_send_Group2();
			  
				break;
			case 3:
				UI_send_Group3();
			  
				break;
			case 4:
				UI_send_Group4();
				break;
			case 5:
				UI_send_char_2();
				break;
			case 6:
				UI_send_char_3();
				break;
			case 7:
				break;
			case 8:
				break;
		}
		task_num++;
		task_num %= task_num_max;
		cnt = 0;
	}
	else 
	{
		cnt++;
	}
}
int32_t cnttt = 0;

void client_info_update(void)
{
	switch(judge.judge_info->game_robot_status.robot_id)
	{
		case 1:
			client_info.robot_id = 1;
		  client_info.client_id = 0x0101;
			break;
		case 101:
			client_info.robot_id = 101;
		  client_info.client_id = 0x0165;
			break;
		default:
			break;
	}
}

/**************************************************���Ĳ�end**************************************************/


/**************************************************������begin**************************************************/

/******************************��������begin******************************/

/******************************��������end******************************/

/**************************************************������end**************************************************/

/**************************************************�ڶ���begin**************************************************/

/******************************��ȡͼ������֡begin******************************/

/**
  * @brief  ��ȡֱ������֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_line(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t width,  //�������
               uint16_t start_x,  //��� x ����
               uint16_t start_y,  //��� y ����
               uint16_t end_x,  //�յ� x ����
               uint16_t end_y)  //�յ� y ����
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 0;
	data.layer = layer;
	data.color = color;
	data.start_angle = 0;
	data.end_angle = 0;
	data.width = width;
	data.start_x = start_x;
	data.start_y = start_y;
	data.radius = 0;
	data.end_x = end_x;
	data.end_y = end_y;
	
	return data;
}

/**
  * @brief  ��ȡ��������֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_rectangle(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫDSFZa
               uint16_t width,  //�������
               uint16_t start_x,  //��� x ����
               uint16_t start_y,  //��� y ����
               uint16_t end_x,  //�ԽǶ��� x ����
               uint16_t end_y)  //�ԽǶ��� y ����
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 1;
	data.layer = layer;
	data.color = color;
	data.start_angle = 0;
	data.end_angle = 0;
	data.width = width;
	data.start_x = start_x;
	data.start_y = start_y;
	data.radius = 0;
	data.end_x = end_x;
	data.end_y = end_y;
	
	return data;
}

/**
  * @brief  ��ȡ��Բ����֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_circle(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t width,  //�������
               uint16_t ciclemid_x,  //Բ�� x ����
               uint16_t ciclemid_y,  //Բ�� y ����
               uint16_t radius)  //�뾶
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 2;
	data.layer = layer;
	data.color = color;
	data.start_angle = 0;
	data.end_angle = 0;
	data.width = width;
	data.start_x = ciclemid_x;
	data.start_y = ciclemid_y;
	data.radius = radius;
	data.end_x = 0;
	data.end_y = 0;
	
	return data;
}

/**
  * @brief  ��ȡ��Բ����֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_ellipse(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t width,  //�������
               uint16_t start_x,  //Բ�� x ����
               uint16_t start_y,  //Բ�� y ����
               uint16_t end_x,  //x ���᳤��
               uint16_t end_y)  //y ���᳤��
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 3;
	data.layer = layer;
	data.color = color;
	data.start_angle = 0;
	data.end_angle = 0;
	data.width = width;
	data.start_x = start_x;
	data.start_y = start_y;
	data.radius = 0;
	data.end_x = end_x;
	data.end_y = end_y;
	
	return data;
}

/**
  * @brief  ��ȡԲ������֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_arc(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t start_angle,  //��ʼ�Ƕ�
               uint16_t end_angle,  //��ֹ�Ƕ�
               uint16_t width,  //�������
               uint16_t circlemin_x,  //Բ�� x ����
               uint16_t circlemin_y,  //Բ�� y ����
               uint16_t end_x,  //x ���᳤��
               uint16_t end_y)  //y ���᳤��
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 4;
	data.layer = layer;
	data.color = color;
	data.start_angle = start_angle;
	data.end_angle = end_angle;
	data.width = width;
	data.start_x = circlemin_x;
	data.start_y = circlemin_y;
	data.radius = 0;
	data.end_x = end_x;
	data.end_y = end_y;
	
	return data;
}

/**
  * @brief  ��ȡ����������֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_float(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t size,  //�����С
               uint16_t decimal,  //С��λ��Ч����
               uint16_t width,  //�������
               uint16_t start_x,  //��� x ����
               uint16_t start_y,  //��� y ����
               int32_t num)  //���� 1000 ���� 32 λ��������int32_t
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 5;
	data.layer = layer;
	data.color = color;
	data.start_angle = size;
	data.end_angle = decimal;
	data.width = width;
	data.start_x = start_x;
	data.start_y = start_y;
	data.radius = num;
	data.end_x = num >> 10;
	data.end_y = num >> 21;
	
	return data;
}

/**
  * @brief  ��ȡ����������֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_int(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t size,  //�����С
               uint16_t width,  //�������
               uint16_t start_x,  //��� x ����
               uint16_t start_y,  //��� y ����
               int32_t num)  //32 λ��������int32_t
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 6;
	data.layer = layer;
	data.color = color;
	data.start_angle = size;
	data.end_angle = 0;
	data.width = width;
	data.start_x = start_x;
	data.start_y = start_y;
	data.radius = num;
	data.end_x = num >> 10;
	data.end_y = num >> 21;
	
	return data;
}

/**
  * @brief  ��ȡ�ַ�����֡
  * @param  
  * @retval ͼ�����ݽṹ��
  */
graphic_data_struct_t draw_char(char *name,  //ͼ����
	             uint8_t operate_tpye,  //ͼ�β���
               uint8_t layer,  //ͼ������0~9
               uint8_t color,  //��ɫ
               uint16_t size,  //�����С
               uint16_t length,  //�ַ�����
               uint16_t width,  //�������
               uint16_t start_x,  //��� x ����
               uint16_t start_y)  //��� y ����
{
	graphic_data_struct_t data;
	
	memcpy(data.graphic_name, name, 3);
	data.operate_tpye = operate_tpye;
	data.graphic_tpye = 7;
	data.layer = layer;
	data.color = color;
	data.start_angle = size;
	data.end_angle = length;
	data.width = width;
	data.start_x = start_x;
	data.start_y = start_y;
	data.radius = 0;
	data.end_x = 0;
	data.end_y = 0;
	
	return data;
}

/******************************��ȡͼ������֡end******************************/


/******************************����֡����begin******************************/

/**
	* @brief  ���ͻ���һ��ͼ��֡����
  * @param  
  * @retval 
  */
uint8_t client_send_single_graphic(ext_client_custom_graphic_single_t data)
{
	frame_t frame;
	ext_student_interactive_header_data_t data_header;
	
	/* ֡ͷ */
	frame.frame_header.SOF = 0xA5;
	frame.frame_header.data_length = LEN_ID_draw_one_graphic;
	frame.frame_header.seq = 0;
	memcpy(client_tx_buf, &frame.frame_header, 4);
	Append_CRC8_Check_Num(client_tx_buf, 5);
	
	/* ������ID */
	frame.cmd_id = 0x301;
	memcpy(&client_tx_buf[5], (void*)&frame.cmd_id, 2);
	
	/* ���ݶ� */
	data_header.data_cmd_id = ID_draw_one_graphic;
	data_header.sender_ID = client_info.robot_id;
	data_header.receiver_ID = client_info.client_id;
	memcpy(&client_tx_buf[7], &data_header, 6);
	memcpy(&client_tx_buf[13], &data.grapic_data_struct, 15);
	
	/* ֡β */
	Append_CRC16_Check_Sum(client_tx_buf, 5 + 2 + LEN_ID_draw_one_graphic + 2);
	
	/* ���� */
	return uart_send_data(client_tx_buf, 5 + 2 + LEN_ID_draw_one_graphic + 2);
}

/**
	* @brief  ���ͻ��ƶ���ͼ��֡����
  * @param  
  * @retval 
  */
uint8_t client_send_double_graphic(ext_client_custom_graphic_double_t data)
{
	frame_t frame;
	ext_student_interactive_header_data_t data_header;
	
	/* ֡ͷ */
	frame.frame_header.SOF = 0xA5;
	frame.frame_header.data_length = LEN_ID_draw_two_graphic;
	frame.frame_header.seq = 0;
	memcpy(client_tx_buf, &frame.frame_header, 4);
	Append_CRC8_Check_Num(client_tx_buf, 5);
	
	/* ������ID */
	frame.cmd_id = 0x301;
	memcpy(&client_tx_buf[5], (void*)&frame.cmd_id, 2);
	
	/* ���ݶ� */
	data_header.data_cmd_id = ID_draw_two_graphic;
	data_header.sender_ID = client_info.robot_id;
	data_header.receiver_ID = client_info.client_id;
	memcpy(&client_tx_buf[7], &data_header, 6);
	memcpy(&client_tx_buf[13], data.grapic_data_struct, 15*2);
	
	/* ֡β */
	Append_CRC16_Check_Sum(client_tx_buf, 5 + 2 + LEN_ID_draw_two_graphic + 2);
	
	/* ���� */
	return uart_send_data(client_tx_buf, 5 + 2 + LEN_ID_draw_two_graphic + 2);
}

/**
	* @brief  ���ͻ������ͼ��֡����
  * @param  
  * @retval 
  */
uint8_t client_send_five_graphic(ext_client_custom_graphic_five_t data)
{
	frame_t frame;
	ext_student_interactive_header_data_t data_header;
	
	/* ֡ͷ */
	frame.frame_header.SOF = 0xA5;
	frame.frame_header.data_length = LEN_ID_draw_five_graphic;
	frame.frame_header.seq = 0;
	memcpy(client_tx_buf, &frame.frame_header, 4);
	Append_CRC8_Check_Num(client_tx_buf, 5);
	
	/* ������ID */
	frame.cmd_id = 0x301;
	memcpy(&client_tx_buf[5], (void*)&frame.cmd_id, 2);
	
	/* ���ݶ� */
	data_header.data_cmd_id = ID_draw_five_graphic;
	data_header.sender_ID = client_info.robot_id;
	data_header.receiver_ID = client_info.client_id;
	memcpy(&client_tx_buf[7], &data_header, 6);
	memcpy(&client_tx_buf[13], data.grapic_data_struct, 15*5);
	
	/* ֡β */
	Append_CRC16_Check_Sum(client_tx_buf, 5 + 2 + LEN_ID_draw_five_graphic + 2);
	
	/* ���� */
	return uart_send_data(client_tx_buf, 5 + 2 + LEN_ID_draw_five_graphic + 2);
}

/**
	* @brief  ���ͻ����߸�ͼ��֡����
  * @param  
  * @retval 
  */
uint8_t client_send_seven_graphic(ext_client_custom_graphic_seven_t data)
{
	frame_t frame;
	ext_student_interactive_header_data_t data_header;
	
	/* ֡ͷ */
	frame.frame_header.SOF = 0xA5;
	frame.frame_header.data_length = LEN_ID_draw_seven_graphic;
	frame.frame_header.seq = 0;
	memcpy(client_tx_buf, &frame.frame_header, 4);
	Append_CRC8_Check_Num(client_tx_buf, 5);
	
	/* ������ID */
	frame.cmd_id = 0x301;
	memcpy(&client_tx_buf[5], (void*)&frame.cmd_id, 2);
	
	/* ���ݶ� */
	data_header.data_cmd_id = ID_draw_seven_graphic;
	data_header.sender_ID = client_info.robot_id;
	data_header.receiver_ID = client_info.client_id;
	memcpy(&client_tx_buf[7], &data_header, 6);
	memcpy(&client_tx_buf[13], data.grapic_data_struct, 15*7);
	
	/* ֡β */
	Append_CRC16_Check_Sum(client_tx_buf, 5 + 2 + LEN_ID_draw_seven_graphic + 2);
	
	/* ���� */
	return uart_send_data(client_tx_buf, 5 + 2 + LEN_ID_draw_seven_graphic + 2);
}

/**
	* @brief  ���ͻ����ַ�֡����
  * @param  
  * @retval 
  */
uint8_t client_send_char(ext_client_custom_character_t data)
{
	frame_t frame;
	ext_student_interactive_header_data_t data_header;
	
	/* ֡ͷ */
	frame.frame_header.SOF = 0xA5;
	frame.frame_header.data_length = LEN_ID_draw_char_graphic;
	frame.frame_header.seq = 0;
	memcpy(client_tx_buf, &frame.frame_header, 4);
	Append_CRC8_Check_Num(client_tx_buf, 5);
	
	/* ������ID */
	frame.cmd_id = 0x301;
	memcpy(&client_tx_buf[5], (void*)&frame.cmd_id, 2);
	
	/* ���ݶ� */
	data_header.data_cmd_id = ID_draw_char_graphic;
	data_header.sender_ID = client_info.robot_id;
	data_header.receiver_ID = client_info.client_id;
	memcpy(&client_tx_buf[7], &data_header, 6);
	memcpy(&client_tx_buf[13], &data.grapic_data_struct, 15);
	memcpy(&client_tx_buf[28], data.data, 30);
	
	/* ֡β */
	Append_CRC16_Check_Sum(client_tx_buf, 5 + 2 + LEN_ID_draw_char_graphic + 2);
	
	/* ���� */
	return uart_send_data(client_tx_buf, 5 + 2 + LEN_ID_draw_char_graphic + 2);
}

/******************************����֡����end****************************************/

/**************************************************�ڶ���end**************************************************/


/**************************************************��һ��begin**************************************************/

void Usart5_Sent_Byte(uint8_t ch)
{
	UART5->DR = ch;
	while((UART5->SR&0x40)==0);
}

void Usart5_Sent_string(uint8_t *string, uint16_t length)
{
	uint16_t i;
	
	for(i = 0; i<length; i++)
	{
		Usart5_Sent_Byte(string[i]);
	}
}

void Client_Sent_String(uint8_t *string, uint16_t length)
{
	Usart5_Sent_string(string, length);
}

/**
	* @brief  ���ڷ�������
  * @param  
  * @retval 
  */
uint8_t uart_send_data(uint8_t *txbuf, uint16_t length)
{
	return HAL_UART_Transmit_DMA(&UI_huart, txbuf, length);
//	Client_Sent_String(txbuf,length);
}

/**************************************************��һ��end**************************************************/




/*ȫ�ֱ�������*/
global_receive_t global_receive;

float global_cap_voltage = 0.0f;
uint16_t global_bullet_remain = 0;
uint8_t global_top_mode = 0; 
uint8_t global_vision_status = 0;
uint8_t global_vision_main_mode = 0;
uint8_t global_fric_status = 0;
uint8_t global_detect_number = 0;
int16_t global_fric_speed = 0;


/*ȫ�ֱ����仯���� */
void global_vary_system(void)
{
	/*����ȫ�ֱ���*/
	global_cap_voltage = cap_receive_data.cap_voltage;
	/*ʣ�൯��*/
	global_top_mode = global_receive.top_mode;
	
	global_vision_status = global_receive.vision_status;
	
	global_vision_main_mode = global_receive.vision_main_mode;
	
	global_fric_status = global_receive.friction_status;
	
	global_detect_number = global_receive.vision_detect_num;
	
	global_fric_speed = global_receive.fric_speed;
}






/********************************************************************�ַ�����ʾ***********************************************************************/
/*�ַ�����ʾ*/

/*�ַ��� ��ʾ��������*/
char supercap_info[30] = {"SUPERCAP:"};
float low_voltage = 15;//�͵�������
/*�ַ��� ��ʾС����״̬*/
char top_info[30] = {"TOP_MODE:"};
/*�ַ��� ��ʾ�Ӿ�״̬*/
char vision_info[30] = {"VISION:"};
/*�ַ��� ��ʾĦ����״̬*/
char fric_info[30] = {"FRIC:"};

/*�ṹ��_����*/
graphic_data_struct_t super_char_buff;
ext_client_custom_character_t super_char_character;
/*�ṹ��_С����*/
graphic_data_struct_t top_mode_buff;
ext_client_custom_character_t top_mode_character;
/*�ṹ��_�Ӿ�*/
graphic_data_struct_t vision_mode_buff;
ext_client_custom_character_t vision_mode_character;
/*�ṹ��_Ħ����*/
graphic_data_struct_t fric_mode_buff;
ext_client_custom_character_t fric_mode_character;

/*�����ַ���*/
void UI_send_char_1(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	
  if(cnt == 0)//add num
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*��ʾС����״̬ begin*/
	top_mode_buff = draw_char("gc0",  //ͼ����
	                        operate_tpye,  //ͼ�β���
                          1,  //ͼ������0~9
                          GREEN,  //��ɫ
                          18,  //�����С
                          strlen(top_info),  //�ַ�����
                          2,  //�������
                          Client_mid_position_x - 750,  //��� x ����
                          Client_mid_position_y + 140);  //��� y ����
	
	top_mode_character.grapic_data_struct = top_mode_buff;
	memcpy(&top_mode_character.data,&top_info,sizeof(top_info));
		
	client_send_char(top_mode_character);

  /*��ʾС����״̬ end*/
	
	cnt++;
  cnt %= 30;
}

void UI_send_char_2(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	
  if(cnt == 0)//add num
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*��ʾ�Ӿ�״̬ begin*/
	vision_mode_buff = draw_char("gc1",  //ͼ����
	                        operate_tpye,  //ͼ�β���
                          1,  //ͼ������0~9
                          GREEN,  //��ɫ
                          18,  //�����С
                          strlen(vision_info),  //�ַ�����
                          2,  //�������
                          Client_mid_position_x - 750,  //��� x ����
                          Client_mid_position_y + 84);  //��� y ����
	
	vision_mode_character.grapic_data_struct = vision_mode_buff;
	memcpy(&vision_mode_character.data,&vision_info,sizeof(vision_info));
		
	client_send_char(vision_mode_character);

  /*��ʾ�Ӿ�״̬ end*/
	
	cnt++;
  cnt %= 30;
}

void UI_send_char_3(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	
  if(cnt == 0)//add num
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*��ʾĦ����״̬ begin*/
	fric_mode_buff = draw_char("gc2",  //ͼ����
	                        operate_tpye,  //ͼ�β���
                          1,  //ͼ������0~9
                          GREEN,  //��ɫ
                          18,  //�����С
                          strlen(fric_info),  //�ַ�����
                          2,  //�������
                          Client_mid_position_x - 750,  //��� x ����
                          Client_mid_position_y + 28);  //��� y ����
	
	fric_mode_character.grapic_data_struct = fric_mode_buff;
	memcpy(&fric_mode_character.data,&fric_info,sizeof(fric_info));
		
	client_send_char(fric_mode_character);

  /*��ʾĦ����״̬ end*/
	
	cnt++;
  cnt %= 30;
}

void UI_send_char_4(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	
  if(cnt == 0)//add num
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*��ʾ�����ַ��� begin*/
	super_char_buff = draw_char("gc3",  //ͼ����
	                        operate_tpye,  //ͼ�β���
                          2,  //ͼ������0~9
                          YELLOW,  //��ɫ
                          15,  //�����С
                          strlen(supercap_info),  //�ַ�����
                          2,  //�������
                          Client_mid_position_x - 750,  //��� x ����//-100
                          Client_mid_position_y + 120);  //��� y ����//-250
	
	super_char_character.grapic_data_struct = super_char_buff;
	memcpy(&super_char_character.data,&supercap_info,sizeof(supercap_info));
	
	client_send_char(super_char_character);

	/*��ʾ�����ַ��� end*/

	cnt++;
  cnt %= 30;
}
/******************************************************************ͼ����ʾ**********************************************************************/

ext_client_custom_graphic_seven_t Group1;
ext_client_custom_graphic_seven_t Group2;
ext_client_custom_graphic_seven_t Group3;
ext_client_custom_graphic_seven_t Group4;

void UI_send_Group1(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	uint8_t super_cap_color = 0;
	uint8_t ry_rectangle_color = 0;
	uint16_t ry_rectangle_width = 0;
  
  if(cnt == 0)
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*��ʾ������*/
	if(global_cap_voltage <= low_voltage)
	{
		super_cap_color = PINK;
	}
	else
	{
		super_cap_color = YELLOW;
	}
	
	Group1.grapic_data_struct[0] = draw_line("g10",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       super_cap_color,  //��ɫ
                                       20,  //�������
                                       Client_mid_position_x - 130,  //��� x ����
                                       Client_mid_position_y - 380,  //��� y ����
                                       (uint16_t)(Client_mid_position_x - 130 + 0.5f * global_cap_voltage * global_cap_voltage),  //�յ� x ����
                                       Client_mid_position_y - 380);
	
	/*������Ӿ�����*/
	Group1.grapic_data_struct[1] = draw_int("g11",  //ͼ����
	                                    operate_tpye,  //ͼ�β���
                                      0,  //ͼ������0~9
                                      GREEN,  //��ɫ
                                      30,  //�����С
                                      2,  //�������
                                      Client_mid_position_x - 568,  //��� x ����
                                      Client_mid_position_y + 200,  //��� y ����
                                      (int32_t)global_detect_number);  //32 λ��������int32_t
																			
  /*��Ħ����ת��*/
	Group1.grapic_data_struct[2] = draw_int("g12",  //ͼ����
	                                    operate_tpye,  //ͼ�β���
                                      0,  //ͼ������0~9
                                      GREEN,  //��ɫ
                                      30,  //�����С
                                      2,  //�������
                                      Client_mid_position_x + 400,  //��� x ����
                                      Client_mid_position_y + 200,  //��� y ����
                                      (int32_t)global_fric_speed);  //32 λ��������int32_t
	
	if(global_vision_main_mode != 1)
	{
		ry_rectangle_color = WHITE;
		ry_rectangle_width = 1;
	}
	else
	{
		ry_rectangle_color = FUCHSIA;
		ry_rectangle_width = 2;
	}
	
	/*���Ӿ�ROI��*/
	Group1.grapic_data_struct[3] = draw_rectangle("g13",  //ͼ����
	                                    operate_tpye,  //ͼ�β���
                                      0,  //ͼ������0~9
                                      ry_rectangle_color,  //��ɫDSFZa
                                      ry_rectangle_width,  //�������
                                      Client_mid_position_x - 100,  //��� x ����
                                      Client_mid_position_y - 130,  //��� y ����
                                      Client_mid_position_x + 100,  //�ԽǶ��� x ����
                                      Client_mid_position_y - 235);  //�ԽǶ��� y ����
	
	client_send_seven_graphic(Group1);
  
  cnt++;
  cnt %= 30;
}


uint16_t AIM_X = 956;
uint16_t AIM_Y = 395;//540


/*��׼��*/
void UI_send_Group2(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	
  
  if(cnt == 0)
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*up to down*/
	/*��0*/
	Group2.grapic_data_struct[0] = draw_line("g20",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       FUCHSIA,  //��ɫ
                                       1,  //�������
                                       AIM_X - 80,  //��� x ����
                                       AIM_Y + 50,  //��� y ����
                                       AIM_X + 80,  //�յ� x ����
                                       AIM_Y + 50);
	
	/*��1*/
	Group2.grapic_data_struct[1] = draw_line("g21",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       GREEN,  //��ɫ
                                       1,  //�������
                                       AIM_X - 100,  //��� x ����
                                       AIM_Y + 10,  //��� y ����
                                       AIM_X + 100,  //�յ� x ����
                                       AIM_Y + 10);
	/*��2*/
	Group2.grapic_data_struct[2] = draw_line("g22",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       GREEN,  //��ɫ
                                       1,  //�������
                                       AIM_X - 80,  //��� x ����
                                       AIM_Y - 30,  //��� y ����
                                       AIM_X + 80,  //�յ� x ����
                                       AIM_Y - 30);
																			 
	 /*��3*/
	Group2.grapic_data_struct[3] = draw_line("g23",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       FUCHSIA,  //��ɫ
                                       1,  //�������
                                       AIM_X - 30,  //��� x ����
                                       AIM_Y - 80,  //��� y ����
                                       AIM_X + 30,  //�յ� x ����
                                       AIM_Y - 80);
																			 
	 /*��1*/
	Group2.grapic_data_struct[4] = draw_line("g24",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       GREEN,  //��ɫ
                                       1,  //�������
                                       AIM_X,  //��� x ����
                                       AIM_Y + 75,  //��� y ����
                                       AIM_X,  //�յ� x ����
                                       AIM_Y - 75);
																			 
	/*б1*/
	Group2.grapic_data_struct[5] = draw_line("g25",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       FUCHSIA,  //��ɫ
                                       1,  //�������
                                       AIM_X - 80,  //��� x ����
                                       AIM_Y + 50,  //��� y ����
                                       AIM_X - 30,  //�յ� x ����
                                       AIM_Y - 80);	

   /*б2*/
	Group2.grapic_data_struct[6] = draw_line("g26",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       FUCHSIA,  //��ɫ
                                       1,  //�������
                                       AIM_X + 80,  //��� x ����
                                       AIM_Y + 50,  //��� y ����
                                       AIM_X + 30,  //�յ� x ����
                                       AIM_Y - 80);	
	
	client_send_seven_graphic(Group2);
  
  cnt++;
  cnt %= 30;
}

/*���Ϳ�ͨ����*/
void UI_send_Group3(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
  
  if(cnt == 0)
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	/*��б*/
	Group3.grapic_data_struct[0] = draw_line("g30",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       CYAN_BLUE,  //��ɫ
                                       3,  //�������
                                       Client_mid_position_x - 130,  //��� x ����
                                       Client_mid_position_y - 150,  //��� y ����
                                       Client_mid_position_x -450,  //�յ� x ����
                                       0);
	
	/*��б*/
  Group3.grapic_data_struct[1] = draw_line("g31",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       CYAN_BLUE,  //��ɫ
                                       3,  //�������
                                       Client_mid_position_x + 130,  //��� x ����
                                       Client_mid_position_y - 150,  //��� y ����
                                       Client_mid_position_x + 450,  //�յ� x ����
                                       0);

  /*����*/
  Group3.grapic_data_struct[2] = draw_line("g32",  //ͼ����
                                       operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       CYAN_BLUE,  //��ɫ
                                       3,  //�������
                                       Client_mid_position_x - 243,  //��� x ����
                                       250,  //��� y ����
                                       Client_mid_position_x + 243,  //�յ� x ����
                                       250);
																			 
	client_send_seven_graphic(Group3);
  
  cnt++;
  cnt %= 30;
}

/*���͸���״̬��־λ*/
void UI_send_Group4(void)
{
	static int cnt = 0;
  static int operate_tpye = ADD;
	uint8_t vision_color = 0;
	uint8_t top_color = 0;
	uint8_t fric_color = 0;
  
  if(cnt == 0)
  {
    operate_tpye = ADD;  //һ����һ����ADD
  }
  else 
  {
    operate_tpye = MODIFY;
  }
	
	if(global_vision_status == 2)
	{
		vision_color = FUCHSIA;
	}
	else
	{
		if(global_vision_status == 1)
		{
			vision_color = GREEN;
		}
		else
		{
			vision_color = YELLOW;
		}
	}
	
	/*��б*/
	Group4.grapic_data_struct[0] = draw_circle("g40",  //ͼ����
	                                     operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       vision_color,  //��ɫ
                                       2,  //�������
                                       Client_mid_position_x - 560,  //Բ�� x ����
                                       Client_mid_position_y + 75,  //Բ�� y ����
                                       13);  //�뾶
	
	Group4.grapic_data_struct[1] = draw_circle("g41",  //ͼ����
	                                     operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       vision_color,  //��ɫ
                                       2,  //�������
                                       Client_mid_position_x - 560,  //Բ�� x ����
                                       Client_mid_position_y + 75,  //Բ�� y ����
                                       22);  //�뾶
	/**/
	if(global_top_mode == 1)
	{
		top_color = GREEN;
	}
	else
	{
		top_color = YELLOW;
	}
	
  Group4.grapic_data_struct[2] = draw_circle("g42",  //ͼ����
	                                     operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       top_color,  //��ɫ
                                       2,  //�������
                                       Client_mid_position_x - 560,  //Բ�� x ����
                                       Client_mid_position_y + 131,  //Բ�� y ����
                                       13);  //�뾶
	
	Group4.grapic_data_struct[3] = draw_circle("g43",  //ͼ����
	                                     operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       top_color,  //��ɫ
                                       2,  //�������
                                       Client_mid_position_x - 560,  //Բ�� x ����
                                       Client_mid_position_y + 131,  //Բ�� y ����
                                       22);  //�뾶
																			 
	client_send_seven_graphic(Group4);
	
	/**/
	if(global_fric_status == 0)
	{
		fric_color = BLACK;
	}
	else
	{
		if(global_fric_status == 1)
		{
			fric_color = YELLOW;
		}
		else
		{
			fric_color = GREEN;
		}
	}
	
  Group4.grapic_data_struct[4] = draw_circle("g44",  //ͼ����
	                                     operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       fric_color,  //��ɫ
                                       2,  //�������
                                       Client_mid_position_x - 560,  //Բ�� x ����
                                       Client_mid_position_y + 19,  //Բ�� y ����
                                       13);  //�뾶
	
	Group4.grapic_data_struct[5] = draw_circle("g45",  //ͼ����
	                                     operate_tpye,  //ͼ�β���
                                       1,  //ͼ������0~9
                                       fric_color,  //��ɫ
                                       2,  //�������
                                       Client_mid_position_x - 560,  //Բ�� x ����
                                       Client_mid_position_y + 19,  //Բ�� y ����
                                       22);  //�뾶
																			 
	client_send_seven_graphic(Group4);
  
  cnt++;
  cnt %= 30;
}

/************************************************************************************************************************************************/






