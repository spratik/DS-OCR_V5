/*
 * componentsimageprinter.cpp
 *
 *  Created on: Apr 7, 2016
 *  Author: Soumyadeep Dhar
 */

#include <string.h>

#include "componentlist.h"
#include "componentsimageprinter.h"
#include "enum.h"

#define _PI_LIGHTRED(a)														\
{																			\
	imageR.drawRectangle(a, 255);											\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_RED(a)															\
{																			\
	imageR.drawRectangle(a, 192);											\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_RED_THICK(a, b)													\
{																			\
	imageR.drawRectangle(a, 192, b);										\
	imageG.drawRectangle(a, 0,   b);										\
	imageB.drawRectangle(a, 0,   b);										\
}

#define _PI_DARKRED(a)														\
{																			\
	imageR.drawRectangle(a, 128);											\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 0);												\
}


#define _PI_LIGHTBLUE(a)													\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 255);											\
}

#define _PI_BLUE(a)															\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 192);											\
}

#define _PI_BLUE_THICK(a, b)												\
{																			\
	imageR.drawRectangle(a, 0, 	 b);										\
	imageG.drawRectangle(a, 0,   b);										\
	imageB.drawRectangle(a, 192, b);										\
}

#define _PI_DARKBLUE(a)														\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 128);											\
}


#define _PI_LIGHTGREEN(a)													\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 255);											\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_GREEN(a)														\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 192);											\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_GREEN_THICK(a, b)												\
{																			\
	imageR.drawRectangle(a, 0,   b);										\
	imageG.drawRectangle(a, 192, b);										\
	imageB.drawRectangle(a, 0,   b);										\
}

#define _PI_DARKGREEN(a)													\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 128);											\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_YELLOW(a)														\
{																			\
	imageR.drawRectangle(a, 192);											\
	imageG.drawRectangle(a, 192);											\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_YELLOW_THICK(a, b)												\
{																			\
	imageR.drawRectangle(a, 192, b);										\
	imageG.drawRectangle(a, 192, b);										\
	imageB.drawRectangle(a,   0, b);											\
}

#define _PI_LIGHTMAGENTA(a)													\
{																			\
	imageR.drawRectangle(a, 255);											\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 255);											\
}

#define _PI_MAGENTA(a)														\
{																			\
	imageR.drawRectangle(a, 192);											\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 192);											\
}

#define _PI_DARKMAGENTA(a)													\
{																			\
	imageR.drawRectangle(a, 128);											\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 128);											\
}

#define _PI_CYAN(a)															\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 192);											\
	imageB.drawRectangle(a, 192);											\
}

#define _PI_GRAY(a)															\
{																			\
	imageR.drawRectangle(a, 192);											\
	imageG.drawRectangle(a, 192);											\
	imageB.drawRectangle(a, 192);											\
}

#define _PI_BLACK(a)														\
{																			\
	imageR.drawRectangle(a, 0);												\
	imageG.drawRectangle(a, 0);												\
	imageB.drawRectangle(a, 0);												\
}

#define _PI_BROWN(a)														\
{																			\
	imageR.drawRectangle(a, 165);											\
	imageG.drawRectangle(a, 42);											\
	imageB.drawRectangle(a, 42);											\
}

void ComponentImagePrinter :: printPageDetails
(CNode *head, CNode *tail,  UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the page object
			PageObject *obj = (PageObject *) temp->getData();

			_PI_BLUE_THICK(obj->getBrect(), 5);
		}
	}
}

void ComponentImagePrinter :: printTextRegionsDetails
(CNode *head, CNode *tail,  UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	CNode *temp = NULL;

	if(head) {
		for (temp = head; temp != tail->next; temp = temp->next) {

//			/// Print Line
//			switch(temp->data->pdata.alignment) {
//			case LeftAligned :
//				_PI_BLUE_THICK(obj->getBrect(), 5);
//				break;
//			case RightAligned :
//				_PI_GREEN_THICK(obj->getBrect(), 5);
//				break;
//			case CenterAligned :
//				_PI_RED_THICK(obj->getBrect(), 5);
//				break;
//			default :
//				//Justify
//				_PI_RED_THICK(obj->getBrect(), 2);
//				break;
//			}

			printTextLineDetails(temp->hchild, temp->tchild, imageR, imageG, imageB);
		}
	}
}

void ComponentImagePrinter :: printTextLineDetails
(CNode *head, CNode *tail,  UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	CNode *temp = NULL;

	if(head) {
		for (temp = head; temp != tail->next; temp = temp->next) {

			/// Print Line
//			_PI_GREEN(obj->getBrect());

			printTextWordDetails(temp->hchild, temp->tchild, imageR, imageG, imageB);
		}
	}
}

void ComponentImagePrinter :: printTextWordDetails
(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {

			if(!temp->hchild) {
				continue;
			}

			/// Get the line object
			TextWordObject *obj = (TextWordObject *)temp->getData();


			switch(obj->getMatchType())
			{

			/// 'Image-Sieve', 'OCR-A', 'OCR-B' 'Dictionary string matching'

			/// All have single word for a bounding box and
			/// string matched in both OCR and as well as with the dictionary.
			case _m1111 : // = 1,

			/// All have single word for a bounding box and only
			/// OCR(A) string matched with dictionary and
			/// OCR(B) string does not match with dictionary.
			case _m111A : // = 2,

			/// All have single word for a bounding box and only
			/// OCR(B) string matched with dictionary and
			/// OCR(A) string does not match with dictionary.
			case _m111B : // = 3,

			/// At least one OCR has multiple words for a
			/// bounding box detected by other OCR or Image-
			/// Sieve and string matched in both OCRs and as
			/// well as with the dictionary.
			//case _m1MM1 : // = 4,

			/// At least one OCR has multiple words for a
			/// bounding box detected by other OCR or Image-
			/// Sieve and only OCR(A) string matched with dictionary
			//case _m1MMA : // = 5,

			/// At least one OCR has multiple words for a
			/// bounding box detected by other OCR or Image-
			/// Sieve and only OCR(B) string matched with dictionary
			//case _m1MMB : // = 6,

			/// OCR(A) & OCR(B) stings match and they have digits in it
			case _m1DD1 : // = 7,

			/// OCR(A) & OCR(B) stings mismatch and
			/// OCR(B) has digits in it whereas
			/// OCR(A) matches with dictionary
			case _m11D1 : // = 8,

			/// OCR(A) & OCR(B) stings mismatch and
			/// OCR(A) has digits in it whereas
			/// OCR(B) matches with dictionary
			case _m1D11 : // = 9,
				imageR.fillBoundary(obj->getBoundary(), 32);
				// imageG.fillBoundary(obj->getBoundary(), 32);
				imageB.fillBoundary(obj->getBoundary(), 32);
				break;




			/// All have single word for a bounding box and both
			/// string matched with dictionary separately even
			/// individually strings are different.
			case _m111X : // = 10,

			/// At least one OCR has multiple words for a
			/// bounding box detected by other OCR or Image-
			/// Sieve and both string matched with dictionary
			/// separately even individually strings are different.
			//case _m1MMX : // = 11,
				/// Print token id for page
				imageR.fillBoundary(obj->getBoundary(), 144);
				//imageG.fillBoundary(obj->getBoundary(), 160);
				imageB.fillBoundary(obj->getBoundary(), 144);
				break;



			/// OCR(A) sting present in dictionary where OCR(B)
			/// does not have any string.
			case _m1101 : // = 12,

			/// OCR(B) sting present in dictionary where OCR(A)
			/// does not have any string.
			case _m1011 : // = 13,
				// imageR.fillBoundary(obj->getBoundary(), 0);
				// imageG.fillBoundary(obj->getBoundary(), 0);
				imageB.fillBoundary(obj->getBoundary(), 0);
				break;



			/// All have single word for a bounding box and
			/// string does not matched with dictionary
			case _m1110 : // = 14,

			/// OCR(A) sting not present in dictionary where
			/// OCR(B) does not have any string.
			case _m1100 : // = 15,

			/// OCR(B) sting not present in dictionary where
			/// OCR(A) does not have any string.
			case _m1010 : // = 16,

			/// OCR(A) has sting and digits in it
			case _m1D00 : // = 17,

			/// OCR(B) has sting and digits in it
			case _m10D0 : // = 18,

			/// OCR(A) has sting and digits in it and OCR(B) has string
			/// Both do not matched with dictionary
			case _m1D10 : // = 19,

			/// OCR(B) has sting and digits in it and OCR(A) has string
			/// Both do not matched with dictionary
			case _m11D0 : // = 20,

			/// OCR(A) & OCR(B) stings do not match and they have digits in it
			case _m1DD0 : // = 21,
				// imageR.fillBoundary(obj->getBoundary(), 0);
				imageG.fillBoundary(obj->getBoundary(), 92);
				imageB.fillBoundary(obj->getBoundary(), 0);
				break;


			/// Image-sieve has bounding box but none of the OCR has string for that
			case _m1000 : // = 23
				/// Both or any of the OCR has multiple string for the Segment
					imageG.fillBoundary(obj->getBoundary(), 0);
					imageB.fillBoundary(obj->getBoundary(), 0);

			/// Both or any of the OCR has multiple string for the Segment and
			/// None of them matches with dictionary
			//case _m1MM0 : // = 22,
					break;

			default:
					/// fill data
					// imageR.fillBoundary(obj->getBoundary(), 0);
					imageG.fillBoundary(obj->getBoundary(), 255);
					imageB.fillBoundary(obj->getBoundary(), 255);
					break;
			}

			_PI_RED_THICK(obj->getBrect(), 2);
			/// Print character details
			// printCharacterDetails(temp->hchild, temp->tchild, imageR, imageG, imageB);
		}
	}
}

void ComponentImagePrinter :: printCharacterDetails
(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the line object
			CharacterObject *obj = (CharacterObject *)temp->getData();

			/// Get token sub type for character
			switch(obj->getCtype()) {
			case TextCharacter :
				_PI_RED(obj->getBrect());
				break;
			case TextAssumedCharacter :
				_PI_CYAN(obj->getBrect());
				break;

			default:
				_PI_BLACK(obj->getBrect());
				break;
			}
		}
	}
}


void ComponentImagePrinter :: printLineDetails
(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the line object
			LineObject *obj = (LineObject *)temp->getData();

			/// Check type
			switch(obj->getCtype())
			{
			case LineNoise :
				_PI_MAGENTA(obj->getBrect());
				break;

			case HorizontalLine :
				_PI_LIGHTRED(obj->getBrect());
				break;

			case VerticalLine :
				_PI_LIGHTGREEN(obj->getBrect());
				break;

			default:
				_PI_BLACK(obj->getBrect());
				break;
			}
		}
	}
}

void ComponentImagePrinter :: printSubImageDetails
(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the line object
			SubImageObject *obj = (SubImageObject *)temp->getData();

			/// Check type
			switch(obj->getCtype())
			{
			case Image :
				_PI_BLUE(obj->getBrect());
				break;

			case SubImage :
				_PI_LIGHTBLUE(obj->getBrect());
				break;

			case ImageNoise :
				_PI_DARKBLUE(obj->getBrect());
				break;

			default:
				_PI_BLACK(obj->getBrect());
				break;
			}
		}
	}
}

void ComponentImagePrinter :: printImageDetails
(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the line object
			ImageObject *obj = (ImageObject *)temp->getData();

			/// Check type
			switch(obj->getCtype())
			{
			case Image :
				_PI_BLUE(obj->getBrect());
				break;

			case SubImage :
				_PI_LIGHTBLUE(obj->getBrect());
				break;

			case ImageNoise :
				_PI_DARKBLUE(obj->getBrect());
				break;

			default:
				_PI_BLACK(obj->getBrect());
				break;
			}

			/// Print sub-image details
			printSubImageDetails(temp->hchild, temp->tchild, imageR, imageG, imageB);
		}
	}
}

void ComponentImagePrinter :: printNoiseDetails
(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB)
{

	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get object
			NoiseObject *obj = (NoiseObject *)temp->getData();

			/// Check type
			switch(obj->getCtype())
			{
			case SmallNoise :
				_PI_MAGENTA(obj->getBrect());
				break;

			case Image :
				_PI_BLUE(obj->getBrect());
				break;

			case SubImage :
				_PI_YELLOW(obj->getBrect());
				break;

			case ImageNoise :
				_PI_BLACK(obj->getBrect());
				break;

			case TextCharacter :
				_PI_GREEN(obj->getBrect());
				break;

			case TextAssumedCharacter :
				_PI_LIGHTGREEN(obj->getBrect());
				break;

			case TextLine :
				_PI_GREEN(obj->getBrect());
				break;

			case TextWord :
				_PI_YELLOW(obj->getBrect());
				break;

			case LineNoise :
				_PI_LIGHTMAGENTA(obj->getBrect());
				break;

			case HorizontalLine :
				_PI_LIGHTRED(obj->getBrect());
				break;

			case VerticalLine :
				_PI_LIGHTGREEN(obj->getBrect());
				break;

			default:
				_PI_BLACK(obj->getBrect());
				break;
			}
		}
	}
}

void ComponentImagePrinter :: printOCRDetails
(COCR *head, COCR *tail, UCharData &imageR, int rvalue, UCharData &imageG, int gvalue, UCharData &imageB, int bvalue)
{
	if(head) {
		for (COCR *temp = head; temp != tail->next; temp = temp->next) {
			/// Check type
			switch(temp->data.characterCount)
			{
			case 0 : // BlankSpace
				_PI_RED(temp->data.brect);
				break;
			default:
				if(rvalue <= _PMAX) {
					imageR.fillBoundary(temp->data.boundary, rvalue);
				}
				if(gvalue <= _PMAX) {
					imageG.fillBoundary(temp->data.boundary, gvalue);
				}
				if(bvalue <= _PMAX) {
					imageB.fillBoundary(temp->data.boundary, bvalue);
				}

				_PI_GREEN_THICK(temp->data.brect, 2);
				break;
			}
		}
	}
}
