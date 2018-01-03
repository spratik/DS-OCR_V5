/*
 * componentmajororderfile.h
 *
 *  Created on: Apr 29, 2016
 *  Author: Soumyadeep
 */

#ifndef COMPONENTMAJORORDERFILE_H_
#define COMPONENTMAJORORDERFILE_H_

#include "componentlist.h"
#include "componentsdetailsprinter.h"

class ComponentMajorOrderFile : public ComponentDetailsPrinter
{
	FILE *fp;
	int fprintfUnicode(FILE *fp, unsigned int value);
public:
	ComponentMajorOrderFile() {}
	virtual ~ComponentMajorOrderFile() {}

	int init(char *fname);
	int close();

	int printPageDetails	 (CNode *head, CNode *tail);

	int printTextRegionDetails(CNode *head, CNode *tail);
	int printTextLineDetails (CNode *head, CNode *tail);
	int printTextWordDetails (CNode *head, CNode *tail);
	int printCharacterDetails(CNode *head, CNode *tail);

	int printLineDetails	 (CNode *head, CNode *tail);
	int printImageDetails	 (CNode *head, CNode *tail);
	int printNoiseDetails	 (CNode *head, CNode *tail);
};

#endif /* COMPONENTMAJORORDERFILE_H_ */
