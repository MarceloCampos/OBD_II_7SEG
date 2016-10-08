/*************************************************************************
    OBD-II-7SEG.ino - Velocidade OBD2 display 7 segmentos !
    
    Versão com MAX7219 : Rev. 08/10/16

    by Marcelo Campos - Garoa Hacker Club
    
  Depende Libs:
    - OBD-II library for Arduino -  http://freematics.com 
    - LedControl Library - thanks to PJRC: https://www.pjrc.com/teensy/td_libs_LedControl.html
      
  conector módulo 317:
    Vm + o  o TxD
    RxD o  o -

  Ligações Max7219-Display Cromatek 
  Display       MAX7219
  1 - E           21
  2 - D           23
  3 - Pto         n.c.
  4 - C           20
  5 - G           17
  6 - Dig1        2
  7 - B           16
  8 - Dig2        11
  9 - Dig3        6
  10- F           15
  11- A           14
  12- Dig4        7 n.c.

  Ligações Max7219-Arduino
  Arduino       MAX7219
  D2            1 - Din
  D3            12- Load
  D4            13- Clk
    
Ligações Arduino-OBD II Módulo
  D0   Rx/Tx serial com módulo   
  D1    

 
*************************************************************************/

#include <Arduino.h>
#include <Wire.h>
#include <OBD.h>

// alterei na lib pra poder ser anodo comum, definir "#define ANODO_COMUM" nela
// hardware com transistores PnP + res ~100R pra cada saída display
#include <LedControl.h>

// OBD pela Serial 
COBD obd;

// inputs: DIN pin, CLK pin, LOAD pin. number of chips
LedControl sevseg = LedControl(2, 4, 3, 1);

void setup()
{

  sevseg.shutdown(0, false);  // turns on display
  sevseg.setIntensity(0, 15); // 15 = brightest

  // mydisplay.setDigit(chip, digit_position, number, dot);  
  sevseg.setDigit(0, 0, 1, false);
  sevseg.setDigit(0, 1, 2, false);
  sevseg.setDigit(0, 2, 3, false);

}


void loop()
{
  int value;
  int Flag_Read = 0;
  int value_remap;
  int i; 
  int Speed = 121;
  
  // start communication with OBD-II UART adapter
  obd.begin();

  sevseg_setNumber(0,3);
  delay(500);
  sevseg_setNumber(100,3);
  delay(1000);
    
  sevseg_setNumber(Speed,3); 
  
  // initiate OBD-II connection until success
  while (!obd.init())
  {
       
  }; 

  Speed = 0;
  sevseg_setNumber(Speed,3); 
      
  while (1)
  {
    
    if (obd.read(PID_SPEED, value)) 
    {   
        Flag_Read = 1;   // sinaliza que teve sucesso na leitura p/ mostrar no Display
        Speed = value;
    }
    
//    if (obd.read(PID_RPM, value)) 
//    {   
//           
//    }
//    
//    if (obd.read(PID_DISTANCE, value))
//    {
// 
//    }
    
    if (Flag_Read)
    {
        Flag_Read = 0;   // re-inicializa flag        
        sevseg_setNumber(Speed,1);
    }
   
    
  }


  
}

void sevseg_setNumber(int value, int dummy)
{
  int _cent;
  int _dez;
  int _uni;

  _cent = value / 100;
  _uni = value % 10;
  _dez = ((value - (_cent*100) - _uni) / 10 );
  
  // mydisplay.setDigit(chip, digit_position, number, dot);  
  sevseg.setDigit(0, 0, _cent, false);
  sevseg.setDigit(0, 1, _dez, false);
  sevseg.setDigit(0, 2, _uni, false);
  
}












