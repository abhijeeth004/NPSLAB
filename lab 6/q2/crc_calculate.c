#include "crc_calculate.h"

#define POLY_CRC_12 0x80F  // CRC-12 polynomial: x^12 + x^11 + x^10 + x^9 + x^7 + x^6 + x^4 + x^3 + x^2 + x + 1
#define POLY_CRC_16 0x8005  // CRC-16 polynomial: x^16 + x^15 + x^2 + 1
#define POLY_CRC_CCITT 0x11021 // CRC-CCITT polynomial: x^16 + x^12 + x^5 + 1

// Function to calculate CRC-12
unsigned short crc12(unsigned char *data, int length) {
    unsigned short crc = 0xFFF;  // Initial value for CRC-12
    for (int i = 0; i < length; i++) {
        crc ^= data[i] << 4;
        for (int j = 0; j < 8; j++) {
            if (crc & 0x800) crc = (crc << 1) ^ POLY_CRC_12;
            else crc <<= 1;
        }
    }
    return crc & 0xFFF;  // Return CRC-12 value (12 bits)
}

// Function to calculate CRC-16
unsigned short crc16(unsigned char *data, int length) {
    unsigned short crc = 0xFFFF;  // Initial value for CRC-16
    for (int i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x0001) crc = (crc >> 1) ^ POLY_CRC_16;
            else crc >>= 1;
        }
    }
    return crc;
}

// Function to calculate CRC-CCITT
unsigned short crc_ccitt(unsigned char *data, int length) {
    unsigned short crc = 0xFFFF;  // Initial value for CRC-CCITT
    for (int i = 0; i < length; i++) {
        crc ^= (data[i] << 8);
        for (int j = 0; j < 8; j++) {
            if (crc & 0x8000) crc = (crc << 1) ^ POLY_CRC_CCITT;
            else crc <<= 1;
        }
    }
    return crc;
}
