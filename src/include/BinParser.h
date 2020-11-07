/**
 *  @file      BinParser.h
 *  @brief     バイナリデータパーサー, ヘッダファイル
 *  @author    fire-peregrine
 *  @date      2019/03/12
 *  @copyright Copyright (C) fire-peregrine all rights reserved.
 */

#ifndef SRC_BIN_PARSER_H_
#define SRC_BIN_PARSER_H_

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>

/* Typedefs */
typedef struct BinParser_ BinParser;


/* Signatures */
int BinParser_reset(BinParser *obj, uint8_t *buf, unsigned int bufLen);
int BinParser_delete(BinParser *obj);
int BinParser_getBool(BinParser *obj, bool *val);
int BinParser_getUInt32(BinParser *obj, unsigned int bits, uint32_t *val);
int BinParser_getUInt64(BinParser *obj, unsigned int bits, uint64_t *val);
int BinParser_getInt32(BinParser *obj, unsigned int bits, int32_t *val);
int BinParser_getInt64(BinParser *obj, unsigned int bits, int64_t *val);
int BinParser_getBytes(BinParser *obj, unsigned int bytes, uint8_t *val);
int BinParser_getGlm(BinParser *obj, uint32_t *val);
int BinParser_getSGlm(BinParser *obj, int32_t *val);
int BinParser_alignByte(BinParser *obj);
int BinParser_alignBytes(BinParser *obj, unsigned int bytes);
bool BinParser_isByteAligned(BinParser *obj);
bool BinParser_isAligned(BinParser *obj, unsigned int bytes);
int BinParser_seek(BinParser *obj, unsigned int posByte, unsigned int posBit);
int BinParser_skip(BinParser *obj, unsigned int bytes, unsigned int bits);
int BinParser_getPos(BinParser *obj, unsigned int *posByte, unsigned int *posBit);
bool BinParser_hasRest(BinParser *obj, unsigned int bytes, unsigned int bits);
int BinParser_dump(BinParser *obj);


#endif // SRC_BIN_PARSER_H_


