#include "lever.h"
#include "lever_motor.h"
#include "config_can.h"
#include "ave_filter.h"
#include "motor.h"
#include "pid.h"
#include "remote.h"
#include "railgun.h"

/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/


float init_speed_param[7]            = {15,0.4,0,0,9000,9000,2000};  //Lever初始化速度环pid
float l_lever_position_in_param[7]   = {9,0.01,0,0,8000,10000,9000};
float l_lever_position_param[7]      = {0.5,0,0,3,8000,10000,9000};
float r_lever_position_in_param[7]   = {9,0.01,0,0,8000,10000,9000};
float r_lever_position_param[7]      = {0.5,0,0,3,8000,10000,9000};
float d_lever_position_in_param[7]   = {9,0.01,0,0,8000,10000,9000};
float d_lever_position_param[7]      = {0.5,0,0,3,8000,10000,9000};
float speed_differ_speed_param[7]    = {-17,-0.015,-8,0,8000,10000,9000};

motor_pid_t speed_differ_pid;

lever_info_t lever_info = 
{
	.twin_stuck_flag = false,
	.d_lever_stuck_flag = false,
	.lever_position_init_flag = false,
	.twin_init_flag = false,
	.d_lever_init_flag = false,
	.twin_init_stuck_cnt = 0,
	.twin_stuck_cnt = 0,
	.d_lever_init_stuck_cnt = 0,
	.d_lever_stuck_cnt = 0,
};

lever_t Lever = 
{
	.info = &lever_info,
};

void lever_init(void)
{
	lever_motor[L_LEVER].init(&lever_motor[L_LEVER]);
	lever_motor[R_LEVER].init(&lever_motor[R_LEVER]);
	lever_motor[D_LEVER].init(&lever_motor[D_LEVER]);
	
	lever_motor[L_LEVER].pid_init(&speed_differ_pid,speed_differ_speed_param);
	
	lever_motor[L_LEVER].pid_init(&lever_motor[L_LEVER].pid.speed,init_speed_param);
	lever_motor[R_LEVER].pid_init(&lever_motor[R_LEVER].pid.speed,init_speed_param);
	lever_motor[D_LEVER].pid_init(&lever_motor[D_LEVER].pid.speed,init_speed_param);

	lever_motor[L_LEVER].pid_init(&lever_motor[L_LEVER].pid.position_in,l_lever_position_in_param);
	lever_motor[L_LEVER].pid_init(&lever_motor[L_LEVER].pid.position,l_lever_position_param);
	lever_motor[R_LEVER].pid_init(&lever_motor[R_LEVER].pid.position_in,l_lever_position_in_param);
	lever_motor[R_LEVER].pid_init(&lever_motor[R_LEVER].pid.position,l_lever_position_param);
	lever_motor[D_LEVER].pid_init(&lever_motor[D_LEVER].pid.position_in,l_lever_position_in_param);
	lever_motor[D_LEVER].pid_init(&lever_motor[D_LEVER].pid.position,l_lever_position_param);
	
	Lever.l_lever = &lever_motor[L_LEVER];
	Lever.r_lever = &lever_motor[R_LEVER];
	Lever.d_lever = &lever_motor[D_LEVER];
}

void lever_heartbeat(void)
{
	Lever.l_lever->heartbeat(Lever.l_lever);
	Lever.r_lever->heartbeat(Lever.r_lever);
	Lever.d_lever->heartbeat(Lever.d_lever);
}

void lever_command_react(lever_t* lever)
{
	
}

void lever_status_react(lever_t* lever)
{
	switch(RAILGUN.status)
	{
		case init_mode:
			lever->status = L_INIT;
			break;
		case camer_mode:
			lever->status = L_LOCK;
			break;
		case shoot_mode:
			lever->status = L_UNLOCK;
			break;
		case shift_mode:
			lever->status = L_LOCK;
			break;
		default:
			break;
	}
}

void lever(void)
{
	lever_command_react(&Lever);
	
	lever_status_react(&Lever);
	
	twin_ctrl(&Lever);
	
	d_lever_ctrl(&Lever);
}
/*3/13/0.22写下代码未测试*/
void lever_position_init(void)
{
	/*TWIN初始化部分*/
	if((abs(Lever.l_lever->pid.speed.info.err) >= 100 && abs(Lever.l_lever->rx_info.speed) <= 3) || \
		 (abs(Lever.r_lever->pid.speed.info.err) >= 100 && abs(Lever.r_lever->rx_info.speed) <= 3))
	{
		Lever.info->twin_init_stuck_cnt++;
	}
	if(Lever.info->twin_init_stuck_cnt >= 10)
	{
		Lever.info->twin_init_stuck_cnt = 0;
		
		Lever.info->twin_init_flag = true;
		
		Lever.l_lever->rx_info.angle_sum = 0;
		Lever.l_lever->rx_info.angle_prev = Lever.l_lever->rx_info.angle;
		Lever.r_lever->rx_info.angle_sum = 0;
		Lever.r_lever->rx_info.angle_prev = Lever.r_lever->rx_info.angle;
		Lever.info->twin_lever_target = 0;
		
		Lever.info->twin_lower_limit = Lever.l_lever->rx_info.angle_sum + 60000;
		Lever.info->twin_superior_limit = Lever.l_lever->rx_info.angle_sum + TWIN_MOION_RANGE;//加还是减 要测试
	}
	/*有电机失联或已完成初始化发0*/
	if(Lever.l_lever->state.work_state == M_OFFLINE || Lever.r_lever->state.work_state == M_OFFLINE || Lever.info->lever_position_init_flag == true)
	{
		can1_0x200_send_buff[Lever.l_lever->id.buff_p] = 0;
	  can1_0x200_send_buff[Lever.r_lever->id.buff_p] = 0;
	}
	/*未完成初始化以恒定速度向下移动*/
	else
	{
		can1_0x200_send_buff[Lever.l_lever->id.buff_p] = Lever.l_lever->c_speed(Lever.l_lever,POS_INIT_SPEED);
	  can1_0x200_send_buff[Lever.r_lever->id.buff_p] = Lever.r_lever->c_speed(Lever.r_lever,POS_INIT_SPEED);
	}
	
	/*...................................................................................................................................................*/
	
	/*D_LEVER初始化部分*/
	if(abs(Lever.d_lever->pid.speed.info.err) >= 100 && abs(Lever.d_lever->rx_info.speed) <= 3)
	{
		Lever.info->d_lever_init_stuck_cnt++;
	}
	if(Lever.info->d_lever_init_stuck_cnt >= 10)
	{
		Lever.info->d_lever_init_stuck_cnt = 0;
		
		Lever.info->d_lever_init_flag = true;
		
		Lever.info->d_lever_target = Lever.d_lever->rx_info.angle_sum;
		
		Lever.info->d_lever_lower_limit = Lever.d_lever->rx_info.angle_sum;
		Lever.info->d_lever_superior_limit = Lever.info->d_lever_lower_limit + D_LEVER_MOTION_RANGE;
	}
	
	if(Lever.d_lever->state.work_state == M_OFFLINE || Lever.info->d_lever_init_flag == true)
	{
		can1_0x200_send_buff[Lever.d_lever->id.buff_p] = 0;
	}
	else
	{
		can1_0x200_send_buff[Lever.d_lever->id.buff_p] = Lever.d_lever->c_speed(Lever.d_lever,POS_INIT_SPEED);
	}
	
	/*...................................................................................................................................................*/
	
	if(Lever.info->d_lever_init_flag == true && Lever.info->twin_init_flag == true)
	{
		Lever.info->lever_position_init_flag = true;
	}
	else
	{
		Lever.info->lever_position_init_flag = false;
	}
}

/*丝杆电控限位,0 -> twin限位 1 -> d_lever限位*/
float lever_position_lock(float tar,uint8_t type)
{
	float tar_temp = tar;
	
	switch (type)
	{
		case 0:
			if(tar_temp > Lever.info->twin_superior_limit)
			{
				tar_temp = Lever.info->twin_superior_limit;
			}
			else if(tar_temp < Lever.info->twin_lower_limit)
			{
				tar_temp = Lever.info->twin_lower_limit;
			}
			break;
		case 1:
			if(tar_temp > Lever.info->d_lever_superior_limit)
			{
				tar_temp = Lever.info->d_lever_superior_limit;
			}
			else if(tar_temp < Lever.info->d_lever_lower_limit)
			{
				tar_temp = Lever.info->d_lever_lower_limit;
			}
			break;
		default:
			break;
	}
	return tar_temp;
}

/*TWIN转速差值PID计算*/
float differ_cal(motor_t* l_motor,motor_t* r_motor)
{
	float delta = 0.0f,bias = 0.0f;
	
	delta = (float)abs(l_motor->rx_info.speed - r_motor->rx_info.speed);
	
	bias = l_motor->c_pid1(&speed_differ_pid,delta,0);
	
	return bias;
}
//ch1 twin ch0 dlever
void twin_ctrl(lever_t* lever)
{
	float bias = 0.0f,buff_operator = 1.0f;
	
	motor_t* l_motor = lever->l_lever;
	motor_t* r_motor = lever->r_lever;
	
	if(DIFFER_PID_ON)
		bias = differ_cal(l_motor,r_motor);
	else 
		bias = 0.0f;
	
	switch(lever->status)
	{
		case L_INIT:
			lever_position_init();
			break;
		case L_LOCK:
			/*不做任何处理，使其目标值等于上一个状态时的目标值*/
			break;
		case L_UNLOCK:
			/*堵转检测*/
	    twin_stuck_test(lever);
	
	    /*堵转处理*/
	    if(lever->info->twin_stuck_flag == true)
	    {
		    lever->info->twin_lever_target = (lever->l_lever->rx_info.angle_sum + lever->l_lever->rx_info.angle_sum) / 2;//需要改进
		
	      lever->info->twin_stuck_flag = false;
		
	      lever->info->twin_stuck_cnt = 0;
      }
      else
  	  {
	  	  lever->info->twin_lever_target += (float)rc.base_info->ch1 * buff_operator;
		
	  	  /*输出限幅*/
		    lever->info->twin_lever_target = lever_position_lock(lever->info->twin_lever_target,0);
	    }
			break;
		default:
      break;			
	}
	  /*检测是否有电机失联*/
	if(lever->l_lever->state.work_state == M_OFFLINE || lever->r_lever->state.work_state == M_OFFLINE)
	{
		can1_0x200_send_buff[l_motor->id.buff_p] = 0;
	  can1_0x200_send_buff[r_motor->id.buff_p] = 0;
	}
	else if(lever->info->lever_position_init_flag == true)
	{
		/*左右丝杆电机同一输出*/
	  can1_0x200_send_buff[l_motor->id.buff_p] = l_motor->c_posit(l_motor,Lever.info->twin_lever_target) + bias;
	  can1_0x200_send_buff[r_motor->id.buff_p] = r_motor->c_posit(r_motor,Lever.info->twin_lever_target) + bias;
	}
}

/*TWIN堵转检测*/
void twin_stuck_test(lever_t* lever)
{
	if(abs(lever->l_lever->pid.position.info.err) >= 500 || \
		 abs(lever->r_lever->pid.position.info.err) >= 500)
	{
		if(abs(lever->l_lever->rx_info.speed) <= 5 || \
			 abs(lever->r_lever->rx_info.speed) <= 5)
		{
			lever->info->twin_stuck_cnt++;
		}
	}
	if(lever->info->twin_stuck_cnt >= LE_STUCK_MAX_CNT)
	{
		lever->info->twin_stuck_flag = true;
	}
}

void d_lever_ctrl(lever_t* lever)
{
	motor_t* d_lever = lever->d_lever;
	lever_info_t* info = lever->info;
	
	float buff_operator = 0.5f;
	
	switch(lever->status)
	{
		case L_INIT:
			lever_position_init();
		  break;
		case L_LOCK:
			/*不做任何处理，使其目标值等于上一个状态时的目标值*/
			break;
		case L_UNLOCK:
			d_lever_stuck_test(lever);
	
	    if(info->d_lever_stuck_flag == true)
	    {
		    info->d_lever_target = d_lever->rx_info.angle_sum;
		
		    info->d_lever_stuck_flag = false;
		
		    info->d_lever_stuck_cnt = 0;
	    }
	    else
	    {
		    info->d_lever_target -= (float)rc.base_info->ch0 * buff_operator;
		
		    info->d_lever_target = lever_position_lock(info->d_lever_target,1);
	    }
			break;
	}
	
	/*离线检测*/
	if(d_lever->state.work_state == M_OFFLINE)
	{
		can1_0x200_send_buff[d_lever->id.buff_p] = 0;
	}
	else if(lever->info->lever_position_init_flag == true)
	{
		can1_0x200_send_buff[d_lever->id.buff_p] = d_lever->c_posit(d_lever,Lever.info->d_lever_target);
	}
}

/*D_LEVER堵转检测*/
void d_lever_stuck_test(lever_t* lever)
{
	if(abs(lever->d_lever->pid.position.info.err) >= 100 && abs(lever->d_lever->rx_info.speed) <= 5)
	{
		lever->info->d_lever_stuck_cnt++;
	}
	if(lever->info->d_lever_stuck_cnt >= LE_STUCK_MAX_CNT)
	{
		lever->info->d_lever_stuck_flag = true;
	}
}


