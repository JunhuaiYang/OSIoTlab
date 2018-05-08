/* 
 实验4.1
 */

#include "Timer.h"
#include "printf.h"

//组件
module BlinkC @safe()
{
  uses interface Timer<TMilli> as Timer0;
  uses interface Leds;
  uses interface Boot;
}

//模块
implementation
{
  uint8_t count = 0;  //计数

//启动加载
  event void Boot.booted()
  {
    //timer0是一个接口
    call Timer0.startPeriodic( 1000 );  //定时器周期  1000ms
  }

  event void Timer0.fired()
  {
    count++;
    printf("Here is a uint8: %u\n", count);

    dbg("BlinkC", "Timer 0 fired @ %s.\n", sim_time_string());

    //ledled0Toggle 的具体作用？状态交换？
    if ((count & 0x01) == 0x01)  //0001
    {
      //0001
      call Leds.led0Toggle();  //打开0
    }
    else if ((count & 0x02) == 0x02) //0010
    {
      call Leds.led0Toggle();   //相当于关闭led0
      call Leds.led1Toggle();   //打开led1
    }
    else if ((count & 0x04) == 0x04)  //0100
    {
      call Leds.led0Toggle();  //关闭0
      call Leds.led1Toggle();  //关闭1
      call Leds.led2Toggle();  //打开2
    }
    else 
    {
      call Leds.led0Off();
      call Leds.led1Off();
      call Leds.led2Off();
    }

    //count 重新置零
    if (count == 8)
      count = 0;
  }
}

