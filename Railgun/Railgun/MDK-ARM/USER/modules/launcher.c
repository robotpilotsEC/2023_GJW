#include "launcher.h"
#include "launcher_motor.h"
#include "remote.h"
#include "rp_math.h"
#include "config_can.h"
#include "railgun.h"
#include "drv_timer.h"

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/

float main_launch_position_in_param[7] = {10,0.4,0,0,8000,8000,15000};
float main_launch_position_param[7] = {0.2,0,0,0,8000,8000,15000};
float main_launch_speed_param[7] = {10,0.4,0,0,8000,8000,3000};

float feeder_position_in_param[7] = {10,0.4,0,0,8000,8000,15000};
float feeder_position_param[7] = {0.2,0,0,0,8000,8000,15000};
float feeder_speed_param[7] = {10,0.4,0,0,8000,8000,8000};

launcher_info_t launcher_info = 
{
	.main_launch_init_stuck_cnt = 0,
	.main_launch_target = 0.0f,
	.main_launch_init_flag = false,
	.main_launch_stuck_flag  = false,
};

launcher_t Launcher = 
{
	.info = &launcher_info,
	
	.main_launch_ctrl = main_launcher_ctrl,
	
	.feeder_ctrl = feeder_ctrl,
};

void launcher_init(void)
{
	launcher_motor[MAIN_LAUNCHER].init(&launcher_motor[MAIN_LAUNCHER]);
	launcher_motor[FEEDER].init(&launcher_motor[FEEDER]);
	
	launcher_motor[MAIN_LAUNCHER].pid_init(&launcher_motor[MAIN_LAUNCHER].pid.position_in,main_launch_position_in_param);
	launcher_motor[MAIN_LAUNCHER].pid_init(&launcher_motor[MAIN_LAUNCHER].pid.position,main_launch_position_param);
	launcher_motor[MAIN_LAUNCHER].pid_init(&launcher_motor[MAIN_LAUNCHER].pid.speed,main_launch_speed_param);
	launcher_motor[FEEDER].pid_init(&launcher_motor[FEEDER].pid.position_in,feeder_position_in_param);
	launcher_motor[FEEDER].pid_init(&launcher_motor[FEEDER].pid.position,feeder_position_param);
	launcher_motor[FEEDER].pid_init(&launcher_motor[FEEDER].pid.speed,feeder_speed_param);
	
	Launcher.main_launch = &launcher_motor[MAIN_LAUNCHER];
	Launcher.feeder = &launcher_motor[FEEDER];
	
	Launcher.info->feeder_init_status = speed_state;
}

void launcher_heartbeat(void)
{
	launcher_motor[MAIN_LAUNCHER].heartbeat(&launcher_motor[MAIN_LAUNCHER]);
	launcher_motor[FEEDER].heartbeat(&launcher_motor[FEEDER]);
}

void launcher_command_react(launcher_t* launch)
{
	switch(RAILGUN.command->shoot_command->launcher_command)
	{
		case fire:
			launch->info->servo_delay_cnt++;
		  if(launch->info->servo_delay_cnt >= SERVO_DELAY_TIME)
			{
				launch->info->fire_ready = false;
				launch->info->servo_delay_cnt = 0;
			}
			servo_unlock();
		  launch->info->feeder_reload_ok = false;
		  launch->info->main_launcher_reload_ok = false;
		  launch->info->main_launcher_reload_sup_ok = false;
		  launch->info->reload_processing = false;
			launch->info->main_launch_up_state_in_cnt = 0;
			launch->info->main_launch_down_state_in_cnt = 0;
			break;
		case reload:
			break;
		case launch_standby:
			break;
		default:
			break;
	}
}

void launcher_command_refresh(launcher_t* launch)
{
  launch->info->feeder_foreback_time = 0;
	launch->info->feeder_init_stuck_cnt = 0;
	launch->info->feeder_moving_state = foreward_state;
	launch->info->feeder_reload_ok = false;
	launch->info->feeder_state_in_cnt = 0;
	launch->info->feeder_stuck_cnt = 0;
	launch->info->feeder_stuck_flag = false;
	launch->info->feeder_target_save = 0;
	launch->info->fire_ready = false;
	launch->info->main_launcher_reload_ok = false;
	launch->info->main_launcher_reload_sup_ok = false;
	launch->info->main_launch_down_state_in_cnt = 0;
	launch->info->main_launch_init_stuck_cnt = 0;
	launch->info->main_launch_stuck_cnt = 0;
	launch->info->main_launch_stuck_flag = false;
	launch->info->main_launch_up_state_in_cnt = 0;
	launch->info->reload_processing = false;
	launch->info->servo_delay_cnt = 0;
}

void launcher_status_react(launcher_t* launch)
{
	switch(RAILGUN.status)
	{
		case init_mode:
			launch->status = LA_INIT;
			break;
		case camer_mode:
			launch->status = LA_LOCK;
			break;
		case shoot_mode:
			launch->status = LA_UNLOCK;
			break;
		case shift_mode:
			launch->status = LA_LOCK;
			break;
		default:
			break;
	}
}

void launcher_position_init(void)
{
	/*MAIN_LAUNCHER初始化部分*/
	if(abs(Launcher.main_launch->pid.speed.info.err) >= 100 && abs(Launcher.main_launch->rx_info.speed) <= 3)
	{
		Launcher.info->main_launch_init_stuck_cnt++;
	}
	if(Launcher.info->main_launch_init_stuck_cnt >= 50)
	{
		Launcher.info->main_launch_init_stuck_cnt = 0;
		
		Launcher.info->main_launch_init_flag = true;
		
		Launcher.info->main_launch_target = Launcher.main_launch->rx_info.angle_sum;
		
		Launcher.info->main_launch_lower_limit = Launcher.main_launch->rx_info.angle_sum;
		Launcher.info->main_launch_superior_limit = Launcher.main_launch->rx_info.angle_sum + LAUNCHER_MOION_RANGE;//加还是减 要测试
	}
	/*有电机失联或已完成初始化发0*/
	if(Launcher.main_launch->state.work_state == M_OFFLINE || Launcher.info-> main_launch_init_flag == true)
	{
		can1_0x200_send_buff[Launcher.main_launch->id.buff_p] = 0;
	}
	/*未完成初始化以恒定速度向下移动*/
	else
	{
		can1_0x200_send_buff[Launcher.main_launch->id.buff_p] = Launcher.main_launch->c_speed(Launcher.main_launch,LAU_MAIN_LAUNCH_POS_INIT_SPEED);
	}
	
	/*FEEDER初始化部分*/
  switch(Launcher.info->feeder_init_status)
	{
		case speed_state:
			can1_0x1FF_send_buff[Launcher.feeder->id.buff_p] = Launcher.feeder->c_speed(Launcher.feeder,LAU_FEEDER_POS_INIT_SPEED);
	    if(abs(Launcher.feeder->pid.speed.info.err) >= 100 && abs(Launcher.feeder->rx_info.speed) <= 3)
	    {
		    Launcher.info->feeder_init_stuck_cnt++;
	    }
	    if(Launcher.info->feeder_init_stuck_cnt >= 200)
	    {
				if(Launcher.info->feeder_foreback_time < 2)
				{
					/*堵转了*/
					Launcher.info->feeder_state_in_cnt = 0;
					Launcher.info->feeder_init_status = position_state;
				}
		    else
				{
					Launcher.info->feeder_init_flag = true;
					
					Launcher.info->fire_ready = false;
					Launcher.info->feeder_moving_state = foreward_state;
					Launcher.info->feeder_foreback_time = 0;
					Launcher.info->feeder_state_in_cnt = 0;
				}
				Launcher.info->feeder_init_stuck_cnt = 0;
	    }
			break;
		case position_state:
			/*向后转一定角度*/
		  if(Launcher.info->feeder_state_in_cnt == 0)//到位了
			{
				Launcher.info->feeder_target = Launcher.feeder->rx_info.angle_sum - FEEDER_HALF_BALL_POS;
			}
		  Launcher.feeder_ctrl(&Launcher,Launcher.info->feeder_target);
			
		  if(feeder_stuck_test(&Launcher) || abs(Launcher.feeder->pid.position.info.err) < 300)//堵转或达到目标值
			{
				Launcher.info->feeder_foreback_time++;
				Launcher.info->feeder_init_status = speed_state;
			}
			/*确保只有第一次加目标值*/
			Launcher.info->feeder_state_in_cnt++;
			break;
		default:
			break;
	}
  if(Launcher.feeder->state.work_state == M_OFFLINE || Launcher.info->feeder_init_flag == true)
	{
		can1_0x1FF_send_buff[Launcher.feeder->id.buff_p] = 0;
	}
	
	if(Launcher.info->feeder_init_flag == true && Launcher.info-> main_launch_init_flag == true)
	{
		Launcher.info->launcher_init_flag = true;
		launcher_command_refresh(&Launcher);
	}
	else
	{
		Launcher.info->launcher_init_flag = false;
	}
}

void launcher(void)
{
	launcher_command_react(&Launcher);
	
	launcher_status_react(&Launcher);
	
	main_launch_ctrl(&Launcher);
	
	feed_ctrl(&Launcher);
}

void main_launch_ctrl(launcher_t* launch)
{
	float buff_operator = 0.3f;
	float rc_buff = (float)rc.base_info->ch3 * buff_operator;
	
	launcher_info_t* info = launch->info;
	
	switch(launch->status)
	{
		case LA_INIT:
			launcher_position_init();
			break;
		case LA_LOCK:
			/*不做任何处理，使其目标值等于上一个状态时的目标值*/
			break;
		case LA_UNLOCK:
			switch(RAILGUN.command->shoot_command->ctrl_command)
			{
				case manual_ctrl:
					main_launcher_stuck_test(launch);
	
	        if(Launcher.info->main_launch_stuck_flag == true)
	        {
		        info->main_launch_target = launch->main_launch->rx_info.angle_sum;
		
		        info->main_launch_stuck_cnt = 0;
		
		        info->main_launch_stuck_flag = false;
	        }
	        else
	        {
		        info->main_launch_target -= rc_buff;
		
		        info->main_launch_target = main_launcher_position_lock(info->main_launch_target);
	        }
					break;
				case auto_ctrl:
					switch(RAILGUN.command->shoot_command->launcher_command)
					{
						case fire:
							
							break;
						case reload:
							launch->info->reload_processing = true;
							if(launch->info->main_launcher_reload_ok == false)
							{
								if(launch->info->main_launch_up_state_in_cnt == 0)
							  {
									servo_unlock();
								  launch->info->main_launch_target = launch->info->main_launch_superior_limit;
							  }
							  else
							  {
								  if(abs(launch->main_launch->pid.position.info.err) <= 2000 || launch->info->main_launcher_reload_sup_ok == true)
								  {
									  servo_lock();
									  launch->info->main_launcher_reload_sup_ok = true;
									  launch->info->servo_delay_cnt++;//考虑舵机到位需要时间，需要对main_launcher目标值改变做延时
								    if(launch->info->servo_delay_cnt >= SERVO_DELAY_TIME)
								    {
										  if(launch->info->main_launch_down_state_in_cnt == 0)
										  {
											  launch->info->main_launch_target = launch->info->main_launch_lower_limit;
										  }
								  	  else
										  {
											  if(abs(launch->main_launch->pid.position.info.err) <= 2000)
										    {
											    launch->info->main_launcher_reload_ok = true;
													/*计数值清零*/
											    launch->info->servo_delay_cnt = 0;
											    launch->info->main_launch_up_state_in_cnt = 0;
												  launch->info->main_launch_down_state_in_cnt = 0;
										    }
										  }
										  launch->info->main_launch_down_state_in_cnt++;
								    }
								  }
								  else
								  {
									  launch->info->main_launcher_reload_sup_ok = false;
								  }
							  }
							  launch->info->main_launch_up_state_in_cnt++;
							  }
							break;
						case launch_standby:
							launch->info->launch_available = true;
							launch->info->main_launch_target = launch->main_launch->rx_info.angle_sum;
				  		launch->info->servo_delay_cnt = 0;
							break;
						default:
							break;
					}
					break;
				default:
					break;
			}
			
			break;
		default:
			break;
	}

	if(launch->main_launch->state.work_state == M_OFFLINE)
	{
		can1_0x200_send_buff[launch->main_launch->id.buff_p] = 0;
	}
	else if(launch->info->main_launch_init_flag == true)
	{
		launch->main_launch_ctrl(launch,info->main_launch_target);
	}
}

void feed_ctrl(launcher_t* launch)
{
	switch(launch->status)
	{
		case LA_INIT:
			launcher_position_init();
			break;
		case LA_LOCK:
			launch->info->feeder_target = launch->feeder->rx_info.angle_sum;
		  launch->feeder_ctrl(launch,launch->info->feeder_target);
			break;
		case LA_UNLOCK:
			switch(RAILGUN.command->shoot_command->ctrl_command)
			{
				case manual_ctrl:
					can1_0x1FF_send_buff[Launcher.feeder->id.buff_p] = 0;
					break;
				case auto_ctrl:
					switch(RAILGUN.command->shoot_command->launcher_command)
	        {
		        case fire:
		    	    break;
		        case reload:
							if(launch->info->main_launcher_reload_ok == true)
							{
								switch(launch->info->feeder_moving_state)
		    			  {
		    				  case foreward_state:
							      if(launch->info->feeder_state_in_cnt == 0)
					          {
						          launch->info->feeder_target = launch->feeder->rx_info.angle_sum - FEEDER_ONE_BALL_POS;
					  	        launch->info->feeder_target_save = launch->info->feeder_target;//target_save只能在拨盘到达target_save的位置后才能被再次改变
					          }
										launch->info->feeder_state_in_cnt++;
							      if(feeder_stuck_test(launch))
							      {
								      if(launch->info->feeder_foreback_time < 2)
						          {
									      launch->info->feeder_moving_state = backward_state;
									      launch->info->feeder_target = launch->feeder->rx_info.angle_sum + FEEDER_HALF_BALL_POS;
									      launch->info->feeder_foreback_time++;
								      }
								      else
								      {
									      RAILGUN.command->shoot_command->launcher_command = launch_standby;
									      launch->info->feeder_state_in_cnt = 0;
									      launch->info->feeder_foreback_time = 0;
												launch->info->reload_processing = false;
									      //不可射击（有光电门的话，要检测光电门数据）
									      //缺失
								      }
								      launch->info->feeder_stuck_cnt = 0;
							      }
							      else//拨盘正在转或者到达目标值
							      {
								      if(abs(launch->feeder->pid.position.info.err) < 300 && launch->info->feeder_state_in_cnt >= 5)//到达目标值,cnt是避免首次误进判断，因为第一次err还未被算出
								      {
												/*一个供弹流程结束*/
									      RAILGUN.command->shoot_command->launcher_command = launch_standby;
									      launch->info->feeder_state_in_cnt = 0;
									      launch->info->feeder_foreback_time = 0;
												launch->info->reload_processing = false;
												launch->info->fire_ready = true;
								      }
							      }
							      break;
						      case backward_state:
							  	   if(feeder_stuck_test(launch) || abs(launch->feeder->pid.position.info.err) < 300)
					  			   {
									     launch->info->feeder_target = launch->info->feeder_target_save;
									     launch->info->feeder_moving_state = foreward_state;
									     launch->info->feeder_stuck_cnt = 0;
								     }
								     else//正在转
								     {
								     }  
							      break;
						      default:
							      break;
					      }
							}
					    
					    break;
						case launch_standby:
					    launch->info->feeder_moving_state = foreward_state;//这句仅用于测试，逻辑上是有问题的
				      launch->info->feeder_target = launch->feeder->rx_info.angle_sum;
			        break;
				    default:
					    break;
			    }
			    break;
		    default:
			    break;
	    }
			break;
		default:
			break;
	}
	if(launch->info->launcher_init_flag == true && launch->feeder->state.work_state == M_ONLINE)
	{
		launch->feeder_ctrl(launch,launch->info->feeder_target);
	}
}



float main_launcher_position_lock(float tar)
{
	float tar_temp = tar;
	
	if(tar_temp > Launcher.info->main_launch_superior_limit)
	{
		tar_temp = Launcher.info->main_launch_superior_limit;
	}
	else if(tar_temp < Launcher.info->main_launch_lower_limit)
	{
		tar_temp = Launcher.info->main_launch_lower_limit;
	}
	
	return tar_temp;
}

void main_launcher_stuck_test(launcher_t* launch)
{
	if(abs(launch->main_launch->pid.position.info.err) >= 20 && abs(launch->main_launch->rx_info.speed) <= 10)
	{
		launch->info->main_launch_stuck_cnt++;
	}
	if(launch->info->main_launch_stuck_cnt >= LA_STUCK_MAX_CNT)
	{
		launch->info->main_launch_stuck_flag = true;
	}
}

/*position ctrl begin*/
void main_launcher_ctrl(launcher_t* launch, float tar)
{
	motor_t* motor = launch->main_launch;
	
	can1_0x200_send_buff[motor->id.buff_p] = motor->c_posit(motor,tar);
}

void feeder_ctrl(launcher_t* launch, float tar)
{
	motor_t* motor = launch->feeder;
	
	can1_0x1FF_send_buff[motor->id.buff_p] = motor->c_posit(motor,tar);
}
/*position ctrl end*/

/*servo ctrl begin*/
void servo_unlock(void)
{
	SERVO_PwmOut(SERVO_UNLOCK_PWM);
}

void servo_lock(void)
{
	SERVO_PwmOut(SERVO_LOCK_PWM);
}

void servo_sleep(void)
{
	SERVO_PwmOut(0);
}
/*servo ctrl end*/

/*供弹方向反方向*/
void feeder_foreback(launcher_t* launch)
{
	launch->info->feeder_target = launch->feeder->rx_info.angle_sum + FEEDER_HALF_BALL_POS;
}

//-157359 -157482 fu shi gong dan 31130yike
bool feeder_stuck_test(launcher_t* launch)
{
	if(abs(launch->feeder->pid.position.info.err) >= 300 && abs(launch->feeder->rx_info.speed) <= 3)
	{
		launch->info->feeder_stuck_cnt++;
	}
	if(launch->info->feeder_stuck_cnt >= 200)
	{
		return true;
	}
	return false;
}
