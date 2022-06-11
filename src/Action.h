/*
 * User actions you can modify these functions to do custom stuff.
 * Have a look at the 3 examples for some inspiration. Also, have a look
 * at Actions.h to see what actions you can use (eg. print, write, press, etc.)
 *
 */
#pragma once
#include "CommonIncludes.h"
#include "Configuration.h"

// After any action you might need a delay, this delay (in ms) is defined here:
#define USER_ACTION_DELAY 50
namespace ZTD {
// Function used to print large pieces of text.
void printLargeString(const char string[]);

void userAction1(){

  // (All OS) This functions prints a large string of text to the active window.
  printLargeString("This is an example of printing long pieces of text.");
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->write(KEY_RETURN);
  printLargeString("After KEY_RETURN it will print on a new line.");

}

void userAction2(){
    // (Windows Only) This functions opens the calculator and does some simple mathematics.

  Configuration::getBleKeyboard()->write(KEY_MEDIA_CALCULATOR);
  delay(1000); // after opening an app, a longer delay is required for the app to completely launch.
  Configuration::getBleKeyboard()->print("22/7");
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->write(KEY_RETURN);
  delay(USER_ACTION_DELAY);
}

void userAction3(){

  // (Windows Only) This function rickroll's you.

  Configuration::getBleKeyboard()->press(KEY_LEFT_GUI);
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->print("r");
  Configuration::getBleKeyboard()->releaseAll();
  delay(500);
  printLargeString("https://youtu.be/dQw4w9WgXcQ");
  Configuration::getBleKeyboard()->write(KEY_RETURN);

}

void userAction4(){

  // (Mac Only) This function rickroll's you.

  Configuration::getBleKeyboard()->press(KEY_LEFT_GUI);
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->print(" ");
  Configuration::getBleKeyboard()->releaseAll();
  delay(USER_ACTION_DELAY);
  printLargeString("https://youtu.be/dQw4w9WgXcQ");
  Configuration::getBleKeyboard()->write(KEY_RETURN);

}

void userAction5(){

  // (Mac only) This opens a new file in Sublime (has to be installed off course and pastes the last thing you copied to the clipboard.
  // I use this to select pieces of text and copy them to a new file.
  Configuration::getBleKeyboard()->press(KEY_LEFT_GUI);
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->print(" ");
  Configuration::getBleKeyboard()->releaseAll();
  printLargeString("Sublime");
  Configuration::getBleKeyboard()->write(KEY_RETURN);
  delay(500);
  Configuration::getBleKeyboard()->press(KEY_LEFT_GUI);
  Configuration::getBleKeyboard()->print("n");
  Configuration::getBleKeyboard()->releaseAll();
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->press(KEY_LEFT_GUI);
  Configuration::getBleKeyboard()->print("v");
  Configuration::getBleKeyboard()->releaseAll();

}

void userAction6(){

  // (Windows only) This opens a new file in Sublime (has to be installed off course and pastes the last thing you copied to the clipboard.
  // I use this to select pieces of text and copy them to a new file.
  Configuration::getBleKeyboard()->press(KEY_LEFT_GUI);
  delay(USER_ACTION_DELAY);
  Configuration::getBleKeyboard()->print("r");
  Configuration::getBleKeyboard()->releaseAll();
  delay(500);
  printLargeString("notepad");
  Configuration::getBleKeyboard()->write(KEY_RETURN);
  delay(500);
  Configuration::getBleKeyboard()->press(KEY_LEFT_CTRL);
  Configuration::getBleKeyboard()->print("v");
  Configuration::getBleKeyboard()->releaseAll();

}

void userAction7(){

  // Nothing yet, create your own!

}



/* A simple function to print large strings using Configuration::getBleKeyboard()-> The function has a delay
 * between keypresses to not miss any, or hang on 1.
*/

void printLargeString(const char string[]){

  for(int i=0; i < strlen(string); i++ ) {
    char c = string[i];
    Configuration::getBleKeyboard()->print(c);
    delay(10); // 10ms is on most systems enough to not miss a character
  }

}
}
