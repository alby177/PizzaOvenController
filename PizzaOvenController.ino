#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const int btnNone   = 0;
const int btnRight  = 1;
const int btnUp     = 2;
const int btnDown   = 3;
const int btnLeft   = 4;
const int btnSelect = 5;

enum AvailableStates {
  ReadTemp = 0,
  SetMinTemp,
  SetMaxTemp
};

AvailableStates currentState = AvailableStates::ReadTemp;
int currentTemp = 0;
int minTemp = 0;
int maxTemp = 1024;

int readLcdButtons()
{
  int adc_key_in = analogRead(0);

  if (adc_key_in < 50)   return btnRight;  
  if (adc_key_in < 195)  return btnUp; 
  if (adc_key_in < 380)  return btnDown; 
  if (adc_key_in < 555)  return btnLeft; 
  if (adc_key_in < 790)  return btnSelect;   

  return btnNone;
}

void runStateMachine()
{
  const int buttonPressed = readLcdButtons();
  switch(currentState)
  {
    case AvailableStates::ReadTemp:
    
      if(buttonPressed == btnSelect)
      {
        currentState = AvailableStates::SetMinTemp;
        lcd.setCursor(0, 0);
        lcd.print("IMPOSTAZIONE");
        lcd.setCursor(1, 0);
        lcd.print("TEMPERATURA");
        delay(2000); 
      }
      else
        // Leggi temperatura sonda
      
    break;

    case AvailableStates::SetMinTemp:
    {
      lcd.setCursor(0, 0);
      lcd.print("MINIMA:");
      lcd.setCursor(1, 0);
      char string[16];
      sprintf(string, "%d °C", minTemp); 
      lcd.print(string);

      switch (buttonPressed)
      {
        case btnSelect:
            // Salvare valore temperatura minima in memoria
          currentState = AvailableStates::SetMinTemp;
        break;

        case btnUp    : minTemp += 1;   break;
        case btnDown  : minTemp -= 1;   break;
        case btnLeft  : minTemp += 10;  break;
        case btnRight : minTemp -= 10;  break;  
      }
      break;
    }

    case AvailableStates::SetMaxTemp:
    {
      lcd.setCursor(0, 0);
      lcd.print("MASSIMA:");
      lcd.setCursor(1, 0);
      char string[16];
      sprintf(string, "%d °C", maxTemp); 
      lcd.print(string);

      switch (buttonPressed)
      {
        case btnSelect:
            // Salvare valore temperatura massima in memoria
          currentState = AvailableStates::SetMaxTemp;
        break;

        case btnUp    : maxTemp += 1;   break;
        case btnDown  : maxTemp -= 1;   break;
        case btnLeft  : maxTemp += 10;  break;
        case btnRight : maxTemp -= 10;  break;  
      }
      break;
    }
  }
}

void setup() {
  // Caricare valori minimi e massimi temperatura
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Controllore");
  lcd.setCursor(1, 0);
  lcd.print("pizza papà");
  delay(2000);
}

void loop() {
  runStateMachine();

  if(currentTemp < minTemp || currentTemp > maxTemp)
  {
    // Aprire relè
  }

  delay(100);
}
