#include <Arduino.h>
#include "wifi.h"

void wifi_module_init(WifiModule *wifi, SoftwareSerial *ss)
{
    Serial.println("Calling wifi_module_init()");

    ss->begin(WIFI_BAUDRATE);
    wifi->serial = ss;

    wifi_module_send_command(wifi, "AT", "OK");
    wifi_module_send_command(wifi, "AT+GMR", "OK");
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
