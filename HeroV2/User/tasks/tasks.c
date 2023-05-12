#include "tasks.h"
#include "cmsis_os.h"
#include "my_judge.h"

void Control_Task(void const * argument)
{
  for(;;)
  {
		tick_task(1);
    osDelay(1);
  }

}




