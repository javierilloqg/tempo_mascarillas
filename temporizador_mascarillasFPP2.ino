/******************************************************************************
#* Copyright (C) 2020 by Javier Quesada Galban
#*
#* Arduino program to count face-mask hours
#*
#******************************************************************************
#
#* @file temporizador_mascarillasFPP2.ino
#* @author Javier Quesada Galban
#* @date 1 August 2020
#* @brief Program to show used hours of a FPP2 face-mask. 
#*
#
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <Chrono.h>
#include <Button.h>
#include <EEPROM.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

unsigned long millisInicio;
unsigned long millisActual;
const unsigned long duracion = 28800; //cambiar
int longitud = 0;
int write_mem = 0;
int tiempo_memoria;
bool aviso = false;

const unsigned char mascarilla_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x60, 0x06, 0x00,
  0x1c, 0x10, 0x08, 0x30, 0x22, 0x08, 0x10, 0x40, 0x41, 0x04, 0x20, 0x82,
  0xbd, 0x03, 0xc0, 0xbd, 0x25, 0x01, 0x80, 0xa4, 0x45, 0x02, 0x40, 0xa2,
  0x4d, 0x00, 0x00, 0xb2, 0x4a, 0x00, 0x00, 0x52, 0x52, 0x00, 0x00, 0x4a,
  0x34, 0x00, 0x00, 0x2c, 0x0c, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x18,
  0x30, 0x00, 0x00, 0x0c, 0x20, 0x00, 0x00, 0x04, 0x40, 0x00, 0x00, 0x02,
  0x40, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x01, 0x00, 0x01, 0x80, 0x00,
  0x00, 0x00, 0x60, 0x00, 0x00, 0x0c, 0x30, 0x00, 0x00, 0x70, 0x0e, 0x00,
  0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char alerta_bits[] PROGMEM = {
  0x00, 0x00, 0xc0, 0x03, 0xe0, 0x07, 0xe0, 0x07, 0xf0, 0x0f, 0x70, 0x0e,
   0x78, 0x1e, 0x78, 0x1e, 0x7c, 0x3e, 0x7c, 0x3e, 0x7e, 0x7e, 0xfe, 0x7f,
   0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x3f
};

const unsigned char blanco_bits[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Chrono myChronoSeconds(Chrono::SECONDS);
Button boton_start(7);
Button boton_pause(6);
Button boton_zero(5);


// End of constructor list


void setup(void) {
  u8g2.begin();
  //EEPROM.get(write_mem, tiempo_memoria);
  myChronoSeconds.add(EEPROM.get(write_mem, tiempo_memoria));
  boton_start.begin();
  boton_pause.begin();
  boton_zero.begin();
  myChronoSeconds.stop();
  //millisInicio = millis(); //incicia el contador
}

void loop(void) {
  //millisActual = millis();
  if (boton_start.pressed()) {
    myChronoSeconds.resume();
  }
  if (boton_pause.pressed()) {
    myChronoSeconds.stop();
    EEPROM.put(write_mem, myChronoSeconds.elapsed());
  }
  if (boton_zero.pressed()) {
    EEPROM.write(write_mem, 0);
    myChronoSeconds.restart();
    myChronoSeconds.stop();
    aviso = false;
  }
  u8g2.clearBuffer();					// clear the internal memory
  u8g2.setFont(u8g2_font_ncenB08_tr);	// choose a suitable font
  //u8g2.setCursor(0, 10);
  //u8g2.print(millisActual / 1000);	// write something to the internal memory
  //u8g2.print(myChronoSeconds.elapsed());
  u8g2.drawXBMP(0, 0, 32, 32, mascarilla_bits);
  u8g2.drawFrame(35, 10, 90, 10);
  //longitud = (millis() / 1000) / 5; // dividendo es segundosmaximos/logintudmaximabarra
  longitud = (myChronoSeconds.elapsed() / 320); //cambiar: duracion/longitud(90)
  u8g2.drawBox(36, 11, longitud, 8);
  u8g2.setCursor(3, 60);
  //u8g2.print(longitud);
  if (aviso == true) {
    u8g2.drawXBMP(70, 6, 16, 16, alerta_bits);
  }
  u8g2.setCursor(0, 38);
  u8g2.print("Tiempo usada: " + String(myChronoSeconds.elapsed()/3600) + " h");
  u8g2.setCursor(0, 50);
  int porcentaje = map(longitud, 0, 90, 0 , 100);
  u8g2.print("Usada: " + String((porcentaje)) + " %");
  u8g2.sendBuffer();					// transfer internal memory to the display
  if (myChronoSeconds.hasPassed(duracion)) {
    //myChronoSeconds.restart();
    myChronoSeconds.stop();
    aviso = true;
  }
}
