//Set the mode the bank buttons are in, pressing expression pedal and the bank/preset down button:
void bankMode()
{
  if (keyPressed[6] == HIGH && loopStatus == false && stompStatus == false && wahVal < 100)
  {
    bankButtonState++;

    if (bankButtonState > 2)
    {
      bankButtonState = 0;
    }

    clearBankMode();
    bankButtons();

    ledFlash();
    digitalWrite(led[lastLed], HIGH);
  }

  switch (bankButtonState)
  {
    case 0:
      pedalboardBank();   //To scroll through pedalboard banks.
      break;

    case 1:
      softwareBank();     //Send MIDI to change banks in software.
      break;

    case 2:
      softwarePreset();   //Send MIDI to change presets in software.
      break;
  }

  //Press buttons 3 and expression pedal simultaneously in preset mode to start calibration:
  if (keyPressed[3] == HIGH && loopStatus == false && stompStatus == false && wahVal < 100)
  {
    calibrate();
  }
}

//Determine what text to print ("BANK", "PRES" or nothing), depending on what mode the bank buttons are in:
void bankButtons()  
{
  if (bankButtonState == 0)
  {
    lastBankButton = 0;
  }

  else if (bankButtonState == 1)
  {
    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("<<BANK", 1, 130);

    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("BANK>>", 90, 130);

    lastBankButton = 1;
  }

  else
  {
    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("<<PRES", 1, 130);

    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("PRES>>", 90, 130);

    lastBankButton = 2;
  }
  keyPressed[6] = false;
}

void pedalboardBank()                                                               //Mode to change pedalboard's banks of presets (no MIDI sent).
{
  if (keyPressed[7] && loopStatus == false && stompStatus == false)                 //If in Preset Mode button 8 is pressed.
  {
    bankNumber++;                                                                   //Move to next bank.

    if (bankNumber > 4)                                                             //Bank 5 (zero indexed) loops back to 1. Increase number for more banks.
    {
      bankNumber = 0;
    }

    presetChanged = true;
    keyPressed[7] = false;

    if (bankNumber != oldBankNumber)                                                 //If banks change, reset preset names and last LED to -1.
    {
      activePreset = -1;
      activeLed = -1;
    }
  }

  else if (keyPressed[6] && loopStatus == false && stompStatus == false)             //Move to previous bank with button 7.
  {
    bankNumber--;

    if (bankNumber < 0)
    {
      bankNumber = 4;                                                                  //Increase number for more banks.
    }

    presetChanged = true;
    keyPressed[6] = false;

    if (bankNumber != oldBankNumber)
    {
      activePreset = -1;
      activeLed = -1;
    }
  }
}

void softwareBank()                                                                  //Mode to change banks of presets in software (MIDI sent).
{
  if (keyPressed[7] && loopStatus == false && stompStatus == false)                  //If in software bank change mode send MIDI notes for bank up/down in software.
  {
    MIDI.sendControlChange(22, 127, 1);
    clearNameTime = millis();
    presetChanged = true;

    ledFlash();

    clearLargeName();
    printBluePreset();
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextScale(2);
    tft.printAlignedOffseted("NEXT BANK", gTextAlignMiddleCenter, 0, -65);

    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("BANK>>", 90, 130);
    keyPressed[7] = false;
    activePreset = -1;
    activeLed = -1;
  }

  else if (keyPressed[6] && loopStatus == false && stompStatus == false)
  {
    MIDI.sendControlChange(21, 127, 1);
    clearNameTime = millis();
    presetChanged = true;

    ledFlash();

    clearLargeName();
    printBluePreset();
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextScale(2);
    tft.printAlignedOffseted("PREV. BANK", gTextAlignMiddleCenter, 0, -65);

    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("<<BANK", 1, 130);
    keyPressed[6] = false;
    activePreset = -1;
    activeLed = -1;
  }

  if (millis() - clearNameTime > 600 && millis() - clearNameTime < 700)                    //After some time, return the red names to yellow and erase large central text.
    {
    clearLargeName();
    bankButtonNames();
    }
}

void softwarePreset()                                                                      //Mode to change presets in software (MIDI sent).
{
  if (keyPressed[7] && loopStatus == false && stompStatus == false)
  {
    MIDI.sendControlChange(24, 127, 1);
    clearNameTime = millis();
    presetChanged = true;

    ledFlash();

    clearLargeName();
    printBluePreset();
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextScale(2);
    tft.printAlignedOffseted("NEXT PRESET", gTextAlignMiddleCenter, 0, -65);

    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("PRES>>", 90, 130);

    keyPressed[7] = false;
    activePreset = -1;
    activeLed = -1;
  }

  else if (keyPressed[6] && loopStatus == false && stompStatus == false)
  {
    MIDI.sendControlChange(23, 127, 1);
    clearNameTime = millis();
    presetChanged = true;

    ledFlash();

    clearLargeName();
    printBluePreset();
    tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
    tft.setTextScale(2);
    tft.printAlignedOffseted("PREV. PRESET", gTextAlignMiddleCenter, 0, -65);

    tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("<<PRES", 1, 130);

    keyPressed[6] = false;
    activePreset = -1;
    activeLed = -1;
  }

   if (millis() - clearNameTime > 600 && millis() - clearNameTime < 700)
    {
     clearLargeName();
     bankButtonNames();
    }
}

void bankButtonNames()
{
  if (lastBankButton == 1)
  {
    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("<<BANK", 1, 130);

    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("BANK>>", 90, 130);
  }

  else if (lastBankButton == 2)
  {
    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("<<PRES", 1, 130);

    tft.setTextColor(ILI9341_YELLOWGREEN, ILI9341_BLACK);
    tft.setTextScale(1);
    tft.printAt("PRES>>", 90, 130);
  }
}