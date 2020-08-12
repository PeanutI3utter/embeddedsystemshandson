#ifndef SWD_HEADER
#define SWD_HEADER

#include <pigpio.h>
#include <stdio.h>
#include <stdlib.h>

/*
  ARM SWD library
  
  DEBUG and DEBUG_DETAIL needs to be set if this library is used
  DEBUG(0 = off):        prints debug messages
  DEBUG_DETAIL(0 = off): prints very detailed debug messages
*/

extern unsigned int DEBUG; 
extern unsigned int DEBUG_DETAIL;

static unsigned char swd_sync_seq[] = {
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00
};

static unsigned int swd_sync_seq_length = 8;



void initDevice(unsigned int swdck, unsigned int swdio);

int clk(unsigned char swdck);

void writeBit(unsigned int swdck, unsigned int swdio, unsigned char bit);

void writeBits(unsigned int swdck, unsigned int swdio, unsigned char *data, unsigned int length);

void writeBytes(unsigned int swdck, unsigned int swdio, unsigned char *data, unsigned int length);

int readBit(unsigned int swdck, unsigned int swdio);

void readBits(int swdck, int swdio, unsigned char* result, unsigned int count);

void readBytes(unsigned int swdck, unsigned int swdio, unsigned char *result, unsigned int length);

void skipBits(unsigned int swdck, unsigned int swdio, int count);

unsigned char parity(unsigned char data);

void reverseByte(unsigned char* data);

unsigned char operation(unsigned char ap, unsigned char read, unsigned char register);

unsigned int readSWD(unsigned int swdck, unsigned int swdio, unsigned char ap, unsigned char register);

void writeSWD(unsigned int swdck, unsigned int swdio, unsigned char ap, unsigned int data, unsigned int reg);

void finishTransaction(unsigned int swdck, unsigned int swdio);
#endif