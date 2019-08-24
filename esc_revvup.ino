#include <Servo.h>

#define ESCPIN 8
#define PASSDLY 50

Servo ESC;

volatile uint16_t accVal = 0;
char c = '-';
bool isupd = false;
bool rampActivated=true;


void setup() {
  //analogWrite(ESCPIN,0);
  Serial.begin(9600);
  ESC.attach(ESCPIN,1000,2000);

}

void escRamp()
{
 volatile uint8_t pls=35;
 bool rampUp=true;
 uint16_t ctcycle=0;

 bool loopon=true;
 delay(1000);
 bool en1=true;
 bool en2=true;

 while(loopon)
 {
     ctcycle++;
         if(pls<160&&rampUp==true)
         {
           if(en1)
           {
             Serial.println("rampUp");
             en1=false;
           }
           pls++;
         }
        else
        {
           rampUp=false;
            pls--;
            if(en2)
            {
              Serial.println("rampdwn");
              en2=false;
            }
           if(pls==35)
           {
                rampUp=true;
                en1=true;
                en2=true;
            }

       }
       if(ctcycle==500)
       {
             loopon=false;
             rampActivated=false;
         }
    ESC.write(pls);
    delay(PASSDLY);
}
 }


void loop()
{
  if(rampActivated)
  {
      escRamp();
  }
  if(Serial.available())
  {
               c=Serial.read();
                if(c=='*')
                {
                  //analogWrite(ESCPIN,0);
                  accVal=0;
                  Serial.println("reset");
                  }
               if(c=='+')
               {
                         if(accVal <= 175)
                         {
                                        accVal += 2;
                                        isupd = true;
                                        //Serial.print("current val:");
                                        //Serial.println(analogRead(ESCPIN));
                          }
                          else
                          {
                                accVal = 179;
                                 isupd = true;
                                 //Serial.print("current val:");
                                 //Serial.println(analogRead(ESCPIN));
                         }
                }

              if(c == '-')
              {
                         if(accVal > 5)
                         {
                                       accVal -= 3;
                                       isupd = true;
                          }
                          else
                          {
                                  accVal = 5;
                          }
               }
    }
   if(isupd)
   {
             //Serial.clear();
             Serial.print("throttle:");

             Serial.println(accVal);
             isupd = false;
   }

   ESC.write(accVal);

}
