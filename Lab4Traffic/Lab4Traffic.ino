#include <scheduler.h>
#define LED_PIN 48
#define D3 6
#define D4 7
#define D5 8
#define D6 9

void TIMER_ISR(void *pvParameters) {
  
  while(1) {
    SCH_Update();
    vTaskDelay(10);
  }
}

int timeCounter = 0;
void taskUpdate(){
  // whole sequence takes 9 seconds. red (0-5), green (6-8), yellow (8-9)
  // change light on first set of leds
  if(timeCounter < 5){
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
  }
  else if(timeCounter < 8){
    
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
  }
  else if(timeCounter < 10){
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
  }
  
  int shiftedTime = (timeCounter + 5) % 10;
  
  // change light on second set of leds
  if(shiftedTime < 5){
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
  }
  else if(shiftedTime < 8){
    
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
  }
  else if(shiftedTime < 10){
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
  }
  
  //increase the tick timeCounter
  timeCounter++;
  
  //reset timer after 9 second sequence
  if (timeCounter >= 10)
    timeCounter = 0;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);

  for(int i = 0; i<=10; i++){
    pinMode(i, OUTPUT);
  }

  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);

  SCH_Init();
  SCH_Add_Task(taskUpdate, 0, 100);

}

void loop() {
  SCH_Dispatch_Tasks();
}
