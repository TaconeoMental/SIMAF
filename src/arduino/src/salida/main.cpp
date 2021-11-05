#include <Arduino.h>
#include <SoftwareSerial.h>

#include "wifi.h"
#include "config.h"
//#include "sonido.h"
//#include "sonidos.h"

// Módulo Wifi
#define WIFI_PIN_RX 6
#define WIFI_PIN_TX 7

// Buzzer obvio
//#define BUZZER_PIN 9

// Sensores de distancia
#define IRSENSOR1_PIN 12
#define IRSENSOR2_PIN 13

WifiModule wifi;
SoftwareSerial wifiSerial(WIFI_PIN_RX, WIFI_PIN_TX);

//Tocador tocador;

int leerSensorDist();
void initSensorDist();

void setup()
{
    Serial.begin(9600);
#if DEBUG
    while (!Serial)
        ;
#endif

    DEBUG_SERIAL.println("Calling setup()");

    //tocador_init(&tocador, BUZZER_PIN);

    initSensorDist();

    wifi_module_init(&wifi, &wifiSerial, WIFI_SSID, WIFI_PASS);
    delay(10000);
    wifi_module_send_http_req(&wifi, "/?action=start_s", SERVER_LOCAL_IP, SERVER_LOCAL_PUERTO);

    //tocador_tocar(&tocador, &inicio);
}

void loop()
{
    if (leerSensorDist() == 0) {
        wifi_module_send_http_req(&wifi, "/?action=restar", SERVER_LOCAL_IP, SERVER_LOCAL_PUERTO);
    }
}


int leerSensorDist()
{
    return digitalRead(IRSENSOR1_PIN) || digitalRead(IRSENSOR2_PIN);
}

void initSensorDist()
{
    pinMode(IRSENSOR1_PIN, INPUT);
    pinMode(IRSENSOR2_PIN, INPUT);
}
