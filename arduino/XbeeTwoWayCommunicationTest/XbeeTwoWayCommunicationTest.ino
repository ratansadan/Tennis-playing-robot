int val;
void setup()
{
  Serial3.begin(9600);
  Serial.begin(9600);
}

void loop()
{
  Serial3.write('b');
  delay(10000);
  Serial.println("********VALUE SENT*********");
  if(Serial3.available())
  {
    val =Serial3.parseInt();
    Serial.println(val);
    if(val=='2')
      Serial.println("received back");
      //comment
  }

}


