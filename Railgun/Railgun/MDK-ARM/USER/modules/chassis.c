#include "chassis.h"
#include "chassis_motor.h"
#include "remote.h"
#include "railgun.h"
#include "rp_math.h"
#include "string.h"

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/
float wheels_speed_param[7] = {8.0f,0.33f,0,0,8000,8000,15000};

/*rp_pack_require begin*/
extern chassis_t Chassis;
Chassis_InitTypeDef Chassis_All;
/*end*/

rc_channel_input_t rc_channel_input;
wasd_channel_input_t wasd_channel_input;

my_chassis_info_t chassis_info = 
{
	.rc_input = &rc_channel_input,
};

my_chassis_t my_Chassis = 
{
	.info = &chassis_info,
};

void chassis_init(void)
{
	chassis_motor[LF_WHEEL].init(&chassis_motor[LF_WHEEL]);
	chassis_motor[RF_WHEEL].init(&chassis_motor[RF_WHEEL]);
	chassis_motor[LB_WHEEL].init(&chassis_motor[LB_WHEEL]);
	chassis_motor[RB_WHEEL].init(&chassis_motor[RB_WHEEL]);
	
	chassis_motor[LF_WHEEL].pid_init(&chassis_motor[LF_WHEEL].pid.speed,wheels_speed_param);
	chassis_motor[RF_WHEEL].pid_init(&chassis_motor[RF_WHEEL].pid.speed,wheels_speed_param);
	chassis_motor[LB_WHEEL].pid_init(&chassis_motor[LB_WHEEL].pid.speed,wheels_speed_param);
	chassis_motor[RB_WHEEL].pid_init(&chassis_motor[RB_WHEEL].pid.speed,wheels_speed_param);
  
	my_Chassis.lf_motor = &chassis_motor[LF_WHEEL];
	my_Chassis.rf_motor = &chassis_motor[RF_WHEEL];
	my_Chassis.lb_motor = &chassis_motor[LB_WHEEL];
	my_Chassis.rb_motor = &chassis_motor[RB_WHEEL];
	
	Chassis_All.motor_LF = &chassis_motor[LF_WHEEL];
	Chassis_All.motor_RF = &chassis_motor[RF_WHEEL];
	Chassis_All.motor_LB = &chassis_motor[LB_WHEEL];
	Chassis_All.motor_RB = &chassis_motor[RB_WHEEL];
	
	my_Chassis.rp_pack = &Chassis;
	
	Chassis.init(&Chassis, &Chassis_All);
	/*裁判系统需要 begin*/
//	Chassis.work_info.power_limit_buffer = &judge.base_info->chassis_power_buffer;
  /*end*/
	
}

void chassis_heartbeat(void)
{
	chassis_motor[LF_WHEEL].heartbeat(&chassis_motor[LF_WHEEL]);
	chassis_motor[RF_WHEEL].heartbeat(&chassis_motor[RF_WHEEL]);
	chassis_motor[LB_WHEEL].heartbeat(&chassis_motor[LB_WHEEL]);
	chassis_motor[RB_WHEEL].heartbeat(&chassis_motor[RB_WHEEL]);
}

/*底盘命令->状态控制*/
void chassis_command_react(my_chassis_t* my_chassis)
{
	/*底盘移动速度响应begin*/
	switch(RAILGUN.command->chassis_command->shifting_speed_command)
	{
		case high_speed:
			my_chassis->info->shifting_buff = HIGH_SPEED_BUFF;
			break;
		case middle_speed:
			my_chassis->info->shifting_buff = MIDDLE_SPEED_BUFF;
			break;
		case low_speed:
			my_chassis->info->shifting_buff = LOW_SPEED_BUFF;
			break;
		default:
			break;
	}
	/*底盘移动速度响应end*/
	
}

/*底盘状态响应*/
void chassis_status_react(my_chassis_t* my_chassis)
{
	switch(RAILGUN.status)
	{
		case init_mode:
			my_chassis->status = LOCK;
			break;
		case camer_mode:
			my_chassis->status = LOCK;
			break;
		case shoot_mode:
			my_chassis->status = LOCK;
			break;
		case shift_mode:
			my_chassis->status = UNLOCK;
			break;
		default:
			break;
	}
}

void chassis_ctrl(my_chassis_t* my_chassis)
{
	switch(my_chassis->status)
	{
		case LOCK:
			switch(RAILGUN.ctrl)
			{
				case RC_CTRL:
					/*底盘锁死*/
					my_chassis->rp_pack->base_info.target.right_speed = 0;
	        my_chassis->rp_pack->base_info.target.front_speed = 0;
	        my_chassis->rp_pack->base_info.target.cycle_speed = 0;
				  memset(&wasd_channel_input,0,sizeof(wasd_channel_input));//这里可能会跑飞
				break;
				case KEY_CTRL:
					my_chassis->rp_pack->base_info.target.right_speed = 0;
	        my_chassis->rp_pack->base_info.target.front_speed = 0;
	        my_chassis->rp_pack->base_info.target.cycle_speed = 0;
				  memset(&wasd_channel_input,0,sizeof(wasd_channel_input));
					break;
				default:
					break;
			}
			break;
		case UNLOCK:
			switch(RAILGUN.ctrl)
			{
				case RC_CTRL:
					rc_input_process(my_chassis->info->rc_input);
				  my_chassis->rp_pack->base_info.target.front_speed = my_chassis->info->rc_input->ch3_process * \
				                                                      (int16_t)(SHIFTING_MAX_SPEED / 660) * my_chassis->info->shifting_buff;
				  my_chassis->rp_pack->base_info.target.right_speed = my_chassis->info->rc_input->ch2_process * \
				                                                      (int16_t)(SHIFTING_MAX_SPEED / 660) * my_chassis->info->shifting_buff;
				  my_chassis->rp_pack->base_info.target.cycle_speed = rc.base_info->ch0 * \
				                                                      (int16_t)(SHIFTING_MAX_SPEED / 660) * my_chassis->info->shifting_buff;
					break;
				case KEY_CTRL:
					
					break;
				default:
					break;
			}
			break;
	}
	
	my_chassis->rp_pack->work(my_chassis->rp_pack);
}

void chassis(void)
{
	chassis_command_react(&my_Chassis);
	
	chassis_status_react(&my_Chassis);
	
	chassis_ctrl(&my_Chassis);
}

/**
  * @brief  遥控输入梯度处理
  * @param  rc_channel_input_t 结构体
  * @retval 
  */
void rc_input_process(rc_channel_input_t *rc_input)
{
	rc_input->ch3_now = rc.base_info->ch3;
	if(rc_input->ch3_now != 0)
	{
		if(abs(rc_input->ch3_now - rc_input->ch3_last) > 30)
		{
			rc_input->ch3_now += 30 * sgn(rc_input->ch3_now - rc_input->ch3_last);
			rc_input->ch3_last = rc_input->ch3_now;
		}
		else
		{
			rc_input->ch3_last = rc_input->ch3_now;
		}
	}
	else
	{
		if(abs(rc_input->ch3_now - rc_input->ch3_last) > 40)
		{
  		rc_input->ch3_now += 60 * sgn(rc_input->ch3_now - rc_input->ch3_last);
		}
		else
		{
			rc_input->ch3_now = 0;
			rc_input->ch3_last = rc_input->ch3_now;
		}
	}
	rc_input->ch2_now = rc.base_info->ch2;
	if(rc_input->ch2_now != 0)
	{
		if(abs(rc_input->ch2_now - rc_input->ch2_last) > 30)
		{
		  rc_input->ch2_now += 30 * sgn(rc_input->ch2_now - rc_input->ch2_last);
			rc_input->ch2_last = rc_input->ch2_now;
		}
		else
		{
			rc_input->ch2_last = rc_input->ch2_now;
		}
	}
	else
	{
		if(abs(rc_input->ch2_now - rc_input->ch2_last) > 40)
		{
			rc_input->ch2_now += 60 * sgn(rc_input->ch2_now - rc_input->ch2_last);
		}
		else
		{
			rc_input->ch2_now = 0;
			rc_input->ch2_last = rc_input->ch2_now;
		}
	}
	
	rc_input->ch3_process = rc_input->ch3_now;
	rc_input->ch2_process = rc_input->ch2_now;
}

/**
  * @brief  键盘输入梯度处理
  * @param  rc_channel_input_t 结构体
  * @retval 
  */
void WASD_input_process(wasd_channel_input_t *wasd_input)
{
	wasd_input->w_now= rc.base_info->W.cnt;
	if(wasd_input->w_now != 0)
	{
		if(abs(wasd_input->w_now - wasd_input->w_last) > 30)
		{
			wasd_input->w_now += 30 * sgn(wasd_input->w_now - wasd_input->w_last);
			wasd_input->w_last = wasd_input->w_now;
		}
		else
		{
			wasd_input->w_last = wasd_input->w_now;
		}
	}
	else
	{
		if(abs(wasd_input->w_now - wasd_input->w_last) > 40)
		{
  		wasd_input->w_now += 60 * sgn(wasd_input->w_now - wasd_input->w_last);
		}
		else
		{
			wasd_input->w_now = 0;
			wasd_input->w_last = wasd_input->w_now;
		}
	}
	
	wasd_input->a_now= rc.base_info->A.cnt;
	if(wasd_input->a_now != 0)
	{
		if(abs(wasd_input->a_now - wasd_input->a_last) > 30)
		{
			wasd_input->a_now += 30 * sgn(wasd_input->a_now - wasd_input->a_last);
			wasd_input->a_last = wasd_input->a_now;
		}
		else
		{
			wasd_input->a_last = wasd_input->a_now;
		}
	}
	else
	{
		if(abs(wasd_input->a_now - wasd_input->a_last) > 40)
		{
  		wasd_input->a_now += 60 * sgn(wasd_input->a_now - wasd_input->a_last);
		}
		else
		{
			wasd_input->a_now = 0;
			wasd_input->a_last = wasd_input->a_now;
		}
	}
	
	wasd_input->s_now= rc.base_info->S.cnt;
	if(wasd_input->s_now != 0)
	{
		if(abs(wasd_input->s_now - wasd_input->s_last) > 30)
		{
			wasd_input->s_now += 30 * sgn(wasd_input->s_now - wasd_input->s_last);
			wasd_input->s_last = wasd_input->s_now;
		}
		else
		{
			wasd_input->s_last = wasd_input->s_now;
		}
	}
	else
	{
		if(abs(wasd_input->s_now - wasd_input->s_last) > 40)
		{
  		wasd_input->s_now += 60 * sgn(wasd_input->s_now - wasd_input->s_last);
		}
		else
		{
			wasd_input->s_now = 0;
			wasd_input->s_last = wasd_input->s_now;
		}
	}
	
	wasd_input->d_now= rc.base_info->D.cnt;
	if(wasd_input->d_now != 0)
	{
		if(abs(wasd_input->d_now - wasd_input->d_last) > 30)
		{
			wasd_input->d_now += 30 * sgn(wasd_input->d_now - wasd_input->d_last);
			wasd_input->d_last = wasd_input->d_now;
		}
		else
		{
			wasd_input->d_last = wasd_input->d_now;
		}
	}
	else
	{
		if(abs(wasd_input->d_now - wasd_input->d_last) > 40)
		{
  		wasd_input->d_now += 60 * sgn(wasd_input->d_now - wasd_input->d_last);
		}
		else
		{
			wasd_input->d_now = 0;
			wasd_input->d_last = wasd_input->d_now;
		}
	}
	
	wasd_input->w_process = wasd_input->w_now;
	wasd_input->a_process = wasd_input->a_now;
	wasd_input->s_process = wasd_input->s_now;
	wasd_input->d_process = wasd_input->d_now;
}

