char val;
int leftorright;
int downorup,baseycorrection;
int cworccw,basethetacorrection;
int hitcommand,encoderangle,prevangleconstant;
int senseValue, pwmvalue;
int angleconstant;

void setup()
{
  Serial.begin(9600);
  Serial3.begin(9600);
}

void loop()
{
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
    }
    
    Serial.println(leftorright);
    Serial.println(downorup);
    Serial.println(baseycorrection);
    Serial.println(cworccw);
    Serial.println(basethetacorrection);
    Serial.println(hitcommand);
    Serial.println(encoderangle);
    
    
  }

}





