/* 
 实验4.4
 */

configuration SensorC
{
}
implementation
{
  components MainC, SensorC, LedsC;

  components new SensirionSht11C();
  components new HamamatsuS1087ParC();
  components new TimerMilliC() as Timer0;

  //添加printf的组件
  components PrintfC;
  components SerialStartC;

  uses interface Read<uint16_t> as readTemp;
  uses interface Read<uint16_t> as readHumidity;
  uses interface Read<uint16_t> as readPhoto;

  SensorC -> MainC.Boot;
  SensorC.readTemp -> SensirionSht11C.Temperature;
  SensorC.readHumidity -> SensirionSht11C.Humidity;
  SensorC.readPhoto -> HamamatsuS1087ParC;
  SensorC.Timer0 -> Timer0;
}