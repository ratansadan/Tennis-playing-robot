//derived from turret_test_april_11
boolean just_after_a_cw_hit= false;
boolean just_after_a_ccw_hit= false;

int turretmotorpin1=26;
int turretmotorpin2=27;
int enableturret=5;


char val;
int leftorright,downorup,baseycorrection,cworccw,basethetacorrection,hitcommand,encoderangle,pwmvalue,angleconstant;

int tripSwitchValue = 0;
int turretMode = 0;


void setup()
{
  attachInterrupt(2,turretHalt,FALLING);
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop()
{
  motorreset();
  delay(5000);
  hit(120);
  delay(250);
  motorreset();
  delay(5000);
  hit(10);
  delay(250);
}
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
  //compare angleValue and decide and the direction in which motor has to be hit
  //give a hit function and with required pwm
  if(angleValue<100)
  {
    Serial.println("inside hit---->angleValue<100");
    turretclockwise(175);
    just_after_a_cw_hit=true;
  }
  else if(angleValue>100)
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

