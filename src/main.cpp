#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#include "wifi.h"
#include "sonido.h"
#include "sonidos.h"
#include "temp.h"

#define DEBUG true

// Módulo Wifi
#define WIFI_PIN_RX 6
#define WIFI_PIN_TX 7

// Pantalla LCD
#define LCD_PIN_RS 12
#define LCD_PIN_EN 11
#define LCD_PIN_D4 5
#define LCD_PIN_D5 4
#define LCD_PIN_D6 3
#define LCD_PIN_D7 2

// Buzzer obvio
#define BUZZER_PIN 9

// Sensor de distancia
#define IRSENSOR_PIN 13

int leerSensorDist()
{
    return digitalRead(IRSENSOR_PIN);
}

void initSensorDist()
{
    pinMode(IRSENSOR_PIN, INPUT);
}

/*
DebugSerial dserial;
debug_serial_init(&dserial, DEBUG, 9600);
*/

WifiModule wifi;
SoftwareSerial wifiSerial(WIFI_PIN_RX, WIFI_PIN_TX);

LiquidCrystal LCD(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Tocador tocador;

void setup()
{
    Serial.begin(9600);
    while (!Serial)
        ;

    Serial.println("Calling setup()");

    LCD.begin(16, 2);
    LCD.print("SIMAF v1.0");

    LCD.setCursor(0, 1);
    LCD.print("Comenzando...");

    mlx.begin();

    wifi_module_init(&wifi, &wifiSerial);

    tocador_init(&tocador, BUZZER_PIN);

    initSensorDist();

    tocador_tocar(&tocador, &inicio);
}

void loop()
{
    LCD.setCursor(0, 0);
    LCD.print("Por favor");
    LCD.setCursor(0, 1);
    LCD.print("acercar mano");

    if (leerSensorDist() == 0)
    {
        // Se detectó un objeto

        // Leemos la temperatura del objeto
        double temp = readStableTemp(1500);
        if (TEMP_NORMAL(temp))
        {
            tocador_tocar(&tocador, &temp_normal);
        }
    }

    /*
    LCD.setCursor(0, 0);
    LCD.print("A: ");
    LCD.print(mlx.readAmbientTempC());
    LCD.setCursor(0, 1);
    LCD.print("O: ");
    LCD.print(readStableTemp(1500);
    Serial.println(leerSensorDist());
    */
}
