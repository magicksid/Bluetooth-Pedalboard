//BOOT SEQUENCE:

void bootSequence()
{
  tft.setFont(Arial_bold_14);

  bootTextSequence();

  for ( int x = 0; x < 2; x++ )
  {
    ledFlash();
  }

  delay(3000);

  tft.fillScreen(ILI9341_BLACK);

  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextScale(2);
  tft.printAlignedOffseted("Choose a Preset", gTextAlignMiddleCenter, 0, -65);

  tft.setTextColor(ILI9341_HOTPINK, ILI9341_BLACK);
  tft.printAt("BANK", 1, 152);

  tft.setTextScale(1);
  tft.setTextColor(ILI9341_RED, ILI9341_BLACK);
  tft.printAt("Vol", 144, 223);

  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.printAt("Wah:", 230, 223);

  tft.fillArc(155, 200, 21, 1, 0, 78, ILI9341_HOTPINK);//Volume arc outline.
  tft.fillArc(155, 200, 12, 1, 0, 78, ILI9341_HOTPINK);

  tft.fillRoundRect(195, 155, 115, 18, 3, ILI9341_GREEN);
  tft.drawRoundRect(194, 154, 117, 20, 3, ILI9341_RED);
  tft.setFontMode(gTextFontModeTransparent);
  tft.setTextColor(ILI9341_WHITE);
  tft.printAt("PRESET MODE", 200, 158);

  //Draw volume arc at boot and give it 50% value:
  volBar = 39;
  tft.setFont(SystemFont5x7);
  tft.setTextColor(ILI9341_WHITE);
  tft.printAt("50", 150, 197);
  tft.fillArc(155, 200, 20, 8, 0 + (volBar), 78, ILI9341_BLACK);
  tft.fillArc(155, 200, 20, 8, 0, (volBar), ILI9341_RED);

  tft.drawImage(Wah, 230, 184, WahWidth, WahHeight);
  tft.fillRoundRect(230, 216, 67, 3, 1, ILI9341_GREEN);
}

void bootTextSequence()
{
  tft.fillScreen(ILI9341_BLACK);

  tft.drawImage(logo1, 122, 20, logo1Width, logo1Height); //MIDI
  tft.drawImage(logo2, 60, 60, logo2Width, logo2Height);  //Pedalboard
  tft.drawImage(logo3, 140, 120, logo3Width, logo3Height);//By
  tft.drawImage(logo4, 120, 150, logo4Width, logo4Height);//Daryl H.

  tft.setTextColor(ILI9341_GREEN, ILI9341_BLACK);
  tft.setTextScale(1);
  tft.printAt("Ver 1.01_5_Banks", 90, 210);

  tft.drawImage(guitar, 210, 110, guitarWidth, guitarHeight);
  tft.drawImage(guitar2, 42, 110, guitar2Width, guitar2Height);
}
