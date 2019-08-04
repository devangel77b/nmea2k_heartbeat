#include "mbed.h"
#include "rtos.h"
#include "nmea2klite.h"

Serial pc(USBTX,USBRX);
Nmea2k n2k(p30,p29);

int main(void){
  Frame m;
  PduHeader h;
  PgnData d; 
  unsigned char c=0;
  int heartbeat_interval = 10;
  unsigned char node_addr = 0x01; 

  pc.printf("nmea2k version %s\r\n",NMEA2K_VERSION);
  pc.printf("Simple Heartbeat PGN 126993 demo\r\n");
  pc.printf("Heartbeat interval %d s\r\n",heartbeat_interval);
  
  pc.printf("Heartbeat PGN 126993 send process starting in main thread\r\n"); 
  while (1){
    
    d = PgnData();
    //Pgn126993(heartbeat_interval*100,c++); // populate data fields
    h = PduHeader(7,d.pgn,node_addr,NMEA2K_BROADCAST); // construct header 
    m = Frame(h.id(),d.data(),d.len()); // construct message
    
    if (n2k.write(m)) // send it
      pc.printf("sent %d\r\n",d.name);
    else
      pc.printf("failed sending %s\r\n",d.name); 

    ThisThread::sleep_for(heartbeat_interval*1000); 
  } // while(1)
  
} // int main(void)


