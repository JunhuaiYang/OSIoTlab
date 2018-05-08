/* 
 实验4.2
 */
#include "Timer.h"
#include "printf.h"


module BlinkC @safe()
{
  uses interface Timer<TMilli> as Timer0;
  uses interface Timer<TMilli> as Timer1;
  uses interface Timer<TMilli> as Timer2;
  uses interface Leds;
  uses interface Boot;
}
implementation
{
  event void Boot.booted()
  {
    call Timer0.startPeriodic( 250 );
    call Timer1.startPeriodic( 500 );
    call Timer2.startPeriodic( 1000 );
  }

  event void Timer0.fired()
  {
    int i;

    dbg("BlinkC", "Timer 0 fired @ %s.\n", sim_time_string());
    printf("1");
    printfflush();
    /* 加入计算？  */
    // for(i=0;i<400001;i++)
    for(i=0;i<10001;i++)
      call Leds.led0Toggle();
    /* 
      大概是因为计算量过大阻塞了进程继续运行 阻塞
      具体为什么？
     */
  }
  
  event void Timer1.fired()
  {
    dbg("BlinkC", "Timer 1 fired @ %s \n", sim_time_string());
    printf("2");
    printfflush();

    call Leds.led1Toggle();
  }
  
  event void Timer2.fired()
  {
    printf("3");
    printfflush();
    dbg("BlinkC", "Timer 2 fired @ %s.\n", sim_time_string());
    call Leds.led2Toggle();
  }
}

