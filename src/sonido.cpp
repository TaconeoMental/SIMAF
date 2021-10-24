#include "sonido.h"
#include <Arduino.h>

void tocador_init(Tocador *tocador, uint8_t buzzzer_pin)
{
    tocador->pin_buzzer = buzzzer_pin;
    pinMode(buzzzer_pin, OUTPUT);
}

void tocador_tocar(Tocador *tocador, Cancion *cancion)
{
    // Iteramos por todas las notas y las tocamos por su duración
    // correspondiente.
    for (uint8_t esta_nota = 0; esta_nota < cancion->cant_notas; esta_nota++)
    {
        tone(tocador->pin_buzzer, cancion->nd[esta_nota].nota);

        // 30% da más o menos un tiempo suficiente para distinguir notas individuales.
        unsigned long pausa_entre_notas = cancion->nd[esta_nota].duracion * 1.30;
        delay(pausa_entre_notas);
        noTone(tocador->pin_buzzer);
    }
}
