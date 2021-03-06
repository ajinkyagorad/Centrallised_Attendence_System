/*
 * GPS_NMEA.cpp
 *
 * Created: 22-05-2015 11:21:01 PM
 *  Author: Ajinkya
 */ 

#ifndef GPS_NMEA_CPP_
#define GPS_NMEA_CPP_

#include "GPS_NMEA.h"
//default  constructor
//does nothing
GPSClass::GPSClass()
{
	
}
//initialize GPS <if any>
//RETURN : true always
//<can be modified for specific GPS sensor >

bool GPSClass::init()
{
	//if at all any initialisation sequence
	return true;
}
//updates the lastLatLon 
// INPUT : timeout <default to 200 ms> ( NOTE:this timeout is for the function to end)
// OUTPUT : true on success/ false otherwise
bool GPSClass::update(unsigned long timeout_ms)
{
	Serial3.begin(9600);			//initialise link with bluetooth
	Serial3.flush();				// remove any data in transmit buffer has no effect on reception
	unsigned long time=millis();	// for checking timeout , get reference time
	String rxTemp;					//var :string store temporary received data
	char status;					//var :store status of gps <A/V> : active / inactive
	Serial3.setTimeout(100);		//set timeout for serial receiving ( by default it is 1000ms )
	while( millis()-time<timeout_ms)
	{
		
			char status ; //'A' ACTIVE, 'V' VOID
			//rxTemp=Serial3.readBytesUntil('r',);	//receive  till line ends
			rxTemp=Serial3.readStringUntil('\n');
			//Serial.println(rxTemp);
			if(rxTemp.indexOf("GPRMC")>=0)
			{
				//found the GPRMC line
				/*get the  data between 3rd and 7th ','*/
				// 3 - 5 lat
				//5 to 7 lon
				//number refer to the i'th ','
				//Serial.println(rxTemp);
				int index=0;
				int initial,final;
				for(int i=0;i<10;i++)		//iterate over all commas
				{
					index=rxTemp.indexOf(",",index+1);
					if(i==1)
					{
						 status=rxTemp.charAt(index+1);		//get the status
						/*Serial.println("status:");
						Serial.print(status);*/
					}
					if(i==2)initial=index;
					if(i==4)
					{
						final=index;
						lastPosition.lat=rxTemp.substring(initial+1,final);
						initial=index;	//for longitude //next data
					}
					if(i==6)
					{
						final=index;
						lastPosition.lon=rxTemp.substring(initial+1,final);
						
					}
					
				}	//end  of loop
				
				if(status=='A')
				{
					//if status is 'ACTIVE'  
					lastPosition.isValid=true;
				}
				else{
					
					lastPosition.isValid=false;
				}
				//make lattitude and longitude proper
				lastPosition.lat	=	String(lastPosition.lat.substring(0,2).toFloat()
									+	lastPosition.lat.substring(2,9).toFloat()/60,6)+lastPosition.lat.substring(10,11);
				lastPosition.lon	=	String(lastPosition.lon.substring(0,3).toFloat()
									+	lastPosition.lon.substring(3,10).toFloat()/60,6)+lastPosition.lon.substring(11,12);
									
								
				
			}
			
		
	}
	Serial3.end();
	
}
//get lattitude and longitude
// INPUT :   timeout in milliseconds
//			 referenced position 'pos' 
// RESULT : modified position with lattitude, longitude and validness
void GPSClass::getLatLon(position & pos, unsigned long timeout_ms)
{
		update(timeout_ms);
		pos=lastPosition;
		
}

//get last updated lattitude and longitude 
//INPUT : referenced position 'pos'
//RESULT : modified pos
void GPSClass::getLastLatLon(position& pos)
{
	pos=lastPosition;
}
#endif /* GPS_NMEA_CPP_ */