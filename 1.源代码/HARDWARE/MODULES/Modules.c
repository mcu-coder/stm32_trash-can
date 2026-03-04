#include "Modules.h"
#include "IR.h"
#include "HW.h"
#include "ultrasonic.h"
#include "delay.h"

// 使用 extern 声明在 main.c 中定义的变量
extern SensorModules sensorData;
extern SensorThresholdValue Sensorthreshold;
extern DriveModules driveData;

static uint8_t DistanceToCapacityPercent(float distance)
{
    uint8_t percent;
    if (distance <= 5.0f)
    {
        percent = 0; // ≤5cm：垃圾桶满，剩余容量0%
    }
    else if (distance >= 30.0f)
    {
        percent = 100; // ≥30cm：垃圾桶空，剩余容量100%
    }
    else
    {
        // 线性映射：(距离-5) × (100% / 25cm) = 剩余百分比（5-30cm对应0-100%）
        percent = (uint8_t)((distance - 5.0f) * 4.0f); 
    }
    return percent;
}


u16 value;
u16 fire;
float distance;
// 状态变量
//static uint8_t temp_read_state = 0;
//static uint32_t temp_start_time = 0;
static uint32_t last_sensor_update[6] = {0};

// 传感器更新间隔（单位：系统滴答，2ms/次）
//#define TEMP_UPDATE_INTERVAL    400   // 800ms
#define HR_UPDATE_INTERVAL      50    // 100ms  
//#define LUX_UPDATE_INTERVAL     100   // 200ms
#define DISTANCE_UPDATE_INTERVAL 150  // 300ms
//#define sensorData.fire
// 局部变量


// 添加手指状态跟踪


void SensorScan(void)
{
  
    uint32_t current_time = delay_get_tick();
    

         sensorData.value=HW_GetData();
	      sensorData.fire = IR_FireData();
//    
    // ==================== 距离读取 ====================
    if(current_time - last_sensor_update[3] > DISTANCE_UPDATE_INTERVAL)
    {
       
        sensorData.distance = UltrasonicGetLength();
       sensorData.capacity_percent = DistanceToCapacityPercent(sensorData.distance);
        last_sensor_update[3] = current_time;
    }
}


