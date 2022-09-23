/* SIMON.h
*
 * Author: Vinicius Borba da Rocha
 * Created: 09/08/2021
 *
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include "CTRMode.h"
#include "config.h"

#ifdef USE_SIMON

typedef struct
{
	uint8_t nrSubkeys;
	uint64_t subkeys[72];
} SimonContext;

void SIMON_init(SimonContext* context, uint64_t* key, uint16_t keyLen);
void SIMON_encrypt(SimonContext* context, uint64_t* block, uint64_t* out);

void crypt_main(CTRCounter* ctrNonce, int key_size);


#endif

