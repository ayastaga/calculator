#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> 

// Sets up LCD display
LiquidCrystal_I2C lcd(0x27,16,2);

// Defines our keypad
const char number_of_rows = 4;
const char number_of_columns = 4;

// Defines the rows and cols in keypad
char row_pins[number_of_rows] = {2, 3, 4, 5};
char column_pins[number_of_columns] = {6, 7, 8, 9};

// Maps out the keypad to each point
char key_array[number_of_rows][number_of_columns] = {  
  {'D', 'C', '-', '+'},
  {'/', '9', '6', '3'},
  {'0', '8', '5', '2'},
  {'*', '7', '4', '1'}
};

// Initializes array and counter
String calcArray;
int i = 0;

// Creates keypad
Keypad k = Keypad(makeKeymap(key_array), row_pins, column_pins, number_of_rows, number_of_columns);

// Basic setup
void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);                     
  lcd.backlight();
  lcd.clear();
  calcArray = "";
}

// this version works for two numbers only
int operations() {
  int ans;
  for (int i = 0; i < calcArray.length(); i++) {
    if (isSymbol(calcArray[i])) {
      Serial.println(calcArray);
      int x = calcArray.substring(0, i).toInt();
      int y = calcArray.substring(i + 1).toInt();
      switch (calcArray[i]) {
        case '+':
          ans = x + y;
          break;
        case '-':
          ans = x - y;
          break;
        case '*':
          ans = x * y;
          break;
        case '/':
          ans = x / y;
          break;
      }
    } 
  }

  return ans;
}

bool isSymbol(char key_pressed) {
  // define a list of symbols
  char symbols[] = {'+', '-', '*', '/'};
  for (int i = 0; i < 4; i++) {
    if (key_pressed == symbols[i]){
      return true;
    }
  }
  return false;
}

// Main loop
void loop()
{
  // Get the key
  char key_pressed = k.getKey();
  if (key_pressed)
  {
    // Cool if statement shortened
    int prev = (i == 0) ? 0 : i - 1;

    // Avoid repeating alphabet characters
    if (key_pressed != 'C' && key_pressed != 'D') {
      if (isSymbol(key_pressed) && !isSymbol(calcArray[prev])) {
        lcd.print(key_pressed);
        calcArray += key_pressed;
        i++;
      } else if (!isSymbol(key_pressed)) {
        lcd.print(key_pressed);
        calcArray += key_pressed;
        i++;
      }
    }

    // If the previous button pressed was 'C', then clear the display
    if (calcArray[i] == 'C' && !isSymbol(calcArray[i])) {
      lcd.clear();
    }

    // Special function for 'C' --> operations
    if (key_pressed == 'C' && !isSymbol(calcArray[prev])) {
      lcd.setCursor(0, 1);
      int ans = operations();
      lcd.print(ans);
      //////////////
      lcd.setCursor(0, 0);
      delay(1000);
      lcd.clear();
      calcArray = "";
      i = 0;
    }

    // Special function for 'D'
    if (key_pressed == 'D') {
      lcd.setCursor(0, 1);
      lcd.print("Deleted");
      lcd.setCursor(0, 0);
      delay(1000);
      lcd.clear();
      calcArray = "";
      i = 0;
    }
  }
}
