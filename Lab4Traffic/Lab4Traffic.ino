#include <scheduler.h>

#define D3 6  // Light 1 - A
#define D4 7  // Light 1 - B
#define D5 8  // Light 2 - A
#define D6 9  // Light 2 - B

// FSM states
enum TrafficState {
  STATE_RED,
  STATE_GREEN,
  STATE_YELLOW
};

// FSM for L1
TrafficState stateL1 = STATE_RED;
int timerL1 = 5;

// FSM for L2
TrafficState stateL2 = STATE_GREEN;
int timerL2 = 3;

// Control Light 1 (L1)
void setLightColorL1(const char* color) {
  if (strcmp(color, "RED") == 0) {
    digitalWrite(D3, HIGH);
    digitalWrite(D4, HIGH);
  } else if (strcmp(color, "GREEN") == 0) {
    digitalWrite(D3, HIGH);
    digitalWrite(D4, LOW);
  } else if (strcmp(color, "YELLOW") == 0) {
    digitalWrite(D3, LOW);
    digitalWrite(D4, HIGH);
  } else {
    digitalWrite(D3, LOW);
    digitalWrite(D4, LOW); // OFF
  }
}

// Control Light 2 (L2)
void setLightColorL2(const char* color) {
  if (strcmp(color, "RED") == 0) {
    digitalWrite(D5, HIGH);
    digitalWrite(D6, HIGH);
  } else if (strcmp(color, "GREEN") == 0) {
    digitalWrite(D5, HIGH);
    digitalWrite(D6, LOW);
  } else if (strcmp(color, "YELLOW") == 0) {
    digitalWrite(D5, LOW);
    digitalWrite(D6, HIGH);
  } else {
    digitalWrite(D5, LOW);
    digitalWrite(D6, LOW); // OFF
  }
}

// FSM logic for L1
void taskUpdateL1() {
  switch (stateL1) {
    case STATE_RED:
      setLightColorL1("RED");
      if (--timerL1 <= 0) {
        stateL1 = STATE_GREEN;
        timerL1 = 3;
      }
      break;

    case STATE_GREEN:
      setLightColorL1("GREEN");
      if (--timerL1 <= 0) {
        stateL1 = STATE_YELLOW;
        timerL1 = 2;
      }
      break;

    case STATE_YELLOW:
      setLightColorL1("YELLOW");
      if (--timerL1 <= 0) {
        stateL1 = STATE_RED;
        timerL1 = 5;
      }
      break;
  }
}

// FSM logic for L2
void taskUpdateL2() {
  switch (stateL2) {
    case STATE_RED:
      setLightColorL2("RED");
      if (--timerL2 <= 0) {
        stateL2 = STATE_GREEN;
        timerL2 = 3;
      }
      break;

    case STATE_GREEN:
      setLightColorL2("GREEN");
      if (--timerL2 <= 0) {
        stateL2 = STATE_YELLOW;
        timerL2 = 2;
      }
      break;

    case STATE_YELLOW:
      setLightColorL2("YELLOW");
      if (--timerL2 <= 0) {
        stateL2 = STATE_RED;
        timerL2 = 5;
      }
      break;
  }
}

// Scheduler timer tick task
void TIMER_ISR(void *pvParameters) {
  while (1) {
    SCH_Update();
    vTaskDelay(pdMS_TO_TICKS(10)); // 10ms tick
  }
}

void setup() {
  for (int i = 0; i <= 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  xTaskCreate(TIMER_ISR, "TIMER_ISR", 2048, NULL, 2, NULL);

  SCH_Init();
  SCH_Add_Task(taskUpdateL1, 0, 100); // every 1s
  SCH_Add_Task(taskUpdateL2, 0, 100); // every 1s
}

void loop() {
  SCH_Dispatch_Tasks();
}







/*
### LEGACY CODE ###
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
*/





