/*
 * componentlist.h
 *
 *  Created on: Jul 12, 2016
 *      Author: root
 */

#ifndef COMPONENTLIST_H_
#define COMPONENTLIST_H_

#include <stdlib.h>
#include "component.h"

class CNode {
	/// Data of the component
	Component *data;

public:
	/// Check assigned or not
	bool assigned;

	/// characters within bounding rectangle child(s)
	CNode *hchild, *tchild;

	/// Next pointer
	CNode *next;

	CNode() {}
	virtual ~CNode() {}

	CNode& operator = (int val) {
		/// Data of the component
//		*data = 0;

		/// Check assigned or not
		assigned = false;

		/// characters within bounding rectangle child(s)
		hchild = tchild = NULL;

		/// Next pointer
		next = NULL;

		return *this;
	}

	static int allocate(CNode *&head, CNode *&tail, int &limit);
	static void releaseElementList(CNode *&bhead, CNode *&btail, CNode *&head, CNode *&tail, int &ccount);

	static CNode* getNew(const int type, CNode *&bhead, CNode *&btail, CNode *&head, CNode *&tail, int &limit, int &count);
	static void removeElement(CNode *&bhead, CNode *&btail, CNode *node, int &ccount);

	Component*& getData() {
    	return data;
    }
};

#endif /* COMPONENTLIST_H_ */
