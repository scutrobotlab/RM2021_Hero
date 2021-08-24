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

    //��ʼ����ʱ�Ļ��ƴ������������������ʽϸߣ���ֵ���ʵ�����
    static uint8_t enable_cnt = 20;             
 
    //ͼ���ȶ���Ҫһ��ʱ�����ʱ
    vTaskDelay(2000);
    Referee.clean_all();

    vTaskDelay(500);                        
 
    for(;;)
    { 
        //�տ�ʼʱ��λ���ͼ�Σ�ȷ������UI�տ���ʱ˳������ͼ��
		if(Hero_chassis.UI_flag.V_flag)	enable_cnt=20;		//���̰���V��ֵ����
		
        if(enable_cnt)			
        {
            //�����ߡ�UI׼�ǻ���
            Referee.Draw_Robot_Limit(180, 80, 961, 3, YELLOW, ADD_PICTURE);
            Referee.UI_Collimator(5, 961, 538, 25, YELLOW, ADD_PICTURE);

            enable_cnt--;
        }

        //���Ƶ��ݵ�ѹ
        Referee.Draw_Cap_Energy(Hero_chassis.SourceManage.capObj.Voltage, 24, 16, 0, enable_cnt, 420,800);

        //�������ֱ�־λ
        Referee.Draw_Boost((uint8_t)unlimit_power, 1600, 840, 10, PINK);		//���ݼ���
        Referee.Draw_Spin(Hero_chassis.UI_flag.shift_flag, 1400, 840, 10, BLUE);			//С���ݴ�
        Referee.Draw_Bullet(Hero_chassis.UI_flag.B_flag, 1800, 840, 8, GREEN);	//����open
		

    }
}