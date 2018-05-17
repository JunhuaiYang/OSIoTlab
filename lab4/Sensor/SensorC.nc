/* 
 实验4.4
 */

#include "TestSensor.h"
#include "SensirionSht11.h"
#include "Timer.h"

module SensorC @safe()
{
  uses interface Timer<TMilli> as Timer0;
  uses interface Leds;
  uses interface Boot;
}
implementation
{
  event void Boot.booted()
  {
    call Timer0.startPeriodic( 1000 );
  }

  event void SampleTimer.fired()
  {	

    call readTemp.read();//读取温度值
    call readHumidity.read();//读取湿度值
    call readPhoto.read();//读取光照值
  }
}