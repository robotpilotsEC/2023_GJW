#include "railgun.h"
#include "remote.h"
#include "string.h"

/*创建整车结构体*/
chassis_command_t CHASSIS_COMMAND;
shoot_command_t SHOOT_COMMAND;
railgun_command_t RAILGUN_COMMAND = 
{
	.chassis_command = &CHASSIS_COMMAND,
	.shoot_command = &SHOOT_COMMAND,
};
railgun_t RAILGUN = 
{
	.command = &RAILGUN_COMMAND,
};


void rail_status_update(railgun_t* rail)
{
	if(Launcher.info->launcher_init_flag == false || Lever.info->lever_position_init_flag == false)
	{
		rail->status = init_mode;
		rc_value_clear();
	}
	else
	{
		Key_Status_Scan(rail);
	  Rc_Status_Scan(rail);
	}
}

/*跳变沿起始不进模式处理*/
void rc_value_clear(void)
{
	rc.base_info->s1.value = 0;
	rc.base_info->s2.value = 0;
}


void Rc_Status_Scan(railgun_t* rail)
{
	/*S2为整车状态的检测，优先级高于S1*/
	Rc_S2_Status_Check(rail);
	
	Rc_S1_Status_Check(rail);
	
	Rc_Thumbwheel_Check(rail);
}

void Rc_S1_Status_Check(railgun_t* rail)
{
	switch(rc.base_info->s1.status)
	{
		case up_R:
			switch(rail->status)
			{
				case camer_mode:
					break;
				case shoot_mode:
					rail->command->shoot_command->ctrl_command = auto_ctrl;
				  if(Launcher.info->launch_available == true)
					{
						if(Launcher.info->fire_ready == false)
				  	{
						  rail->command->shoot_command->launcher_command = reload;
			  		}
				  	else
				  	{
				  		rail->command->shoot_command->launcher_command = fire;
				  	}
					}
					break;
				case shift_mode:
					rail->command->chassis_command->shifting_speed_command = high_speed;
					break;
				default:
					break;
			}
			break;
		case mid_R:
			switch(rail->status)
			{
				case camer_mode:
					break;
				case shoot_mode:
					rail->command->shoot_command->ctrl_command = auto_ctrl;
				  if(Launcher.info->reload_processing == false && Launcher.info->fire_ready == false)//在补弹过程中或射弹过程中不能改变状态
					{
						rail->command->shoot_command->launcher_command = launch_standby;
					}
					break;
				case shift_mode:
					rail->command->chassis_command->shifting_speed_command = middle_speed;
					break;
				default:
					break;
			}
			break;
		case down_R:
			switch(rail->status)
			{
				case camer_mode:
					rail->ctrl = KEY_CTRL;
					break;
				case shoot_mode:
					if(Launcher.info->launch_available == true)
					{
						if(Launcher.info->fire_ready == false)
					  {
					    rail->command->shoot_command->ctrl_command = manual_ctrl;
				  	}
			  		else
				  	{
					  	if(Launcher.info->reload_processing == false)
				  		{
			  				rail->command->shoot_command->launcher_command = reload;
			  			}
			  		}
					}			
					break;
				case shift_mode:
					rail->command->chassis_command->shifting_speed_command = low_speed;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Rc_S2_Status_Check(railgun_t* rail)
{
	switch(rc.base_info->s2.status)
	{
		case up_R:
			rail->status = camer_mode;
			break;
		case mid_R:
			rail->ctrl = RC_CTRL;
			rail->status = shoot_mode;
			break;
		case down_R:
			rail->ctrl = RC_CTRL;
			rail->status = shift_mode;
			break;
		default:
			break;
	}
}

void Rc_Thumbwheel_Check(railgun_t* rail)
{
	switch(rc.base_info->thumbwheel.status)
	{
		case up_R:
			switch(rail->status)
			{
				case camer_mode://重新复位
					Launcher.info->main_launch_init_flag = false;
				  Launcher.info->feeder_init_flag = false;
				  Launcher.info->launcher_init_flag = false;
				  Lever.info->d_lever_init_flag = false;
				  Lever.info->twin_init_flag = false;
				  Lever.info->lever_position_init_flag = false;
				  break;
				default:
					break;
			}
			break;
		case mid_R:
			break;
		case down_R:
			break;
			default:
			break;
	}
}




void Key_Mouse_l_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->mouse_btn_l.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}	

void Key_Mouse_r_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->mouse_btn_r.status)
	{
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Q_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->Q.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_W_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->W.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_E_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->E.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_S_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->S.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_D_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->D.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_F_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->F.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Z_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->Z.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_C_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->C.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_V_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->V.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_B_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->B.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			break;
		default:
			break;
	}
}

void Key_Shift_Status_Check(railgun_t* rail)
{
  switch(rc.base_info->Shift.status)
	{
		case relax_K:
			break;
		case up_K:
			rail->command->chassis_command->key_accelerate_command = key_chassis_accelerate_n_ok;
		  rail->command->shoot_command->key_accelerate_command = key_lever_accelerate_n_ok;
			break;
		case down_K:
		case short_press_K:
		case long_press_K:
			switch(rail->status)
			{
				case shoot_mode:
					rail->command->shoot_command->key_accelerate_command = key_lever_accelerate_ok;
					break;
				case shift_mode:
					rail->command->chassis_command->key_accelerate_command = key_chassis_accelerate_ok;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
}

void Key_Ctrl_Status_Check(railgun_t* rail)
{
	static uint32_t mode_num_cnt = 0;
	
  switch(rc.base_info->Ctrl.status)
	{
		case relax_K:
			break;
		case down_K:
			break;
		case up_K:
			break;
		case short_press_K:
			break;
		case long_press_K:
			/*mode change begin*/
		  switch(mode_num_cnt % 2)
			{
				case 0:
					rail->status = shoot_mode;
					break;
				case 1:
					rail->status = shift_mode;
					break;
				default:
					break;
			}
			mode_num_cnt++;
			break;
		default:
			break;
	}
}

void Key_Status_Scan(railgun_t* rail)
{
	Key_Mouse_l_Status_Check(rail);
	Key_Mouse_r_Status_Check(rail);
	Key_Q_Status_Check(rail);
  Key_W_Status_Check(rail);
  Key_E_Status_Check(rail);
  Key_S_Status_Check(rail);
  Key_D_Status_Check(rail);
  Key_F_Status_Check(rail);
  Key_Z_Status_Check(rail);
  Key_C_Status_Check(rail);
  Key_V_Status_Check(rail);
  Key_B_Status_Check(rail);
  Key_Shift_Status_Check(rail);
  Key_Ctrl_Status_Check(rail);
}

