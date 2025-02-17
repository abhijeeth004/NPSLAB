#ifndef CRC_CALCULATE_H
#define CRC_CALCULATE_H

unsigned short crc12(unsigned char *data, int length);
unsigned short crc16(unsigned char *data, int length);
unsigned short crc_ccitt(unsigned char *data, int length);

#endif // CRC_CALCULATE_H
