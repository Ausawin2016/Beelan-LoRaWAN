#include <lorawan.h>

const char *devAddr = "26021477";
const char *nwkSKey = "DEC93648B4E98B369E592181148566A0";
const char *appSKey = "F51D8B6949759258B5EBF1C9BAF39851";

unsigned long interval = 10000;    // 10 s interval to send message
unsigned previousMillis = 0;  // will store last time message sent
unsigned long currentMillis;
unsigned int counter = 0;     // message counter
char myStr[50];
char outStr[255];
byte recvStatus = 0;

// from electronic cats usb stick 
const sRFM_pins RFM_pins = {
  .CS = SS,
  .RST = RFM_RST,
  .DIO0 = RFM_DIO0,
  .DIO1 = RFM_DIO1,
  .DIO2 = RFM_DIO2,
  .DIO5 = RFM_DIO5,
};

void setup() {
  // Setup loraid access
  Serial.begin(115200);
  delay(2000);
  if(!lora.init()){
    Serial.println("RFM95 not detected");
    delay(5000);
    return;
  }

  // Set LoRaWAN Class
  lora.setDeviceClass(CLASS_C);

  // Set Data Rate
  lora.setDataRate(SF9BW125);

  // set channel to random
  lora.setChannel(MULTI);
  
  // Put ABP Key and DevAddress here
  lora.setNwkSKey((char *)nwkSKey);
  lora.setAppSKey((char *)appSKey);
  lora.setDevAddr((char *)devAddr);
}

void loop() {
  currentMillis = millis();

  // Check interval overflow
  if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis; 

    sprintf(myStr, "Counter-%d", counter); 
    Serial.print("Sending: ");
    Serial.println(myStr);
    lora.sendUplink((unsigned char *)myStr, strlen(myStr), 0);
    counter++;
  }

  recvStatus = lora.readData(outStr);
  if(recvStatus) {
    Serial.println(outStr);
  }
  
  // Check Lora RX
  lora.update();
}