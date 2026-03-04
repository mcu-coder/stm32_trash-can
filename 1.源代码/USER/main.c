#include "stm32f10x.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "delay.h"
#include "oled.h"
#include "key.h"
#include "Modules.h"
#include "TIM2.h"
#include "adcx.h"
#include "flash.h"
#include "usart2.h"
#include "usart3.h"
#include "ultrasonic.h"
#include "IR.h"
#include "HW.h"
#include "servo.h"
#include "timer.h"




#define KEY_Long1	11

#define KEY_1	1
#define KEY_2	2
#define KEY_3	3
#define KEY_4	4

#define FLASH_START_ADDR	0x0801f000	//写入的起始地址

 SensorModules sensorData;								//声明传感器数据结构体变量
 SensorThresholdValue Sensorthreshold;		//声明传感器阈值结构体变量
 DriveModules driveData;									//声明驱动器状态结构体变量

uint8_t mode = 1;	//系统模式  1自动  2手动  3设置
u8 dakai;//串口3使用的传递变量
u8 Flag_dakai;//串口3接收标志位
uint8_t is_secondary_menu = 0;  // 0一级菜单，1二级菜单
uint8_t secondary_pos = 1;      // 二级菜单光标位置（1-3对应时/分/秒）
uint8_t secondary_type = 0;   // 二级菜单类型：0=RTC时间，1=定时开启，2=定时关闭

uint8_t send_data[] = "";//语音播放曲目1
uint8_t send_data1[] = "";//语音播放曲目2




float Angle;			//定义角度变量


//系统静态变量
//static uint8_t count_a = 1;  //自动模式按键数
static uint8_t count_m = 1;  //手动模式按键数
//static uint8_t count_s = 1;	 //设置模式按键数
//static uint8_t last_mode = 0;      // 记录上一次的模式
//static uint8_t last_count_s = 0;   // 记录设置模式内上一次的页面
/**
  * @brief  显示菜单内容
  * @param  无
  * @retval 无
  */
enum 
{
	AUTO_MODE = 1,
	MANUAL_MODE,
	SETTINGS_MODE
	
}MODE_PAGES;


  
/**
  * @brief  显示菜单1的固定内容
  * @param  无
  * @retval 无
  */
void OLED_autoPage1(void)		//自动模式菜单第一页
{
	
	
	
	//显示“是否有人”
	OLED_ShowChinese(0,0,0,16,1);	
	OLED_ShowChinese(16,0,1,16,1);	
	OLED_ShowChinese(32,0,2,16,1);	
	OLED_ShowChinese(48,0,3,16,1);	
	OLED_ShowChar(64,0,':',16,1);
	
	 
	 
	
}
void OLED_autoPage2(void)   //自动模式菜单第二页
{
	

}



void SensorDataDisplay1(void)		//传感器数据显示第一页
{
  
   char all_data[128];  // 足够大的缓冲区容纳所有数据
   const char* person_status = sensorData.value ? "是" : "否";
	 const char* fire_status = sensorData.fire ? "是" : "否";
   const char* we_status = sensorData.capacity_percent ? "未满" : "已满";
    // 一次性格式化所有数据到同一个字符串
    sprintf(all_data, " 是否有人: %s\r\n 是否着火: %s \r\n 垃圾桶剩余容量: %d\r\n  距离：%d CM\r\n 垃圾桶状态: %s\r\n",
        person_status,  // p 是字符串，用 %s 输出
        fire_status,
        sensorData.capacity_percent,
        sensorData.distance,
       we_status);
    
    USART2_SendString(all_data);  // 一次发送所有数据
    
		
	
	 

		if(sensorData.value)
		{
			OLED_ShowChinese(90,0,0,16,1);	//火
			//driveData.Servo_Flag = 1;
		
		}
		else
		{
			OLED_ShowChinese(90,0,1,16,1);	//正
				//driveData.Servo_Flag = 0;
			
		}
	

		
		// 显示容量百分比 + "%" 符号
    OLED_ShowNum(48, 32, sensorData.capacity_percent, 3, 16, 1); // 显示百分比（0-100）
    OLED_ShowChar(72, 32, '%', 16, 1); // 显示百分号

		
		if(sensorData .capacity_percent ==0)
		{
			 USART2_SendString("垃圾已满\r\n");
		}
   
}

void SensorDataDisplay2(void)		//传感器数据显示第二页
{


}

/**
  * @brief  显示手动模式设置界面1
  * @param  无
  * @retval 无
  */
void OLED_manualPage1(void)
{
	//显示“灯光”
	OLED_ShowChinese(16,0,6,16,1);	
	OLED_ShowChinese(32,0,7,16,1);	
	OLED_ShowChinese(48,0,8,16,1);	
	OLED_ShowChar(64,0,':',16,1);
 
	
}

/**
  * @brief  显示手动模式设置参数界面1
  * @param  无
  * @retval 无
  */
void ManualSettingsDisplay1(void)
{
	if(driveData.Servo_Flag)
	{
		OLED_ShowChinese(96,0,17,16,1); 	//开
	}
	 
}

/**
  * @brief  显示系统阈值设置界面1
  * @param  无
  * @retval 无
  */
void OLED_settingsPage1(void)
{


	
}

void OLED_settingsPage2(void)//显示系统阈值设置界面2
{


}
void OLED_settingsPage3(void)//显示系统阈值设置界面3
{

}

void SettingsThresholdDisplay1(void)//实际阈值1
{

}

void SettingsThresholdDisplay2(void)//实际阈值2
{

}

void SettingsThresholdDisplay3(void)//实际阈值3
{

}

/**
  * @brief  记录自动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetAuto(void)  
{

 return 1;  // 自动模式只有一页，始终返回1
}

/**
  * @brief  记录手动模式界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetManual(void)  
{
	if(KeyNum == KEY_2)
	{
		KeyNum = 0;
		count_m++;
//		if (count_m == 1)
//		{
//			OLED_Clear();
//			
//		}
		if (count_m > 2)  		//一共可以控制的外设数量
		{
			OLED_Clear();
			count_m = 1;
		}
	}
	return count_m;
}

/**
  * @brief  记录阈值界面下按KEY2的次数
  * @param  无
  * @retval 返回次数
  */
uint8_t SetSelection(void)
{


	return 0;
}

/**
  * @brief  显示手动模式界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_manualOption(uint8_t num)
{
	switch(num)
	{
		case 1:	
			OLED_ShowChar(0, 0,'>',16,1);
			OLED_ShowChar(0,16,' ',16,1);
			OLED_ShowChar(0,32,' ',16,1);
			OLED_ShowChar(0,48,' ',16,1);
			break;
	 
	}
}

/**
  * @brief  显示阈值界面的选择符号
  * @param  num 为显示的位置
  * @retval 无
  */
void OLED_settingsOption(uint8_t num)
{

}

/**
  * @brief  自动模式控制函数
  * @param  无
  * @retval 无
  */
void AutoControl(void)//自动控制
{
	
	
	    // 优先级1：火灾检测（最高优先级，忽略其他逻辑）
    if (sensorData.fire == 1)
    {
        driveData.BEEP_Flag = 1;        // 蜂鸣器报警
        driveData.LED_Flag = 1;         // 第一个指示灯亮
        driveData.LED1_Flag = 1;        // 第二个指示灯亮
        driveData.Servo_Flag = 0;       // 不打开桶盖
        driveData.LED2_Flag = 0;   // 火灾时停止消毒
    }
    // 优先级2：垃圾桶已满（次高优先级）
    else if (sensorData.capacity_percent == 0)
    {
			
			if(sensorData.value == 1)
			{
				 driveData.BEEP_Flag = 1;        // 蜂鸣器报警
			}
		 
       
       
    }
    // 优先级3：有人 + 垃圾桶未满（正常使用场景）
    else if (sensorData.value == 1 && sensorData.capacity_percent > 0)
    {
        driveData.BEEP_Flag = 0;        // 蜂鸣器关闭
        driveData.LED_Flag = 1;         // 指示灯关闭
        driveData.LED1_Flag = 0;        // 指示灯关闭
        driveData.Servo_Flag = 1;       // 打开桶盖
         driveData.LED2_Flag = 0;   // 开盖时停止消毒
    }
     

}

/**
  * @brief  手动模式控制函数
  * @param  无
  * @retval 无
  */
void ManualControl(uint8_t num)
{
	switch(num)
	{
		case 1:  
            if(KeyNum == KEY_3)
            {
                driveData.Servo_Flag = 1;  
                KeyNum = 0;  
            }
            if(KeyNum == KEY_4)
            {
                driveData.Servo_Flag = 0; 
                KeyNum = 0;  
            }
            break;

	 

		default: break;
	}
}

/**
  * @brief  控制函数
  * @param  无
  * @retval 无
  */
void Control_Manager(void)
{
	 if(driveData.Servo_Flag )
    {	
       Servo_SetAngle(180.0f);
    }
    
		
		if(driveData.LED1_Flag )
    {	
        LED1_On(); 
    }
    else 
    {
        LED1_Off();
    }
		
		
		
			 
}

/**
  * @brief  阈值设置函数
  * @param  无
  * @retval 无
  */
void ThresholdSettings(uint8_t num)
{
	
}


int main(void)
{ 
    SystemInit();//配置系统时钟为72M	
    delay_init(72);  // 系统时钟72MHz
    ADCX_Init();
    LED_Init();
	 
	   
	  Servo_Init();


    
    // 添加的状态管理变量
    static uint8_t last_mode = 0;  // 记录上一次模式
    static uint32_t last_sensor_time = 0; // 传感器扫描时间控制
    static uint32_t last_display_time = 0; // 显示刷新时间控制

    
    TIM2_Init(72-1,1000-1);  // 2ms定时中断
    printf("Start \n");

    
    while (1)
    {	
        // ==================== 获取当前系统时间 ====================
        uint32_t current_time = delay_get_tick(); // 使用系统滴答计数
        
        // ==================== 优化传感器扫描频率 ====================
        if(current_time - last_sensor_time > 100) // 每200ms扫描一次传感器 (100 * 2ms = 200ms)
        {
            SensorScan(); 	//获取传感器数据
            last_sensor_time = current_time;
        }
        
        // ==================== 立即处理按键 ====================
        uint8_t current_key_num = KeyNum; // 保存当前按键值
        
        // 模式切换按键立即处理
        if(current_key_num != 0)
        {
            switch(mode)
            {
                case AUTO_MODE:
                    if(current_key_num == KEY_1)
                    {
                        mode = MANUAL_MODE;
                        count_m = 1;
                        // 切换到手动模式时关闭灯和蜂鸣器
                        driveData.LED_Flag = 0;
											  driveData.LED2_Flag = 0;
                        driveData.BEEP_Flag = 0;
											
                        KeyNum = 0; // 立即清除按键
                    }
//                    else if(current_key_num == KEY_Long1)
//                    {
//                        mode = SETTINGS_MODE;
//                        count_s = 1;
//                        KeyNum = 0; // 立即清除按键
//                    }
                   break;
                    
              
            }
        }
        
        // ==================== 模式切换优化 ====================
        if(last_mode != mode)
        {
            OLED_Clear();
            last_mode = mode;
            
            // 立即绘制新模式的固定内容
            switch(mode)
            {
                case AUTO_MODE:
                    OLED_autoPage1();
                    break;
                case MANUAL_MODE:
                    OLED_manualPage1();
                    break;
//                case SETTINGS_MODE:
//                    OLED_settingsPage1();
//                    break;
            }
            OLED_Refresh(); // 立即刷新显示
        }
        
        // ==================== 模式处理 ====================
        switch(mode)
        {
            case AUTO_MODE:
                // 直接调用显示，不使用SetAuto()
                SensorDataDisplay1();	//显示传感器1数据
                AutoControl();
                Control_Manager();
                break;
                
            case MANUAL_MODE:
            {
                static uint8_t manual_page_initialized = 0;
                static uint8_t last_manual_count = 0;
                static uint8_t last_LED_Flag = 0;
                static uint8_t last_BEEP_Flag = 0;
                static uint8_t force_refresh = 0;  // 强制刷新标志
                
                
                uint8_t current_manual_count = SetManual();
                
                // 检查设备状态是否改变，如果改变则强制刷新显示
                uint8_t need_refresh = 0;
                if(driveData.LED_Flag != last_LED_Flag || driveData.BEEP_Flag != last_BEEP_Flag)
                {
                    need_refresh = 1;
                    last_LED_Flag = driveData.LED_Flag;
                    last_BEEP_Flag = driveData.BEEP_Flag;
                }
                
                // 确保页面已初始化或光标位置改变或设备状态改变或强制刷新时重新绘制
                if(!manual_page_initialized || current_manual_count != last_manual_count || need_refresh || force_refresh)
                {
                    OLED_manualPage1();          // 固定文字
                    OLED_manualOption(current_manual_count); // 光标
                    ManualSettingsDisplay1();    // 状态
                    manual_page_initialized = 1;
                    last_manual_count = current_manual_count;
                    force_refresh = 0;  // 清除强制刷新标志
                    OLED_Refresh(); // 强制刷新显示
                }
                
              
                
                // 确保显示内容始终正确
                OLED_manualPage1();          // 固定文字
                OLED_manualOption(current_manual_count); // 光标
                ManualSettingsDisplay1();    // 状态
                
                Control_Manager();
                break;
            }
                

        }
        
        // ==================== 限制显示刷新频率 ====================
        if(current_time - last_display_time > 25) // 每50ms刷新一次显示 (25 * 2ms = 50ms)
        {
            // 所有模式都需要刷新显示
            OLED_Refresh();
            last_display_time = current_time;
        }
    }
}
