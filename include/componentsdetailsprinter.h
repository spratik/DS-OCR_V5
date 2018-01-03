/*
 * componentsdetailsprinter.h
 *
 *  Created on: Apr 7, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef COMPONENTSDETAILSPRINTER_H_
#define COMPONENTSDETAILSPRINTER_H_

#include <stdio.h>
#include <datatype.h>

class ComponentDetailsPrinter
{
public:
	ComponentDetailsPrinter() {}
	virtual ~ComponentDetailsPrinter() {}

	virtual int init(char *fname) = 0;
	virtual int close() = 0;

	virtual int printPageDetails	 (CNode *head, CNode *tail) = 0;

	virtual int printTextRegionDetails(CNode *head, CNode *tail) = 0;
	virtual int printTextLineDetails (CNode *head, CNode *tail) = 0;
	virtual int printTextWordDetails (CNode *head, CNode *tail) = 0;
	virtual int printCharacterDetails(CNode *head, CNode *tail) = 0;

	virtual int printLineDetails	 (CNode *head, CNode *tail) = 0;
	virtual int printImageDetails	 (CNode *head, CNode *tail) = 0;
	virtual int printNoiseDetails	 (CNode *head, CNode *tail) = 0;
};

#endif /* COMPONENTSDETAILSPRINTER_H_ */
