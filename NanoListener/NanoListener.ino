/*

Simple Serial to SDcard Shield 

Uses an Arduino Nano to forward the serial output of a second Arduino to an SD card

Author: Wilson Cosmo
Date: 14-07-2022

*/

#include <SPI.h>
#include <SD.h>
#include<math.h>

File myFile;

int c = 0; //Counter
String mfn = ""; //Next file name

//RGB
int led_r = 8;
int led_g = 7;
int led_b = 6;

void setup() {  

  Serial.begin(9600); //IMPORTANT - Set the same baud rate as the board to be listened to
  while (!Serial) {
  
  }
  
  pinMode(led_r, OUTPUT);
  pinMode(led_g, OUTPUT);
  pinMode(led_b, OUTPUT);

  setLedAlert(0);
  
  //SD card initialization - start
  Serial.println("Initializing SD card"); //debug
  if (!SD.begin(10)) {
    Serial.println("initialization failed!"); //debug
    setLedAlert(1);
    while (1); 
  }
  Serial.println("Initialization done");
  setLedAlert(2);
  //SD card initialization - end

  //Default txt files - start  
  if(SD.exists("readme.txt") == 1){
    Serial.println("Default files detected"); //debug    
    }
  else{
    Serial.println("New startup - creating default files"); //debug
    myFile = SD.open("readme.txt", FILE_WRITE);
    myFile.println("This is a default file generated by the program\nThe 'counter.cfg' is also a default file\nManually changing files may cause problems\n\nThe generated files are stored in the 'logs' directory"); 
    myFile.close();

    myFile = SD.open("counter.cfg", FILE_WRITE);
    myFile.println("1"); 
    myFile.close();

    SD.mkdir("logs"); //the name of the directory where the files will be created
    
    }
  //Default txt files - end

  //Read the counter file - start
  myFile = SD.open("counter.cfg");
  String c_str = "";
  while (myFile.available()){c_str += (char)myFile.read();}
  myFile.close(); 
  c = c_str.toInt();  
  //Read the counter file - end

  //Define the next log file name - start
  int nd = log10(c)+1;    
  mfn += "logs/"; //the name of the directory where the files will be created
  if(nd > 5){
    Serial.print("Counter file reached limit");//debug
    mfn += "extra.txt"; 
  }
  else{
    for(int i = 0; i < 5-nd; i++){
      mfn += '0';           
      }
     mfn += c;
     mfn += ".txt";     
  }
  Serial.print("Current log file: ");//debug
  Serial.println(mfn);//debug
  //Define the next log file name - end

  //Create the next log file - start
  myFile = SD.open(mfn, FILE_WRITE);

  /*
  File header
  A optional line in the start of the file.
  You can modify this header according to the data stored in the log or simply comment the next line to omit the log header.
  */
  myFile.println("Lat_a\tLong_a\tRSSI_a\tSNR_a\tRec_a\tEnv_a\tLat_p\tLong_p\tRSSI_p\tSNR_p\tRec_p\tEnv_p");  
  myFile.close();
  //Create the next log file - end

  //Update the counter file - start
  SD.remove("counter.cfg");
  myFile = SD.open("counter.cfg", FILE_WRITE);
  myFile.println(c + 1); 
  myFile.close();
  //Update the counter file - end
  
}

void loop() {  
  if (Serial.available() > 0){
    myFile = SD.open(mfn, FILE_WRITE);
    String msg = Serial.readString();
    Serial.println(msg); //debug
    myFile.println(msg); 
    myFile.close();
    setLedAlert(3);
  }
}

void setLedAlert(int state){ //Flashes an RGB LED in different colors to display visual alerts.

	int l = state;
  int ledtimer = 100; //delay in led blink
	
	if (l = 0){
    digitalWrite(led_r,LOW);
    digitalWrite(led_g,LOW);
    digitalWrite(led_b,LOW);		
	}
  if (l = 1){
    digitalWrite(led_r,HIGH);
    delay(ledtimer);
    digitalWrite(led_r,LOW);		
	}
  if (l = 2){
    digitalWrite(led_g,HIGH);
    delay(ledtimer);
    digitalWrite(led_g,LOW);		
	}
  if (l = 3){
    digitalWrite(led_b,HIGH);
    delay(ledtimer);
    digitalWrite(led_b,LOW);		
	}

}
