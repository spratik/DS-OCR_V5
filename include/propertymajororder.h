/*
 * propertymajororder.h
 *
 *  Created on: Apr 29, 2016
 *  Author: Soumyadeep
 */

#ifndef PROPERTYMAJORORDER_H_
#define PROPERTYMAJORORDER_H_

#include "componentsdetailsprinter.h"

class PropertyMajorOrder : public ComponentDetailsPrinter
{

public:
	PropertyMajorOrder() {}
	virtual ~PropertyMajorOrder() {}

	int initOutFile(char *fname);
	int closeOutFile();

	int printPageDetails	 (Rectangle rect, int &getId);

	int printTextRegionDetails(CNode *head, CNode *tail,  int parentId);
	int printTextLineDetails (CNode *head, CNode *tail, int pid);
	int printTextWordDetails (CNode *head, CNode *tail, int pid);
	int printCharacterDetails(CNode *head, CNode *tail, int pid);

	int printLineDetails	 (CNode *head, CNode *tail, int pid);

	int printImageDetails	 (CNode *head, CNode *tail, int pid);
	int printSubImageDetails (CNode *head, CNode *tail, int pid);

	int printNoiseDetails	 (CNode *head, CNode *tail, int pid);
};

#endif /* PROPERTYMAJORORDER_H_ */
