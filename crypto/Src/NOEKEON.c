/* NOEKEON.c
*
 * Author: Vinicius Borba da Rocha
 * Created: 27/07/2021
 * Updated: 21/05/2022
 *
 * Implementation of the NOEKEON block cipher with
 * 128 bits block length and 128 bits key length.
 *
 * This code follows a specification:
 *		- http://gro.noekeon.org/Noekeon-spec.pdf
 *
 * and uses other codebases as references:
 *		- https://github.com/jl777/libjl777/blob/master/NXTservices/noekeon.c
 *		- https://github.com/stamparm/cryptospecs/blob/master/symmetrical/sources/noekeon.c
 *		- https://github.com/cantora/avr-crypto-lib/blob/master/noekeon/noekeon.c
 *
 */

#include "NOEKEON.h"
#include "config.h"

#ifdef USE_NOEKEON

#define NR_ROUNDS 16
#define STREAM (256 * 8)

uint8_t LFSR() {

        unsigned char in_s, cs, cp, p, nbit, s[STREAM];
        int i, j, k=0;


        in_s = 0xb4;     /* this can be any 8 bit value */
        p = 0x71;        /* max length polynomial x^8+x^4+x^3+x^2+1 = 0b01110001 */

        cs = in_s;      /* copy initial state */

                for (j = 0;j < 8;j++,k++) {
                        cp = nbit = cs & p;

                        for (i = 1;i < 8; i++) { /* xor all bits together */
                                nbit ^= (cp >> i);
                        }
                        s[k] = cs & 0x01;
                        cs = (cs >> 1) | (nbit << 7); /*  rotate in new bit */
                }

                if (cs == in_s) {
                         return 0x00;
                }
		else{
			return cs;
		}

}

static const uint32_t RC[] =
{
   0x80, 0x1b, 0x36, 0x6c,
   0xd8, 0xab, 0x4d, 0x9a,
   0x2f, 0x5e, 0xbc, 0x63,
   0xc6, 0x97, 0x35, 0x6a,
   0xd4
};

static void MOV_128(uint32_t* y, uint32_t* x)
{
	y[0] = x[0];
	y[1] = x[1];
	y[2] = x[2];
	y[3] = x[3];
}

// Rotate Left circular shift 32 bits
static uint32_t ROL_32(uint32_t x, uint32_t n)
{
	return x << n | x >> (32 - n);
}

// Rotate Right circular shift 32 bits
static uint32_t ROR_32(uint32_t x, uint32_t n)
{
	return x >> n | x << (32 - n);
}

static void pi1(uint32_t* a)
{
	a[1] = ROL_32(a[1], 1);
	a[2] = ROL_32(a[2], 5);
	a[3] = ROL_32(a[3], 2);
}

static void pi2(uint32_t* a)
{
	a[1] = ROR_32(a[1], 1);
	a[2] = ROR_32(a[2], 5);
	a[3] = ROR_32(a[3], 2);
}

static void gamma(uint32_t* a)
{
	uint32_t tmp;

	a[1] ^= ~a[3] & ~a[2];
	a[0] ^= a[2] & a[1];

	tmp = a[3];
	a[3] = a[0];
	a[0] = tmp;

	a[2] ^= a[0] ^ a[1] ^ a[3];
	a[1] ^= ~a[3] & ~a[2];
	a[0] ^= a[2] & a[1];
}

static void theta(const uint32_t* k, uint32_t* a)
{
	uint32_t temp = a[0] ^ a[2];
	temp ^= ROR_32(temp, 8) ^ ROL_32(temp, 8);

	a[1] ^= temp;
	a[3] ^= temp;

	a[0] ^= k[0];
	a[1] ^= k[1];
	a[2] ^= k[2];
	a[3] ^= k[3];

	temp = a[1] ^ a[3];
	temp ^= ROR_32(temp, 8) ^ ROL_32(temp, 8);

	a[0] ^= temp;
	a[2] ^= temp;
}

static void NOEKEON_round(uint32_t* key, uint32_t* block, uint32_t c1, uint32_t c2)
{
	block[0] ^= c1;
	theta(key, block);
	block[0] ^= c2;
	pi1(block);
	gamma(block);
	pi2(block);
}

void NOEKEON_encrypt(uint32_t* block, uint32_t* key, uint32_t* encryptdBlock)
{
	MOV_128(encryptdBlock, block);
	for (int i = 0; i < NR_ROUNDS; i++)
	{
		NOEKEON_round(key, encryptdBlock, RC[i], 0);
	}

	encryptdBlock[0] ^= RC[NR_ROUNDS];
	theta(key, encryptdBlock);
}

void crypt_main(CTRCounter* ctrNonce, int key_size)
{
	int i;
	uint32_t counter32[4];
	uint8_t counter[16];
	for(i=0; i<16; i++){
		counter[i] = LFSR();
	}
	for(i=0; i<4; i++){
		counter32[i] = (((uint32_t)counter[4*i+3]) << 24) + (((uint32_t)counter[4*i+2]) << 16) + (((uint32_t)counter[4*i+1]) << 8) + (uint32_t)counter[4*i];
	}

	NOEKEON_encrypt(counter32, ctrNonce->Key, ctrNonce->cipherText);	
	return;
}

#endif
