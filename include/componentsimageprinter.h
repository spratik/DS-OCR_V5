/*
 * componentsimageprinter.h
 *
 *  Created on: Apr 7, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef COMPONENTSIMAGEPRINTER_H_
#define COMPONENTSIMAGEPRINTER_H_

#include <stdio.h>
#include <datatype.h>
#include <tdata.h>

class ComponentImagePrinter
{
public:
	ComponentImagePrinter() {}
	~ComponentImagePrinter() {}

	void printPageDetails	 (CNode *head, CNode *tail,  UCharData &imageR, UCharData &imageG, UCharData &imageB);

	void printTextRegionsDetails(CNode *head, CNode *tail,  UCharData &imageR, UCharData &imageG, UCharData &imageB);
	void printTextLineDetails (CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);
	void printTextWordDetails (CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);
	void printCharacterDetails(CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);

	void printLineDetails	 (CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);

	void printImageDetails	 (CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);
	void printSubImageDetails (CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);

	void printNoiseDetails	 (CNode *head, CNode *tail, UCharData &imageR, UCharData &imageG, UCharData &imageB);

	void printOCRDetails(COCR *head, COCR *tail, UCharData &imageR, int rvalue, UCharData &imageG, int gvalue, UCharData &imageB, int bvalue);
};

#endif /* COMPONENTSIMAGEPRINTER_H_ */
