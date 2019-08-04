/** @file TESTS/heartbeat/send/main.cpp
    @brief Periodically sends heartbeat 
    D Evangelista, 2019 

    This code is an example of a simple node that only sends the required
    nmea2k heartbeat (PGN 126993). 
*/

#include "mbed.h"
#include "rtos.h"
#include "nmea2k.h"
#include "pgn/Pgn126993.h" 

Serial pc(USBTX,USBRX);
nmea2k::CANLayer n2k(p30,p29); // used for sending nmea2k messages



int main(void){

  nmea2k::Frame m;     // holds nmea2k data frame before sending
  nmea2k::PduHeader h; // ISO11783-3 header information 
  nmea2k::PgnData d;   // for PGN data fields
  
  unsigned char c=0;           // heartbeat sends a heartbeat counter
  int heartbeat_interval = 10; // nominally at a 60 s interval
  
  unsigned char node_addr = 0x01; // will need our own address


  
  pc.printf("nmea2k version %s\r\n",NMEA2K_VERSION);
  pc.printf("Simple Heartbeat PGN 126993 demo\r\n");
  pc.printf("Heartbeat interval %d s\r\n",heartbeat_interval);
  

  pc.printf("Heartbeat PGN 126993 send process starting in main thread\r\n"); 
  while (1){
    
    d = nmea2k::Pgn126993(heartbeat_interval*100,c++); // form PGN fields
    h = nmea2k::PduHeader(d.p,d.pgn,node_addr,NMEA2K_BROADCAST); // form header 
    m = nmea2k::Frame(h.id(),d.data(),d.dlen); // assemble message
    if (n2k.write(m)) // send it!
      pc.printf("sent %s\r\n",d.name);
    else
      pc.printf("failed sending %s\r\n",d.name); 

    ThisThread::sleep_for(heartbeat_interval*1000); 
  } // while(1)
  
} // int main(void)


