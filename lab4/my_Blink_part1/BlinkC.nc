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
    call Timer0.startPeriodic( 1000 );  //定时器周期  1000ms
  }

  event void Timer0.fired()
  {
    count++;
    printf("Here is a uint8: %u\n", count);

    dbg("BlinkC", "Timer 0 fired @ %s.\n", sim_time_string());

    // if ((count & 0x01) == 0x01)  //0001
    // {
    //   //0001
    //   call Leds.led0Toggle();
    // }
    // else if ((count & 0x02) == 0x02) //0010
    // {
    //   //0010 0011 0110 0111
    //   call Leds.led0Toggle();
    //   call Leds.led1Toggle();
    // }
    // else if ((count & 0x04) == 0x04)  //0100
    // {
    //   call Leds.led0Toggle();
    //   call Leds.led1Toggle();
    //   call Leds.led2Toggle();
    // }
    // else 
    // {
    //   call Leds.led0Off();
    //   call Leds.led1Off();
    //   call Leds.led2Off();
    // }

    switch (count)
    {
    case 1:   // 001
      call Leds.led0Toggle();
      call Leds.led1Off();
      call Leds.led2Off();
      break;

    case 2:  //  010
      call Leds.led0Off();
      call Leds.led1Toggle();
      call Leds.led2Off();
      break;

    case 3:  // 011
      call Leds.led0Toggle();
      call Leds.led1Toggle();
      call Leds.led2Off();
      break;

    case 4:  //100
      call Leds.led0Off();
      call Leds.led1Off();
      call Leds.led2Toggle();
      break;

    case 5: //101
      call Leds.led0Toggle();
      call Leds.led1Off();
      call Leds.led2Toggle();
      break;

    case 6: //110
      call Leds.led0Off();
      call Leds.led1Toggle();
      call Leds.led2Toggle();
      break;

    case 7: // 111
      call Leds.led0Toggle();
      call Leds.led1Toggle();
      call Leds.led2Toggle();
      break;

    default:
      call Leds.led0Off();
      call Leds.led1Off();
      call Leds.led2Off();
      break;
    }

    //count 重新置零
    if (count == 8)
      count = 0;
  }
}

