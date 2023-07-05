#include "gimbal.h"
#include "gimbal_motor.h"
#include "config_gimbal.h"
#include "drv_can.h"
#include "mathematics.h"
#include "car.h"
#include "stdbool.h"
#include "config_can.h"
#include "vision.h"
#include "config_status.h"

extern motor_t Gim_Yaw;
extern motor_t Gim_Pitch;

ave_filter_t mec_yaw_angle_fil;
ave_filter_t mec_pitch_angle_fil;

float vision_see_yaw;
float vision_see_pitch;

bool gimbal_init_ok = false;
/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/

gimbal_info_t gimbal_info;

gimbal_t Gimbal = 
{
	.info = &gimbal_info,
	
	.imu_update = Gimbal_Imu_Update,
	.yaw_mec_ctrl = Gimbal_Yaw_Mec_Ctrl,
	.pitch_mec_ctrl = Gimbal_Pitch_Mec_Ctrl,
	.yaw_imu_ctrl = Gimbal_Yaw_Imu_Ctrl,
	.pitch_imu_ctrl = Gimbal_Pitch_Imu_Ctrl,
};

float gimb_y_mec_angle_in_pid_param[7]   = {-500,-6,0,0,8000,6000,28000};

float gimb_y_mec_angle_pid_param[7]      = {0.5,0,0,0,0,0,700};

float gimb_p_mec_angle_in_pid_param[7]   = {-300,-6,0,0,5000,5000,28000};

float gimb_p_mec_angle_pid_param[7]      = {1,0,0,5,0,0,500};

float gimb_y_imu_angle_in_pid_param[7]   = {-380,-2,0,0,4000,4000,28000};

float gimb_y_imu_angle_pid_param[7]      = {0.45,0,0,0,0,0,700};

float gimb_p_imu_angle_in_pid_param[7]   = {-320,-2,0,0,5000,5000,28000};

float gimb_p_imu_angle_pid_param[7]      = {0.75,0,0,0,0,0,500};

void Gimbal_Init_All(void)
{
  Gim_Pitch.init(&Gim_Pitch);
	Gim_Yaw.init(&Gim_Yaw);

	Gim_Yaw.pid_init(&Gim_Yaw.pid.angle_in,gimb_y_mec_angle_in_pid_param);
	Gim_Yaw.pid_init(&Gim_Yaw.pid.angle,gimb_y_mec_angle_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.angle_in,gimb_p_mec_angle_in_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.angle,gimb_p_mec_angle_pid_param);
	
	Gim_Yaw.pid_init(&Gim_Yaw.pid.imu_angle_in,gimb_y_imu_angle_in_pid_param);
	Gim_Yaw.pid_init(&Gim_Yaw.pid.imu_angle,gimb_y_imu_angle_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.imu_angle_in,gimb_p_imu_angle_in_pid_param);
	Gim_Pitch.pid_init(&Gim_Pitch.pid.imu_angle,gimb_p_imu_angle_pid_param);
	
	Gimbal.yaw_gimbal   = &Gim_Yaw;
	Gimbal.pitch_gimbal = &Gim_Pitch;
	
	Gimbal_Mode_Init(&Gimbal);
	Gimbal_Command_Init(&Gimbal);
}
/**
  * @brief  云台心跳包
  * @param  
  * @retval 
  */
void Gimbal_Heartbeat(void)
{
	Gim_Pitch.heartbeat(&Gim_Pitch);
	Gim_Yaw.heartbeat(&Gim_Yaw);
}

/**
  * @brief  云台陀螺仪角度，速度，机械角度更新
  * @param  gimbal_t* gimbal
  * @retval 
  */
void Gimbal_Imu_Update(gimbal_t* gimbal)
{
	gimbal->info->yaw_speed_imu_measure = imu.base_info->yaw_dif_speed_ave;
	
	gimbal->info->pitch_speed_imu_measure = imu.base_info->pitch_dif_speed_ave;
	
	gimbal->info->yaw_angle_imu_measure = -itm(imu.base_info->yaw);
	
	Gimbal_Yaw_Angle_Check(gimbal);
	
	gimbal->info->pitch_angle_imu_measure = -itm(imu.base_info->pitch);
  Gimbal_Pitch_Angle_Check(gimbal);
	
	gimbal->info->yaw_angle_mec_measure = gim_yaw_middle_angle - gimbal->yaw_gimbal->rx_info.angle;
	Gimbal_Yaw_Angle_Check(gimbal);
	gimbal->info->yaw_angle_mec_measure = ave_fil_update(&mec_yaw_angle_fil,gimbal->info->yaw_angle_mec_measure,10);
	
	if(gimbal->pitch_gimbal->rx_info.angle <= gim_pitch_middle_angle - 4096)
	{
		gimbal->info->pitch_angle_mec_measure = gim_pitch_middle_angle - 8191 - gimbal->pitch_gimbal->rx_info.angle;
	}
	else
	{
		gimbal->info->pitch_angle_mec_measure = gim_pitch_middle_angle - gimbal->pitch_gimbal->rx_info.angle;
	}
	gimbal->info->pitch_angle_mec_measure = ave_fil_update(&mec_pitch_angle_fil,gimbal->info->pitch_angle_mec_measure,15);
}

/**
  * @brief  云台模式初始化
  * @param  gimbal_t* gimbal
  * @retval 
  */
void Gimbal_Mode_Init(gimbal_t* gimbal)
{
	gimbal->info->yaw_mode = G_Y_sleep;
	gimbal->info->pitch_mode = G_P_sleep;
}

/**
  * @brief  云台指令初始化
  * @param  gimbal_t* gimbal
  * @retval 
  */
void Gimbal_Command_Init(gimbal_t* gimbal)
{
	gim_left_turn_90 = false;
	gim_right_turn_90 = false;
}

/**
  * @brief  云台模式状态更新
  * @param  gimbal_t* gimbal
  * @retval 
  */
void Gimbal_Mode_Update(gimbal_t* gimbal)
{
	if(car_mode_change == true)
	{
		switch(Car.car_move_status)
		{
			case sleep_car:
				gimbal->info->yaw_mode = G_Y_sleep;
			  gimbal->info->pitch_mode = G_P_sleep;
				break;
			case init_car:
				gimbal->info->yaw_mode = G_Y_init;
			  gimbal->info->pitch_mode = G_P_init;
			  if(abs(gimbal->info->yaw_angle_mec_measure) < 15 && \
 					abs(gimbal->info->pitch_angle_mec_measure) < 15)
				{
					gimbal_init_ok = true;
				}
			  break;
			case mec_car:
				gimbal->info->yaw_mode = G_Y_mec;
			  gimbal->info->pitch_mode = G_P_mec;
			  gimbal->info->pitch_angle_target = gimbal->info->pitch_angle_mec_measure;
			  break;
			case imu_car:
				gimbal->info->yaw_angle_target = gimbal->info->yaw_angle_imu_measure;
			  gimbal->info->pitch_angle_target = gimbal->info->pitch_angle_imu_measure;
				gimbal->info->yaw_mode = G_Y_imu;
			  gimbal->info->pitch_mode = G_P_imu;
			default:
				break;
		}
	}
}    

uint16_t headturn_cnt = 0;
int16_t mec_store = 0;
int16_t mec_change = 0;

/**
  * @brief  云台指令反应
  * @param  gimbal_t* gimbal
  * @retval 
  */
void Gimbal_Command_React(gimbal_t* gimbal)
{
	if(gim_left_turn_90 == true)
	{
		if(Car.car_move_status == imu_car)
		{
			gimbal->info->yaw_angle_target -= (0.25f * 8191.0f);
		}
	}
	if(gim_right_turn_90 == true)
	{
		if(Car.car_move_status == imu_car)
		{
			gimbal->info->yaw_angle_target += (0.25f * 8191.0f);
		}
	}
	if(gim_head_turn == true)
	{
		headturn_cnt++;
		if(Car.car_move_status == imu_car)
		{
			if(headturn_cnt == 1)
			{
				if(gimbal->info->yaw_angle_imu_measure <= 0)
				{
					gimbal->info->yaw_angle_target += 4096.0f;
				}
				else
				{
					gimbal->info->yaw_angle_target -= 4096.0f;
				}
				
				mec_store = gimbal->yaw_gimbal->rx_info.angle;
			}
			else
			{
				if(abs(gimbal->yaw_gimbal->pid.imu_angle.info.err) <= 100)
				{
					gim_head_turn = false;
					headturn_cnt = 0;
					
					mec_change = gimbal->yaw_gimbal->rx_info.angle - mec_store;
				  if(abs(mec_change) > 4096)
				  {
					  mec_change -= (sgn(mec_change) * 8191);
				  }
				}
			}
		}
	}
	if(gim_auto_on == true)
	{
		switch(Car.car_move_status)
		{
			case imu_car:
				gimbal->info->pitch_mode = G_P_auto;
			  gimbal->info->yaw_mode = G_Y_auto;
			  break;
			default:
				break;
		}
	}
	if(gim_auto_off == true)
	{
		switch(Car.car_move_status)
		{
			case imu_car:
		    gimbal->info->pitch_mode = G_P_imu;
		    gimbal->info->yaw_mode = G_Y_imu;
			  break;
			case mec_car:
				gimbal->info->pitch_mode = G_P_mec;
		    gimbal->info->yaw_mode = G_Y_mec;
			  break;
			default:
				break;
		}	
	}
}

void Gimbal_Work(gimbal_t* gimbal)
{
	switch(gimbal->info->yaw_mode)
	{
		case G_Y_sleep:
			break;
		case G_Y_init:
			gimbal->info->yaw_angle_target = 0;
		  gimbal->yaw_mec_ctrl(gimbal);
			break;
		case G_Y_imu:
			switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					gimbal->info->yaw_angle_target += rc.base_info->ch0 / 100.0f;
				  Gimbal_Yaw_Angle_Check(gimbal);
					gimbal->yaw_imu_ctrl(gimbal);
				  break;
				case KEY_CTRL:
					if(gim_head_turn == false)
					{
						gimbal->info->yaw_angle_target += rc.info->mouse_x_K / 8.0f;
						Gimbal_Yaw_Angle_Check(gimbal);
					}
					gimbal->yaw_imu_ctrl(gimbal);
			    break;
				default:
					break;
			}
			break;
		case G_Y_mec:
			  gimbal->info->yaw_angle_target = 0;
			  gimbal->yaw_mec_ctrl(gimbal);
			break;
		case G_Y_auto:
		  if(vision.base_info->capture_Y_O_N != 0 && vision.info->status == DEV_ONLINE)
			{
				gimbal->info->yaw_angle_target = vision.pro_info->yaw_move;
			}
			else
			{
				/*如果没有识别目标，操作手可以对云台操作*/
				gimbal->info->yaw_angle_target += rc.info->mouse_x_K / 8.0f;
			  Gimbal_Yaw_Angle_Check(gimbal);
			}
			gimbal->yaw_imu_ctrl(gimbal);
			break;
		default:
			break;
	}
	
	switch(gimbal->info->pitch_mode)
	{
		case G_P_sleep:
			break;
		case G_P_init:
			gimbal->info->pitch_angle_target = 0;
		  gimbal->pitch_mec_ctrl(gimbal);
			break;
		case G_P_imu:
			switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					gimbal->info->pitch_angle_target += (float)rc.base_info->ch1 / 200.f;
				  Gimbal_Pitch_Angle_Check(gimbal);
					gimbal->pitch_imu_ctrl(gimbal);
				  break;
				case KEY_CTRL:
					gimbal->info->pitch_angle_target -= (float)rc.info->mouse_y_K / 15.0f;
				  Gimbal_Pitch_Angle_Check(gimbal);
					gimbal->pitch_imu_ctrl(gimbal);
			    break;
				default:
					break;
			}
			break;
		case G_P_mec:
			  switch(Car.ctrl_mode)
			{
				case RC_CTRL:
					gimbal->info->pitch_angle_target += (rc.base_info->ch1 / 180.f) * 0.5f;//gimbal->info->pitch_angle_target += rc.base_info->ch1 / 180.f;
				  Gimbal_Pitch_Angle_Check(gimbal);
					gimbal->pitch_mec_ctrl(gimbal);
				  break;
				case KEY_CTRL:
					gimbal->info->pitch_angle_target -= (float)rc.info->mouse_y_K / 20.0f;
				  Gimbal_Pitch_Angle_Check(gimbal);
				  gimbal->pitch_mec_ctrl(gimbal);
			    break;
				default:
					break;
			}
			break;
		case G_P_auto:
			if(vision.base_info->capture_Y_O_N != 0 && vision.info->status == DEV_ONLINE)
			{
				gimbal->info->pitch_angle_target  = vision.pro_info->pitch_move;
			}
			else
			{
				/*如果没有识别目标，操作手可以对云台操作*/
				gimbal->info->pitch_angle_target -= (float)rc.info->mouse_y_K / 15.0f;
				Gimbal_Pitch_Angle_Check(gimbal);
			}
			
			gimbal->pitch_imu_ctrl(gimbal);
			break;
	}
}


void Gimbal_Ctrl(gimbal_t* gimbal)
{
	Gimbal_Mode_Update(gimbal);
	
	Gimbal_Command_React(gimbal);
	
	Gimbal_Work(gimbal);
	
	Gimbal_Command_Init(gimbal);
}

void Gimbal_Pitch_Angle_Check(gimbal_t* gimbal)
{
	float angle = 0.0f;
	
	gimbal_info_t* info = gimbal->info;
	
	angle = info->pitch_angle_target;
	if(angle > 900)//900//614
	{
		angle = 900;//900//614
	}
	if(angle < -380)
	{
		angle = -380;
	}
	info->pitch_angle_target = angle;
	
	angle = info->pitch_angle_imu_measure;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->pitch_angle_imu_measure = angle;
}

void Gimbal_Yaw_Angle_Check(gimbal_t* gimbal)
{
	float angle = 0.0f;
	
	gimbal_info_t* info = gimbal->info;
	
  angle = info->yaw_angle_target;
	if(abs(angle) > 4096)
	{
	  angle -= 8191 * sgn(angle);
	}
	info->yaw_angle_target = angle;
	
	angle = info->yaw_angle_mec_measure;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->yaw_angle_mec_measure = angle;
	
	angle = info->yaw_angle_imu_measure;
	if(abs(angle) > 4096)
	{
		angle -= 8191 * sgn(angle);
	}
	info->yaw_angle_imu_measure = angle;
}


void Gimbal_Yaw_Mec_Ctrl(gimbal_t* gim)
{
	motor_t *motor = gim->yaw_gimbal;
	
	can1_0x1FF_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.angle,&motor->pid.angle_in,
	                                                        gim->info->yaw_angle_mec_measure,gim->info->yaw_speed_imu_measure,
	                                                        gim->info->yaw_angle_target,1);
}

void Gimbal_Pitch_Mec_Ctrl(gimbal_t* gim)
{
	motor_t *motor = gim->pitch_gimbal;
	
	can2_0x1FF_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.angle,&motor->pid.angle_in,
	                                                        gim->info->pitch_angle_mec_measure,gim->info->pitch_speed_imu_measure,
	                                                        gim->info->pitch_angle_target,1);
}


void Gimbal_Yaw_Imu_Ctrl(gimbal_t* gimbal)
{
	gimbal_info_t*  info = gimbal->info;
	
	motor_t *motor = gimbal->yaw_gimbal;
	
	can1_0x1FF_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
	                                                        info->yaw_angle_imu_measure,info->yaw_speed_imu_measure,
	                                                        gimbal->info->yaw_angle_target,1);
}

void Gimbal_Pitch_Imu_Ctrl(gimbal_t* gimbal)
{
	gimbal_info_t*  info = gimbal->info;
	
	motor_t *motor = gimbal->pitch_gimbal;
	
	can2_0x1FF_send_buff[motor->id.buff_p] = motor->c_pid2(&motor->pid.imu_angle,&motor->pid.imu_angle_in,
	                                                        info->pitch_angle_imu_measure,info->pitch_speed_imu_measure,
	                                                        gimbal->info->pitch_angle_target,1);
}

