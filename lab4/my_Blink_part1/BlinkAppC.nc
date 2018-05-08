/* 
实验4.1 组件文件
 */

configuration BlinkAppC
{
}
implementation
{
  components MainC, BlinkC, LedsC;
  //timer0是一个组件
  components new TimerMilliC() as Timer0;

  //添加printf的组件
  components PrintfC;
  components SerialStartC;

  BlinkC -> MainC.Boot;
  BlinkC.Timer0 -> Timer0;
  BlinkC.Leds -> LedsC;
}

