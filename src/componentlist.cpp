/*
 * componentlist.cpp
 *
 *  Created on: Jul 12, 2016
 *      Author: root
 */

#include <string.h>
#include "component.h"
#include "componentlist.h"

int CNode :: allocate(CNode *&head, CNode *&tail, int &limit) {

	/// Check already component buffer allocated or not
	if(head != NULL) {
		/// If allocated append new buffer
		for (int index = 0; index < COMPONENT_BUF; index++) {
			tail->next = new CNode();
			if(!tail->next) {
				return MemoryOverFlow;
			}

			/// Move to next node
			tail = tail->next;

			/// Assign null to next of last element
			tail->next = NULL;
		}

		/// Update total node count
		limit += COMPONENT_BUF;
	} else {
		/// Allocate head node
		head = new CNode();
		if(!head) {
			return MemoryOverFlow;
		}

		/// Set null to Next
		head->next = NULL;

		/// Set the head node as tail node
		tail = head;

		/// As not allocated append new nodes to buffer
		for (int index = 0; index < COMPONENT_BUF - 1; index++) {
			tail->next = new CNode();
			if(!tail->next) {
				return MemoryOverFlow;
			}
			/// Move to next node
			tail = tail->next;

			/// Assign null to next of last element
			tail->next = NULL;
		}

		/// Initialize total node count
		limit = COMPONENT_BUF;
	}

#if DEBUG_PRINT_CA
	printf("CNode Limit = %d\n", limit);
	fflush(stdout);
#endif

	return Success;
}

CNode* CNode :: getNew(const int type, CNode *&bhead, CNode *&btail, CNode *&head, CNode *&tail, int &limit, int &count)
{
	/// Check buff has more than one element left
	if(bhead == btail) {
		if(allocate(bhead, btail, limit) != Success) {
			return NULL;
		}
	}

	/// Check any data present in the list or not
	if(tail) {
		/// Get the head element
		tail->next = bhead;

		/// Move the tail to new element
		tail = tail->next;

	} else {
		/// Get the first node from buffer
		head = tail = bhead;
	}


	/// Move buffer head to next
	bhead = bhead->next;

	/// Clear next
	tail->next = NULL;

	/// Increment total component count
	count ++;


	// Set new component
	switch (type) {

	case Page:
		tail->data = new PageObject();
		if(!tail->data) return NULL;
		break;
	case TextRegion:
		tail->data = new TextRegionObject();
		if(!tail->data) return NULL;
		break;
	case TextLine:
		tail->data = new TextLineObject();
		if(!tail->data) return NULL;
		break;
	case TextWord:
		tail->data = new TextWordObject();
		if(!tail->data) return NULL;
		break;
	case TextCharacter:
		tail->data = new TextCharacterObject();
		if(!tail->data) return NULL;
		break;
	case TextAssumedCharacter:
		tail->data = new TextAssumedCharacterObject();
		if(!tail->data) return NULL;
		break;
	case BlankSpace:
		tail->data = new BlankSpaceObject();
		if(!tail->data) return NULL;
		break;
	case Line:
		tail->data = new LineObject();
		if(!tail->data) return NULL;
		break;
	case HorizontalLine:
		tail->data = new HorizontalLineObject();
		if(!tail->data) return NULL;
		break;
	case VerticalLine:
		tail->data = new VerticalLineObject();
		if(!tail->data) return NULL;
		break;
	case Image:
		tail->data = new ImageObject();
		if(!tail->data) return NULL;
		break;
	case SubImage:
		tail->data = new SubImageObject();
		if(!tail->data) return NULL;
		break;
	case Noise:
		tail->data = new NoiseObject();
		if(!tail->data) return NULL;
		break;
	case SmallNoise:
		tail->data = new SmallNoiseObject();
		if(!tail->data) return NULL;
		break;
	case LineNoise:
		tail->data = new LineNoiseObject();
		if(!tail->data) return NULL;
		break;
	case ImageNoise:
		tail->data = new ImageNoiseObject();
		if(!tail->data) return NULL;
		break;

	default:
		tail->data = new Component("NULL", "NULL", "NULL");
		if(!tail->data) return NULL;
		break;
	};


	/// Set default as not assigned
	*tail = 0;

	/// Return the newly added element
	return tail;
}

void CNode :: releaseElementList(CNode *&bhead, CNode *&btail, CNode *&head, CNode *&tail, int &ccount) {
	CNode *temp = NULL;

	while(head) {
		temp = head;
		head = head->next;
		removeElement(bhead, btail, temp, ccount);
	}
	head   = tail   = NULL;
}

void CNode :: removeElement(CNode *&bhead, CNode *&btail, CNode *node, int &ccount)
{
	/// Check node is not null
	if(node) {

		/// Check any data present in the list or not
		if(btail) {
			/// Set as last node
			btail->next = node;

			/// Move the tail to new element
			btail = btail->next;

		} else {
			/// Get the first node from buffer
			bhead = btail = node;
		}

		/// Delete internal data;
		if(btail->data) {
			delete btail->data;
			btail->data = NULL;
		}

		/// Clear child information child(s) if exists
		releaseElementList(bhead, btail, btail->hchild, btail->tchild, ccount);

		/// Mark the empty element next as NULL
		btail->next = NULL;

		/// Decrement total component count
		ccount --;
	}
}
