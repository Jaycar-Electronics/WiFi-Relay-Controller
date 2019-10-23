
// Uno portion of WiFi Relay Controller Project 

// make sure this matches esp
const long serial_baud = 115200;

//quick debug macros, so you can test with serial monitor
//first line prints to serial, second does nothing
//either one will have to be compiled
//#define D(...) Serial.println(__VA_ARGS__);
#define D(...) 


const int relay_start = 2;
const int relay_length = 8;
const int status_led = LED_BUILTIN;


void setup(){

  Serial.begin(serial_baud); 

  for(int i = 0; i < relay_length; i++){
    pinMode(relay_start + i, OUTPUT);

    D("set relay on pin")
    D(relay_start + i)

  }

}

void loop(){

  /*command packets are in the form of:
    >Ha 
    
    > - start command 
    H - hex digit of relay ( 1 -> 16), '1' to '8' is what we need
    a - activate, will leave on until it reaches
    c - close, or any other character in this place
  */


  //this will skip until read reaches '>'

  while(readSerial() != '>' )
    continue; 

  D("read >")
  //now we have already read '>'
  //next character will be the relay
  char relayCharacter = readSerial();
  int relay = charToRelay(relayCharacter);

  D("RelayCode:")
  D(relay)
  //error code
  if ( relay == -1){
    //skip this one, start loop again
    return;
  }

  //now the final character should be the mode 
  char modeCharacter = readSerial();

  //"inline if" statement, is it 'a' ? make mode HIGH else LOW;
  int mode = (modeCharacter == 'a')? HIGH : LOW;
  D("Mode: char")
  D(modeCharacter)
  D("Mode: bool")
  D(mode)

  //then finally run it and loop again

  digitalWrite(relay_start + relay, mode);
}

//simple 'blocking' function to wait until we receive something
char readSerial(){
  while(! Serial.available())
    continue;
  return Serial.read();
}

int charToRelay(char incoming){

  if (incoming >= '0' && incoming <= '9' ){
    //just a standard digit;
    return incoming - '0';
  } else if ( incoming >= 'A' && incoming <= 'F'){
    //hex digit
    return incoming - 'A' + 10;
  } else {
    //out of range, not good
    return -1; 
  }
}
