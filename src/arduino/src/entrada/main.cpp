#include <Arduino.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>

#include "wifi.h"
#include "sonido.h"
#include "sonidos.h"
#include "temp.h"
#include "config.h"

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

#define LCD_P(arriba, abajo) \
    do { \
        LCD.clear(); \
        LCD.setCursor(0, 0); \
        LCD.print(arriba); \
        LCD.setCursor(0, 1); \
        LCD.print(abajo); \
    } while (0)

// Buzzer obvio
#define BUZZER_PIN 9

// Sensor de distancia
#define IRSENSOR_PIN 13

#define MEASURE_SAMPLE_MS 1500
#define MEASURE_TIMEOUT 4500

WifiModule wifi;
SoftwareSerial wifiSerial(WIFI_PIN_RX, WIFI_PIN_TX);

LiquidCrystal LCD(LCD_PIN_RS, LCD_PIN_EN, LCD_PIN_D4, LCD_PIN_D5, LCD_PIN_D6, LCD_PIN_D7);

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Tocador tocador;

int leerSensorDist();
void initSensorDist();

double waitToRead();

void setup()
{
    Serial.begin(9600);
#if DEBUG
    while (!Serial)
        ;
#endif

    DEBUG_SERIAL.println("Calling setup()");

    LCD.begin(16, 2);

    LCD_P("SIMAF v1.0", "Comenzando...");

    mlx.begin();

    tocador_init(&tocador, BUZZER_PIN);

    initSensorDist();

    wifi_module_init(&wifi, &wifiSerial, WIFI_SSID, WIFI_PASS);
    delay(10000);
    wifi_module_send_http_req(&wifi, "/?action=nuevo", SERVER_LOCAL_IP, SERVER_LOCAL_PUERTO);

    tocador_tocar(&tocador, &inicio);
}

void loop()
{
    char temp_str[10];

    LCD_P("Por favor", "Acercar su mano");
    double temp = waitToRead();

    uint8_t intento = 1;
    while (!temp)
    {
        if (intento > 2)
        {
            return;
        }

        tocador_tocar(&tocador, &error);
        LCD_P("Intente", "De nuevo");
        temp = waitToRead();
        ++intento;
    }

    if (TEMP_NORMAL(temp))
    {
        dtostrf(temp, 4, 2, temp_str);
        sprintf(temp_str, "%s %cC", temp_str, 223);

        LCD_P("SU TEMPERATURA:", temp_str);
        tocador_tocar(&tocador, &temp_normal);

        wifi_module_send_http_req(&wifi, "/?action=sumar", SERVER_LOCAL_IP, SERVER_LOCAL_PUERTO);
        //delay(3000);
    }
    else
    {
        // FIEBRE AAAA
        LCD_P("TEMPERATURA ALTA", "NO INGRESAR");
        tocador_tocar(&tocador, &fiebre);

        char ruta[30];
        dtostrf(temp, 4, 2, temp_str);
        sprintf(ruta, "/index.php?a=fiebre&t=%s", temp_str);
        wifi_module_send_http_req(&wifi, ruta, SERVER_LOCAL_IP, SERVER_LOCAL_PUERTO);
    }
}


int leerSensorDist()
{
    return digitalRead(IRSENSOR_PIN);
}

void initSensorDist()
{
    pinMode(IRSENSOR_PIN, INPUT);
}

double waitToRead()
{
    int ans = leerSensorDist();
    while (ans)
    {
        ans = leerSensorDist();
    }
    return readStableTemp(MEASURE_SAMPLE_MS, MEASURE_TIMEOUT);
}
