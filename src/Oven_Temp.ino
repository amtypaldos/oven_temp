/*
 * Project oven_temp_mon
 * Description: Monitor and Alert on an oven that has been left on
 * Author: Alex Typaldos
 * Date: Feb 7th 2017
 */
 // This #include statement was automatically added by the Particle IDE.

 // This #include statement was automatically added by the Particle IDE.
 #include "application.h"

 /*
   LiquidCrystal Library - Hello Sparky

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

   The circuit:
  * LCD RS pin to digital pin D0
  * LCD EN pin to digital pin D1
  * LCD D4 pin to digital pin D2
  * LCD D5 pin to digital pin D3
  * LCD D6 pin to digital pin D4
  * LCD D7 pin to digital pin D5
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)
  * connect R/W (pin 5) to ground

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 8 Feb 2010
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
  */

 #include "LiquidCrystal/LiquidCrystal.h"

 // Make sure to update these to match how you've wired your pins.
 // pinout on LCD [RS, EN, D4, D5, D6, D7];
 // pin nums LCD  [ 4,  6, 11, 12, 13, 14];
 // Shield Shield [RS, EN, D4, D5, D6, D7];
 // Spark Core    [D3, D5, D2, D4, D7, D8];
 LiquidCrystal lcd(D0, D1, D5, D4, D3, D2);


 //TMP36 Pin Variables
 int tempPin = A0; 	//the analog pin the TMP36's Vout (sense) pin is connected to

 //Set to 1 to display Celsius instead of Fahrenheit
 int celsius = 0;

 //int REDPIN  = D1;
 int GREENPIN = A4;
 int BLUEPIN = A5;


 double temperature = 0.0;
 double temperatureF = 0.0;
 bool notify = false;
 bool ack = false;

int ackalert(String command);


 // Last time, we only needed to declare pins in the setup function.
 // This time, we are also going to register our Spark function

 void setup(){


   // setting pins to output
   //pinMode(REDPIN, OUTPUT);
   pinMode(GREENPIN, OUTPUT);
   pinMode(BLUEPIN, OUTPUT);
   analogWrite(GREENPIN, 20);
   analogWrite(BLUEPIN, 75);
   // set up the LCD's number of columns and rows:
   lcd.begin(16,2);
   // Print a message to the LCD.
   lcd.print("Oven Temp");

   //set mode of temp pin
   pinMode(tempPin, INPUT);

   // Register a Particle variable here
   Particle.variable("temperature", &temperature, DOUBLE);
   Particle.variable("temperatureF", &temperatureF, DOUBLE);
   // register the cloud function
   Particle.function("acknowledge", ackalert);

 }

// acknowledge alert function
int ackalert(String command) {

  if(command == "acknowledge") {
    ack = true;
    return 1;
  }

  else {
    return -1;
  }

}


 void loop() {
   // set the cursor to column 0, line 1
   // (note: line 1 is the second row, since counting begins with 0):
   lcd.setCursor(0, 1);

   // print the number of seconds since reset:
   //lcd.print(millis()/100);


   //getting the voltage reading from the temperature sensor
   int reading = analogRead(tempPin);
   // converting that reading to voltage, for 3.3v arduino use 3.3
   // The returned value from the device is going to be in the range from 0 to 4095
   // Calculate the voltage from the sensor reading
   double voltage = (reading * 3.3) / 4095.0;

   // Calculate the temperature and update our static variable
   temperature = (voltage - 0.5) * 100;

   // Now convert to Farenheight
   temperatureF = ((temperature * 9.0) / 5.0) + 32.0;


   lcd.print(temperatureF);
   lcd.print("\337F");
   lcd.print(" ");
   lcd.print(reading);


   //check to see if temp is above threshold and then publish if so
   if (temperatureF >= 110 && notify == false){
     notify = true;
     Particle.publish("ovenstatus","on");
   }

   else if (temperatureF < 110 && notify == true){
     Particle.publish("ovenstatus","off");
     notify = false;
   }

   else{
   }


 delay(10000);
 }
