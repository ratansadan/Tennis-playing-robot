#define encoder_motor_pin_A 2 //change to corresponding pins in mega
#define encoder_motor_pin_B 3 //change to corresponding pins in mega
#define encoder_motor_pin_input_1 8
#define encoder_motor_pin_input_2 9
#define encoder_motor_pin_enable 10

volatile unsigned int encoder_motor_pos = 0;

void setup() {

  pinMode(encoder_motor_pin_A, INPUT);
  pinMode(encoder_motor_pin_B, INPUT);
  attachInterrupt(0, doEncoderA, CHANGE);
  attachInterrupt(1, doEncoderB, CHANGE);

  pinMode(encoder_motor_pin_input_1, OUTPUT);
  pinMode(encoder_motor_pin_input_2, OUTPUT);
  pinMode(encoder_motor_pin_enable, OUTPUT);

  Serial.begin (9600);

}
void loop()
{
 encoderMotorCW(900);
 delay(4000);
 encoderMotorCCW(900);
 delay(4000);
 encoderMotorCW(1800);
 delay(4000);
 encoderMotorCCW(1800);
 delay(4000);
 
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
      encoder_motor_pos = encoder_motor_pos + 1;         // CW
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;         // CCW
    }
  }
  else
  {
    if (digitalRead(encoder_motor_pin_B) == HIGH) {
      encoder_motor_pos = encoder_motor_pos + 1;          // CW
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;          // CCW
    }
  }
}

void doEncoderB(){
  if (digitalRead(encoder_motor_pin_B) == HIGH) {
    if (digitalRead(encoder_motor_pin_A) == HIGH) {
      encoder_motor_pos = encoder_motor_pos + 1;         // CW
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;         // CCW
    }
  }
  else {
    if (digitalRead(encoder_motor_pin_A) == LOW) {
      encoder_motor_pos = encoder_motor_pos + 1;          // CW
    }
    else {
      encoder_motor_pos = encoder_motor_pos - 1;          // CCW
    }
  }
}





