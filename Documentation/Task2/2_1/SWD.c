#include "SWD.h"



/*
    Initialize device by sending 50 HIGH ticks

    swdck  - SWD clock Pin GPIO number
    swdio  - SWD data Pin GPIO number
*/
void initDevice(unsigned int swdck, unsigned int swdio)
{
    writeBytes(swdck, swdio, &swd_sync_seq, swd_sync_seq_length);
}


/*
    method to make clock tick

    swdck  - SWD clock Pin GPIO number
*/
int clk(unsigned char swdck)
{
    uint32_t status = 0;
    status |= gpioWrite(swdck, 1);
    
    gpioDelay(100);
    status |= gpioWrite(swdck, 0);
    gpioDelay(100);
    if(DEBUG_DETAIL)
        printf("[Debug] CLK\n");
    if(status)
    {
        printf("[Error] Error occured in clock function\n");
        exit(1);
    }
    return status;
}

/*
    write a single bit over SWD

    swdck - SWD clock Pin GPIO number
    swdio - SWD data Pin GPIO number
    bit   - bit to be written over SWD
*/
void writeBit(unsigned int swdck, unsigned int swdio, unsigned char bit)
{
    unsigned int status = 0;
    bit = bit & 1;
    status |= gpioWrite(swdio, bit);

    if(status){
        printf("[Error] Error setting SWDIO pin in function writeBit\n");
        exit(1);
    }
        
    clk(swdck);

    if(DEBUG_DETAIL)
        printf("[Debug] writing bit %d\n", bit);

}


/*
    reads a single bit over SWD

    swdck  - SWD clock Pin GPIO number
    swdio  - SWD data Pin GPIO number
*/
int readBit(unsigned int swdck, unsigned int swdio)
{

    if(DEBUG_DETAIL)
        printf("[Debug] Reading Bit %d\n", read);

    // read before clock to ensure data is correct
    int read = gpioRead(swdio);
    clk(swdck);

    if(read == PI_BAD_GPIO)
    {
        printf("[Error] Read PI_BAD_GPIO\n");
        exit(1);
    }
    return read;
}

/*
    Writes first 'length' bits of 'data' to the SWD port. MSB is sent first.

    swdck  - SWD clock Pin GPIO number
    swdio  - SWD data Pin GPIO number
    data   - data to be written over SWD
    length - length of data to be written over SWD
*/
void writeBits(unsigned int swdck, unsigned int swdio, unsigned char *data, unsigned int length)
{
    unsigned int status = 0;
    unsigned int num_data_chunks = length / 8 + (length % 8 ? 1 : 0);                       // round up size of data in bytes
    if(DEBUG)                                                                               // DEBUG print function
    {
        printf("[Debug] writing %d bits, data: ", length);
        for(int chunk_num = 0; chunk_num < num_data_chunks; chunk_num++)
        {
            char current_chunk = data[chunk_num];
            char chunk_size = 8 > length - chunk_num * 8 ? length - chunk_num * 8 : 8;
            for(int bit_offset = 0; bit_offset < chunk_size; bit_offset++)
            {
                unsigned char current_bit = (current_chunk >> (7 - bit_offset)) & 1;
                printf("%d", current_bit);
            }
        }
        printf("\n");
    }


    for(int chunk_num = 0; chunk_num < num_data_chunks; chunk_num++)                        // divide data in bytes
    {
        char current_chunk = data[chunk_num];
        char chunk_size = 8 > length - chunk_num * 8 ? length - chunk_num * 8 : 8;          // bits left in the chunk
        for(int bit_offset = 0; bit_offset < chunk_size; bit_offset++)
        {
            unsigned char current_bit = (current_chunk >> (7 - bit_offset)) & 1;
            writeBit(swdck, swdio, current_bit);
        }
    }
    if(status)
    {
        printf("[Error] Writing operation failed\n");
        exit(1);
    }
}


/*
    Read 'count' bits off of SWD port and writes them into *result.

    swdck  - SWD clock Pin GPIO number
    swdio  - SWD data Pin GPIO number
    result - buffer in which read data will be written to (no count check)
    count  - number of bits to be read
*/
void readBits(int swdck, int swdio, unsigned char* result,  unsigned int count)
{
    gpioSetMode(swdio, PI_INPUT);
    if(count >= 8)                                                                      // if read data is more then a byte, data will be padded with zeroes at the end (ex. 01111010 111 will be read as 01111010 11100000)
    {
        unsigned int status = 0;
        unsigned int num_data_chunks = count / 8 + (count % 8 ? 1 : 0);


        for(int chunk_num = 0; chunk_num < num_data_chunks; chunk_num++)
        {
            char chunk_size = 8 > count - chunk_num * 8 ? count - chunk_num * 8 : 8;
            result[chunk_num] = 0;
            for(int bit_offset = 0; bit_offset < chunk_size; bit_offset++)
            {
                int current_bit = readBit(swdck, swdio);
                if(current_bit < 0)
                {
                    printf("[Error] Reading operation failed\n");
                    exit(0);
                }
                result[chunk_num] |= current_bit << (7 - bit_offset);
            }
        }
    } 
    else                                                                                // else read data unpadded (101 will be read as 00000101 instead of 10100000)
    {
        *result = 0;
        for(int i = 0; i < count; i++)
        {
            int current_bit = readBit(swdck, swdio);
            if(current_bit < 0)
            {
                printf("[Error] Reading operation failed\n");
                exit(0);
            }
            *result |= current_bit << (count - i - 1);
        }
    }
    gpioSetMode(swdio, PI_OUTPUT);
    gpioWrite(swdio, 0);
}

/*
    write 'length' bytes over SWD

    swdck  - SWD clock Pin GPIO number
    swdio  - SWD data Pin GPIO number
    *data  - data to be written over SWD
    length - length of data to be written
*/
void writeBytes(unsigned int swdck, unsigned int swdio, unsigned char *data, unsigned int length)
{
    for(int i = 0; i < length; i++)
    {
        writeBits(swdck, swdio, &(data[i]), 8);
    }
}



/*
    read 'length' bytes over SWD and write them to *result

    swdck   - SWD clock Pin GPIO number
    swdio   - SWD data Pin GPIO number
    *result - buffer in which data will be read to
    length  - length of data to be read
*/
void readBytes(unsigned int swdck, unsigned int swdio, unsigned char *result, unsigned int length)
{
    for(int i = 0; i < length; i++)
    {
        readBits(swdck, swdio, &(result[i]), 8);
    }
}


/*
    Make clock tick without reading or writing

    swdck - SWD clock Pin GPIO number
    swdio - SWD data Pin GPIO number
    count - amount of clock cycles to be skipped
*/
void skipBits(unsigned int swdck, unsigned int swdio, int count)
{
    for(int i = 0; i < count; i++)
        readBit(swdck, swdio);
}

/*
    calculate the parity of data byte

    data - byte which the parity should be calculated for
*/
unsigned char parity(unsigned char data)
{
    unsigned char result = 0;
    for(int i = 0; i < 8; i++)
    {
        result += (data >> i) & 1;
    }
    return result & 1;
}


/*
    reverse a byte

    data - byte to be reversed
*/
void reverseByte(unsigned char* data)
{
    data[0] = ((data[0] & 0xAA) >> 1) | ((data[0] & 0x55) << 1);
    data[0] = ((data[0] & 0xCC) >> 2) | ((data[0] & 0x33) << 2);
    data[0] = ((data[0] & 0xF0) >> 4) | ((data[0] & 0x0F) << 4);
}


/*
    generate a operation code

    ap   - ap mode
    read - read operation
    reg  - target register
*/
unsigned char operation(unsigned char ap, unsigned char read, unsigned char reg)
{
    unsigned char opcode = 0;
    if(ap) opcode |= 0x40;                          // ap set bit
    if(read) opcode |= 0x20;                        // read bit
    opcode |= ((reg & 1) << 4) | ((reg & 2) << 2);  // target register address
    opcode |= parity(opcode) << 2;                  // parity bit of opcode
    opcode |= 0x81;                                 // start, park and stop bit
    return opcode;
}


/*
    make a SWD read operation

    swdck - SWD clock Pin GPIO number
    swdio - SWD data Pin GPIO number
    ap    - ap mode
    reg   - target register to be read from
*/
unsigned int readSWD(unsigned int swdck, unsigned int swdio, unsigned char ap, unsigned char reg)
{
    readSWDStart:                                                           // label for retry in case of ACK WAIT
    {

    }
    unsigned char opcode = operation(ap, 1, reg);
    if(DEBUG)
        printf("[Debug] Reading SWD reg %d, opcode %d\n", reg, opcode);
    writeBytes(swdck, swdio, &opcode, 1);
    readBit(swdck, swdio);                                                          // turnaround phase
    char ack = 0;
    readBits(swdck, swdio, &ack, 3);                                                // read ACK
    if(DEBUG)
        printf("[ACK] %d\n", ack);
    if(ack != 4)                                                                    // ACK OK was not returned
    {
        if(ack == 2)                                                                // ACK WAIT received
        {
            printf("[Message] ACK WAIT\n");
            goto readSWDStart;                                                      // retry 
        }
        else if(ack == 1)                                                           // ACK Fault received
        {
            printf("[Error] ACK Fault\n");
            exit(1);
        }
    }
    char rawdata[4] = {0, 0, 0, 0};
    
    readBytes(swdck, swdio, &rawdata, 4);                                           // read 4 bytes of data

    unsigned char data[4];                                                  
    for(int i = 0; i < 4; i++)                                                      // data postprocessing(endianess, byte reversing)
    {   
        reverseByte(&(rawdata[i]));
        data[3 - i] = rawdata[i];
    }

    unsigned char par = 0;
    readBits(swdck, swdio, &par, 1);                                                // receive parity
    skipBits(swdck, swdio, 2);
    unsigned char sum = 0;
    for(int i = 0; i < 4; i++)                                                      // calculate parity for received data
    {
        sum += parity(data[i]);
    }

    unsigned int result = data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];   // convert data to int
    if(DEBUG)
        printf("[Debug] Read 0x%08x\n", result);
    if((sum & 1) != par)
    {
        printf("[Error] Parity Check failed, expected %d but calculated %d\n", par, sum);
        exit(1);
    }
    
    gpioSetMode(swdio, PI_OUTPUT);
    return result;
}


/*
    make a SWD write operation

    swdck - SWD clock Pin GPIO number
    swdio - SWD data Pin GPIO number
    ap    - ap mode
    data  - 4 byte data
    reg   - target register to be read from
*/
void writeSWD(unsigned int swdck, unsigned int swdio, unsigned char ap, unsigned int data, unsigned int reg)
{
    writeSWDStart:
    {

    }
    unsigned char opcode = operation(ap, 0, reg);
    if(DEBUG)
        printf("[Debug] Writing SWD reg %d, opcode %d\n", reg, opcode);
    char ack = 0;
    writeBytes(swdck, swdio, &opcode, 1);
    gpioSetMode(swdio, PI_INPUT);
    readBit(swdck, swdio);                                                  // turnaround phase
    readBits(swdck, swdio, &ack, 3);
    if(DEBUG)
        printf("[ACK] %d\n", ack);
    if(ack != 4)                                                            // refer to SWD read method
    {
        if(ack == 2)
        {
            printf("[Message] ACK WAIT\n");
            goto writeSWDStart;
        }
        else if(ack == 1)
        {
            printf("[Error] ACK Fault\n");
            exit(1);
        }
    }
    skipBits(swdck, swdio, 1);                                              // turnaround phase
    char dataShift[4] = {0, 8, 16, 24};
    
    unsigned char par = 0;
    for(int i = 0; i < 4; i++)
    {
        char transferByte = (char)(data >> dataShift[i]);
        writeBytes(swdck, swdio, &transferByte, 1);
        par ^= parity((char)(data >> dataShift[i]));
    }
    
    writeBit(swdck, swdio, par);                                            // parity
    writeBit(swdck, swdio, 0);
    writeBit(swdck, swdio, 0);
    unsigned char sum = 0;

    if(DEBUG)
        printf("[Debug] Wrote 0x%08x\n", data);
}


/*
    finishes last transaction, necessary to reset DAP to known state

    swdck - SWD clock Pin GPIO number
    swdio - SWD data Pin GPIO number
*/
void finishTransaction(unsigned int swdck, unsigned int swdio)
{
    unsigned char finishT[] = {0x00, 0x00};
    writeBytes(swdck, swdio, &finishT, 2);
}