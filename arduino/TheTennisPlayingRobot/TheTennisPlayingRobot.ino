//changed to motor 3 pins
#define motor1pin1 22 //28 //IN1 of motor 1
#define motor1pin2 23  //29 //IN2 of motor 1
#define enable1 9 //6
//changed to motor 1 pins
#define motor2pin1 28 //24 //IN1 of motor 2
#define motor2pin2 29 //25 //IN2 of motor 2
#define enable2 6 //4
//changed to motor 2 pins
#define motor3pin1 24 //28 //22 //IN1 of motor 3
#define motor3pin2 25 //29 //23 //IN2 of motor 3
#define enable3 4 //6 //3
//so currently motor 1 is not used
#define turretmotorpin1 26
#define turretmotorpin2 27
#define enableturret 5

#define encoder_motor_pin_A 2 //change to corresponding pins in mega
#define encoder_motor_pin_B 3 //change to corresponding pins in mega
#define encoder_motor_pin_input_1 10//CHANGE IN ATTACH INTERRUPT IN THE SETUP
#define encoder_motor_pin_input_2 12
#define encoder_motor_pin_enable 11

volatile unsigned int encoder_motor_pos = 2250;
char val;
int leftorright;
int downorup,baseycorrection;
int cworccw,basethetacorrection;
int hitcommand,encoderangle;
int senseValue, pwmvalue;
int angleconstant=2250;
int prevangleconstant=2250;
boolean just_after_a_cw_hit= false;
boolean just_after_a_ccw_hit= false;
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////.............SETUP................./////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
  pinMode(motor1pin1,OUTPUT);
  pinMode(motor1pin2,OUTPUT);
  pinMode(motor2pin1,OUTPUT);
  pinMode(motor2pin2,OUTPUT);
  pinMode(motor3pin1,OUTPUT);
  pinMode(motor3pin2,OUTPUT);

  pinMode(enable1,OUTPUT);
  pinMode(enable2,OUTPUT);
  pinMode(enable3,OUTPUT);

  pinMode(encoder_motor_pin_A, INPUT);
  pinMode(encoder_motor_pin_B, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);
  attachInterrupt(2,turretHalt,FALLING);

  pinMode(encoder_motor_pin_input_1, OUTPUT);
  pinMode(encoder_motor_pin_input_2, OUTPUT);
  pinMode(encoder_motor_pin_enable, OUTPUT);

  Serial.begin(9600);
  Serial3.begin(9600);



}
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////.............LOOP................./////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
  //////////////////////////////////////////////////////////////////////////////////////////////
  //////////////.............SERIAL COMMUNICATION BETWEEN XBee S2.................//////////////
  //////////////////////////////////////////////////////////////////////////////////////////////

  Serial3.write('z');
  delay(1);
  Serial.println("REQUEST SENT____________________");

  if (Serial3.available() > 15  )
  {
    Serial.println("________________RESPONSE RECEIVED");
    val = Serial3.read();

    if (val == 'a')
    {
      leftorright = Serial3.parseInt();
      downorup = Serial3.parseInt();
      baseycorrection=Serial3.parseInt();
      cworccw=Serial3.parseInt();
      basethetacorrection=Serial3.parseInt();
      hitcommand=Serial3.parseInt();
      encoderangle=Serial3.parseInt();
      if(encoderangle>0)
      {
        if(encoderangle<225)
        {
          prevangleconstant=angleconstant;
          angleconstant=2250+(10*encoderangle);
        }
        else if(encoderangle<270)
        {
          prevangleconstant=angleconstant;
          angleconstant=2250+(10*225);
        }
        else if(encoderangle<315)
        {
          prevangleconstant=angleconstant;
          angleconstant=2250-(10*45);
        }
        else
        {
          prevangleconstant=angleconstant;
          angleconstant=2250+(10*encoderangle)-(360*10);
        }
      }
      //////////////////////////////////////////////////////////////////////////////////////////////
      //////////////////////.............ALL DATA RECEIVED................./////////////////////////
      //////////////////////////////////////////////////////////////////////////////////////////////

      if(leftorright==1)
      {
        reverse(120);
        if(abs(prevangleconstant-angleconstant)>200)
          encoderMotorPos(angleconstant);
        motorreset();

      }

      else if(leftorright==0)
      {
        forward(120);
        if(abs(prevangleconstant-angleconstant)>200)
          encoderMotorPos(angleconstant);
        motorreset();
      }

      else if(leftorright==3)
      {
        if(cworccw==7)
        {
          cw(basethetacorrection);
          if(abs(prevangleconstant-angleconstant)>200)
            encoderMotorPos(angleconstant);
          if(hitcommand==10){
            hit(angleconstant);
          }
          else
          {
            motorreset();
          }

        }
        else if(cworccw==8)
        {
          ccw(basethetacorrection);
          if(abs(prevangleconstant-angleconstant)>200)
            encoderMotorPos(angleconstant);
          if(hitcommand==10){
            hit(angleconstant);
          }
          else
          {
            motorreset();
          }
        }
        else
        {
          halt();
          if(abs(prevangleconstant-angleconstant)>200)
            encoderMotorPos(angleconstant);
          if(hitcommand==10){
            hit(angleconstant);
          }
          else
          {
            motorreset();
          }
        }

      }






    }
  }


}





//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////.............BASE MOVEMENTS.................////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void forward( int valPWM)
{
  Serial.println("FORWARD");

  digitalWrite(motor1pin1,HIGH);//OFF
  digitalWrite(motor1pin2,LOW);
  analogWrite(enable1,valPWM);

  digitalWrite(motor2pin1,LOW);// CW
  digitalWrite(motor2pin2,HIGH);
  analogWrite(enable2,valPWM);

  digitalWrite(motor3pin1,LOW);  //CCW
  digitalWrite(motor3pin2,LOW);
  analogWrite(enable3,0);
}

void reverse(int valPWM )
{
  Serial.println("REVERSE");

  digitalWrite(motor1pin1,LOW);//OFF
  digitalWrite(motor1pin2,HIGH);
  analogWrite(enable1,valPWM+15);

  digitalWrite(motor2pin1,HIGH);// CW
  digitalWrite(motor2pin2,LOW);
  analogWrite(enable2,valPWM);

  digitalWrite(motor3pin1,LOW);  //CCW
  digitalWrite(motor3pin2,LOW);
  analogWrite(enable3,0);
}

void right()
{
  Serial.println("RIGHT");

  analogWrite(enable1,100);//
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,HIGH);
  delay(1);
  digitalWrite(motor2pin1,LOW);//CCW
  digitalWrite(motor2pin2,HIGH);
  analogWrite(enable2,100);
  delay(1);
  digitalWrite(motor3pin1,HIGH);//CCW
  digitalWrite(motor3pin2,LOW);
  analogWrite(enable3,100);
  delay(1);
}

void left()
{
  Serial.println("LEFT");

  digitalWrite(motor1pin1,HIGH);//CCW
  digitalWrite(motor1pin2,LOW);
  analogWrite(enable1,100);

  digitalWrite(motor2pin1,HIGH);//CW
  digitalWrite(motor2pin2,LOW);
  analogWrite(enable2,100);

  digitalWrite(motor3pin1,LOW);//CW
  digitalWrite(motor3pin2,HIGH);
  analogWrite(enable3,100);
}


void halt()
{
  Serial.println("STOP");
  digitalWrite(motor1pin1,HIGH);//OFF
  digitalWrite(motor1pin2,HIGH);
  analogWrite(enable1,0);

  digitalWrite(motor2pin1,HIGH);//OFF
  digitalWrite(motor2pin2,HIGH);
  analogWrite(enable2,0);

  digitalWrite(motor3pin1,HIGH);//OFF
  digitalWrite(motor3pin2,HIGH);
  analogWrite(enable3,0);

}
void cw(int valPWM)
{
  Serial.println("CLOCK-WISE");

  analogWrite(enable1,valPWM);//
  digitalWrite(motor1pin1,LOW);
  digitalWrite(motor1pin2,HIGH);

  digitalWrite(motor2pin1,LOW);//CCW
  digitalWrite(motor2pin2,HIGH);
  analogWrite(enable2,valPWM);

  digitalWrite(motor3pin1,LOW);//CCW
  digitalWrite(motor3pin2,HIGH);
  analogWrite(enable3,valPWM);
}
void ccw(int valPWM)
{
  Serial.println("COUNTER-CLOCK-WISE");

  digitalWrite(motor1pin1,HIGH);//CCW
  digitalWrite(motor1pin2,LOW);
  analogWrite(enable1,valPWM);


  digitalWrite(motor2pin1,HIGH);//CCW
  digitalWrite(motor2pin2,LOW);
  analogWrite(enable2,valPWM);

  digitalWrite(motor3pin1,HIGH);//CCW
  digitalWrite(motor3pin2,LOW);
  analogWrite(enable3,valPWM);

}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////.............TURRET MOVEMENT................////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void motorreset()
{
  Serial.println("inside motorreset");
  if(just_after_a_cw_hit==true)
  {
    just_after_a_cw_hit=false;
    turretanticlockwise(80);
    Serial.println("inside motorreset --> just_after_a_cw_hit==true");
  }
  else if(just_after_a_ccw_hit==true)
  {
    just_after_a_ccw_hit=false;
    turretclockwise(80);
    Serial.println("inside motorreset --> just_after_a_cw_hit==true");
  }
  else
    turretHalt();
}

void hit(int angleValue)
{
  Serial.println("inside hit");
  if(angleValue<3150)
  {
    Serial.println("inside hit---->angleValue<100");
    turretclockwise(175);
    just_after_a_cw_hit=true;
  }
  else if(angleValue>3150)
  {
    Serial.println("inside hit----->angleValue>100");
    turretanticlockwise(175);
    just_after_a_ccw_hit=true;
  }

}

void turretclockwise(int valPWM)
{
  Serial.println("clockwise");

  digitalWrite(turretmotorpin1,HIGH);
  digitalWrite(turretmotorpin2,LOW);
  analogWrite(enableturret,valPWM);

}

void turretanticlockwise(int valPWM)
{
  Serial.println("anticlockwise");

  digitalWrite(turretmotorpin1,LOW);
  digitalWrite(turretmotorpin2,HIGH);
  analogWrite(enableturret,valPWM);

}

void turretHalt()
{
  Serial.println("turrethalt");

  digitalWrite(turretmotorpin1,HIGH);
  digitalWrite(turretmotorpin2,HIGH);
  analogWrite(enableturret,0);
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////.............ENCODER MOTOR MOVEMENT.................////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
void encoderMotorPos(int encoder_motor_final_pos)
{
  while(encoder_motor_pos<encoder_motor_final_pos)
  {
    digitalWrite(encoder_motor_pin_input_1, HIGH);
    digitalWrite(encoder_motor_pin_input_2, LOW);
    analogWrite(encoder_motor_pin_enable, 120);
  }
  while(encoder_motor_pos>encoder_motor_final_pos)
  {
    digitalWrite(encoder_motor_pin_input_1, LOW);
    digitalWrite(encoder_motor_pin_input_2, HIGH);
    analogWrite(encoder_motor_pin_enable, 120);
  }
  digitalWrite(encoder_motor_pin_input_1, HIGH);
  digitalWrite(encoder_motor_pin_input_2, HIGH);
  analogWrite(encoder_motor_pin_enable, 0);
}

void encoderMotorCW(int encoder_motor_steps)
{
  int encoder_motor_final_pos= encoder_motor_pos+encoder_motor_steps;
  while(encoder_motor_final_pos>encoder_motor_pos)
  {
    digitalWrite(encoder_motor_pin_input_1, HIGH);
    digitalWrite(encoder_motor_pin_input_2, LOW);
    analogWrite(encoder_motor_pin_enable, 120);
  }
  digitalWrite(encoder_motor_pin_input_1, HIGH);
  digitalWrite(encoder_motor_pin_input_2, HIGH);
  analogWrite(encoder_motor_pin_enable, 0);
}

void encoderMotorCCW(int encoder_motor_steps)
{
  int encoder_motor_final_pos= encoder_motor_pos-encoder_motor_steps;
  while(encoder_motor_final_pos<encoder_motor_pos)
  {
    digitalWrite(encoder_motor_pin_input_1, LOW);
    digitalWrite(encoder_motor_pin_input_2, HIGH);
    analogWrite(encoder_motor_pin_enable, 120);
  }
  digitalWrite(encoder_motor_pin_input_1, HIGH);
  digitalWrite(encoder_motor_pin_input_2, HIGH);
  analogWrite(encoder_motor_pin_enable, 0);
}

void doEncoderA(){
  if (digitalRead(encoder_motor_pin_A) == HIGH) {
    if (digitalRead(encoder_motor_pin_B) == LOW) {
      encoder_motor_pos = encoder_motor_pos + 1;
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;
    }
  }
  else
  {
    if (digitalRead(encoder_motor_pin_B) == HIGH) {
      encoder_motor_pos = encoder_motor_pos + 1;
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;
    }
  }
}

void doEncoderB(){
  if (digitalRead(encoder_motor_pin_B) == HIGH) {
    if (digitalRead(encoder_motor_pin_A) == HIGH) {
      encoder_motor_pos = encoder_motor_pos + 1;
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;
    }
  }
  else {
    if (digitalRead(encoder_motor_pin_A) == LOW) {
      encoder_motor_pos = encoder_motor_pos + 1;
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;
    }
  }
}















