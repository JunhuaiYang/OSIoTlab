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

  event void Boot.booted()
  {
    call Timer0.startPeriodic( 1000 );  //定时器时间
  }

  event void Timer0.fired()
  {
    count++;
    printf("Here is a uint8: %u\n", count);

    dbg("BlinkC", "Timer 0 fired @ %s.\n", sim_time_string());
    if ((count & 0x01) == 0x01)  //0001
    {
      call Leds.led0Toggle();
    }
    else if ((count & 0x02) == 0x02) //0010
    {
      call Leds.led0Toggle();
      call Leds.led1Toggle();
    }
    else if ((count & 0x04) == 0x04)  //0100
    {
      call Leds.led0Toggle();
      call Leds.led1Toggle();
      call Leds.led2Toggle();
    }
    else 
    {
      call Leds.led0Off();
      call Leds.led1Off();
      call Leds.led2Off();
    }
    if (count == 8)
      count = 0;
  }
}

