/* IDEA.c
*
 * Author: Vinicius Borba da Rocha
 * Created: 11/07/2021
 * Updated: 21/05/2022
 *
 * Implementation of the IDEA block cipher with
 * 64 bits block length and 128 bits key length.
 *
 * This code follows a specification:
 *		- https://github.com/stamparm/cryptospecs/blob/master/symmetrical/specs/idea.pdf
 *
 * and uses other codebases as references:
 *		- https://github.com/stamparm/cryptospecs/blob/master/symmetrical/sources/idea.c
 *		- https://github.com/bgreenlee/PassKeeper/blob/master/CRYPTLIB/IDEA/IDEA.C
 *
 */

#include "IDEA.h"
#include "config.h"


#ifdef USE_IDEA

#define NR_ROUNDS 8
#define ENCRYPTION_KEY_LEN 6 * NR_ROUNDS + 4 // 52 subkeys

static uint16_t mul(uint16_t a, uint16_t b)
{
	long p;
	unsigned long q;

	if (a == 0)
		return (uint16_t)(1 - b);
	else if (b == 0)
		return (uint16_t)(1 - a);

	q = (unsigned long)a * (unsigned long)b;
	p = (q & 65535) - (q >> 16);

	if (p <= 0)
		p++;
	return (uint16_t)p;
}

static void generateEncryptionKeys(uint16_t* key, uint16_t Z[52])
{
	int i;

	// copy initial values from original key
	for (i = 0; i < 8; i++)
	{
		Z[i] = key[i];
	}

	// generate remaining subkeys in shift phase
	for (; i < 52; i++)
	{
		if ((i & 7) == 6)
		{
			Z[i] = (Z[i - 7] << 9) ^ (Z[i - 14] >> 7);
		}
		else if ((i & 7) == 7)
		{
			Z[i] = (Z[i - 15] << 9) ^ (Z[i - 14] >> 7);
		}
		else
		{
			Z[i] = (Z[i - 7] << 9) ^ (Z[i - 6] >> 7);
		}
	}
}

static void idea(uint16_t* block, uint16_t* Z, uint16_t* out)
{
	uint16_t i;
	uint16_t a;
	uint16_t b;
	uint16_t x0 = block[0];
	uint16_t x1 = block[1];
	uint16_t x2 = block[2];
	uint16_t x3 = block[3];

	// round phase
	for (i = 1; i <= NR_ROUNDS; i++)
	{
		// confusion / group operations
		x0 = mul(*Z++, x0);
		x1 += *Z++;
		x2 += *Z++;
		x3 = mul(*Z++, x3);

		// diffusion / MA (multiplication-addition) structure
		b = mul(*Z++, x0 ^ x2);
		a = mul(*Z++, b + (x1 ^ x3));
		b += a;

		// involuntary permutation
		x0 = a ^ x0;
		x3 = b ^ x3;
		b ^= x1;
		x1 = a ^ x2;
		x2 = b;
	}

	// output transformation
	out[0] = mul(*Z++, x0);
	out[1] = *Z++ + x2;
	out[2] = *Z++ + x1;
	out[3] = mul(*Z++, x3);
}

void IDEA_init(IdeaContext* context, uint16_t* key)
{
	generateEncryptionKeys(key, context->encryptionKeys);
}

void IDEA_encrypt(IdeaContext* context, uint16_t* block, uint16_t* out)
{
	idea(block, context->encryptionKeys, out);
}

void crypt_main(CTRCounter* ctrNonce, int key_size)
{
	IdeaContext context;
	uint16_t key[8];
	uint16_t text[4];
	uint16_t cipherText[4];

	text[0] = ctrNonce->ctrNonce[0] >> 16;
	text[1] = ctrNonce->ctrNonce[0];
	text[2] = ctrNonce->ctrNonce[1] >> 16;
	text[3] = ctrNonce->ctrNonce[1];

	key[0] = ctrNonce->Key[0];
	key[1] = ctrNonce->Key[1];
	key[2] = ctrNonce->Key[2];
	key[3] = ctrNonce->Key[3];
	key[4] = ctrNonce->Key[4];
	key[5] = ctrNonce->Key[5];
	key[6] = ctrNonce->Key[6];
	key[7] = ctrNonce->Key[7];

	IDEA_init(&context, key);
	IDEA_encrypt(&context, text, cipherText);

	ctrNonce->cipherText[0] = (uint32_t)(cipherText[0] << 16) | (uint32_t)(cipherText[1]);
	ctrNonce->cipherText[1] = (uint32_t)(cipherText[2] << 16) | (uint32_t)(cipherText[3]);
	ctrNonce->cipherText[2] = 0x00000000;
	ctrNonce->cipherText[3] = 0x00000000;

}

#endif