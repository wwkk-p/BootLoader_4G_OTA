#include "gd32f10x.h"
#include "main.h"
#include "usart.h"
#include "delay.h"
#include "4g.h"
#include "boot.h"

/*-------------------------------------------------*/
/*函数名：初始化4G模块                             */
/*参  数：                                         */
/*返回值：无                                       */
/*-------------------------------------------------*/
void GM4G_Init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);	                                //打开GPIOB时钟
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);     //设置PB2，PP输出，控制4G模块的复位IO
	
	u0_printf("复位4G模块，请耐心等到... ...\r\n");
	gpio_bit_set(GPIOB,GPIO_PIN_2);    //拉高PB2，复位4G模块
	Delay_Ms(500);                     //延迟
	gpio_bit_reset(GPIOB,GPIO_PIN_2);  //拉低PB2，释放4G模块的复位IO
}
/*-------------------------------------------------*/
/*函数名：处理4G模块串口数据                       */
/*参  数：data：数据指针      datalen：数据长度    */
/*返回值：无                                       */
/*-------------------------------------------------*/
void GM4G_Event(uint8_t *data, uint16_t datalen)
{ 
	if(strstr((char *)data,"chaozi")){                                      //搜索到关键词chaozi
		u0_printf("4G模块复位成功\r\n");                                    //串口0输出信息
		u0_printf("准备退出透传状态，从而4G模块可以处理AT指令\r\n");        //串口0输出信息
		u2_printf("+++");	                                                //向4G模块发送 +++
	}
	if((datalen == 1)&&(data[0] == 'a')){                                   //收到4G模块发送的a，进入if
		u2_printf("a");	                                                    //向4G模块发送 a
	}
	if(strstr((char *)data,"+ok")){                                         //搜索到关键词+ok
		u0_printf("4G模块退出透传成功\r\n");                                //串口0输出信息
		BootLoader_Info();                                                  //串口输出命令行信息
	}
}


