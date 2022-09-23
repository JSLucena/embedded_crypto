/* NOEKEON.h
*
 * Author: Vinicius Borba da Rocha
 * Created: 27/07/2021
 *
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include "CTRMode.h"
#include "config.h"


#ifdef USE_NOEKEON

void NOEKEON_encrypt(uint32_t* block, uint32_t* key, uint32_t* encryptdBlock);

void crypt_main(CTRCounter* ctrNonce, int key_size);

#endif
