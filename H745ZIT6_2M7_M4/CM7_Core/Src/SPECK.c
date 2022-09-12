/* SPECK.h
*
 * Author: Vinicius Borba da Rocha
 * Created: 08/08/2021
 * Updated: 21/05/2022
 *
 * Implementation of the SPECK block cipher with
 * 128 bits block length and 128/192/256 bits key length.
 *
 * This code follows a specification:
 *		- https://eprint.iacr.org/2013/404.pdf
 *
 * and uses other codebases as references:
 *		- https://github.com/nsacyber/simon-speck-supercop/tree/master/crypto_stream
 *
 */

#include "SPECK.h"

// Rotate Left circular shift 32 bits
static uint64_t ROL_64(uint64_t x, uint32_t n)
{
	return x << n | x >> (64 - n);
}

// Rotate Right circular shift 32 bits
static uint64_t ROR_64(uint64_t x, uint32_t n)
{
	return x >> n | x << (64 - n);
}

static void R(uint64_t* x, uint64_t* y, uint64_t k)
{
	*x = ROR_64(*x, 8);
	*x += *y;
	*x ^= k;
	*y = ROL_64(*y, 3);
	*y ^= *x;
}

void SPECK_init(SpeckContext* context, uint64_t* key, uint16_t keyLen)
{
	uint64_t A;
	uint64_t B;
	uint64_t C;
	uint64_t D;
	uint64_t i;

	if (keyLen == 128)
	{
		context->nrSubkeys = 32;

		A = key[1];
		B = key[0];

		for (i = 0; i < 32; i++)
		{
			context->subkeys[i] = A;
			R(&B, &A, i);
		}
	}
	else if (keyLen == 192)
	{
		context->nrSubkeys = 33;

		A = key[2];
		B = key[1];
		C = key[0];

		for (i = 0; i < 32; i += 2)
		{
			context->subkeys[i] = A;
			R(&B, &A, i);
			context->subkeys[i + 1] = A;
			R(&C, &A, i + 1);
		}
		context->subkeys[32] = A;
	}
	else // 256
	{
		context->nrSubkeys = 34;

		A = key[3];
		B = key[2];
		C = key[1];
		D = key[0];

		for (i = 0; i < 33; i += 3)
		{
			context->subkeys[i] = A;
			R(&B, &A, i);
			context->subkeys[i + 1] = A;
			R(&C, &A, i + 1);
			context->subkeys[i + 2] = A;
			R(&D, &A, i + 2);
		}
		context->subkeys[33] = A;
	}
}

void SPECK_encrypt(SpeckContext* context, uint64_t* block, uint64_t* out)
{
	uint8_t i;
	uint64_t x = block[0];
	uint64_t y = block[1];

	for (i = 0; i < context->nrSubkeys; i++)
	{
		R(&x, &y, context->subkeys[i]);
	}

	out[0] = x;
	out[1] = y;
}

void SPECK_main(CTRCounter* ctrNonce, int key_size)
{
	SpeckContext context;
	uint64_t key[4];
	uint64_t text[2];
	uint64_t cipherText[2];
	uint64_t val0 = ctrNonce->ctrNonce[0];
	uint64_t val1 = ctrNonce->ctrNonce[1];
	uint64_t val2 = ctrNonce->ctrNonce[2];
	uint64_t val3 = ctrNonce->ctrNonce[3];

	uint64_t key0 = ctrNonce->Key[0];
	uint64_t key1 = ctrNonce->Key[1];
	uint64_t key2 = ctrNonce->Key[2];
	uint64_t key3 = ctrNonce->Key[3];
	uint64_t key4 = ctrNonce->Key[4];
	uint64_t key5 = ctrNonce->Key[5];
	uint64_t key6 = ctrNonce->Key[6];
	uint64_t key7 = ctrNonce->Key[7];

	text[0] = (val0 << 32) | val1;
	text[1] = (val2 << 32) | val3;

	switch (key_size)
	{
	case 128 :
		key[0] = (key0 << 32) | key1;
		key[1] = (key2 << 32) | key3;
		key[2] = 0x0000000000000000;
		key[3] = 0x0000000000000000;		
		break;
	case 192 :
		key[0] = (key0 << 32) | key1;
		key[1] = (key2 << 32) | key3;
		key[2] = (key4 << 32) | key5;
		key[3] = 0x0000000000000000;
		break;
	case 256 :
		key[0] = (key0 << 32) | key1;
		key[1] = (key2 << 32) | key3;
		key[2] = (key4 << 32) | key5;
		key[3] = (key6 << 32) | key7;
		break;
	
	default:
		break;
	}

	SPECK_init(&context, key, key_size);
	SPECK_encrypt(&context, text, cipherText);

	ctrNonce->cipherText[0] = (uint32_t)(cipherText[0] >> 32);
	ctrNonce->cipherText[1] = (uint32_t)(cipherText[0]);
	ctrNonce->cipherText[2] = (uint32_t)(cipherText[1] >> 32);
	ctrNonce->cipherText[3] = (uint32_t)(cipherText[1]);
	
	return;
}
