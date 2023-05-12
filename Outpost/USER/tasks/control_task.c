#include "control_task.h"
#include "cmsis_os.h"

void control_task(void const * argument)
{
  /* USER CODE BEGIN control_task */
	armer_init();
  /* Infinite loop */
  for(;;)
  {
		armer_control();
    osDelay(1);
  }
  /* USER CODE END control_task */
}


