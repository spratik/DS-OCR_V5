/*
 * componentmajororderlmdb.h
 *
 *  Created on: Jul 20, 2016
 *  Author: Soumyadeep, Cornelious
 */

#ifndef COMPONENTMAJORORDERLMDB_H_
#define COMPONENTMAJORORDERLMDB_H_

#include "componentlist.h"
#include "componentsdetailsprinter.h"
#include "wrapperLMDB.h"

class ComponentMajorOrderLMDB : public ComponentDetailsPrinter
{
	WrapperLMDB *lmdbHandle;
	int fprintfUnicode(FILE *fp, unsigned int value);
public:
	ComponentMajorOrderLMDB() {
		lmdbHandle = NULL;
	}
	virtual ~ComponentMajorOrderLMDB() {}

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

#endif /* ComponentMajorOrderLMDB */
