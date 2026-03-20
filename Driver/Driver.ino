// Motor A
int M1 = 26; 
int E1 = 14;

// Motor B
int M2 = 27;
int E2 = 12;

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop()
{
  int value;
  for(value = 0 ; value <= 255; value+=5)
  {
    digitalWrite(M1,HIGH);
    digitalWrite(M2,LOW);
    analogWrite(E1, value);
    analogWrite(E2, value); 
    delay(30);
  }
}