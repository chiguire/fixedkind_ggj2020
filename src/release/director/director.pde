import processing.serial.*;

GameStateEnum gameState;
WaitingTicks ticks;

Serial screenPort;
Serial trellisPort;

byte[] screenBytes = new byte[]{0, 0};

boolean screenReady;
boolean trellisReady;

void setup() {
  size(800, 500);
  background(255);
  
  gameState = GameStateEnum.START_STATE;
  ticks = new WaitingTicks();
  ticks.setNoWaiting();

  screenReady = false;
  trellisReady = false;

  printArray(Serial.list());
  screenPort = new Serial(this, "COM4", 9600);
  trellisPort = new Serial(this, "COM5", 9600);
}

void draw()
{
  background(255);

  drawGameState();

  switch (gameState)
  {
  case START_STATE:
    start_state(); 
    break;
  case REPRODUCING_MESSAGE:
    reproducing_message_state();
    break;
  case WAITING_MESSAGE:
    waiting_message_state();
    break;
  }
  ticks.increase();
}

void drawGameState()
{
  int x0 = 10;
  int y0 = 10;
  int yd = 12;
  fill(0);
  text("Game State: " + gameState, x0, y0+yd*0);
  text("Waiting ticks: " + ticks.toString(), x0, y0+yd*1);
  text("Screen ready: " + screenReady, x0, y0+yd*2);
  text("Trellis ready: " + trellisReady, x0, y0+yd*3);
  //text("", x0, y0+yd*4);
}


void set_state(GameStateEnum new_state)
{
  exit_current_state(gameState);
  gameState = new_state;
  enter_new_state(gameState);
}

void exit_current_state(GameStateEnum st)
{
  switch (st)
  {
  case START_STATE:
    start_state_exit(); 
    break;
  case REPRODUCING_MESSAGE:
    reproducing_message_state_exit();
    break;
  case WAITING_MESSAGE:
    waiting_message_state_exit();
    break;
  }
}

void enter_new_state(GameStateEnum st)
{
  switch (st)
  {
  case START_STATE:
    start_state_enter(); 
    break;
  case REPRODUCING_MESSAGE:
    reproducing_message_state_enter();
    break;
  case WAITING_MESSAGE:
    waiting_message_state_enter();
    break;
  }
}

void start_state()
{
  if (!screenReady) {
    if (screenPort.available() > 0) {
      char inByte = screenPort.readChar();
      if (inByte == 'r') {
        screenReady = true;
      }
    }
  }
  if (!trellisReady) {
    if (trellisPort.available() > 0) {
      int lf = 10;
      String inString = trellisPort.readStringUntil(lf);
      if (inString != null && inString.contains("ready0")) {
        trellisReady = true;
      }
    }
  }
  if (screenReady && trellisReady) {
    set_state(GameStateEnum.REPRODUCING_MESSAGE);
  }
}

void start_state_exit() {}
void start_state_enter() {}

void reproducing_message_state()
{

}

void reproducing_message_state_enter()
{
  
}

void reproducing_message_state_exit()
{

}

void waiting_message_state()
{

}

void waiting_message_state_enter()
{
  
}

void waiting_message_state_exit()
{

}
/*
void reproducing_message_state()
{

}

void reproducing_message_state_enter()
{
  
}

void reproducing_message_state_exit()
{

}
*/
