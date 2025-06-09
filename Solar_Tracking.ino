#include <Servo.h> 

#define SERVOPINH  5 //horizontal servo
#define SERVOPINV  6 //vertical servo


int tol=100;      //The response range of illuminance, the smaller the value, the more sensitive the response, otherwise it is slow 
                        //(the value is 10~100, the sensitivity is different depending on the ambient light intensity, the indoor light source changes greatly, and the change is smaller under the sun)

int dtime=100;    //delay parameter. The smaller the value, the faster the response speed. 
                        //On the contrary, the larger the value, the slower the response speed. Unit: milliseconds General value (10~100) 
            
// Horizontal servo
Servo horizontal;           
int servoh = 90;             
int servohLimitHigh = 180;   
int servohLimitLow = 0;      


// Vertical Servo
Servo vertical;              
int servov = 120;             
int servovLimitHigh = 180;   
int servovLimitLow = 90;     

// Fotootporni senzori
const int ldrlt = A0;   //top left
const int ldrrt = A1;   //top right
const int ldrld = A2;   //down left
const int ldrrd = A3;   //down right

void setup()
{
  horizontal.attach(SERVOPINH); 
  vertical.attach(SERVOPINV);
  horizontal.write(servoh);
  vertical.write(servov);
  delay(100);
  Serial.begin(9600);
}

void loop() 
{

  int lt = analogRead(ldrlt);
  int rt = analogRead(ldrrt);
  int ld = analogRead(ldrld);
  int rd = analogRead(ldrrd);

  Serial.print("lt:");Serial.print(lt);Serial.print(" ");
  Serial.print("rt:");Serial.print(rt);Serial.print(" ");
  Serial.print("ld:");Serial.print(ld);Serial.print(" ");
  Serial.print("rd:");Serial.println(rd);
   
  int avt = (lt + rt) / 2; 
  int avd = (ld + rd) / 2; 
  int avl = (lt + ld) / 2; 
  int avr = (rt + rd) / 2;

  int veg=(avt + avd + avl + avr) / 4;


 if(0<veg&&veg<300)
 { tol=map(veg,10,300,5,100);
   dtime=map(veg,10,300,100,50);
 }
 else
 { tol=50;
   dtime=50;
 }

  Serial.print("veg= ");Serial.println(veg);
  Serial.print("tol= ");Serial.println(tol);
  Serial.print("dtime= ");Serial.println(dtime);
  

  
  //Then calculate the difference between the upper and lower rows and the average value of the left and right rows
  int dvert = avt - avd;    //upper and lower rows 
  int dhoriz = avl - avr;   //left and right rows

  
   if (-1*tol > dvert || dvert > tol) 
   {
      if (avt > avd)
      {
        servov = ++servov;
         if (servov > servovLimitHigh) 
         { 
           servov = servovLimitHigh;
         }
      }
      else if (avt < avd)
      {
        servov= --servov;
        if (servov < servovLimitLow)
        {
          servov = servovLimitLow;
        }
      }
      vertical.write(servov);
    }


  if (-1*tol > dhoriz || dhoriz > tol) 
  {
    if (avl < avr)
    {
      servoh = --servoh;
      if (servoh < servohLimitLow)
      {
        servoh = servohLimitLow;
      }
    }
    else if (avl > avr)
    {
       servoh = ++servoh;
       if (servoh > servohLimitHigh)
       {
         servoh = servohLimitHigh;
       }
    }
    horizontal.write(servoh);
  }

   delay(dtime);
}