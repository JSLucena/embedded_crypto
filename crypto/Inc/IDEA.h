/* IDEA.h
*
 * Author: Vinicius Borba da Rocha
 * Created: 11/07/2021
 *
 */

#pragma once

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "CTRMode.h"
#include "config.h"

#ifdef USE_IDEA
typedef struct
{
	uint16_t encryptionKeys[52];
} IdeaContext;

void IDEA_init(IdeaContext* context, uint16_t* key);
void IDEA_encrypt(IdeaContext* context, uint16_t* block, uint16_t* out);

void crypt_main(CTRCounter* ctrNonce, int key_size);

#endif