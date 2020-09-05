#include <LiquidCrystal.h>
#include "max6675.h"

// LCD constant variables
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const int btnNone   = 0;
const int btnRight  = 1;
const int btnUp     = 2;
const int btnDown   = 3;
const int btnLeft   = 4;
const int btnSelect = 5;

// Thermocouple variables
const int sckPin = 10;
const int csPin = 11;
const int soPin = 12;
MAX6675 ktc(sckPin, csPin, soPin);

enum AvailableStates {
  ReadTemp = 0,
  SetMinTemp,
  SetMaxTemp
};

AvailableStates currentState = AvailableStates::ReadTemp;
float minTemp = 0;
float maxTemp = 1024;
float currentTemp = 0;

int readLcdButtons()
{
  int adc_key_in = analogRead(0);

  if (adc_key_in < 10)   return btnRight;  
  if (adc_key_in < 120)  return btnUp; 
  if (adc_key_in < 260)  return btnDown; 
  if (adc_key_in < 420)  return btnLeft; 
  if (adc_key_in < 650)  return btnSelect;   

  return btnNone;
}

void runStateMachine()
{
  const int buttonPressed = readLcdButtons();
  lcd.clear();
  switch(currentState)
  {
    case AvailableStates::ReadTemp:
    {     
      if(buttonPressed == btnSelect)
      {
        currentState = AvailableStates::SetMinTemp;
        lcd.setCursor(0, 0);
        lcd.print("IMPOSTAZIONE");
        lcd.setCursor(0, 1);
        lcd.print("TEMPERATURA");
        delay(2000); 
      }
      else
      {
        lcd.setCursor(0, 0);
        lcd.print("TEMP CORRENTE:");
        lcd.setCursor(3, 1);
        char string[13];
        sprintf(string, "%d C", currentTemp);
        lcd.print(string);
      }
      
    break;
    }
    
    case AvailableStates::SetMinTemp:
    {
      lcd.setCursor(0, 0);
      lcd.print("MINIMA:");
      lcd.setCursor(0, 1);
      lcd.print(minTemp);
      lcd.print(" C");
      
      switch (buttonPressed)
      {
        case btnSelect:
            // Salvare valore temperatura minima in memoria
          currentState = AvailableStates::SetMaxTemp;
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
      lcd.setCursor(0, 1);
      lcd.print(maxTemp);
      lcd.print(" C");

      switch (buttonPressed)
      {
        case btnSelect:
            // Salvare valore temperatura massima in memoria
          currentState = AvailableStates::ReadTemp;
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
  digitalWrite(SS, HIGH);

  Serial.begin(9600); // DEBUG
  
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("CONTROLLORE");
  lcd.setCursor(0, 1);
  lcd.print("PIZZA PAPA'");
  delay(2000);
  lcd.clear();
}

void loop() {
  currentTemp = ktc.readCelsius();

  if(currentTemp < minTemp || currentTemp > maxTemp)
  {
    // Aprire relè
  }
  else
  {
    // Chiudere Relè
  }
  
  runStateMachine();
  delay(200);
}
