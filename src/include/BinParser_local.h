/**
 *  @file      BinParser.c
 *  @brief     Binary Parser
 *  @author    fire-peregrine
 *  @date      2020/11/07
 *  @copyright Copyright (C) fire-peregrine all rights reserved.
 */

#ifndef SRC_BIN_PERSER_LOCAL_H_
#define SRC_BIN_PERSER_LOCAL_H_

/* Macro Functions */
/**
 *  @brief Advance the current position.
 *  @param [in] bytes   Bytes to advance.
 *  @param [in] bits    Bits to advance.
 */
#define BIN_PARSER_ADVANCE_POS(bytes, bits) \
    do { \
        obj->posByte += bytes; \
        obj->posByte += ((obj->posBit + bits) >> 3); \
        obj->posBit = (obj->posBit + bits) & 0x7; \
    } while(0) 


/* Structs */
/**
 *  @param BinParser object structure.
 */
struct BinParser_
{
    uint8_t      *buf;     ///< Buffer
    uint64_t      bufLen;  ///< Buffer length
    uint64_t      posByte; ///< Current byte position in the buffer
    unsigned int  posBit;  ///< Current bit position in the buffer

};


#endif // SRC_BIN_PERSER_LOCAL_H_


