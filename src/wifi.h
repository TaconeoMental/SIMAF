#ifndef WIFI_H
#define WIFI_H

#include <SoftwareSerial.h>

#define WIFI_BAUDRATE 9600
#define TIMEOUT 5000 //ms
#define MAX_RESPONSE_LENGTH 500

struct WifiModule
{
    SoftwareSerial *serial;
};
void wifi_module_init(WifiModule *wifi, SoftwareSerial *ss);
bool wifi_module_send_command(WifiModule *wifi, char *cmd, char *expect);

#endif

