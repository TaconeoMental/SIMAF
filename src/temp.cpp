#include "temp.h"

#include <Arduino.h>
#include <Adafruit_MLX90614.h>

#define CLOSE(x, y) (abs((x) - (y)) <= TEMP_ERROR_MARGIN)

extern Adafruit_MLX90614 mlx;

double readStableTemp(uint16_t ms)
{
    // TODO: Añadir un timeout?
    double temp;
    double last_temp;

    unsigned long start_time;

    bool start_measure = true;

    while (1)
    {
        if (start_measure)
        {
            start_time = millis();
            start_measure = false;
        }

        if (millis() - start_time >= ms)
        {
            if (last_temp < TEMP_N_MIN)
            {
                start_measure = true;
                continue;
            }
            break;
        }

        last_temp = temp;
        temp = mlx.readObjectTempC();
        Serial.println(temp);

        if (!CLOSE(temp, last_temp))
        {
            start_measure = true;
            continue;
        }
    }

    return last_temp;
}
