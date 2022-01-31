/**
 * This example turns the ESP32 into a Bluetooth LE keyboard to control ERG/level modes in Wahoo SYSTM app.
 */
#include <BleKeyboard.h>
#include <ezButton.h>

#define BUTTON1_PIN 25
#define BUTTON2_PIN 21

BleKeyboard bleKeyboard;

ezButton botao1(BUTTON1_PIN);  // create ezButton object that attach to pin 15;
ezButton botao2(BUTTON2_PIN);  // create ezButton object that attach to pin 15;


void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");
  bleKeyboard.begin();
  botao1.setDebounceTime(200); // set debounce time to 100 milliseconds
  botao2.setDebounceTime(200); // set debounce time to 100 milliseconds

}
int level =2;
bool doublepress = 0;
bool inERG = 1;

void loop() {
     botao1.loop(); // Debounce button - MUST call the loop() function first
     botao2.loop(); // Debounce button - MUST call the loop() function first

     bool doubletap = 0;
     bool increase = 0;
     bool decrease = 0;

      if (doublepress) {//last event was a double button press, wait for release
       if (botao1.getState()==1 &&  botao2.getState()==1 ) //buttons have been released
         doublepress = 0;
       else
         delay(10);
                      }
      else {
        if ( botao1.getState()==0 &&  botao2.getState()==0 ) {
          Serial.println("AMBOS OS DOIS");
          doublepress=1;
          doubletap =1;
          }
      else {
        if (botao1.isPressed() && botao2.getStateRaw()==1) {
          Serial.println("botao 1 pressed");
          increase =1;}
        else if (botao2.isPressed() && botao1.getStateRaw()==1 ) {
          Serial.println("botao 2 pressed");
          decrease =1;}
          }
      }

      //handle the keyboard stuff
      if(bleKeyboard.isConnected()) {

        if (doubletap) {
          uint8_t ergkeycode = 96;
         bleKeyboard.write(ergkeycode);  
         Serial.println("ERG");
         inERG =1;
        }
        else if (increase) {
          if (inERG ){
            inERG =0;
           } else {
            if (level != 9){
              level=level+1;
             }
           }
          bleKeyboard.print(level);
          Serial.print("Level ");
          Serial.println(level);

        } else if (decrease) {
          if (inERG ){
            inERG =0;
          } else {
            if (level != 0){
              level=level-1;
              }
            }
          bleKeyboard.print(level);
          Serial.print("Level ");
          Serial.println(level);
        }  
      }


}
