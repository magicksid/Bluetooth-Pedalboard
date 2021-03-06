//STOMP ACTIVE/INACTIVE:

void stompActive()
{
  stompStatus = !stompStatus;

  for (int s = 0; s < 6; s++)
  {
    stompState[s] = true;
  }

  if (stompStatus == true)
  {
    ledFlash();
    
    digitalWrite(led[6], HIGH);

    clearMode();

    clearTopHalf();//Clear all the way down to 'BANK X'.

    tft.fillRoundRect(195, 155, 110, 18, 3, ILI9341_BLUE);
    tft.drawRoundRect(194, 154, 112, 20, 3, ILI9341_WHITE);
    tft.setFontMode(gTextFontModeTransparent);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextScale(1);
    tft.printAt("STOMP MODE", 200, 158);

    //Draw pedals:
    tft.drawImage(pedal1, 4, 80, pedal1Width, pedal1Height);
    tft.drawImage(pedal2, 143, 80, pedal2Width, pedal2Height);
    tft.drawImage(pedal3, 279, 80, pedal3Width, pedal3Height);
    tft.drawImage(pedal4, 4, 3, pedal4Width, pedal4Height);
    tft.drawImage(pedal5, 143, 3, pedal5Width, pedal5Height);
    tft.drawImage(pedal6, 279, 3, pedal6Width, pedal6Height);

    //Print numbers in each pedal:
    tft.printAt("1", 20, 128);
    tft.printAt("2", 158, 128);
    tft.printAt("3", 295, 128);
    tft.printAt("4", 20, 52);
    tft.printAt("5", 158, 52);
    tft.printAt("6", 295, 52);

    for (int b = 0; b < 5; b++)
    {
      statusBank[b] = false;
    }
    loopStatus = false;
  }

  else
  {
    ledFlash();

    loopStatus = false;

    clearTopHalf();
    clearMode();
    tft.setFont(Arial_bold_14);

    return2lastBank();

    if (presetChanged == false)
    {
      printActivePreset();
      digitalWrite(led[lastLed], HIGH);
    }
    bankButtonNames();

    tft.fillRoundRect(195, 155, 115, 18, 3, ILI9341_GREEN);
    tft.drawRoundRect(194, 154, 117, 20, 3, ILI9341_RED);
    tft.setFontMode(gTextFontModeTransparent);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextScale(1);
    tft.printAt("PRESET MODE", 200, 158);

    digitalWrite(led[6], LOW);
  }
}

//STOMP MODE:

void stompMode()
{
  unsigned int pedalOutline = 0;
  unsigned int numberColour = 0;
  tft.setFontMode(gTextFontModeTransparent);
  //Footswitch code:

  for (int s = 0; s < 6; s++)
  {
    if (keyPressed[s])
    {
      stompState[s] = !stompState[s];

      if (stompState[s] == true)
      {
        MIDI.sendControlChange(s + 2, 0, 1);
        midiLedOn();
        pedalOutline = ILI9341_BLACK;
        numberColour = ILI9341_WHITE;
        digitalWrite(led[s], LOW);
      }

      else
      {
        MIDI.sendControlChange(s + 2, 127, 1);
        midiLedOn();
        pedalOutline = ILI9341_RED;
        numberColour = ILI9341_RED;
        digitalWrite(led[s], HIGH);
      }

      //Draw pedal outlines and numbers accordingly:
      switch (s)                                              
      {
        case 0:

          tft.drawRoundRect(3, 79, 40, 72, 5, pedalOutline);
          tft.drawRoundRect(2, 78, 42, 74, 5, pedalOutline);

          tft.setTextColor(numberColour);
          tft.setTextScale(1);
          tft.printAt("1", 20, 128);
          break;

        case 1:
          tft.drawRoundRect(142, 79, 40, 72, 5, pedalOutline);
          tft.drawRoundRect(141, 78, 42, 74, 5, pedalOutline);

          tft.setTextColor(numberColour);
          tft.setTextScale(1);
          tft.printAt("2", 158, 128);
          break;

        case 2:
          tft.drawRoundRect(278, 79, 40, 72, 5, pedalOutline);
          tft.drawRoundRect(277, 78, 42, 74, 5, pedalOutline);

          tft.setTextColor(numberColour);
          tft.setTextScale(1);
          tft.printAt("3", 295, 128);
          break;

        case 3:
          tft.drawRoundRect(3, 2, 40, 72, 5, pedalOutline);
          tft.drawRoundRect(2, 1, 42, 74, 5, pedalOutline);

          tft.setTextColor(numberColour);
          tft.setTextScale(1);
          tft.printAt("4", 20, 52);
          break;

        case 4:
          tft.drawRoundRect(142, 2, 40, 72, 5, pedalOutline);
          tft.drawRoundRect(141, 1, 42, 74, 5, pedalOutline);

          tft.setTextColor(numberColour);
          tft.setTextScale(1);
          tft.printAt("5", 158, 52);
          break;

        case 5:
          tft.drawRoundRect(278, 2, 40, 72, 5, pedalOutline);
          tft.drawRoundRect(277, 1, 42, 74, 5, pedalOutline);

          tft.setTextColor(numberColour);
          tft.setTextScale(1);
          tft.printAt("6", 295, 52);
          break;
      }
      keyPressed[s] = false;
    }
  }

  //Switches 7 & 8 control volume when stomp mode is active:

  //For footswitch 7 (volume down):
  if ((keyPressed[6]) && (stompStatus == true))
  {
    lastVolVal = volVal - 4; //Change by increments of 4.

    if (lastVolVal <= 0)
    {
      lastVolVal = 0;
    }

    MIDI.sendControlChange(0, lastVolVal, 1);
    midiLedOn();

    volBar = lastVolVal;
    volBar = map(volBar, 0, 127, 0, 78);
    volBar = constrain(volBar, 0, 78);
    volPerc = lastVolVal;
    volPerc = map(volPerc, 0, 127, 0, 100);
    volPerc = constrain(volPerc, 0, 100);

    String sensorVal7 = String(volPerc);

    //Convert the reading to a char array
    sensorVal7.toCharArray(sensorPrintout, 4);

    tft.fillArc(155, 200, 20, 8, 0 + (volBar), 78, ILI9341_BLACK);

    tft.fillArc(155, 200, 20, 8, 0, (volBar), ILI9341_RED);

    tft.fillCircle(155, 200, 10, ILI9341_BLACK);

    tft.setFont(SystemFont5x7);
    tft.setFontMode(gTextFontModeSolid);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextScale(1);
    
    //Keep number centred in arc:
    if (volPerc == 100)
    {
      tft.printAt(sensorPrintout, 146, 197);
    }

    else if (volPerc < 100 && volPerc > 9)
    {
      tft.printAt(sensorPrintout, 150, 197);
    }

    else if (volPerc < 10)
    {
      tft.printAt(sensorPrintout, 153, 197);
    }

    volVal = lastVolVal;

    delay(60); //Delay for increments while holding switch down.
  }

  //For footswitch 8 (volume up):
  if ((keyPressed[7]) && (stompStatus == true))
  {
    lastVolVal = volVal + 4;

    if (lastVolVal >= 127)
    {
      lastVolVal = 127;
    }

    MIDI.sendControlChange(0, lastVolVal, 1);
    midiLedOn();

    volBar = lastVolVal;
    volBar = map(volBar, 0, 127, 0, 78);
    volBar = constrain(volBar, 0, 78);
    volPerc = lastVolVal;
    volPerc = map(volPerc, 0, 127, 0, 100);
    volPerc = constrain(volPerc, 0, 100);

    String sensorVal7 = String(volPerc);

    sensorVal7.toCharArray(sensorPrintout, 4);

    tft.fillArc(155, 200, 20, 8, 0 + (volBar), 78, ILI9341_BLACK);

    tft.fillArc(155, 200, 20, 8, 0, (volBar), ILI9341_RED);

    tft.fillCircle(155, 200, 10, ILI9341_BLACK);

    tft.setFont(SystemFont5x7);
    tft.setFontMode(gTextFontModeSolid);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextScale(1);

    if (volPerc == 100)
    {
      tft.printAt(sensorPrintout, 146, 197);
    }

    else if (volPerc < 100 && volPerc > 9)
    {
      tft.printAt(sensorPrintout, 150, 197);
    }

    else if (volPerc < 10)
    {
      tft.printAt(sensorPrintout, 153, 197);
    }

    volVal = lastVolVal;

    delay(60);
  }
}
