/**
 *  @file      BinParser.c
 *  @brief     Binary Parser
 *  @author    fire-peregrine
 *  @date      2020/11/07
 *  @copyright Copyright (C) fire-peregrine all rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BinParser.h"
#include "BinParser_local.h"

/* Signatures */
static int getBool(BinParser *obj, bool *val);
static int getUInt32(BinParser *obj, unsigned int bits, uint32_t *val);
static int getUInt64(BinParser *obj, unsigned int bits, uint64_t *val);
static int getInt32(BinParser *obj, unsigned int bits, int32_t *val);
static int getInt64(BinParser *obj, unsigned int bits, int64_t *val);
static int getGlm(BinParser *obj, uint32_t *val);
static int getSGlm(BinParser *obj, int32_t *val);
static bool isByteAligned(BinParser *obj);
static int alignByte(BinParser *obj);
static int alignBytes(BinParser *obj, unsigned int bytes);
static bool isAligned(BinParser *obj, unsigned int bytes);
static int seek_(BinParser *obj, unsigned int posByte, unsigned int posBit);
static bool hasRest(uint64_t posByte, unsigned int posBit, unsigned int bytes, unsigned int bits, uint64_t bufLen);
static bool isInBuf(uint64_t posByte, uint64_t bufLen);
static int32_t castInt32(uint32_t val, unsigned int width);
static int64_t castInt64(uint64_t val, unsigned int width);


/* Functions */
/**
 *  @brief Create a new BinParser object.
 *  @param [in] buf    buffer
 *  @param [in] bufLen buffer length
 *  @return A new BinParser object.
 */
BinParser* BinParser_new(uint8_t *buf, size_t bufLen)
{
    /* Create a BinParser object */
    BinParser *obj = (BinParser *) malloc(sizeof(BinParser));
    if(obj == NULL)
        goto error;
   
    /* Populate the created object. */
    memset(obj, 0x00, sizeof(BinParser));
    obj->buf    = buf;
    obj->bufLen = bufLen;

    return obj;

error: /* error handling */

    if(obj != NULL)
        free(obj);

    return NULL;
}


/**
 *  @brief Reset buffer and its length again, without deleting the object.
 *  @param [in] obj    BinParser object
 *  @param [in] buf    buffer
 *  @param [in] bufLen buffer length
 *  @return Execution status.
 */
int BinParser_reset(BinParser *obj, uint8_t *buf, unsigned int bufLen)
{
    /* Reset the buffer and its length .*/
    obj->buf = buf;
    obj->bufLen = bufLen;
    /* Reset the buffer position. */
    obj->posByte = 0; 
    obj->posBit = 0; 

    return 0;
}


/**
 *  @brief Delete a BinParser object.
 *  @param [in] obj BinParser object
 *  @return Execution status
 */
int BinParser_delete(BinParser *obj)
{
    free(obj);
    return 0;
}


/**
 *  @brief Get next 1 bit as a boolean value.
 *  @param [in]  obj BinParser object
 *  @param [out] val Boolean value
 *  @return Execution status.
 */
int BinParser_getBool(BinParser *obj, bool *val)
{
    return getBool(obj, val);
}


/**
 *  @brief Get next specified-bytes/bits as a uint32_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val uint32_t type value.
 *  @return Execution status.
 */
int BinParser_getUInt32(BinParser *obj, unsigned int bits, uint32_t *val)
{
    return getUInt32(obj, bits, val);
}


/**
 *  @brief Get next specified-bytes/bits as a uint64_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val uint64_t type value.
 *  @return Execution status.
 */
int BinParser_getUInt64(BinParser *obj, unsigned int bits, uint64_t *val)
{
    return getUInt64(obj, bits, val);
}


/**
 *  @brief Get next specified-bytes/bits as a int32_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status.
 */
int BinParser_getInt32(BinParser *obj, unsigned int bits, int32_t *val)
{
    return getInt32(obj, bits, val);
}


/**
 *  @brief Get next specified-bytes/bits as a int64_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status.
 */
int BinParser_getInt64(BinParser *obj, unsigned int bits, int64_t *val)
{
    /* Get specified number of bits */
    return getInt64(obj, bits, val);
}


/**
 *  @brief Get next specified number of byte data.
 *  @param [in]  obj   BinParser object
 *  @param [in]  bytes Number of bytes
 *  @param [out] val   Byte data
 *  @return Execution status.
 */
int BinParser_getBytes(BinParser *obj, unsigned int bytes, uint8_t *val)
{
    if(isByteAligned(obj) != true)
        return 1; 
    
    /* Check the buffer rest. */
    if(hasRest(obj->posByte, obj->posBit, bytes, 0 /* bits */, obj->bufLen) != true)
        return 1;    
  
    unsigned int i, posByte;
    for(i = 0, posByte = obj->posByte; i < bytes; i++, posByte++)
    {
        val[i] = obj->buf[posByte];
    }

    /* Update the current position. */
    BIN_PARSER_ADVANCE_POS(bytes  /* bytes */, 0 /* bits */);
    return 0;
}


/**
 *  @brief Get next unsigned-golomb value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status
 */
int BinParser_getGlm(BinParser *obj, uint32_t *val)
{
    return getGlm(obj, val);
}


/**
 *  @brief Get next signed-golomb value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status
 */
int BinParser_getSGlm(BinParser *obj, int32_t *val)
{
    return getSGlm(obj, val);
}


/**
 *  @brief Align the current position to byte boundary.
 *         If the current position is already byte-aligned, the position does not change.
 *  @param [in]  obj BinParser object
 *  @return Execution status.
 */
int BinParser_alignByte(BinParser *obj)
{
    return alignByte(obj);
}


/**
 *  @brief Align the current position to specified byte boundary.
 *         If the current position is already aligned the voundary, the position does not change.
 *  @param [in] obj   BinParser object
 *  @param [in] bytes byte-boundary 
 *  @return Execution status.
 */
int BinParser_alignBytes(BinParser *obj, unsigned int bytes)
{
    return alignBytes(obj, bytes);
}


/**
 *  @brief Check if the current position is byte aligned.
 *  @param [in]  obj BinParser object
 *  @retval true  The current position is byte-aligned.
 *  @retval false The current position is not byte-aligned.
 */
bool BinParser_isByteAligned(BinParser *obj)
{
    return isByteAligned(obj);
}


/**
 *  @brief Check if the current position is aligned to the specified bytes boundary.
 *  @param [in] obj   BinParser object
 *  @param [in] bytes Bytes.
 *  @retval true  The current position is aligned to the specified bytes boundary.
 *  @retval false The current position is not aligned to the specified bytes boundary.
 */
bool BinParser_isAligned(BinParser *obj, unsigned int bytes)
{
    return isAligned(obj, bytes);
}


/**
 *  @brief Seek to specified postion.
 *  @param [in] obj BinParser object
 *  @param [in] posByte Seek position (byte)
 *  @param [in] posBit  Seek position (bit)
 *  @return Execution status
 */
int BinParser_seek(BinParser *obj, unsigned int posByte, unsigned int posBit)
{
    return seek_(obj, posByte, posBit);
}


/**
 *  @brief Seek to specified postion.
 *  @param [in] obj BinParser object
 *  @param [in] posByte Bytes to skip.
 *  @param [in] posBit  Bits to skip.
 *  @return Execution status
 */
int BinParser_skip(BinParser *obj, unsigned int bytes, unsigned int bits)
{
    unsigned int nextPosByte = obj->posByte + bytes + ((obj->posBit + bits) >> 3);
    unsigned int nextPosBit = (obj->posBit + bits) & 0x7;

    return seek_(obj, nextPosByte, nextPosBit);
}


/**
 *  @brief Get the current buffer poistion.
 *  @param [in] obj BinParser object
 *  @param [out] posByte The current byte position.
 *  @param [out] posBit  The current bit position.
 *  @return Execution status
 */
int BinParser_getPos(BinParser *obj, unsigned int *posByte, unsigned int *posBit)
{
    *posByte = obj->posByte;
    *posBit  = obj->posBit;
    return 0;
}


/**
 *  @brief Check rest of the buffer.
 *         (Check if the specified size can be read from the buffer.)
 *  @param [in] obj BinParser object
 *  @param [in]  bytes   Bytes
 *  @param [in]  bits    Bits
 *  @retval true  Can be read from the buffer.
 *  @retval false Cannot be read from the buffer.
 */
bool BinParser_hasRest(BinParser *obj, unsigned int bytes, unsigned int bits)
{
    return hasRest(obj->posByte, obj->posBit, bytes, bits, obj->bufLen);
}




/* Static functions */
/**
 *  @brief Dump internal information.
 *  @param [in] obj BinParser object
 */
int BinParser_dump(BinParser *obj)
{
    fprintf(stderr, "***** Syntax Reader Dump *****\n");
    fprintf(stderr, "bufLen  = %" PRIu64 "\n", obj->bufLen );
    fprintf(stderr, "posByte = %" PRIu64 "\n", obj->posByte);
    fprintf(stderr, "posBit  = %u\n"         , obj->posBit );
    fprintf(stderr, "\n\n");
    return 0;
}


/* Static functions */
/**
 *  @brief Get next 1 bit as a boolean value.
 *  @param [in]  obj BinParser object
 *  @param [out] val Boolean value
 *  @return Execution status.
 */
static int getBool(BinParser *obj, bool *val)
{
    /* Check the buffer rest. */
    if(hasRest(obj->posByte, obj->posBit, 0 /* bytes */,  1 /* bits */, obj->bufLen) != true)
        return 1;    

    /* Read from the buffer. */
    *val = (obj->buf[obj->posByte] >> (7 - obj->posBit)) & 0x1;

    /* Update the current position. */
    BIN_PARSER_ADVANCE_POS(0 /* bytes */, 1 /* bits */);

    return 0;
}


/**
 *  @brief Get next specified-bytes/bits as a uint32_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val uint32_t type value.
 *  @return Execution status.
 */
static int getUInt32(BinParser *obj, unsigned int bits, uint32_t *val)
{
    if(bits == 0)
    {
        *val = 0;
        return 0;
    }

    /* Check the buffer rest. */
    if(hasRest(obj->posByte, obj->posBit, 0 /* bytes */,  bits , obj->bufLen) != true)
        return 1;

    unsigned int posByte = obj->posByte;
    unsigned int posBit  = obj->posBit;

    /* Read from the buffer. */
    uint32_t tmpVal = 0;
    unsigned int i;
    for(i = 0; i < bits; i++)
    {
        tmpVal = (tmpVal << 1) | ((obj->buf[posByte] >> (7 - posBit)) & 0x1);
        posByte += (posBit + 1 /* bit */) >> 3;
        posBit   = (posBit + 1 /* bit */) & 0x7;
    }
    *val = tmpVal;

    /* Update the current position. */
    BIN_PARSER_ADVANCE_POS(0 /* bytes */,  bits);

    return 0;
}


/**
 *  @brief Get next specified-bytes/bits as a uint64_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val uint64_t type value.
 *  @return Execution status.
 */
static int getUInt64(BinParser *obj, unsigned int bits, uint64_t *val)
{
    if(bits == 0)
    {
        *val = 0;
        return 0;
    }

    /* Check the buffer rest. */
    if(hasRest(obj->posByte, obj->posBit, 0 /* bytes */,  bits , obj->bufLen) != true)
        return 1;

    unsigned int posByte = obj->posByte;
    unsigned int posBit  = obj->posBit;

    /* Read from the buffer. */
    uint32_t tmpVal = 0;
    unsigned int i;
    for(i = 0; i < bits; i++)
    {
        tmpVal = (tmpVal << 1) | ((obj->buf[posByte] >> (7 - posBit)) & 0x1);
        posByte += (posBit + 1 /* bit */) >> 3;
        posBit   = (posBit + 1 /* bit */) & 0x7;
    }
    *val = tmpVal;

    /* Update the current position. */
    BIN_PARSER_ADVANCE_POS(0 /* bytes */,  bits);
    return 0;
}


/**
 *  @brief Get next specified-bytes/bits as a int32_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status.
 */
static int getInt32(BinParser *obj, unsigned int bits, int32_t *val)
{
    /* Read the specified bit in unsigned form. */
    uint32_t tmpVal;
    int status = getUInt32(obj, bits, &tmpVal);
    if(status != 0)
        return status;

    *val = castInt32(tmpVal, bits); 
    return 0;
}


/**
 *  @brief Get next specified-bytes/bits as a int64_t value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int64_t type value.
 *  @return Execution status.
 */
static int getInt64(BinParser *obj, unsigned int bits, int64_t *val)
{
    /* Read the specified bit in unsigned form. */
    uint64_t tmpVal;
    int status = getUInt64(obj, bits, &tmpVal);
    if(status != 0)
        return status;

    *val = castInt64(tmpVal, bits); 
    return 0;
}


/**
 *  @brief Get next unsigned-golomb value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status
 */
static int getGlm(BinParser *obj, uint32_t *val)
{
    unsigned int posByte = obj->posByte;
    unsigned int posBit  = obj->posBit;
    uint8_t *buf = obj->buf;

    /* Count consecutive '0's (prefix). */
    unsigned int zeros = 0;
    while(posByte < obj->bufLen)
    {
        // Check the buffer position.
        if(posByte >= obj->bufLen)
            return 1;

        unsigned int bit = (buf[posByte] >> (7 - posBit)) & 0x1;
        if(bit == 1)
            break;
       
        zeros++;
        posByte += ((posBit + 1) >> 3);
        posBit = (posBit + 1) & 0x7;
    }

    /* Skip '1'. */
    // Check the buffer position.
    if(posByte >= obj->bufLen)
        return 1;    

    // Read and Skip '1' */
    posByte += ((posBit + 1) >> 3);
    posBit = (posBit + 1) & 0x7;
    
    /* Read suffix. */
    unsigned int i;
    uint32_t tmpVal = 0;
    for(i = 0; (i < zeros) && (posByte < obj->bufLen); i++)
    {
        // Check the buffer position.
        if(posByte >= obj->bufLen)
            return 1;

        unsigned int bit = (buf[posByte] >> (7 - posBit)) & 0x1;
        tmpVal = (tmpVal << 1) | bit;
    
        posByte += ((posBit + 1) >> 3);
        posBit = (posBit + 1) & 0x7;
    }
    tmpVal += ((1 << zeros) - 1);
    *val = tmpVal;

    /* Update the current position. */
    obj->posBit = posBit;
    obj->posByte = posByte;
    return 0;
}


/**
 *  @brief Get next signed-golomb value.
 *  @param [in]  obj BinParser object
 *  @param [out] val int32_t type value.
 *  @return Execution status
 */
static int getSGlm(BinParser *obj, int32_t *val)
{
    uint32_t tmpVal;
    int status = getGlm(obj, &tmpVal);
    if(status != 0)
        return status;

    /* 符号付きGolombへのマッピング */
    if(tmpVal & 0x1)
        *val = (tmpVal >> 1) + 1;
    else
        *val = -(tmpVal >> 1);

    return 0;
}

    

/**
 *  @brief Check if the current position is byte aligned.
 *  @param [in]  obj BinParser object
 *  @retval true  The current position is byte-aligned.
 *  @retval false The current position is not byte-aligned.
 */
static bool isByteAligned(BinParser *obj)
{
    return (obj->posBit == 0);    
}


/**
 *  @brief Align the current position to byte boundary.
 *         If the current position is already byte-aligned, the position does not change.
 *  @param [in]  obj BinParser object
 *  @return Execution status.
 */
static int alignByte(BinParser *obj)
{
    /* Check the current buffer position. */
    if(isInBuf(obj->posByte, obj->bufLen) != true)
        return 0;

    /* Align to next byte boundary. */
    if(obj->posBit != 0)
    {
        obj->posByte++;
        obj->posBit = 0;
    }

    return 0;
}


/**
 *  @brief Align the current position to specified byte boundary.
 *         If the current position is already aligned the voundary, the position does not change.
 *  @param [in] obj   BinParser object
 *  @param [in] bytes byte-boundary 
 *  @return Execution status.
 */
static int alignBytes(BinParser *obj, unsigned int bytes)
{
    if(bytes == 0)
        return 1;

    /* Check the current buffer position. */
    if(isInBuf(obj->posByte, obj->bufLen) != true)
        return 0;

    /* Check if the current position is aligned to specified byte boundary. */
    unsigned int rem = obj->posByte % bytes;
    if((rem == 0) & (obj->posBit == 0))
        return 0;
 
    /* Calculate the boundary. */
    unsigned int alignedByte = obj->posByte - rem + bytes;
    if(isInBuf(alignedByte, obj->bufLen) != true)
        return 1;

    /* Align to the boundary */
    obj->posByte = alignedByte;
    obj->posBit = 0;

    return 0;
}


/**
 *  @brief Check if the current position is aligned to the specified bytes boundary.
 *  @param [in] obj   BinParser object
 *  @param [in] bytes Bytes.
 *  @retval true  The current position is aligned to the specified bytes boundary.
 *  @retval false The current position is not aligned to the specified bytes boundary.
 */
static bool isAligned(BinParser *obj, unsigned int bytes)
{
    unsigned int rem = obj->posByte % bytes;
    return ((rem == 0) & (obj->posBit == 0));
}


/**
 *  @brief Seek to specified postion.
 *  @param [in] obj BinParser object
 *  @param [in] posByte Seek position (byte)
 *  @param [in] posBit  Seek position (bit)
 *  @return Execution status
 */
static int seek_(BinParser *obj, unsigned int posByte, unsigned int posBit)
{
    /* Check seek position. */
    if(isInBuf(posByte, obj->bufLen) != true)
        return 1;

    /* Seek position. */
    obj->posByte = posByte;
    obj->posBit = posBit;

    return 0;
}


/**
 *  @brief Check rest of the buffer.
 *         (Check if the specified size can be read from the buffer.)
 *  @param [in]  posByte Current byte posion
 *  @param [in]  posBit  Current bit posion
 *  @param [in]  bytes   Bytes to read
 *  @param [in]  bits    Bits to read
 *  @param [out] bufLen  Buffer length.
 *  @retval true  Can be read from the buffer.
 *  @retval false Cannot be read from the buffer.
 */
static bool hasRest(uint64_t posByte, unsigned int posBit, unsigned int bytes, unsigned int bits, uint64_t bufLen)
{
    unsigned int nextByte = posByte + bytes + ((posBit + bits) >> 3);
    unsigned int nextBit = (posBit + bits) & 0x7;

    if(nextByte < bufLen)
        return true;
        
    if((nextByte == bufLen) && (nextBit == 0))
        return true;

    return false;
}


/**
 *  @brief Check if the current position is within the buffer.
 *  @param [in]  posByte Current byte position.
 *  @param [in]  posBit  Current bit position.
 *  @retval true  The current position is within the buffer.
 *  @retval false The current position is not within the buffer.
 */
static bool isInBuf(uint64_t posByte, uint64_t bufLen)
{
    if(posByte < bufLen)
        return true;

    return false;
}


/**
 *  @brief Perform sign extension (32bit)
 *  @param [in] val   Unsigned value
 *  @param [in] width Bit width
 *  @return value after sign extension
 */
static int32_t castInt32(uint32_t val, unsigned int width)
{
    uint32_t mask = (0x1ULL << width) - 1;

    if((val >> (width - 1)) & 0x1)
        return (~mask) | (val & mask);
    else
        return val & mask;
}


/**
 *  @brief Perform sign extension (64bit)
 *  @param [in] val   Unsigned value
 *  @param [in] width Bit width
 *  @return value after sign extension
 */
static int64_t castInt64(uint64_t val, unsigned int width)
{
    uint64_t mask = (0x1ULL << width) - 1;

    if((val >> (width - 1)) & 0x1)
        return (~mask) | (val & mask);
    else
        return val & mask;
}
