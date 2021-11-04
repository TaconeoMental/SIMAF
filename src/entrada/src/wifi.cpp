#include <Arduino.h>
#include "wifi.h"
#include "config.h"

void wifi_module_init(WifiModule *wifi, SoftwareSerial *ss)
{
    Serial.println("Calling wifi_module_init()");

    ss->begin(WIFI_BAUDRATE);
    wifi->serial = ss;

    wifi_module_send_command(wifi, "AT", "OK");
    wifi_module_send_command(wifi, "AT+CWQAP", "OK");
    wifi_module_send_command(wifi, "AT+CWMODE=1", "OK");
    //wifi_module_send_command(wifi, "AT+CIPMUX=1", "OK");

    char comando[50];
    sprintf(comando, "AT+CWJAP=\"%s\",\"%s\"", WIFI_SSID, WIFI_PASS);
    wifi_module_send_command(wifi, comando, "WIFI CONNECTED");
}

bool wifi_module_send_command(WifiModule *wifi, char *cmd, char *expect)
{
    wifi->serial->println(cmd);
    Serial.print("Enviando: ");
    Serial.println(cmd);

    uint8_t curr_index = 0;
    uint8_t exp_length = strlen(expect);
    uint32_t deadline = millis() + TIMEOUT;

    uint8_t line_index = 0;
    char full_line[MAX_RESPONSE_LENGTH] = {0};

    char curr_c;
    char peek_c;

    bool start = true;

    Serial.print("Respuesta: ");
    // Esta cosa es como un mini parser para analizar las respuestas a los
    // comandos AT. Ya que la respuesta siempre está en la última línea, es
    // esta la que queremos guardar.
    while (millis() < deadline)
    {
        if (wifi->serial->available())
        {
            if (start)
            {
                curr_c = wifi->serial->read();
                peek_c = wifi->serial->read();
                start = false;
            }
            else
            {
                curr_c = peek_c;
                peek_c = wifi->serial->read();
            }

            if (curr_c == '\n' && peek_c != '\n')
            {
                line_index = 0;
                full_line[line_index] = '\0';
                continue;
            }

            full_line[line_index++] = curr_c;
            //Serial.write(c);
        }
    }
    full_line[--line_index] = '\0';
    Serial.println(full_line);

    return strcmp(full_line, expect) == 0;
}

void wifi_module_send_http_req(WifiModule *wifi, char *ruta)
{
    char comando[50];
    sprintf(comando, "AT+CIPSTART=\"TCP\",\"%s\",%s", SERVER_LOCAL_IP, SERVER_LOCAL_PUERTO);
    wifi_module_send_command(wifi, comando, "OK");

    char contenido[50];
    sprintf(contenido, "GET %s HTTP/1.1\r\n", ruta);

    Serial.println(contenido);
    //String com = String("AT+CIPSEND=") + String(content.length() + 4);
    comando[0] = '\0';
    sprintf(comando, "AT+CIPSEND=%d", strlen(contenido) + 4);
    //delay(5000);
    wifi_module_send_command(wifi, comando, "OK");
    Serial.println(comando);
    wifi->serial->println(contenido);

    wifi_module_send_command(wifi, "AT+CIPCLOSE", "OK");
}
