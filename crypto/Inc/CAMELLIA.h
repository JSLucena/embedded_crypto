/* CAMELLIA.h
*
 * Author: Vinicius Borba da Rocha
 * Created: 08/08/2021
 *
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include "CTRMode.h"
#include "config.h"

#ifdef USE_CAMELLIA

typedef struct
{
	uint16_t feistelIterations;
	uint8_t nrSubkeys;
	uint64_t k[34];
} CamelliaContext;

void CAMELLIA_init(CamelliaContext* context, const uint64_t* key, uint16_t keyLen);
void CAMELLIA_encrypt(const CamelliaContext* context, const uint64_t* block, uint64_t* out);

void crypt_main(CTRCounter* ctrNonce, int key_size);

#endif