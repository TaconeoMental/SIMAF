#ifndef DEBUG_SERIAL_H
#define DEBUG_SERIAL_H

struct DebugSerial
{
    bool debug;
    int (*read)(void);
    size_t (*write)(uint8_t);
    size_t (*print)(char *);
};

#endif

