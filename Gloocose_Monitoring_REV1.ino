#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
/* Include Header Files*/
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include<LiquidCrystal.h>
/*GPIO pin declarations*/
#define RS  2
#define EN  3
#define D4  4
#define D5  5
#define D6  6
#define D7  7
#define BUZZER  8

/*Global Variable Definitions*/
float temp=0;
int myBPM, GL_Value;
//  Variables
const int PulseWire = A0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"

LiquidCrystal lcd(RS,EN,D4,D5,D6,D7);


void setup(void)
{
  Serial.begin(9600);
  lcd.begin(16,2);
  pinMode(BUZZER,OUTPUT);
  lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("NON INVASIVE");
    lcd.setCursor(0,1);
   lcd.print("GLUCOSE SENSOR");
     delay(3000);
  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   
  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }
  
}

void loop(void)
{ 
    lcd.clear();
     
  myBPM = pulseSensor.getBeatsPerMinute();  
 GL_Value=map(myBPM , 40, 130, 70, 130);
  if (pulseSensor.sawStartOfBeat())
  
  {            // Constantly test to see if "a beat happened". 
   Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
   Serial.print("BPM: ");                        // Print phrase "BPM: " 
   Serial.println(GL_Value); // Print the value inside of myBPM. 
  lcd.clear();
   lcd.setCursor(0,1);
   lcd.print("GL:");
   lcd.print(GL_Value);
   lcd.print("mg/dl");
   delay(2000);
  }
   if(  myBPM>130|| ( myBPM<40 &&  myBPM>0))
  {
    digitalWrite(BUZZER,HIGH);
     lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("GLUCOSE LEVEL");
    lcd.setCursor(0,1);
    lcd.print("OUT OF RANGE");
    delay(2000);
    digitalWrite(BUZZER,LOW);
    delay(1000);
     lcd.clear();
     SEND();
     upload(GL_Value);
  }

  delay(1000);
}
void SEND()
{
  lcd.clear();
lcd.setCursor(0,0);
lcd.print("MESSAGE SENDING..");
   Serial.println("AT");
    delay(1000);
    Serial.println("ATE0");
    delay(1000);
    Serial.println("AT+CMGF=1");
    delay(1000);
    Serial.println("AT+CMGS=\"+919900525322\"");
    delay(1000);
    Serial.println("GLUCOSE LEVEL OUT OF RANGE");
     Serial.print(GL_Value);
   Serial.print("mg/dl");
    delay(300);
    Serial.write(26);
lcd.clear();
lcd.setCursor(0,0);
lcd.print("MESSAGE SENT");
delay(2000);
}
 void upload(int a)
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data uploading");
  Serial.println("AT");
  delay(1000);

 Serial.println("AT+CPIN?");
 delay(1000);
 
 Serial.println("AT+CREG?");
 delay(1000);

 Serial.println("AT+CGATT?");
 delay(1000);

 Serial.println("AT+CIPSHUT");
 delay(1000);

 Serial.println("AT+CIPSTATUS");
 delay(2000);

 Serial.println("AT+CIPMUX=0");
delay(2000);
 
  
 
 Serial.println("AT+CSTT=\"Airtal Internet\"");//start task and setting the APN,
//Serial.println("AT+CSTT=\"IDEA GPRS(40407)\"");//start task and setting the APN,
 delay(1000);



 Serial.println("AT+CIICR");//bring up wireless connection
 delay(6000);

 
 
Serial.println("AT+CIFSR");//get local IP adress
 delay(1000);
  
 Serial.println("AT+CIPSPRT=0");
 delay(3000);
 
 Serial.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");//start up the connection

delay(3000);
          
 
 Serial.println("AT+CIPSEND");//begin send data to remote server
 delay(10000);//
  String str="GET https://api.thingspeak.com/update?api_key=11Z7MCISJOZ6OOCO&field1="+ String(a);
// String str="GET https://api.thingspeak.com/update?api_key=BIVTT8PFTWVI2IMN&field1="+ String(a)+"&field2="+String(b)+"&field3="+String(c);
 //+"&field2=" + String(k) +"&field3=" + String(l) +"&field4=" + String(m) +"&field5=" + String(n) +"&field6=" + String(o);//+ "&field2="+ "100";
  Serial.println(str);//begin send data to remote server
//delay(2000);
// String str1="GET https://api.thingspeak.com/update?api_key=BIVTT8PFTWVI2IMN&field5="+ String(total);
// Serial.println(str1);
 delay(6000);
Serial.write(26);//sending
           
 Serial.println();
 Serial.println("AT+CIPSHUT");//close the connection
 //close the connection
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Data uploaded");
delay(2000);
}
