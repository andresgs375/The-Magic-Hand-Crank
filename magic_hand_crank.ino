/*Magic Hand Crank
 * A stepper controller with a rotary encoder by Andrés González
 * Connect the Arduino pin D5 to Dir and pin D6 to step at the stepper driver.
 */
#include <AccelStepper.h>

#define PinSW 2
#define PinCLK 3  
#define PinDT 4
AccelStepper stepper(1, 6, 5); // pin D6 = step, pin D5 = direction driver A4988
int flag=0;
volatile boolean TurnDetected;  
volatile boolean rotationdirection; 
unsigned long switch0=0;
unsigned long rotary0=0;
int setPos = 0;

void Switch()  //Switch detection
{
 if(millis()-switch0>500)
 {flag=flag+1;}
 switch0=millis();
}

void Rotary()  //Rotary detection
{
delay(75);
if (digitalRead(PinCLK))
rotationdirection= digitalRead(PinDT);
else
rotationdirection= !digitalRead(PinDT);
TurnDetected = true;
delay(75);
}

void setup() 
{
  Serial.begin(9600);
  pinMode(PinSW,INPUT_PULLUP); 
  pinMode(PinCLK,INPUT_PULLUP); 
  pinMode(PinDT,INPUT_PULLUP);

  attachInterrupt (digitalPinToInterrupt(2),Switch,RISING); // SW connected to D2
  attachInterrupt (digitalPinToInterrupt(3),Rotary,RISING); // CLK Connected to D3
  stepper.setMaxSpeed(3000);

}

void loop()
  {if (flag == 1)//constant speed activation
  {stepper.setSpeed(250);//change this to a constant speed that accommodate to your project
  stepper.runSpeed();
    
  }
  else if (flag==2){flag=0;}
  else if (TurnDetected)  
  {     TurnDetected = false;  // do NOT repeat IF loop until new rotation detected
        if (rotationdirection) 
        { setPos = setPos + 40;}//change this to match a full turn of the hand crank to a full turn of the motor
        if (!rotationdirection) 
        { setPos = setPos + 40;}// a negative number will follow the direction of the hand crank
        Serial.println(setPos); 
        stepper.move(setPos); //move = the desired position relative to the current position.
    while(stepper.distanceToGo() != 0)
    {
      stepper.setSpeed(1000);   //change this if the motor has problems to move
      stepper.runSpeedToPosition();
      setPos = 0;      
    }   
  }
  }
