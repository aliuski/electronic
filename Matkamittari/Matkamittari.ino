#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <gp20u7.h>

SoftwareSerial mySerial(8, 9); // RX, TX
GP20U7 gps = GP20U7(mySerial);
Geolocation currentLocation;
//TinyGPS tiny; <- Have to check this library...

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int second = 0;
float latitude;
float longitude;
float latitude_mid = 0;
float longitude_mid = 0;
int counter = 0;

float latitude_dist;
float longitude_dist;
float distance = 0;

void setup()
{
/*
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Goodnight moon1 aki!");
*/
  mySerial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Alustus!");
}

void loop()
{
  if(gps.read()){
    currentLocation = gps.getGeolocation();    
    if(latitude == 0){
      latitude = currentLocation.latitude;
      longitude = currentLocation.longitude;
      latitude_dist = currentLocation.latitude;
      longitude_dist = currentLocation.longitude;
    }

    counter++;
    latitude_mid += currentLocation.latitude;
    longitude_mid += currentLocation.longitude;
//    Serial.print("counter:");
//    Serial.println(counter);
    int newsecond = millis() / 1000;
    if(newsecond > second+2){
//      Serial.println(newsecond);
      lcd.setCursor(0, 0);
      float latitude_tmp = latitude_mid / (float)counter;
      float longitude_tmp = longitude_mid / (float)counter;
      lcd.print((int)(distance_between (latitude, longitude, latitude_tmp, longitude_tmp)*1.2+0.5)); // 3.6
      lcd.print(" km/h  ");
      second = newsecond;
      latitude = latitude_tmp;
      longitude = longitude_tmp;
      counter = 0;
      latitude_mid = 0;
      longitude_mid = 0;
//      Serial.print("latitude:");
//      Serial.println(latitude);
//      Serial.print("longitude:");
//      Serial.println(longitude);
      float dist = distance_between (latitude_dist, longitude_dist, currentLocation.latitude, currentLocation.longitude);
      if(dist > 10){
        latitude_dist = currentLocation.latitude;
        longitude_dist = currentLocation.longitude;
        distance += dist;
        lcd.setCursor(0, 1);
        lcd.print((float)((int)(distance * 1.0095)/1000.0));
        lcd.print(" km");
      }
    }
  }
}

float distance_between (float lat1, float long1, float lat2, float long2) 
{
  // returns distance in meters between two positions, both specified 
  // as signed decimal-degrees latitude and longitude. Uses great-circle 
  // distance computation for hypothetical sphere of radius 6372795 meters.
  // Because Earth is no exact sphere, rounding errors may be up to 0.5%.
  // Courtesy of Maarten Lamers
  float delta = radians(long1-long2);
  float sdlong = sin(delta);
  float cdlong = cos(delta);
  lat1 = radians(lat1);
  lat2 = radians(lat2);
  float slat1 = sin(lat1);
  float clat1 = cos(lat1);
  float slat2 = sin(lat2);
  float clat2 = cos(lat2);
  delta = (clat1 * slat2) - (slat1 * clat2 * cdlong); 
  delta = sq(delta); 
  delta += sq(clat2 * sdlong); 
  delta = sqrt(delta); 
  float denom = (slat1 * slat2) + (clat1 * clat2 * cdlong); 
  delta = atan2(delta, denom); 
  return delta * 6372795; 
}
