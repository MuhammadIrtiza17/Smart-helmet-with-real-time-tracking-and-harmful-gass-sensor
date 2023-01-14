#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include<TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial SerialGPS(4, 5); 
// Update HOST URL here
const int button = 16;
int temp = 0;
WiFiClient wifiClient;
#define HOST "smarthelmetcoal.000webhostapp.com"          // Enter HOST URL without "http:// "  and "/" at the end of URL

#define WIFI_SSID " "            // WIFI SSID here                                   
#define WIFI_PASSWORD " "        // WIFI password here

// Declare global variables which will be uploaded to server

// SoftwareSerial espSerial(9, 10);


float Latitude , Longitude;
String sendval, message,sendlatitude, sendlongitude,postData;


void setup() {

     
Serial.begin(115200); 
SerialGPS.begin(9600);
Serial.println("Communication Started \n\n");  
delay(1000);
  

pinMode(button, INPUT);     // initialize built in led on the board
 


WiFi.mode(WIFI_STA);           
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                     //try to connect with wifi
Serial.print("Connecting to ");
Serial.print(WIFI_SSID);
while (WiFi.status() != WL_CONNECTED) 
{ Serial.print(".");
    delay(500); }

Serial.println();
Serial.print("Connected to ");
Serial.println(WIFI_SSID);
Serial.print("IP Address is : ");
Serial.println(WiFi.localIP());    //print local IP address

delay(30);
}



void loop() { 
while (SerialGPS.available() > 0)
    if (gps.encode(SerialGPS.read()))
    {
      if (gps.location.isValid())
      {
        Latitude = gps.location.lat();
        sendlatitude = String(Latitude , 6);
        Longitude = gps.location.lng();
        sendlongitude = String(Longitude , 6);
        Serial.println("Latitude="+sendlatitude);
        Serial.println("Longitude="+sendlongitude);
      

    

  
      
      HTTPClient http;    // http object of clas HTTPClient


// Convert integer variables to string
      sendval = String(message);  
      
      // sendlatitude= String(latitude);
      // sendlongitude=String(longitude); 

// + "&sendlatitude=" + sendlatitude +"&sendlongitude" + sendlongitude
      postData = "sendval=" + sendval;
      postData += "&sendlatitude="+sendlatitude;
      postData += "&sendlongitude="+sendlongitude;

// We can post values to PHP files as  example.com/dbwrite.php?name1=val1&name2=val2&name3=val3
// Hence created variable postDAta and stored our variables in it in desired format
// For more detials, refer:- https://www.tutorialspoint.com/php/php_get_post.htm

// Update Host URL here:-  

      http.begin(wifiClient,"http://smarthelmetcoal.000webhostapp.com/dbwrite.php");              // Connect to host where MySQL databse is hosted
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");            //Specify content-type header



      int httpCode = http.POST(postData);   // Send POST request to php file and store server response code in variable named httpCode
      Serial.println("Values are, sendval = " + sendval + " and sendlatitude = "+sendlatitude+" and sendlongitude="+sendlongitude );
      delay(2000);

// if connection eatablished then do this
      if (httpCode == 200) { Serial.println("Values uploaded successfully."); Serial.println(httpCode); 
      String webpage = http.getString();    // Get html webpage output and store it in a string
      Serial.println(webpage + "\n"); 
      }

// if failed to connect then return and restart

      else { 
        Serial.println(httpCode); 
        Serial.println("Failed to upload values. \n"); 
        http.end(); 
        return; }

        temp = digitalRead(button);
            if (temp == HIGH) {

              message="Alert";
              Serial.println(message);
              delay(2000);
        
         
       }
            else {
       
              message="NotAlert";
              Serial.println(message);
              delay(2000);
        
          
     } 
    }

  }
}
