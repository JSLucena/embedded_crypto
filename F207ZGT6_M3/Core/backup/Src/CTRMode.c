/* CTRMode.h
*
 * Author: Nicolas Moura
 * Created: 21/05/2021 - CTR mode Implementation
 *
 */

#include "CTRMode.h"
//#include "ARIA.h"

#include "CAMELLIA.h"
/*
#include "NOEKEON.h"
#include "SEED.h"
#include "SIMON.h"
#include "SPECK.h"
#include "IDEA.h"
#include "PRESENT.h"
#include "HIGHT.h"
#include "GOST.h"
*/
#include "validator.h"

void Select_Algorithm(CTRCounter* ctrCounter, enum Algorithm algorithm){
	switch (algorithm)
		{
		case ARIA_128 :
			ARIA_main(ctrCounter, 128);
			break;
		/*
		case ARIA_192 :
			ARIA_main(ctrCounter, 192);
			break;
		case ARIA_256 :
			ARIA_main(ctrCounter, 256);
			break;
		case CAMELLIA_128 :
			CAMELLIA_main(ctrCounter, 128);
			break;
		case CAMELLIA_192 :
			CAMELLIA_main(ctrCounter, 192);
			break;
		case CAMELLIA_256 :
			CAMELLIA_main(ctrCounter, 256);
			break;
		case NOEKEON_128 :
			NOEKEON_main(ctrCounter, 128);
			break;
		case SEED_128 :
			SEED_main(ctrCounter, 128);
			break;
		case SIMON_128 :
			SIMON_main(ctrCounter, 128);
			break;
		case SIMON_192 :
			SIMON_main(ctrCounter, 192);
			break;
		case SIMON_256 :
			SIMON_main(ctrCounter, 256);
			break;
		case SPECK_128 :
			SPECK_main(ctrCounter, 128);
			break;
		case SPECK_192 :
			SPECK_main(ctrCounter, 192);
			break;
		case SPECK_256 :
			SPECK_main(ctrCounter, 256);
			break;
		case IDEA_128 :
			IDEA_main(ctrCounter, 128);
			break;		
		case PRESENT_80 :
			PRESENT_main(ctrCounter, 80);
			break;
		case PRESENT_128 :
			PRESENT_main(ctrCounter, 128);
			break;
		case HIGHT_128 :
			HIGHT_main(ctrCounter, 128);
			break;
		case GOST_256 :
			GOST_main(ctrCounter, 256);
			break;
		*/
		default:
			break;
		}
}



int CTRMode_main(CTRCounter ctrCounter, enum Algorithm algorithm, int SIZE, int contText){

    // ENCRYPT SIDE	
    Select_Algorithm(&ctrCounter, algorithm);

	// printf("\nCypher before XOR: \t");
	for (int i = 0; i < 4; i++)
	{
		// printf("%08x ", ctrCounter.cipherText[i]);
	}

	ctrCounter.cipherTemp[0] = ctrCounter.text[0] ^ ctrCounter.cipherText[0];
	ctrCounter.cipherTemp[1] = ctrCounter.text[1] ^ ctrCounter.cipherText[1];
	if(SIZE == 4){
		ctrCounter.cipherTemp[2] = ctrCounter.text[2] ^ ctrCounter.cipherText[2];
		ctrCounter.cipherTemp[3] = ctrCounter.text[3] ^ ctrCounter.cipherText[3];
	}else{
		ctrCounter.cipherTemp[2] = 0x00000000;
		ctrCounter.cipherTemp[3] = 0x00000000;
	}

	// printf("\nCypher after XOR: \t");
	for (int i = 0; i < 4; i++)
	{
		// printf("%08x ", ctrCounter.cipherTemp[i]);
	}
	
	// DECRYPT SIDE
	Select_Algorithm(&ctrCounter, algorithm);
	
	ctrCounter.cipherText[0] = ctrCounter.cipherTemp[0] ^ ctrCounter.cipherText[0];
	ctrCounter.cipherText[1] = ctrCounter.cipherTemp[1] ^ ctrCounter.cipherText[1];
	ctrCounter.cipherText[2] = ctrCounter.cipherTemp[2] ^ ctrCounter.cipherText[2];
	ctrCounter.cipherText[3] = ctrCounter.cipherTemp[3] ^ ctrCounter.cipherText[3];
	
	// Decrypt validation
	for (int i = 0; i < SIZE; i++)
	{
		// verify if decrypt and TextList is the same
		if (!(ctrCounter.cipherText[i] == TEXT_LIST2[contText - SIZE + i]))
			return 1;
	}		
	return 0;
}
