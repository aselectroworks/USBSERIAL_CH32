#ifndef __USBSERIAL_H
#define __USBSERIAL_H

#include <stddef.h>
#include <stdint.h>
#include "Stream.h"

class USBSerial : public Stream {
   public:
    USBSerial(void);

    void begin();
    void end();

    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);

    virtual size_t write(uint8_t);
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    size_t write(const uint8_t *buffer, size_t size);
    using Print::write;  // pull in write(str) from Print
};
extern USBSerial SerialUSB;

#endif
