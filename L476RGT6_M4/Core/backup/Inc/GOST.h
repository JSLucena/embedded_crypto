/* GOST.h
*
 * Author: Vinicius Borba da Rocha
 * Created: 06/06/2021
 *
 */

#pragma once

#include <stdio.h>
#include <stdint.h>
#include "CTRMode.h"

uint64_t GOST_encrypt(uint64_t block, uint32_t* key);

void GOST_main(CTRCounter* ctrNonce, int key_size);
