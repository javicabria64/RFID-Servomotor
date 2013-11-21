#include <SoftwareSerial.h>
#include <Servo.h> 
 

#define RFIDEnablePin 2 //Pin that enables reading. Set as OUTPUT and LOW to read an RFID tag
#define RFIDSerialRate 2400 //Parallax RFID Reader Serial Port Speed

#define RxPin 5 //Pin to read data from Reader 
#define TxPin 4 //Pin to write data to the Reader NOTE: The reader doesn't get written to, don't connect this line.
SoftwareSerial RFIDReader(RxPin,TxPin);

String RFIDTAG=""; //Holds the RFID Code read from a tag
String DisplayTAG = ""; //Holds the last displayed RFID Tag

Servo myservo;  
                
 
int cerrado = 0;   
int abierto = 180; 

void setup() 
{
 
  RFIDReader.begin(RFIDSerialRate);

  pinMode(RFIDEnablePin,OUTPUT); 

  digitalWrite(RFIDEnablePin, LOW);

  Serial.begin(9600);        

  Serial.println("Control de acceso");  
  
   myservo.attach(9); 

}

void loop() 


{
  
  myservo.write(cerrado); 
  
  if(RFIDReader.available() > 0) // If data available from reader
  { 
    ReadSerial(RFIDTAG);  //Read the tag number from the reader. Should return a 10 digit serial number
  if (RFIDTAG.equals("0F02781ECB"))
   {
     digitalWrite(RFIDEnablePin, HIGH);
     
     myservo.write(abierto);              // tell servo to go to position in variable 'pos' 
    delay(10); 
     
     delay(2000);
     digitalWrite(RFIDEnablePin, LOW);
     Serial.println("Acceso Usuario 1");
     
     myservo.write(cerrado);              // tell servo to go to position in variable 'pos' 
    delay(10); 
   }
   
  
    if (RFIDTAG.equals("0F02782183"))
   {
     digitalWrite(RFIDEnablePin, HIGH);
     
     myservo.write(abierto);              // tell servo to go to position in variable 'pos' 
    delay(10); 
     
     delay(2000);
     digitalWrite(RFIDEnablePin, LOW);
     Serial.println("Acceso Usuario 2");
     
     myservo.write(cerrado);              // tell servo to go to position in variable 'pos' 
    delay(10); 
   }
   
   if (!RFIDTAG.equals("0F02782183")||RFIDTAG.equals("0F02781ECB"))
  {
          Serial.println("Acceso Denegado");
   }
   
  }
  
  
  //This only displays a tag once, unless another tag is scanned
  if(DisplayTAG!=RFIDTAG)
  {
    DisplayTAG=RFIDTAG;
    Serial.println(RFIDTAG);
  }
}

void ReadSerial(String &ReadTagString)
{
  int bytesread = 0;
  int  val = 0; 
  char code[10];
  String TagCode="";

  if(RFIDReader.available() > 0) {          // If data available from reader 
    if((val = RFIDReader.read()) == 10) {   // Check for header 
      bytesread = 0; 
      while(bytesread<10) {                 // Read 10 digit code 
        if( RFIDReader.available() > 0) { 
          val = RFIDReader.read(); 
          if((val == 10)||(val == 13)) {   // If header or stop bytes before the 10 digit reading 
            break;                         // Stop reading 
          } 
          code[bytesread] = val;           // Add the digit           
          bytesread++;                     // Ready to read next digit  
        } 
      } 
      if(bytesread == 10) {                // If 10 digit read is complete 

        for(int x=0;x<10;x++)              //Copy the Chars to a String
        {
          TagCode += code[x];
        }
        ReadTagString = TagCode;          //Update the caller
        while(RFIDReader.available() > 0) //Burn off any characters still in the buffer
        {
          RFIDReader.read();
        } 

      } 
      bytesread = 0;
      TagCode="";
    } 
  } 
}



