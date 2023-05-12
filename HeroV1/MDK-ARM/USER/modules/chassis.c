#include "chassis.h"
#include "motor.h"
#include "drv_can.h"
#include "chassis_motor.h"
#include "config_status.h"
#include "rp_chassis.h"
#include "remote.h"
#include "math.h"
#include "mathematics.h"
#include "car.h"
#include "config_chassis.h"
#include "config_can.h"
#include "judge.h"
#include "config_gimbal.h"

/*跟随模式换头标志位*/
bool face_back_or_n = false;

extern bool chas_speed_up;
extern bool gimbal_init_ok;
extern chassis_t Chassis;

Chassis_InitTypeDef chassis_all;

Chassis_Mode_t Chassis_Mode;

Chassis_Info_t Chassis_Info;

/*遥控通道值暂存变量*/
Chassis_Channel_Input_t Chassis_Channel_Input;

Chassis_Config_Info_t Chassis_Config = 
{
	.speed_max = chassis_speed_max_init,
	.top_period = chassis_top_period,
	.top_mode = chassis_top_mode,
};

float chassis_speed_pid_param[7] = {8.0f,0.33f,0,0,10000,6000,15000};

/**
  * @brief  底盘电机初始化
  * @param  
  * @retval 
  */
void Chassis_Init_All(void)
{
	chassis_motor[LF].init(&chassis_motor[LF]);
	chassis_motor[RF].init(&chassis_motor[RF]);
	chassis_motor[LB].init(&chassis_motor[LB]);
	chassis_motor[RB].init(&chassis_motor[RB]);
	
	chassis_motor[LF].pid_init(&chassis_motor[LF].pid.speed,chassis_speed_pid_param);
	chassis_motor[RF].pid_init(&chassis_motor[RF].pid.speed,chassis_speed_pid_param);
	chassis_motor[LB].pid_init(&chassis_motor[LB].pid.speed,chassis_speed_pid_param);
	chassis_motor[RB].pid_init(&chassis_motor[RB].pid.speed,chassis_speed_pid_param);
	
	chassis_all.motor_LB = &chassis_motor[LB];
	chassis_all.motor_LF = &chassis_motor[LF];
	chassis_all.motor_RF = &chassis_motor[RF];
	chassis_all.motor_RB = &chassis_motor[RB];
  
	Chassis.init(&Chassis, &chassis_all);
	Chassis.work_info.power_limit_buffer = &judge.base_info->chassis_power_buffer;

	Chassis_Mode_Init(&Chassis_Mode);
	Chassis_Command_Init();
}

/**
  * @brief  底盘电机心跳包
  * @param  
  * @retval 
  */
void Chassis_Heartbeat(void)
{
	chassis_motor[LF].heartbeat(&chassis_motor[LF]);
	chassis_motor[RF].heartbeat(&chassis_motor[RF]);
	chassis_motor[LB].heartbeat(&chassis_motor[LB]);
	chassis_motor[RB].heartbeat(&chassis_motor[RB]);
}

/**
  * @brief  底盘运动模式初始化
  * @param  底盘运动模式结构体
  * @retval 
  */
void Chassis_Mode_Init(Chassis_Mode_t* chassis_mode)
{
	chassis_mode->chassis_move_mode = C_M_sleep;
	chassis_mode->chassis_spin_mode = C_S_sleep;
}

/**
  * @brief  底盘指令更新，目前只有小陀螺
  * @param  
  * @retval 
  */
void Chassis_Command_Init(void)
{
	top_car_on = false;
  top_car_off = false;
	top_switch = false;
}

/**
  * @brief  底盘运动模式状态更新
  * @param  底盘运动模式结构体
  * @retval 
  */
void Chassis_Mode_Update(Chassis_Mode_t* chassis_mode)
{
	if(car_mode_change == true)
	{
		switch(Car.car_move_status)
		{
			case sleep_car:
				chassis_mode->chassis_move_mode = C_M_stop;
			  chassis_mode->chassis_spin_mode = C_S_stop;
				break;
			case init_car:
				chassis_mode->chassis_move_mode = C_M_stop;
			  chassis_mode->chassis_spin_mode = C_S_stop;
		    break;
			case imu_car:
				chassis_mode->chassis_move_mode = C_M_special;
			  chassis_mode->chassis_spin_mode = C_S_follow;
			  break;
			case mec_car:
				chassis_mode->chassis_move_mode = C_M_normal;
			  chassis_mode->chassis_spin_mode = C_S_normal;
			default:
				break;
		}
	}
}

/**
  * @brief  底盘指令控制，目前只有小陀螺的指令
  * @param  底盘运动模式结构体
  * @retval 
  */
void Chassis_Command_React(Chassis_Mode_t* chassis_mode)
{
	switch(Car.ctrl_mode)
	{
		case RC_CTRL:
			if(top_car_off == true)
	    {
		    if(chassis_mode->chassis_spin_mode == C_S_top)
		    {
		  	chassis_mode->chassis_spin_mode = C_S_follow;
		    }
	    }
	    if(top_car_on == true)
	    {
		    if(chassis_mode->chassis_move_mode == C_M_special)
		    {
		    	chassis_mode->chassis_spin_mode = C_S_top;
		    }
		    else
		    {
		  	  
		    }
	    }
			break;
		case KEY_CTRL:
			if(top_switch == true)
		  {
			  if(chassis_mode->chassis_move_mode == C_M_special)
		    {
			  	if(chassis_mode->chassis_spin_mode == C_S_top)
			  	{
			  		chassis_mode->chassis_spin_mode = C_S_follow;
			  	}
		    	else
		  		{
		  			chassis_mode->chassis_spin_mode = C_S_top;
		  		}
	  	  }
		    else
		    {
			    
		    }
	  	}
			break;
		default:
			break;
	}
}

/**
  * @brief  底盘总控制函数
  * @param  底盘运动模式结构体
  * @retval 
  */
void Chassis_Ctrl(Chassis_Mode_t* chassis_mode)
{
	Chassis_Mode_Update(chassis_mode);
	
	Chassis_Command_React(chassis_mode);
	
	Chassis_Process(chassis_mode);
		
	Chassis_Command_Init();
}

float err_test = 0.0f;

/**
  * @brief  底盘总控制函数
  * @param  底盘运动模式结构体
  * @retval 
  */
void Chassis_Process(Chassis_Mode_t* chassis_mode)
{
	float err_yaw = 0.0f, err_yaw_zero = 0.0f, err_radian_yaw = 0.0f, double_pi = 3.14159f * 2.0f;
	int16_t front = 0,right = 0;
	float rc_ctrl_buff = 0.0f;
	
	rc_ctrl_buff = Chassis_Config.speed_max / 660;
	
	/*yaw角度换算*/
	err_yaw = chassis_yaw_cal();
	/*yaw差值弧度制换算*/
	err_radian_yaw = err_yaw / 8191.0f * double_pi;

	if(err_yaw <= 2048 || err_yaw >= 6144)
	{
		face_back_or_n = false;
	}
	else
	{
		face_back_or_n = true;
	}
	
	switch(chassis_mode->chassis_move_mode)
	{
		case C_M_sleep:
			break;
		case C_M_stop:
			Chassis.base_info.target.front_speed = 0;
		  Chassis.base_info.target.right_speed = 0;
		  break;
		case C_M_normal:
			switch(Car.ctrl_mode)
			{
				case RC_CTRL:
						Chassis.base_info.target.front_speed = (int16_t)((float)rc.base_info->ch3 * 4);
						Chassis.base_info.target.right_speed = (int16_t)((float)rc.base_info->ch2 * 4);
					break;
				case KEY_CTRL:
					front += (int16_t)((float)rc.base_info->W.cnt / (float)KEY_W_CNT_MAX * (float)Chassis_Config.speed_max);
				  front -= (int16_t)((float)rc.base_info->S.cnt / (float)KEY_S_CNT_MAX * (float)Chassis_Config.speed_max);
				  right += (int16_t)((float)rc.base_info->D.cnt / (float)KEY_D_CNT_MAX * (float)Chassis_Config.speed_max);
				  right -= (int16_t)((float)rc.base_info->A.cnt / (float)KEY_A_CNT_MAX * (float)Chassis_Config.speed_max);
				
				  Chassis.base_info.target.front_speed = front;
				  Chassis.base_info.target.right_speed = right;
					break;
				default:
					break;
			}
		  break;
		case C_M_special:
		  switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					rc_input_process(&Chassis_Channel_Input);
					Chassis.base_info.target.front_speed = (Chassis_Channel_Input.ch3_process * rc_ctrl_buff) * cos(err_radian_yaw) - \
					                                       (Chassis_Channel_Input.ch2_process * rc_ctrl_buff) * sin(err_radian_yaw);
		      Chassis.base_info.target.right_speed = (Chassis_Channel_Input.ch2_process * rc_ctrl_buff) * cos(err_radian_yaw) + \
					                                       (Chassis_Channel_Input.ch3_process * rc_ctrl_buff) * sin(err_radian_yaw);
					break;
		case KEY_CTRL:
				front += (((float)rc.base_info->W.cnt / (float)KEY_W_CNT_MAX) * (float)Chassis_Config.speed_max);
				front -= (((float)rc.base_info->S.cnt / (float)KEY_S_CNT_MAX) * (float)Chassis_Config.speed_max);
				right += (((float)rc.base_info->D.cnt / (float)KEY_D_CNT_MAX) * (float)Chassis_Config.speed_max);
				right -= (((float)rc.base_info->A.cnt / (float)KEY_A_CNT_MAX) * (float)Chassis_Config.speed_max);

				front = front * cos(err_radian_yaw) - right * sin(err_radian_yaw);
				right = right * cos(err_radian_yaw) + front * sin(err_radian_yaw);
					
				if(chas_speed_up == true)
		    {
			    Chassis.base_info.target.front_speed = (front * 1.2f);
		      Chassis.base_info.target.right_speed = (right * 1.2f);
  		  }
			  else
			  {
		  		Chassis.base_info.target.front_speed = (front / 1.5f);
		  		Chassis.base_info.target.right_speed = (right / 1.5f);
		  	}
				break;
		default:
			break;
			}
		  break;	
			
		default:
			break;
	}
	
	switch(chassis_mode->chassis_spin_mode)
	{
		case C_S_sleep:
			break;
		case C_S_stop:
			Chassis.base_info.target.cycle_speed = 0;
			break;
		case C_S_normal:
			Chassis.base_info.target.cycle_speed = (int16_t)((float)rc.base_info->ch0 * rc_ctrl_buff);
		  break;
		case C_S_follow:
			/*过零点处理*/
			if(face_back_or_n == false)
			{
				if(err_yaw <= 2048)
				{
					err_yaw_zero = err_yaw;
				}
				else
				{
					err_yaw_zero = err_yaw - 8191;
				}
			}
			else
			{
				err_yaw_zero = err_yaw - 4096;
			}
			
			/*底盘跟随模式开环折线控制*/
			if(abs(err_yaw_zero) >= 100)
			{
				Chassis.base_info.target.cycle_speed = 8 * err_yaw_zero - sgn(err_yaw_zero) * 400;
			}
			else
			{
				Chassis.base_info.target.cycle_speed = 4 * err_yaw_zero;
			}
			
			err_test = Chassis.base_info.target.cycle_speed;
			
			//换头转动输出为零
			if(gim_head_turn == true)
			{
				Chassis.base_info.target.cycle_speed = 0;
			}
		  
		  break;
		case C_S_top:
			switch(Chassis_Config.top_mode)
			{
				case 0:
					Chassis.base_info.target.cycle_speed = chassis_top_speed_level_1;
				  if(abs(Chassis.base_info.target.front_speed) >= 500 || abs(Chassis.base_info.target.right_speed) >= 500)
					{
						Chassis.base_info.target.cycle_speed *= 0.5f;
//						Chassis.base_info.target.front_speed *= 0.4f;
//						Chassis.base_info.target.right_speed *= 0.4f;
					}
					break;
				case 1:
					/*匀加速小陀螺*/

					break;
				default:
					break;
			}
			  
			break;
		default:
			break;
	}
	
	Chassis.work(&Chassis);
}

/**
  * @brief  底盘--yaw云台正对角度误差计算，返回角度误差值,0~8191,顺时针值变大
  * @param  
  * @retval 
  */
float chassis_yaw_cal(void)
{
	float angle_cal = 0.0f;
	
	if(Gimbal.yaw_gimbal->rx_info.angle <= gim_yaw_middle_angle && Gimbal.yaw_gimbal->rx_info.angle >= 0)
	{
		angle_cal = gim_yaw_middle_angle - Gimbal.yaw_gimbal->rx_info.angle;
	}
	else
	{
		angle_cal = gim_yaw_middle_angle  + (8191 - Gimbal.yaw_gimbal->rx_info.angle);
	}
	
	return angle_cal;
}

/**
  * @brief  遥控输入梯度处理
  * @param  Chassis_Channel_Input_t 结构体
  * @retval 
  */
void rc_input_process(Chassis_Channel_Input_t* channel)
{
	channel->ch3_now = rc.base_info->ch3;
	if(channel->ch3_now != 0)
	{
		if(abs(channel->ch3_now - channel->ch3_last) > 30)
		{
			channel->ch3_now += 30 * sgn(channel->ch3_now - channel->ch3_last);
			channel->ch3_last = channel->ch3_now;
		}
		else
		{
			channel->ch3_last = channel->ch3_now;
		}
	}
	else
	{
		if(abs(channel->ch3_now - channel->ch3_last) > 40)
		{
  		channel->ch3_now += 60 * sgn(channel->ch3_now - channel->ch3_last);
		}
		else
		{
			channel->ch3_now = 0;
			channel->ch3_last = channel->ch3_now;
		}
	}
	channel->ch2_now = rc.base_info->ch2;
	if(channel->ch2_now != 0)
	{
		if(abs(channel->ch2_now - channel->ch2_last) > 30)
		{
		  channel->ch2_now += 30 * sgn(channel->ch2_now - channel->ch2_last);
			channel->ch2_last = channel->ch2_now;
		}
		else
		{
			channel->ch2_last = channel->ch2_now;
		}
	}
	else
	{
		if(abs(channel->ch2_now - channel->ch2_last) > 40)
		{
			channel->ch2_now += 60 * sgn(channel->ch2_now - channel->ch2_last);
		}
		else
		{
			channel->ch2_now = 0;
			channel->ch2_last = channel->ch2_now;
		}
	}
	
	channel->ch3_process = channel->ch3_now;
	channel->ch2_process = channel->ch2_now;
}
