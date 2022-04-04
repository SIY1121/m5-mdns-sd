#include "display.h"

void displayProgress(String message, int rcv_qa, int rcv_ptr, int rcv_http, int speed) {
  static char working_label[4] =  {'|', '/', '-', '\\'};
  static int prevSpeed = 1;
  static int w = 0;

  if(prevSpeed != speed) {
    prevSpeed = speed;
    w = 0;
  }
  M5.Lcd.setCursor(0,0);
  M5.Lcd.println(message + " " + working_label[w / speed]);
  w = (w + 1) % (4 * speed);

  M5.Lcd.setCursor(0,40);
  M5.Lcd.println("A: " + String(rcv_qa) + "\nPTR: " + String(rcv_ptr) + "\nHTTP: " + String(rcv_http));
}
