/* SIMON.c
*
 * Author: Vinicius Borba da Rocha
 * Created: 09/08/2021
 * Updated: 21/05/2022
 *
 * Implementation of the SIMON block cipher with
 * 128 bits block length and 128/192/256 bits key length.
 *
 * This code follows a specification:
 *		- https://eprint.iacr.org/2013/404.pdf
 *
 * and uses other codebases as references:
 *		- https://github.com/nsacyber/simon-speck-supercop/blob/master/crypto_stream/simon128128ctr/ref/stream.c
 *
 */

#include "SIMON.h"

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

static uint64_t f(uint64_t x)
{
	return (ROL_64(x, 1) & ROL_64(x, 8)) ^ ROL_64(x, 2);
}

static void R2(uint64_t* x, uint64_t* y, uint64_t k, uint64_t l)
{
	*y ^= f(*x);
	*y ^= k;
	*x ^= f(*y);
	*x ^= l;
}

void SIMON_init(SimonContext* context, uint64_t* key, uint16_t keyLen)
{
	uint64_t c = 0xfffffffffffffffcLL;
	uint64_t z;
	uint64_t i;

	if (keyLen == 128)
	{
		context->nrSubkeys = 68;

		z = 0x7369f885192c0ef5LL;

		context->subkeys[0] = key[1];
		context->subkeys[1] = key[0];

		for (i = 2; i < 66; i++)
		{
			context->subkeys[i] = c ^ (z & 1) ^ context->subkeys[i - 2] ^ ROR_64(context->subkeys[i - 1], 3) ^ ROR_64(context->subkeys[i - 1], 4);
			z >>= 1;
		}

		context->subkeys[66] = c ^ 1 ^ context->subkeys[64] ^ ROR_64(context->subkeys[65], 3) ^ ROR_64(context->subkeys[65], 4);
		context->subkeys[67] = c ^ context->subkeys[65] ^ ROR_64(context->subkeys[66], 3) ^ ROR_64(context->subkeys[66], 4);
	}
	else if (keyLen == 192)
	{
		context->nrSubkeys = 69;

		z = 0xfc2ce51207a635dbLL;

		context->subkeys[0] = key[2];
		context->subkeys[1] = key[1];
		context->subkeys[2] = key[0];

		for (i = 3; i < 67; i++)
		{
			context->subkeys[i] = c ^ (z & 1) ^ context->subkeys[i - 3] ^ ROR_64(context->subkeys[i - 1], 3) ^ ROR_64(context->subkeys[i - 1], 4);
			z >>= 1;
		}

		context->subkeys[67] = c ^ context->subkeys[64] ^ ROR_64(context->subkeys[66], 3) ^ ROR_64(context->subkeys[66], 4);
		context->subkeys[68] = c ^ 1 ^ context->subkeys[65] ^ ROR_64(context->subkeys[67], 3) ^ ROR_64(context->subkeys[67], 4);
	}
	else // 256
	{
		context->nrSubkeys = 72;

		z = 0xfdc94c3a046d678bLL;

		context->subkeys[0] = key[3];
		context->subkeys[1] = key[2];
		context->subkeys[2] = key[1];
		context->subkeys[3] = key[0];

		for (i = 4; i < 68; i++)
		{
			context->subkeys[i] = c ^ (z & 1) ^ context->subkeys[i - 4] ^ ROR_64(context->subkeys[i - 1], 3) ^ context->subkeys[i - 3] ^ ROR_64(context->subkeys[i - 1], 4) ^ ROR_64(context->subkeys[i - 3], 1);
			z >>= 1;
		}

		context->subkeys[68] = c ^ context->subkeys[64] ^ ROR_64(context->subkeys[67], 3) ^ context->subkeys[65] ^ ROR_64(context->subkeys[67], 4) ^ ROR_64(context->subkeys[65], 1);
		context->subkeys[69] = c ^ 1 ^ context->subkeys[65] ^ ROR_64(context->subkeys[68], 3) ^ context->subkeys[66] ^ ROR_64(context->subkeys[68], 4) ^ ROR_64(context->subkeys[66], 1);
		context->subkeys[70] = c ^ context->subkeys[66] ^ ROR_64(context->subkeys[69], 3) ^ context->subkeys[67] ^ ROR_64(context->subkeys[69], 4) ^ ROR_64(context->subkeys[67], 1);
		context->subkeys[71] = c ^ context->subkeys[67] ^ ROR_64(context->subkeys[70], 3) ^ context->subkeys[68] ^ ROR_64(context->subkeys[70], 4) ^ ROR_64(context->subkeys[68], 1);
	}
}

void SIMON_encrypt(SimonContext* context, uint64_t* block, uint64_t* out)
{
	uint8_t i;
	uint64_t x = block[0];
	uint64_t y = block[1];
	uint64_t t;

	if (context->nrSubkeys == 69)
	{
		for (i = 0; i < 68; i += 2)
		{
			R2(&x, &y, context->subkeys[i], context->subkeys[i + 1]);
		}

		y ^= f(x);
		y ^= context->subkeys[68];
		t = x;
		x = y;
		y = t;
	}
	else
	{
		for (i = 0; i < context->nrSubkeys; i += 2)
		{
			R2(&x, &y, context->subkeys[i], context->subkeys[i + 1]);
		}
	}

	out[0] = x;
	out[1] = y;
}

void SIMON_main(CTRCounter* ctrNonce, int key_size)
{
	SimonContext context;
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

	SIMON_init(&context, key, key_size);
	SIMON_encrypt(&context, text, cipherText);

	ctrNonce->cipherText[0] = (uint32_t)(cipherText[0] >> 32);
	ctrNonce->cipherText[1] = (uint32_t)(cipherText[0]);
	ctrNonce->cipherText[2] = (uint32_t)(cipherText[1] >> 32);
	ctrNonce->cipherText[3] = (uint32_t)(cipherText[1]);
	
	return;
}
