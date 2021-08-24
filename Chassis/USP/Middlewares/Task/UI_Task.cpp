#include "UI_Task.h"
#include "Hero_chassis.h"


TaskHandle_t UI_Handle;


void Device_Referee_UI(void *arg);


void UI_Init(){
	xTaskCreate(Device_Referee_UI,    "UI.Referee"   , Small_Stack_Size,    NULL, PriorityRealtime,  &UI_Handle);
}



void Device_Referee_UI(void *arg)
{
    static TickType_t _xPreviousWakeTime;

    //起始绘制时的绘制次数。若服务器丢包率较高，该值可适当给大
    static uint8_t enable_cnt = 20;             
 
    //图传稳定需要一段时间的延时
    vTaskDelay(2000);
    Referee.clean_all();

    vTaskDelay(500);                        
 
    for(;;)
    { 
        //刚开始时多次绘制图形，确保能在UI刚开启时顺利绘制图形
		if(Hero_chassis.UI_flag.V_flag)	enable_cnt=20;		//键盘按下V该值自增
		
        if(enable_cnt)			
        {
            //车界线、UI准星绘制
            Referee.Draw_Robot_Limit(180, 80, 961, 3, YELLOW, ADD_PICTURE);
            Referee.UI_Collimator(5, 961, 538, 25, YELLOW, ADD_PICTURE);

            enable_cnt--;
        }

        //绘制电容电压
        Referee.Draw_Cap_Energy(Hero_chassis.SourceManage.capObj.Voltage, 24, 16, 0, enable_cnt, 420,800);

        //绘制三种标志位
        Referee.Draw_Boost((uint8_t)unlimit_power, 1600, 840, 10, PINK);		//电容加速
        Referee.Draw_Spin(Hero_chassis.UI_flag.shift_flag, 1400, 840, 10, BLUE);			//小陀螺打开
        Referee.Draw_Bullet(Hero_chassis.UI_flag.B_flag, 1800, 840, 8, GREEN);	//弹仓open
		

    }
}