String IncomingData = "";
String Temp = "";
char var;
int tmp;
int pwm_out[4];

void setup()
{
    Serial.begin(115200);

    Serial.println("Ready");
}

void loop()
{
    if(Serial.available() > 16)
    {

        while(Serial.read() != 'a') {}

        for(int i = 0; i < 4; i++)
        {
            tmp = (Serial.read() - '0');
            tmp = 10 * tmp + (Serial.read() - '0');
            tmp = 10 * tmp + (Serial.read() - '0');
            tmp = 10 * tmp + (Serial.read() - '0');
            if(tmp < 2001 && tmp > 999)
                pwm_out[i] = tmp;
            Serial.print(pwm_out[i]);         Serial.println();
        }

    }
}
