#include "my_motor_control.h"
#include "motor.h"
#include "my_motor.h"
#include "config_can.h"
#include "drv_haltick.h"




/*
	float	  kp;
	float 	ki;
	float 	kd;
	
	float   blind_err;	
	float 	integral_max;	
	float   iout_max;
	float 	out_max;
*/


/*3508位置环 begin*/
float motor_3508_position_in_pid_param[7]   = {10,0.4,0,0,0,10000,10000};

float motor_3508_position_pid_param[7]      = {0.2,0,0,0,0,0,3000};
/*3508位置环 end*/

/*3508速度环 begin*/
float motor_3508_speed_pid_param[7]         = {27,0.5,0,0,6000,6000,10000};
/*3508速度环 end*/


/*2006速度环 begin*/
float motor_2006_speed_pid_param[7]         = {15,0.6,0,0,10000,10000,9000};
/*2006速度环 end*/

/*6020速度环 begin*/
float motor_6020_speed_pid_param[7]         = {0,0,0,0,0,0,0};
/*6020速度环 end*/


void My_Motor_Init(void)
{
	motor_3508.init(&motor_3508);
	motor_2006.init(&motor_2006);
	motor_6020.init(&motor_6020);
	
	#ifdef MODE_0
	
	motor_3508.pid_init(&motor_3508.pid.speed,motor_3508_speed_pid_param);
	motor_2006.pid_init(&motor_2006.pid.speed,motor_2006_speed_pid_param);
	motor_6020.pid_init(&motor_6020.pid.speed,motor_6020_speed_pid_param);
	
	#endif
	
	#ifdef MODE_1
	
	motor_3508.pid_init(&motor_3508.pid.position_in,motor_3508_position_in_pid_param);
	motor_3508.pid_init(&motor_3508.pid.position,motor_3508_position_pid_param);
	
	#endif
	
}

void Shooting_Heartbeat(void)
{
	motor_3508.heartbeat(&motor_3508);
	motor_2006.heartbeat(&motor_2006);
	motor_6020.heartbeat(&motor_6020);
}

float motor_3508_speed_tar = 0.0f;
float motor_2006_speed_tar = 0.0f;

void Motor_3508_Speed_Ctrl()
{
	if(motor_3508.state.work_state == M_ONLINE)
	{
		if(motor_3508.id.drive_type == M_CAN1)
	  {
		  if(motor_3508.id.rx_id == 0x201 || motor_3508.id.rx_id == 0x202 || motor_3508.id.rx_id == 0x203 || motor_3508.id.rx_id == 0x204)
	    {
		    can1_0x200_send_buff[motor_3508.id.buff_p] = motor_3508.c_speed(&motor_3508,motor_3508_speed_tar);
	    }
	    else
	    {
	  	  can1_0x1FF_send_buff[motor_3508.id.buff_p] = motor_3508.c_speed(&motor_3508,motor_3508_speed_tar);
	    }
	  }
	  else
	  {
		  if(motor_3508.id.rx_id == 0x201 || motor_3508.id.rx_id == 0x202 || motor_3508.id.rx_id == 0x203 || motor_3508.id.rx_id == 0x204)
	    {
		    can2_0x200_send_buff[motor_3508.id.buff_p] = motor_3508.c_speed(&motor_3508,motor_3508_speed_tar);
	    }
	    else
	    {
		    can2_0x1FF_send_buff[motor_3508.id.buff_p] = motor_3508.c_speed(&motor_3508,motor_3508_speed_tar);
	    }
  	}
	}
	else
	{
		can1_0x200_send_buff[motor_3508.id.buff_p] = 0;
		can1_0x1FF_send_buff[motor_3508.id.buff_p] = 0;
		can2_0x200_send_buff[motor_3508.id.buff_p] = 0;
		can2_0x1FF_send_buff[motor_3508.id.buff_p] = 0;
	}
}

void Motor_2006_Speed_Ctrl()
{
	if(motor_2006.state.work_state == M_ONLINE)
	{
		if(motor_2006.id.drive_type == M_CAN1)
	  {
		  if(motor_2006.id.rx_id == 0x201 || motor_2006.id.rx_id == 0x202 || motor_2006.id.rx_id == 0x203 || motor_2006.id.rx_id == 0x204)
	    {
	  	  can1_0x200_send_buff[motor_2006.id.buff_p] = motor_2006.c_speed(&motor_2006,motor_2006_speed_tar);
	    }
	    else
	    {
	  	  can1_0x1FF_send_buff[motor_2006.id.buff_p] = motor_2006.c_speed(&motor_2006,motor_2006_speed_tar);
	    }
	  }
	  else
  	{
  		if(motor_2006.id.rx_id == 0x201 || motor_2006.id.rx_id == 0x202 || motor_2006.id.rx_id == 0x203 || motor_2006.id.rx_id == 0x204)
	    {
	  	  can2_0x200_send_buff[motor_2006.id.buff_p] = motor_2006.c_speed(&motor_2006,motor_2006_speed_tar);
	    }
	    else
	    {
	  	  can2_0x1FF_send_buff[motor_2006.id.buff_p] = motor_2006.c_speed(&motor_2006,motor_2006_speed_tar);
	    }
  	}
	}
	else
	{
		can1_0x200_send_buff[motor_2006.id.buff_p] = 0;
		can1_0x1FF_send_buff[motor_2006.id.buff_p] = 0;
		can2_0x200_send_buff[motor_2006.id.buff_p] = 0;
		can2_0x1FF_send_buff[motor_2006.id.buff_p] = 0;
	}
}

void My_Motor_Control(void)
{
	#ifdef MODE_0
	
	Motor_3508_Speed_Ctrl();
	Motor_2006_Speed_Ctrl();
	
	#endif
	
	#ifdef MODE_1
	#endif
}
