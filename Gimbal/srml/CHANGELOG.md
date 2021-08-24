# Changelog

最新的修改写在最上面，方便快速查找

## [2.2] - 2021-04-25
### 更改：
- `dr16.h`：`LinkageStatus_Typedef`缩短名称长度
### 修复：
- 所有计算绝对值函数由`std::abs`改为`fabs`
- `PID.cpp`：`myPID::Adjust`函数中使用`epsilon`比较浮点数
- `motor_dji.h`：`MotorMsgPack`函数使用引用传参
- `motor_ctrl.h`：`MotorCascadeCtrl::Adjust`函数添加`virtual`和`override`

## [2.1] - 2021-04-22
### 新增：
- `drv_can.h`：添加`CAN_COB`结构体定义
- `drv_uart.h`：添加`USART_COB`结构体定义
- `motor_dji.h`：添加`MotorMsgPack`函数
- `motor_ctrl.h`：电机控制库
### 更改：
- `motor.h`：重命名为`motor_dji.h`

## [2.0] - 2021-04-12
### 新增：
- 所有c/cpp文件添加包含`srml.h`，并启用条件编译
- `srml_config_template.h`：配置模板
- `shit_mountain.cpp/h`：常用函数统一存放点
- `asuwave.c/h`：网页版上位机
### 更改：
- 所有自定义计算绝对值函数改为`std::abs`
- 所有`_constrain`改为`shit::constrain`
- `drv_i2c.h`：`IIC_PIN_Typedef`结构体内引脚类型由`uint32_t`改成`uint16_t`
- `BMX055_Config.c`：重命名为`BMX055_Config.cpp`
- `BMX055_Config.cpp`：`BMX055_Init`函数中分支山用`shit::get_gpio_pin_num`函数代替
- `mpu6050_config.cpp`：`MPU6050_Init`函数中分支山用`shit::get_gpio_pin_num`函数代替
- `dr16.h`：键位定义由宏定义改为枚举；`SW_Status_Typedef`、`LinkageStatus_Typedef`添加`DR16_`前缀
- `SerialLineIP.cpp`：重命名为`serial_line_ip.cpp`
- `SerialLineIP.h`：重命名为`serial_line_ip.h`
- `myAssert.h`：重命名为`my_assert.h`
### 修复：
- `drv_flash.h`：`get_sector`函数不被导出
- `drv_i2c.c`：`IIC_Delay`函数添加`volatile`，避免被优化
- `drv_timer.c`：`Timer_Init`函数中删去`HAL_TIM_Base_Start`，避免重复无用启动
- `VSEC.cpp`：`VSEC_UnPack`函数中修复变量类型不匹配问题
- `chassis.cpp`：`CChassis::position_control`函数中`memset`改为赋空值；`_Chassis_BubbleSort`改为`std::sort`
### 删除：
- `Drivers.h`
- `Kalman.cpp/h/md`
- `myMat.h`
- `old_PID.cpp/h`
- `Middlewares.h`
- `unified_frame.h`
- `control_analysis.cpp/h`
- `dev_manager.h`
- `linux_kfifo.c/h`

## [1.0] - 2021-03-23
正式发布
