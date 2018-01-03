/*
 * componentanalyzer.cpp
 *
 *  Created on: Mar 17, 2016
 *  Author: Soumyadeep Dhar
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "tdata.h"
#include "componentanalyzer.h"
#include "componentmajororderfile.h"
#include "componentmajororderlmdb.h"

#define IMAGE_PRINT_CA	0
#define DEBUG_PRINT_CA	0
#define LMDB_ENTRY		1

template <typename T>
int ComponentAnalyzer :: allocate(T *&head, T *&tail, int &limit) {

	/// Check already component buffer allocated or not
	if(head != NULL) {
		/// If allocated append new buffer
		for (int index = 0; index < COMPONENT_BUF; index++) {
			tail->next = (T*) calloc(1, sizeof(T));
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
		head = (T*) calloc(1, sizeof(T));
		if(!head) {
			return MemoryOverFlow;
		}

		/// Set null to Next
		head->next = NULL;

		/// Set the head node as tail node
		tail = head;

		/// As not allocated append new nodes to buffer
		for (int index = 0; index < COMPONENT_BUF - 1; index++) {
			tail->next = (T*) calloc(1, sizeof(T));
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
	printf("Limit = %d\n", limit);
	fflush(stdout);
#endif

	return Success;
}

template <typename T>
T* ComponentAnalyzer :: getNew(T *&bhead, T *&btail, T *&head, T *&tail, int &limit, int &count)
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

	/// Set default as not assigned
	*tail = 0;

	/// Increment total component count
	count ++;

	/// Return the newly added element
	return tail;
}

Component* ComponentAnalyzer :: getNewComponent(char *token, char *subtoken) {

	CNode *cnode = NULL;
	/// Get component list
	if(!strcmp(token, "PAGE")) {
		cnode = CNode :: getNew(Page, bufHead, bufTail, pg_hdata, pg_tdata, compLimit, compCount);
	} else if (!strcmp(token, "CHUNK")) {
		if(!strcmp(subtoken, "TEXT-REGION")) {
			cnode = CNode :: getNew(TextRegion, bufHead, bufTail, tx_hdata, tx_tdata, compLimit, compCount);
		} else if(!strcmp(subtoken, "TEXT-LINE")) {
			cnode = CNode :: getNew(TextLine, bufHead, bufTail, tx_tdata->hchild, tx_tdata->tchild, compLimit, compCount);
		} else if(!strcmp (subtoken, "TEXT-WORD"))  {
			cnode = CNode :: getNew(TextWord, bufHead, bufTail, tx_tdata->tchild->hchild, tx_tdata->tchild->tchild, compLimit, compCount);
		}
	} else if (!strcmp(token, "CHARACTER")) {
		if (!strcmp(subtoken, "TEXT")) {
			cnode = CNode :: getNew(TextCharacter, bufHead, bufTail, tx_tdata->tchild->tchild->hchild, tx_tdata->tchild->tchild->tchild, compLimit, compCount);
		} else if (!strcmp(subtoken, "ASSUMED-TEXT")) {
			cnode = CNode :: getNew(TextAssumedCharacter, bufHead, bufTail, tx_tdata->tchild->tchild->hchild, tx_tdata->tchild->tchild->tchild, compLimit, compCount);
		}
	} else if (!strcmp(token, "LINE")) {
		if(!strcmp(subtoken, "HORIZONTAL")){
			cnode = CNode :: getNew(HorizontalLine, bufHead, bufTail, hl_hdata, hl_tdata, compLimit, compCount);
		} else  if(!strcmp(subtoken, "VERTICAL"))  {
			cnode = CNode :: getNew(VerticalLine, bufHead, bufTail, vl_hdata, vl_tdata, compLimit, compCount);
		}
	} else if (!strcmp(token, "IMAGE")) {
		if (!strcmp(subtoken, "IMAGE")) {
			cnode = CNode :: getNew(Image, bufHead, bufTail, im_hdata, im_tdata, compLimit, compCount);
		} else  if (!strcmp(subtoken, "SUB-IMAGE"))  {
			cnode = CNode :: getNew(SubImage, bufHead, bufTail, im_tdata->hchild, im_tdata->tchild, compLimit, compCount);
		}
	} else  if (!strcmp(token, "NOISE"))  {
		if (!strcmp(subtoken, "LINE")) {
			cnode = CNode :: getNew(LineNoise, bufHead, bufTail, sn_hdata, sn_tdata, compLimit, compCount);
		} else if (!strcmp(subtoken, "IMAGE")) {
			cnode = CNode :: getNew(ImageNoise, bufHead, bufTail, sn_hdata, sn_tdata, compLimit, compCount);
		} else if (!strcmp(subtoken, "SMALL-OBJ")) {
			cnode = CNode :: getNew(SmallNoise, bufHead, bufTail, sn_hdata, sn_tdata, compLimit, compCount);
		}
	}

	return cnode == NULL ? NULL : cnode->getData();
}

int ComponentAnalyzer :: allocate(int r, int c) {

	int rs = Success;
	compLimit = textLimit = ocrdataLimit = strNodeLimit = strLimit = COMPONENT_BUF;
	compCount = textCount = ocrdataCount = strNodeCount = strCount = 0;

	/// Allocate and get the data from buffer
	_mrow = r;
	_mcol = c;

	bufHead = bufTail = NULL;
	rs = CNode :: allocate(bufHead, bufTail, compLimit);
	if(rs != Success) return rs;


	tbufHead = tbufTail = NULL;
	rs = allocate(tbufHead, tbufTail, textLimit);
	if(rs != Success) return rs;

	ocrHead = ocrTail = NULL;
	rs = allocate(ocrHead, ocrTail, ocrdataLimit);
	if(rs != Success) return rs;

	nstrHead = nstrTail = NULL;
	rs = allocate(nstrHead, nstrTail, strNodeLimit);
	if(rs != Success) return rs;

	sstrHead = sstrTail = NULL;
	rs = allocate(sstrHead, sstrTail, strLimit);
	if(rs != Success) return rs;

	/// all symbols are mapped to unique position
	rs = stringMap.initializeHashMap(HASH_INITIAL_LEN);
	if(rs != Success) return rs;

	pg_hdata = pg_tdata = NULL;
	sn_hdata = sn_tdata = NULL;
	im_hdata = im_tdata = NULL;
	tx_hdata = tx_tdata = NULL;
	hl_hdata = hl_tdata = NULL;
	vl_hdata = vl_tdata = NULL;

	ocr1_hdata   = ocr1_tdata   = NULL;
	ocr2_hdata   = ocr2_tdata   = NULL;

	/// Create object based on file writing style
#if LMDB_ENTRY
	cdp = new ComponentMajorOrderLMDB();
#else
	cdp = new ComponentMajorOrderFile();
#endif

	return rs;
}

void ComponentAnalyzer :: releaseHasedElements(HashMap < SNode * > &map)
{
	int i, hashsize;
	SNode *stemp;

	/// Get size of the hash table
	hashsize = map.getSize();

	/// check All elements
	for (i = 0; i < hashsize; i++) {

		/// Get element
		stemp = map.getElementAt(i);

		/// For all valid element
		if(stemp != NULL) {

			/// Clear internal hass
			if(stemp->ishashed) {

				/// Clear internal hash
				releaseHasedElements(stemp->stringMap);
			}
#if DEBUG_PRINT_CA
			/// Test print
			printf("##### Label :: %d(%d) Ecount :: %d\n", stemp->label, i, stemp->stringCount); fflush(stdout);
#endif
			/// Clear all component list information
			releaseStringList(stemp);
		}
	}

	/// Clear string map
	map.release();
}

int ComponentAnalyzer :: release()
{
	CNode  	*ctemp = NULL;
	Tascii 	*ttemp = NULL;
	COCR    *tocr  = NULL;
	SNode	*stemp = NULL;
	SData 	*dtemp = NULL;

	/// Clear string map
	releaseHasedElements(stringMap);

	releaseComponentList(ocrHead, ocrTail, ocr1_hdata, ocr1_tdata, compCount);
	releaseComponentList(ocrHead, ocrTail, ocr2_hdata, ocr2_tdata, compCount);

	/// Clear all component list information
	CNode :: releaseElementList(bufHead, bufTail, pg_hdata, pg_tdata, compCount);
	CNode :: releaseElementList(bufHead, bufTail, sn_hdata, sn_tdata, compCount);
	CNode :: releaseElementList(bufHead, bufTail, im_hdata, im_tdata, compCount);
	CNode :: releaseElementList(bufHead, bufTail, tx_hdata, tx_tdata, compCount);
	CNode :: releaseElementList(bufHead, bufTail, hl_hdata, hl_tdata, compCount);
	CNode :: releaseElementList(bufHead, bufTail, vl_hdata, vl_tdata, compCount);

	/// Clear buffer data
	while(nstrHead) {
		stemp = nstrHead;
		nstrHead = nstrHead->next;

		/// Clear hashmap if allocated
		stemp->stringMap.release();

		free(stemp);
		stemp = NULL;
	}
	nstrHead = nstrTail  = NULL;
	strNodeCount = 0;


	/// Clear buffer data
	while(sstrHead) {
		dtemp = sstrHead;
		sstrHead = sstrHead->next;
		free(dtemp);
	}
	sstrHead = sstrTail  = NULL;
	strCount = 0;


	/// Clear buffer data
	while(bufHead) {
		ctemp = bufHead;
		bufHead = bufHead->next;
		delete ctemp;
	}
	bufHead = bufTail  = NULL;
	compCount = 0;


	/// Clear buffer data
	while(tbufHead) {
		ttemp = tbufHead;
		tbufHead = tbufHead->next;
		free(ttemp);
	}
	tbufHead = tbufTail  = NULL;
	textCount = 0;

	/// Clear ocr buffer data
	while(ocrHead) {
		tocr = ocrHead;
		ocrHead = ocrHead->next;
		free(tocr);
	}
	ocrHead = ocrTail  = NULL;
	ocrdataCount = 0;

	/// Component details printer to file
	if(cdp != NULL) {
		cdp->close();
		delete cdp;
		cdp = NULL;
	}

	return Success;
}

void ComponentAnalyzer :: removeTextComponent(Tascii *node) {

	/// Check node is not null
	if(node) {

		/// Check any data present in the list or not
		if(tbufTail) {
			/// Set as last node
			tbufTail->next = node;

			/// Move the tail to new element
			tbufTail = tbufTail->next;

		} else {
			/// Get the first node from buffer
			tbufHead = tbufTail = node;
		}

		/// Mark the empty element next as NULL
		tbufTail->next = NULL;
	}
}

/// Before calling remove node, it is required to clear all child node information
template <typename T>
void ComponentAnalyzer :: removeComponent(T *&bhead, T *&btail, T *node, int &ccount) {

	Tascii *temp = NULL;
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

		/// Clear text information if text(s) exists
		while(btail->data.txhchild) {
			temp = btail->data.txhchild;
			btail->data.txhchild = btail->data.txhchild->next;
			removeTextComponent(temp);
			btail->data.characterCount --;
		}
		btail->data.txhchild   = btail->data.txtchild   = NULL;

		/// Clear child information child(s) if exists
		releaseComponentList(bhead, btail, btail->hchild, btail->tchild, ccount);

		/// Mark the empty element next as NULL
		btail->next = NULL;

		/// Decrement total component count
		ccount --;
	}
}


void ComponentAnalyzer :: releaseStringList(SNode *node) {
	SData  *temp  = NULL;
	Tascii *ttemp = NULL;
	/// Check for node validity
	if(node != NULL) {
		while(node->hchild) {
			temp = node->hchild;
			node->hchild = node->hchild->next;

			/// Remove all character information
			while(temp->tbufHead) {
				ttemp = temp->tbufHead;
				temp->tbufHead = temp->tbufHead->next;
				removeTextComponent(ttemp);
				temp->strlenght = 0;
			}

			/// Free string information
			/// Check any data present in the list or not
			if(sstrTail) {
				/// Set as last node
				sstrTail->next = temp;

				/// Move the tail to new element
				sstrTail = sstrTail->next;

			} else {
				/// Get the first node from buffer
				sstrHead = sstrTail = temp;
			}

			/// Terminate list
			sstrTail->next = NULL;

			/// Decrement total component count
			strCount --;
			node->stringCount --;
		}

		/// Check any data present in the list or not
		if(nstrTail) {
			/// Set as last node
			nstrTail->next = node;

			/// Move the tail to new element
			nstrTail = nstrTail->next;

		} else {
			/// Get the first node from buffer
			nstrHead = nstrTail = node;
		}

		/// Terminate list
		nstrTail->next = NULL;

		/// Decrement total component count
		strNodeCount --;
	}
}

template <typename T>
void ComponentAnalyzer :: releaseComponentList(T *&bhead, T *&btail, T *&head, T *&tail, int &ccount) {
	T *temp = NULL;

	while(head) {
		temp = head;
		head = head->next;
		removeComponent(bhead, btail, temp, ccount);
	}
	head   = tail   = NULL;
}


int ComponentAnalyzer :: fprintfUnicode(FILE *fp, unsigned int value) {
	if((value >> 24) > 0) {
		return fprintf(fp, "%c%c%c%c",
			(value & 0xFF000000) >> 24,
			(value & 0x00FF0000) >> 16,
			(value & 0x0000FF00) >> 8,
			(value & 0x000000FF));
	} else if((value >> 16) > 0) {
		return fprintf(fp, "%c%c%c",
			(value & 0x00FF0000) >> 16,
			(value & 0x0000FF00) >> 8,
			(value & 0x000000FF));

	} else if((value >> 8) > 0) {
		return fprintf(fp, "%c%c",
			(value & 0x0000FF00) >> 8,
			(value & 0x000000FF));

	} else {
		return fprintf(fp, "%c",
			(value & 0x000000FF));
	}
}

unsigned int ComponentAnalyzer :: fscanUnicode(FILE *fp) {
	int value;
	unsigned int _1st, _2nd, _3rd, _4th;
	value = getc(fp);
	_1st = (unsigned int) value;
	if(value == EOF) {
		return value;
	} else if(((_1st & 0x000000FF) >> 7) == 0x00) {
		return _1st;
	} else if(((_1st & 0x000000FF) >> 3) == 0x1E) {
		value = getc(fp);
		if(value == EOF) {
			return value;
		} else {
			_2nd = (unsigned int) value;
			value = getc(fp);
			if(value == EOF) {
				return value;
			} else {
				_3rd = (unsigned int) value;
				value = getc(fp);
				if(value == EOF) {
					return value;
				} else {
					_4th = (unsigned int) value;
					return (_1st << 24) + (_2nd << 16) + (_3rd << 8) + _4th;
				}
			}
		}
	} else if(((_1st & 0x000000FF) >> 4) == 0x0E) {
		value = getc(fp);
		if(value == EOF) {
			return value;
		} else {
			_2nd = (unsigned int) value;
			value = getc(fp);
			if(value == EOF) {
				return value;
			} else {
				_3rd = (unsigned int) value;
				return (_1st << 16) + (_2nd << 8) + _3rd;
			}
		}
	} else if(((_1st & 0x000000FF) >> 5) == 0x06) {
		value = getc(fp);
		if(value == EOF) {
			return value;
		} else {
			_2nd = (unsigned int) value;
			return (_1st << 8) + _2nd;
		}
	}   else {
		return (unsigned int) value;
	}
}

int ComponentAnalyzer :: readOCRComponents(char *infname, COCR *&head, COCR *&tail)
{
	int rs = Success;
	Rectangle rect;
	OCRData *compInfo = NULL;
	char c;
	int value, id;
	unsigned int data;

	// IntData &image = intbuf[0];
	FILE *fp = fopen(infname, "r+");

	/// If a valid file descriptor
	if(fp) {
		value = 0;
		/// Skip headers
		while(value != EOF && value != '\n') {
			value = getc(fp);
		}

		/// Get component information information
		do {
			/// If no more data present exit
			if(value == EOF) {
				break;
			}
			/// Get object coordinate
			fscanf(fp, "%d%c%d%c%d%c%d%c%d%c", &id, &c, &rect.left, &c, &rect.top, &c, &rect.right, &c, &rect.bottom, &c);

			/// Adjust right bottom
			rect.right  += (rect.left - 1);
			rect.bottom += (rect.top - 1);
#if DEBUG_PRINT_CA
			printf("%d %d %d %d %d \n", id, rect.left, rect.top, rect.right, rect.bottom);
			fflush(stdout);
#endif

			/// Check valid empty component found or not
			if(!getNew(ocrHead, ocrTail, head, tail, ocrdataLimit, ocrdataCount)) {
				return MemoryOverFlow;
			}

			/// Get the store position
			compInfo = &tail->data;

			/// Set component information
			compInfo->brect 		= rect;

			compInfo->boundary.topleft.row  	= rect.top;
			compInfo->boundary.topleft.col  	= rect.left;
			compInfo->boundary.topright.row 	= rect.top;
			compInfo->boundary.topright.col 	= rect.right;
			compInfo->boundary.bottomleft.row   = rect.bottom;
			compInfo->boundary.bottomleft.col   = rect.left;
			compInfo->boundary.bottomright.row  = rect.bottom;
			compInfo->boundary.bottomright.col  = rect.right;

			compInfo->cheight		= rect.bottom - rect.top + 1;
			compInfo->cwidth		= rect.right - rect.left + 1;

			compInfo->marker        = id;

			/// read the OCR string
			do {
				data = fscanUnicode(fp);
				value = (int) data;
				if (value == EOF) {
					break;
				}

				/// Check for line end
				if(data == '\r' || data == '\n') {
					break;
				}

				/// Check valid empty component found or not
				if(!getNew(tbufHead, tbufTail, compInfo->txhchild,
					compInfo->txtchild, textLimit, textCount)) {
					return MemoryOverFlow;
				}


				/// Set the character information
				compInfo->txtchild->value = data;

				/*if(data == '-')
				{
					cout << "- is present" << endl;
				}*/

				/// Increase character count
				compInfo->characterCount ++;
			} while(true);


#if DEBUG_PRINT_CA
			if(compInfo->txhchild) {
				value = 0;
				printf("#");
				for (Tascii *t = compInfo->txhchild; t != compInfo->txtchild->next; t = t->next) {
					if((t->value >> 24) > 0) {
						printf("(%u, %d)%c%c%c%c", t->value, value ++,
							(t->value & 0xFF000000) >> 24,
							(t->value & 0x00FF0000) >> 16,
							(t->value & 0x0000FF00) >> 8,
							(t->value & 0x000000FF));
					} else if((t->value >> 16) > 0) {
						printf("(%u, %d)%c%c%c", t->value, value ++,
							(t->value & 0x00FF0000) >> 16,
							(t->value & 0x0000FF00) >> 8,
							(t->value & 0x000000FF));

					} else if((t->value >> 8) > 0) {
						printf("(%u, %d)%c%c", t->value, value ++,
							(t->value & 0x0000FF00) >> 8,
							(t->value & 0x000000FF));

					} else {
						printf("(%u, %d)%c", t->value, value ++,
							(t->value & 0x000000FF));
					}
				}

				printf("\n");
				fflush(stdout);
			}

#endif

		} while(true);

		/// Close file descriptor
		fclose(fp);
	} else {
		rs = FileReadError;
		printf("Can't open file :: %s\n", infname);
		fflush(stdout);
	}

	return rs;
}


int ComponentAnalyzer :: readImageSieveComponents(char *infname)
{
	int rs = Success;

	Rectangle rect;
	Component *compInfo = NULL;
	int id, value, r, g, b;
	char c, buf[STRING_BUF], buftt[STRING_BUF], buftst[STRING_BUF];

	FILE *fp = fopen(infname, "r+");

	/// If a valid file descriptor
	if(fp) {
		value = 0;
		/// Skip headers
		while(value != EOF && value != '\n') {
			value = getc(fp);
		}

		/// Get component information information
		do {
			/// If no more data present exit
			if(value == EOF) {
				break;
			}

			/// Get object id, token type and token sub type
			fscanf(fp, "%d%c%s%c%s", &id, &c, buftt, &c, buftst);
#if DEBUG_PRINT_CA
			printf("id :: %d Token :: %s Sub-Token :: %s\n", id, buftt, buftst);
			fflush(stdout);
#endif

			/// Get the store position
			/// Here, only one main Structure is there in which all the substructure
			/// pointing to each other.
			/// and again create the sub-structure link list , that is pointing to the main structure
			compInfo = getNewComponent(buftt, buftst);

			if(!compInfo) {
				return MemoryOverFlow;
			}
			fflush(stdout);

			/// Get coordinates
			fscanf(fp, "%d%c%d%c%d%c%d", &rect.left, &c, &rect.top, &c, &rect.right, &c, &rect.bottom);

			/// Adjust right bottom
			/// Don't know,why it is use
			rect.right  += (rect.left - 1);
			rect.bottom += (rect.top - 1);

			/// Set rectangle
			compInfo->setBrect(rect);

#if DEBUG_PRINT_CA
			printf("%d %d %d %d\n", rect.left, rect.top, rect.right, rect.bottom);
			fflush(stdout);
#endif

			/// Set id
			compInfo->setMarker(id);

			/// read the other parameters
			/// Get object parent id information
			fscanf(fp, "%c%s", &c, buf);
			if(!strcmp(buf,"NULL")) {
				compInfo->setPmarker(0);
			} else {
				compInfo->setPmarker(atoi(buf));
			}
#if DEBUG_PRINT_CA
			printf("Parent ID :: %d BUF :: %s\n", compInfo->ggetPmarker(0), buf);
			fflush(stdout);
#endif

			/// Get object FCOLOR information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				 r = g = b = 0;
				 /// Take the input from the buf, and 02% means take two value at a time
				sscanf(buf, "%c%02X%02X%02X", &c, &r, &g, &b);
				((CharacterObject *)compInfo)->setFGRed(r);
				((CharacterObject *)compInfo)->setFGGreen(g);
				((CharacterObject *)compInfo)->setFGBlue(b);

#if DEBUG_PRINT_CA
			printf("FCOLOR   :: #%02X%02X%02X BUF :: %s ", ((CharacterObject *)compInfo)->getFGRed(), ((CharacterObject *)compInfo)->getFGGreen(), ((CharacterObject *)compInfo)->getFGBlue(), buf);
			fflush(stdout);
#endif
			}

			/// Get object BCOLOR information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				r = g = b = 0;
				sscanf(buf, "%c%02X%02X%02X", &c, &r, &g, &b);
				((CharacterObject *)compInfo)->setBGRed(r);
				((CharacterObject *)compInfo)->setBGGreen(g);
				((CharacterObject *)compInfo)->setBGBlue(b);

#if DEBUG_PRINT_CA
				printf("BCOLOR   :: #%02X%02X%02X BUF :: %s", ((CharacterObject *)compInfo)->getBGRed(), ((CharacterObject *)compInfo)->getBGGreen(), ((CharacterObject *)compInfo)->getBGBlue(), buf);
			fflush(stdout);
#endif
			}

			/// Get object STYLE information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"BOLD")) {
					((CharacterObject *)compInfo)->setStyle(Bold);
				} else if(!strcmp(buf,"ITALIC")) {
					((CharacterObject *)compInfo)->setStyle(Italic);
				} else if(!strcmp(buf,"BOLDITALIC")) {
					((CharacterObject *)compInfo)->setStyle(BoldItalic);
				} else {
					((CharacterObject *)compInfo)->setStyle(Regular);
				}
			}
#if DEBUG_PRINT_CA
			printf("STYLE    :: %d BUF :: %s\n", ((CharacterObject *)compInfo)->getStyle(), buf);
			fflush(stdout);
#endif

			/// Get object UNDERLINE information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"YES")) {
					((CharacterObject *)compInfo)->setIsUnderLine(true);
				} else /* if(!strcmp(buf,"NO")) */ {
					((CharacterObject *)compInfo)->setIsUnderLine(false);
				}
			}
#if DEBUG_PRINT_CA
			printf("UNDERLINE    :: %d BUF :: %s\n", ((CharacterObject *)compInfo)->getIsUnderLine(), buf);
			fflush(stdout);
#endif

			/// Get object STRIKETHRU information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"YES")) {
					((CharacterObject *)compInfo)->setIsStrikeThru(true);
				} else /* if(!strcmp(buf,"NO")) */ {
					((CharacterObject *)compInfo)->setIsStrikeThru(false);
				}
			}
#if DEBUG_PRINT_CA
			printf("STRIKETHRU    :: %d BUF :: %s\n", ((CharacterObject *)compInfo)->getIsStrikeThru(), buf);
			fflush(stdout);
#endif

			/// Get object FONT-SIZE information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(compInfo->getToken(), "CHARACTER")) {
					((CharacterObject *)compInfo)->setFontSize(atoi(buf));
				} else /* if(strcmp(compInfo->getToken(), "LINE")) */{
					((LineObject *)compInfo)->setSize(atoi(buf));
				}
			}
#if DEBUG_PRINT_CA
			printf("FONT-SIZE    :: %d BUF :: %s\n", compInfo->pdata.fontSize, buf);
			fflush(stdout);
#endif
			/// Get object ALIGNMENT information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"LEFT")) {
					((TextRegionObject *)compInfo)->setAlignment(LeftAligned);
				} else if(!strcmp(buf,"RIGHT")) {
					((TextRegionObject *)compInfo)->setAlignment(RightAligned);
				} else if(!strcmp(buf,"CENTER")) {
					((TextRegionObject *)compInfo)->setAlignment(CenterAligned);
				} else {
					((TextRegionObject *)compInfo)->setAlignment(Justify);
				}
			}
#if DEBUG_PRINT_CA
			printf("ALIGNMENT    :: %d BUF :: %s\n", compInfo->pdata.alignment, buf);
			fflush(stdout);
#endif
			/// Get object LINE-ABOVE information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"YES")) {
					((CharacterObject *)compInfo)->setIsLineAbove(true);
				} else /* if(!strcmp(buf,"NO")) */ {
					((CharacterObject *)compInfo)->setIsLineAbove(false);
				}
			}
#if DEBUG_PRINT_CA
			printf("LINE-ABOVE    :: %d BUF :: %s\n", compInfo->pdata.isLineAbove, buf);
			fflush(stdout);
#endif

			/// Get object LINE-BELOW information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"YES")) {
					((CharacterObject *)compInfo)->setIsLineBelow(true);
				} else /* if(!strcmp(buf,"NO")) */ {
					((CharacterObject *)compInfo)->setIsLineBelow(false);
				}
			}
#if DEBUG_PRINT_CA
			printf("LINE-BELOW    :: %d BUF :: %s\n", compInfo->pdata.isLineBelow, buf);
			fflush(stdout);
#endif

			/// Get object TEXT-ABOVE information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"YES")) {
					((LineObject *)compInfo)->setTextAbove(true);
				} else /* if(!strcmp(buf,"NO")) */ {
					((LineObject *)compInfo)->setTextAbove(false);
				}
			}
#if DEBUG_PRINT_CA
			printf("TEXT-ABOVE    :: %d BUF :: %s\n", compInfo->pdata.isTextAbove, buf);
			fflush(stdout);
#endif

			/// Get object TEXT-BELOW information
			fscanf(fp, "%c%s", &c, buf);
			if(strcmp(buf,"-")) {
				if(!strcmp(buf,"YES")) {
					((LineObject *)compInfo)->setTextBelow(true);
				} else /* if(!strcmp(buf,"NO")) */ {
					((LineObject *)compInfo)->setTextBelow(false);
				}
			}
#if DEBUG_PRINT_CA
			printf("TEXT-BELOW    :: %d BUF :: %s\n", compInfo->pdata.isTextBelow, buf);
			fflush(stdout);
#endif

			/// Get object SCOPE information
			fscanf(fp, "%c%s", &c, buf);
#if DEBUG_PRINT_CA
			printf("SCOPE    :: %s\n", buf);
			fflush(stdout);
#endif

			value = getc(fp);
		} while(value != EOF);

		/// Close file descriptor
		fclose(fp);
	} else {
		rs = FileReadError;
		printf("Can't open file\n");
		fflush(stdout);
	}

	fflush(stdout);

	return rs;
}

void ComponentAnalyzer :: printDictionary(HashMap < SNode * > &map)
{
	int i, hashsize;
	SNode *stemp;
	SData *temp  = NULL;
	Tascii *txt  = NULL;

	/// Get size of the hash table
	hashsize = map.getSize();

	/// check All elements
	for (i = 0; i < hashsize; i++) {

		/// Get element
		stemp = map.getElementAt(i);

		/// For all valid element
		if(stemp != NULL) {

			/// Clear internal hash
			if(stemp->ishashed) {

				/// Print internal hash
				printDictionary(stemp->stringMap);
			} else {
				FILE *fpd = fopen("dictionary.txt","a+");
				if(!fpd) {
					exit(0);
				}

				/// Print list of words
				for (temp = stemp->hchild; temp !=  stemp->tchild->next; temp = temp->next) {
					for (txt = temp->tbufHead; txt != temp->tbufTail->next; txt = txt->next) {
						if((txt->value >> 24) > 0) {
							fprintf(fpd, "%c%c%c%c",
									(txt->value & 0xFF000000) >> 24,
									(txt->value & 0x00FF0000) >> 16,
									(txt->value & 0x0000FF00) >> 8,
									(txt->value & 0x000000FF));
						} else if((txt->value >> 16) > 0) {
							fprintf(fpd, "%c%c%c",
									(txt->value & 0x00FF0000) >> 16,
									(txt->value & 0x0000FF00) >> 8,
									(txt->value & 0x000000FF));

						} else if((txt->value >> 8) > 0) {
							fprintf(fpd, "%c%c",
									(txt->value & 0x0000FF00) >> 8,
									(txt->value & 0x000000FF));

						} else {
							fprintf(fpd, "%c",
									(txt->value & 0x000000FF));
						}

					}
					fprintf(fpd, "\n");
					fflush(fpd);
				}

				/// Close file descriptor
				fclose(fpd);
			}
		}
	}
}

int ComponentAnalyzer :: getString(Component *&comp)
{
	int rs = Success;
	TextWordObject *temp = (TextWordObject *)comp;
	COCR *ocr1, *ocr2;
	int maxt, minb, maxl, minr, area, minarea;
	Rectangle rect = temp->getBrect();

	/// If OCR1 data found
	if(ocr1_hdata) {
		/// Get mapped strings from OCR1
		/// For all words in OCR1
		for (ocr1 = ocr1_hdata; ocr1 != ocr1_tdata->next; ocr1 = ocr1->next) {

			/// If string exists
			if(ocr1->data.txhchild != NULL) {

				maxt = MAX(rect.top, ocr1->data.brect.top);
				minb = MIN(rect.bottom, ocr1->data.brect.bottom);

				maxl = MAX(rect.left, ocr1->data.brect.left);
				minr = MIN(rect.right, ocr1->data.brect.right);

				if(maxt < minb && maxl < minr) {
					area	 = (minb - maxt) * (minr - maxl);
					minarea  = MIN((temp->getCheight() * temp->getCwidth()),
								   (ocr1->data.cheight * ocr1->data.cwidth));

					/// Check for matching word
					if((area * 2) > minarea) {
						/// Mark previously assigned OCR result found
						if(ocr1->isAssigned == false) {
							/// Set OCR string already assigned
							ocr1->isAssigned = true;

							/// Set OCR1 string to the word
							if(temp->isPresentString1() == false) {
								rs = temp->setString1(ocr1->data.txhchild, ocr1->data.characterCount);
								if(rs != Success) return rs;
							} else {
								rs = temp->appendString1(ocr1->data.txhchild, ocr1->data.characterCount);
								if(rs != Success) return rs;
							}
						}
					}
				}
			}
		}
	}

	/// If OCR1 data found
	if(ocr2_hdata) {
		/// Get mapped strings from OCR1
		/// For all words in OCR1
		for (ocr2 = ocr2_hdata; ocr2 != ocr2_tdata->next; ocr2 = ocr2->next) {

			/// If string exists
			if(ocr2->data.txhchild != NULL) {

				maxt = MAX(rect.top, ocr2->data.brect.top);
				minb = MIN(rect.bottom, ocr2->data.brect.bottom);

				maxl = MAX(rect.left, ocr2->data.brect.left);
				minr = MIN(rect.right, ocr2->data.brect.right);

				if(maxt < minb && maxl < minr) {
					area	 = (minb - maxt) * (minr - maxl);
					minarea  = MIN((temp->getCheight() * temp->getCwidth()),
								   (ocr2->data.cheight * ocr2->data.cwidth));

					/// Check for matching word
					if((area * 2) > minarea) {
						/// Mark previously assigned OCR result found
						if(ocr2->isAssigned == false) {
							/// Set OCR string already assigned
							ocr2->isAssigned = true;

							/// Set OCR2 string to the word
							if(temp->isPresentString2() == false) {
								rs = temp->setString2(ocr2->data.txhchild, ocr2->data.characterCount);
								if(rs != Success) return rs;
							} else {
								rs = temp->appendString2(ocr2->data.txhchild, ocr2->data.characterCount);
								if(rs != Success) return rs;
							}
						}
					}
				}
			}
		}
	}

	/// Compare strings to set matching types
	rs = temp->compare(stringMap);
	if(rs != Success) return rs;

	return Success;
}

/// Merge Overlapped Segments in words
int ComponentAnalyzer :: mergeOverlappedSegments(CNode *&head, CNode *&tail, const Rectangle &prect)
{
	Rectangle rect1, rect2, rect3;
	COCR  *ocr1, *ocr2;
	CNode *temp, *xtemp, *ytemp;
	Component *data, *data1, *data2;
	int maxt, minb, maxl, minr, area, minarea;
	int wordarea, lineArea;

	/// Get parent rectangle area
	lineArea = (prect.bottom - prect.top + 1) * (prect.right - prect.left + 1);

	/// For all words in the line
	for (temp = head; temp != tail->next; temp = temp->next) {

		/// if character are not present in the region do nothing
		if(!temp->hchild) {
			continue;
		}
		/// Get data
		data = (Component *) temp->getData();

		/// Initialize bounding rectangle
		rect1 = rect2 = rect3 = data->getBrect();


		/// If OCR1 data found
		if(ocr1_hdata) {
			/// Get mapped strings from OCR1
			/// For all words in OCR1
			for (ocr1 = ocr1_hdata; ocr1 != ocr1_tdata->next; ocr1 = ocr1->next) {

				/// Get rectangle area
				wordarea = (ocr1->data.brect.bottom - ocr1->data.brect.top  + 1) *
						   (ocr1->data.brect.right  - ocr1->data.brect.left + 1);

				/// If string exists
				if(ocr1->data.txhchild != NULL) {

					/// Check for valid bounding box within line
					if(lineArea > wordarea) {
						maxt = MAX(data->getBrect().top,    ocr1->data.brect.top);
						minb = MIN(data->getBrect().bottom, ocr1->data.brect.bottom);

						maxl = MAX(data->getBrect().left,  ocr1->data.brect.left);
						minr = MIN(data->getBrect().right, ocr1->data.brect.right);

						if(maxt < minb && maxl < minr) {
							area	 = (minb - maxt) * (minr - maxl);
							minarea  = MIN((data->getCheight() * data->getCwidth()),
										   (ocr1->data.cheight * ocr1->data.cwidth));

							/// Check merge required or not
							if((area * 2) > minarea) {
								/// Get Bounding rectangle
								rect1.left   = MIN(rect1.left,     ocr1->data.brect.left);
								rect1.right  = MAX(rect1.right,   ocr1->data.brect.right);
								rect1.top    = MIN(rect1.top, 	    ocr1->data.brect.top);
								rect1.bottom = MAX(rect1.bottom, ocr1->data.brect.bottom);

								rect1.left   = BOUND(rect1.left,   prect.left, prect.right);
								rect1.right  = BOUND(rect1.right,  prect.left, prect.right);
								rect1.top    = BOUND(rect1.top ,   prect.top,  prect.bottom);
								rect1.bottom = BOUND(rect1.bottom, prect.top,  prect.bottom);

							}
						}
					}
				}
			}
		}


		/// If OCR2 data found
		if(ocr2_hdata) {
			/// Get mapped strings from OCR2
			/// For all words in OCR2
			for (ocr2 = ocr2_hdata; ocr2 != ocr2_tdata->next; ocr2 = ocr2->next) {

				/// Get rectangle area
				wordarea = (ocr2->data.brect.bottom - ocr2->data.brect.top  + 1) *
						   (ocr2->data.brect.right  - ocr2->data.brect.left + 1);

				/// If string exists
				if(ocr2->data.txhchild != NULL) {

					/// Check for valid bounding box within line
					if(lineArea > wordarea) {

						maxt = MAX(data->getBrect().top,    ocr2->data.brect.top);
						minb = MIN(data->getBrect().bottom, ocr2->data.brect.bottom);

						maxl = MAX(data->getBrect().left,  ocr2->data.brect.left);
						minr = MIN(data->getBrect().right, ocr2->data.brect.right);

						if(maxt < minb && maxl < minr) {
							area	 = (minb - maxt) * (minr - maxl);
							minarea  = MIN((data->getCheight() * data->getCwidth()),
									(ocr2->data.cheight * ocr2->data.cwidth));

							/// Check merge required or not
							if((area * 2) > minarea) {
								/// Bounding rectangle
								rect2.left   = MIN(rect2.left, 	   ocr2->data.brect.left);
								rect2.right  = MAX(rect2.right,   ocr2->data.brect.right);
								rect2.top    = MIN(rect2.top, 	    ocr2->data.brect.top);
								rect2.bottom = MAX(rect2.bottom, ocr2->data.brect.bottom);

								rect2.left   = BOUND(rect2.left,   prect.left, prect.right);
								rect2.right  = BOUND(rect2.right,  prect.left, prect.right);
								rect2.top    = BOUND(rect2.top,    prect.top,  prect.bottom);
								rect2.bottom = BOUND(rect2.bottom, prect.top,  prect.bottom);
							}
						}
					}
				}
			}
		}

		/// Update bounding rectangle of the component
		rect3.left   = MIN(rect1.left,     rect2.left);
		rect3.right  = MAX(rect1.right,   rect2.right);
		rect3.top    = MIN(rect1.top,  	    rect2.top);
		rect3.bottom = MAX(rect1.bottom, rect2.bottom);

		/// Set new bounding rectangle
		data->setBrect(rect3);
	}

	/// For all words in the line (merge overlap)
	for (xtemp = head; xtemp != tail->next; xtemp = xtemp->next) {
		/// if character are not present in the region do nothing
		if(!xtemp->hchild) {
			continue;
		}

		/// Get data
		data1 = (Component *) xtemp->getData();

		for (ytemp = head; ytemp != tail->next; ytemp = ytemp->next) {
			/// If same component do nothing
			if(xtemp == ytemp) {
				continue;
			}

			/// if character are not present in the region ignore them
			if(!ytemp->hchild) {
				continue;
			}

			/// Get data
			data2 = (Component *) ytemp->getData();

			/// first rectangle
			rect1 = data1->getBrect();
			rect2 = data2->getBrect();

			maxt = MAX(rect1.top, 	 rect2.top);
			minb = MIN(rect1.bottom, rect2.bottom);

			maxl = MAX(rect1.left, 	 rect2.left);
			minr = MIN(rect1.right,  rect2.right);

			if(maxt < minb && maxl < minr) {
				area	 = (minb - maxt) * (minr - maxl);
				minarea  = MIN((data1->getCheight()  * data1->getCwidth()),
							   (data2->getCheight()  * data2->getCwidth()));

				/// If more than 25% overlap found merge them
				if((area * 4) > minarea) {

					/// Bounding rectangle
					rect1.left   = MIN(rect1.left, 	 rect2.left);
					rect1.right  = MAX(rect1.right,  rect2.right);
					rect1.top    = MIN(rect1.top, 	 rect2.top);
					rect1.bottom = MAX(rect1.bottom, rect2.bottom);

					/// Set new bounding rectangle
					data1->setBrect(rect1);

					/// Get character bounding boxes from overlapped component
					xtemp->tchild->next = ytemp->hchild;
					xtemp->tchild 	    = ytemp->tchild;
					ytemp->hchild 		= ytemp->tchild = NULL;
				}
			}
		}
	}

	/// Remove empty string
	removeEmptySting(head, tail);

	return Success;
}


int ComponentAnalyzer :: removeEmptySting(CNode *&head, CNode *&tail)
{
	/// Initialize list variables
	CNode *xtemp, *ytemp, *temp;

	xtemp = ytemp = NULL;
	temp  = head;

	/// Check for valid components
	while(temp != NULL) {
		/// Move head to next child
		head = head->next;

		/// Clear tailing list
		temp->next = NULL;

		/// If the word has inner characters(s)
		if(temp->hchild != NULL) {
			if(xtemp == NULL) {
				xtemp = ytemp = temp;
			} else {
				ytemp->next = temp;
				ytemp = ytemp->next;
			}
		}

		/// Release word
		else {
			/// Release that component
			CNode :: removeElement(bufHead, bufTail, temp, compCount);
		}

		/// Get next element
		temp = head;
	}

	/// Update word list pointers
	head = xtemp;
	tail = ytemp;

	return Success;
}

int ComponentAnalyzer :: discardOverlappedSting()
{
	int rs = Success;
	CNode *rtemp1, *ltemp1, *wtemp1;
	CNode *rtemp2, *ltemp2, *wtemp2;

	Rectangle rect1, rect2;
	Component *data1, *data2;
	int maxt, minb, maxl, minr, cArea;

	/// From detected text components
	/// If regions exists
	if(tx_hdata) {
		/// Search line within all regions
		for (rtemp1 = tx_hdata; rtemp1 != tx_tdata->next; rtemp1 = rtemp1->next) {

			/// if lines are not present in the region do nothing
			if(!rtemp1->hchild) {
				continue;
			}

			/// For all lines within the region
			for (ltemp1 = rtemp1->hchild; ltemp1 != rtemp1->tchild->next; ltemp1 = ltemp1->next) {
				/// if words are not present in the region do nothing
				if(!ltemp1->hchild) {
					continue;
				}

				/// For all words in the line
				for (wtemp1 = ltemp1->hchild; wtemp1 != ltemp1->tchild->next; wtemp1 = wtemp1->next) {

					/// if character are not present in the region do nothing
					if(!wtemp1->hchild) {
						continue;
					}

					/// If empty string ignore
					if( ((TextWordObject *)wtemp1->getData())->isPresentString1() == false &&
						((TextWordObject *)wtemp1->getData())->isPresentString2() == false) {
						continue;
					}

					/// Get data
					data1 = (Component *) wtemp1->getData();

					/// Search line within all regions
					for (rtemp2 = tx_hdata; rtemp2 != tx_tdata->next; rtemp2 = rtemp2->next) {

						/// if lines are not present in the region do nothing
						if(!rtemp2->hchild) {
							continue;
						}

						/// For all lines within the region
						for (ltemp2 = rtemp2->hchild; ltemp2 != rtemp2->tchild->next; ltemp2 = ltemp2->next) {
							/// if words are not present in the region do nothing
							if(!ltemp2->hchild) {
								continue;
							}

							/// For all words in the line
							for (wtemp2 = ltemp2->hchild; wtemp2 != ltemp2->tchild->next; wtemp2 = wtemp2->next) {
								/// if character are not present in the region do nothing
								if(!wtemp2->hchild) {
									continue;
								}

								/// If not a empty string ignore
								if( ((TextWordObject *)wtemp2->getData())->isPresentString1() == true ||
									((TextWordObject *)wtemp2->getData())->isPresentString2() == true) {
									continue;
								}

								/// Get data
								data2 = (Component *) wtemp2->getData();

								/// first rectangle
								rect1 = data1->getBrect();
								rect2 = data2->getBrect();

								maxt = MAX(rect1.top, 	 rect2.top);
								minb = MIN(rect1.bottom, rect2.bottom);

								maxl = MAX(rect1.left -  MAX_NOISE_WIDTH, 	 rect2.left);
								minr = MIN(rect1.right + MAX_NOISE_WIDTH,  rect2.right);

								if(maxt < minb && maxl < minr) {
									cArea	 = (minb - maxt) * (minr - maxl);

									/// If overlap found merge them
									if(cArea > 0) {

										/// Bounding rectangle
										rect1.left   = MIN(rect1.left, 	 rect2.left);
										rect1.right  = MAX(rect1.right,  rect2.right);
										rect1.top    = MIN(rect1.top, 	 rect2.top);
										rect1.bottom = MAX(rect1.bottom, rect2.bottom);

										/// Set new bounding rectangle
										data1->setBrect(rect1);

										if(wtemp1->tchild) {
											/// Get character bounding boxes from overlapped component
											wtemp1->tchild->next = wtemp2->hchild;
											wtemp1->tchild 	     = wtemp2->tchild;
											wtemp2->hchild 		 = wtemp2->tchild = NULL;
										} else {
											wtemp1->hchild 	     = wtemp2->hchild;
											wtemp1->tchild 	     = wtemp2->tchild;
											wtemp2->hchild 		 = wtemp2->tchild = NULL;
										}
									}
								}
							}
						}
					}
				}

				/// Remove empty string
				removeEmptySting(ltemp1->hchild, ltemp1->tchild);
			}
		}
	}



	/// Discard non text overlap regions
	/// If regions exists
	if(tx_hdata) {
		/// Search line within all regions
		for (rtemp1 = tx_hdata; rtemp1 != tx_tdata->next; rtemp1 = rtemp1->next) {

			/// if lines are not present in the region do nothing
			if(!rtemp1->hchild) {
				continue;
			}

			/// For all lines within the region
			for (ltemp1 = rtemp1->hchild; ltemp1 != rtemp1->tchild->next; ltemp1 = ltemp1->next) {
				/// if words are not present in the region do nothing
				if(!ltemp1->hchild) {
					continue;
				}

				/// For all words in the line
				for (wtemp1 = ltemp1->hchild; wtemp1 != ltemp1->tchild->next; wtemp1 = wtemp1->next) {

					/// if character are not present in the region do nothing
					if(!wtemp1->hchild) {
						continue;
					}

					/// If empty string ignore
					if( ((TextWordObject *)wtemp1->getData())->isPresentString1() == true ||
						((TextWordObject *)wtemp1->getData())->isPresentString2() == true) {
						continue;
					}

					/// Get data
					data1 = (Component *) wtemp1->getData();

					/// Search line within all regions
					for (rtemp2 = tx_hdata; rtemp2 != tx_tdata->next; rtemp2 = rtemp2->next) {

						/// if lines are not present in the region do nothing
						if(!rtemp2->hchild) {
							continue;
						}

						/// For all lines within the region
						for (ltemp2 = rtemp2->hchild; ltemp2 != rtemp2->tchild->next; ltemp2 = ltemp2->next) {
							/// if words are not present in the region do nothing
							if(!ltemp2->hchild) {
								continue;
							}

							/// For all words in the line
							for (wtemp2 = ltemp2->hchild; wtemp2 != ltemp2->tchild->next; wtemp2 = wtemp2->next) {
								/// if character are not present in the region do nothing
								if(!wtemp2->hchild) {
									continue;
								}

								/// If not a empty string ignore
								if( ((TextWordObject *)wtemp2->getData())->isPresentString1() == false &&
									((TextWordObject *)wtemp2->getData())->isPresentString2() == false) {
									continue;
								}

								/// Get data
								data2 = (Component *) wtemp2->getData();

								/// first rectangle
								rect1 = data1->getBrect();
								rect2 = data2->getBrect();

								maxt = MAX(rect1.top, 	 rect2.top);
								minb = MIN(rect1.bottom, rect2.bottom);

								maxl = MAX(rect1.left, 	 rect2.left);
								minr = MIN(rect1.right,  rect2.right);

								if(maxt < minb && maxl < minr) {

									/// If component has inner objexts
									if(wtemp2->tchild) {
										/// Get character bounding boxes from overlapped component
										wtemp2->tchild->next = wtemp1->hchild;
										wtemp2->tchild 	     = wtemp1->tchild;
										wtemp1->hchild 		 = wtemp1->tchild = NULL;
									} else {
										wtemp2->hchild = wtemp1->hchild;
										wtemp2->tchild = wtemp1->tchild;
										wtemp1->hchild 		 = wtemp1->tchild = NULL;
									}
								}
							}
						}
					}
				}

				/// Remove empty string
				removeEmptySting(ltemp1->hchild, ltemp1->tchild);
			}
		}
	}
	return rs;
}

int ComponentAnalyzer :: updateStingInfo()
{
	int rs = Success;
	CNode *rtemp, *ltemp, *wtemp;

	/// From detected text components
	/// If regions exists
	if(tx_hdata) {
		/// Search line within all regions
		for (rtemp = tx_hdata; rtemp != tx_tdata->next; rtemp = rtemp->next) {

			/// if lines are not present in the region do nothing
			if(!rtemp->hchild) {
				continue;
			}

			/// For all lines within the region
			for (ltemp = rtemp->hchild; ltemp != rtemp->tchild->next; ltemp = ltemp->next) {
				/// if words are not present in the region do nothing
				if(!ltemp->hchild) {
					continue;
				}

				/// Merge over segmented words based on OCR results
				rs = mergeOverlappedSegments(ltemp->hchild, ltemp->tchild, ltemp->getData()->getBrect());
				if(rs != Success) return rs;

				/// For all words in the line
				for (wtemp = ltemp->hchild; wtemp != ltemp->tchild->next; wtemp = wtemp->next) {
					/// if character are not present in the region do nothing
					if(!wtemp->hchild) {
						continue;
					}

					/// Get OCR strings for the words
					rs = getString(wtemp->getData());
					if(rs != Success) return rs;
				}
			}
		}
	}
	return rs;
}


int ComponentAnalyzer :: allocateHasedElements(HashMap < SNode * > &map, const int label)
{
	int rs = Success;
	int offset, moffset;
	Tascii *txt = NULL;
	SNode  *strlhead = NULL, *strltail = NULL, *strldata = NULL, *slist = NULL;
	SData *temp  = NULL;
	int lcount, size;

	/// Get current size
	size = map.getSize();

	/// For all elements
	for (offset = 0; offset < size; offset++) {
		/// Get hash element
		slist = map.getElementAt(offset);

		/// If its a valid element
		if(slist != NULL) {

			/// Check for max number of strings to create hash
			if(slist->stringCount > MAX_STRING_COUNT && slist->label < MAX_HASH_LABLE) {

				/// Mark as hashed
				slist->ishashed = true;

				/// all symbols are mapped to unique position
				rs = slist->stringMap.initializeHashMap(HASH_INITIAL_LEN);
				if(rs != Success) return rs;

				/// Get head node
				temp = slist->hchild;
				while( temp != NULL) {

					/// Move head to next node
					slist->hchild = slist->hchild->next;

					/// Clear link
					temp->next = NULL;

					/// Get string list offset
					if(temp->strlenght > slist->label) {
						/// Move upto label + 1
						for (txt = temp->tbufHead, lcount = 0; txt != temp->tbufTail && lcount <= slist->label; txt = txt->next, lcount ++);
					} else {
						txt = temp->tbufTail;
					}
					moffset = slist->stringMap.getPosition(tolower(txt->value));

					/// Check validity of offset
					if(moffset == INVALID_INDEX) {
						/// Get new string list
						if(!getNew(nstrHead, nstrTail, strlhead, strltail, strNodeLimit, strNodeCount)) {
							return MemoryOverFlow;
						}

						/// Set label for inner hashed elements
						strltail->label = slist->label + 1;

						// Get array offset for the string
						if( slist->stringMap.addElement(tolower(txt->value), strltail) != Success) {
							return MemoryOverFlow;
						}

						/// Character offset
						moffset = slist->stringMap.getPosition(tolower(txt->value));
						if(moffset == INVALID_INDEX) {
							return InavidParameter;
						}
					}

					/// get the new string position
					strldata = slist->stringMap.getElementAt(moffset);


					/// Set the new string
					if(strldata->hchild == NULL) {
						strldata->hchild = strldata->tchild = temp;
					} else {
						strldata->tchild->next = temp;
						strldata->tchild = strldata->tchild->next;
					}

					/// Increase string count of the list
					strldata->stringCount ++;

					/// Get next node
					temp = slist->hchild;
				}

				/// Set null as it is cleared
				slist->hchild = slist->tchild = NULL;

				/// Create internal hash if required
				rs = allocateHasedElements(slist->stringMap, slist->label + 1);
				if(rs != Success) return rs;
			}
		}
	}

	return rs;
}

int ComponentAnalyzer :: readPredefinedStrings(char *fname)
{
	int rs = Success;
	int value, offset, counter, strcounter;
	unsigned int data;
	Tascii *thead, *ttail;
	SNode  *strlhead, *strltail, *strldata;

	// Open input file descriptor
	FILE *fp = fopen(fname, "r+");

	/// If a valid file descriptor
	if(fp) {
		counter 	= 0;
		strcounter 	= 0;
		thead = ttail = NULL;
		strlhead = strltail = NULL;
		do {
			// Get & Store each string in corresponding file bucket
			// data   = getc(fp);
			data   = fscanUnicode(fp);
			value  = (int) data;

			// If not new line then add character to string
			if(data != '\r') {
				if(value != EOF && data != '\n') {

					/// Check valid empty component found or not
					if(!getNew(tbufHead, tbufTail, thead, ttail, textLimit, textCount)) {
						return MemoryOverFlow;
					}

					/// Set current data
					ttail->value = data;

					/// Increase character count
					counter ++;
				} else {

					/// If string elements presents
					if(thead != NULL) {
						/// Get string list offset
						offset = stringMap.getPosition(tolower(thead->value));

						/// Check validity of offset
						if(offset == INVALID_INDEX) {
							/// Get new string list
							if(!getNew(nstrHead, nstrTail, strlhead, strltail, strNodeLimit, strNodeCount)) {
								return MemoryOverFlow;
							}

							// Get array offset for the string
							if( stringMap.addElement(thead->value, strltail) != Success) {
								return MemoryOverFlow;
							}

							/// Character offset
							offset = stringMap.getPosition(tolower(thead->value));
							if(offset == INVALID_INDEX) {
								return InavidParameter;
							}
						}

						/// Set the new string
						strldata = stringMap.getElementAt(offset);


						/// Get new string
						if(!getNew(sstrHead, sstrTail, strldata->hchild, strldata->tchild, strLimit, strCount)) {
							return MemoryOverFlow;
						}

						/// Store new string
						strldata->tchild->tbufHead  = thead;
						strldata->tchild->tbufTail  = ttail;
						strldata->tchild->strlenght = counter;
						thead = ttail = NULL;

						/// Increase total number of strings
						strldata->stringCount ++;

						/// Set character count to zero
						counter = 0;

						/// Count total number of strings
						strcounter ++;
					} else {
						printf("Total String Read :: %d\n", strcounter); fflush(stdout);
					}
				}
			}

		} while(value != EOF);

		// Close source input file descriptor
		fclose(fp);
	}

	/// Allocate internal hash maps
	rs = allocateHasedElements(stringMap, 0);

	 printDictionary(stringMap);
	return rs;
}


int ComponentAnalyzer :: analyzeComponents(char *infname, char *ocrfname1, char *ocrfname2, char *stringList, char *outfname)
{
#if IMAGE_PRINT_CA
	char fn[200];
#endif

#if	DEBUG_PRINT_CA
	printf("infname :: %s\n", infname); fflush(stdout);
#endif

	/// Initialize return status
	int rs = Success;

	/// Read and store image segmentation component results
	rs = readImageSieveComponents(infname);
	if(rs != Success) return rs;

	/// Read and store input OCR component and related information

	/// First Store relative infromation into the ocr1_hdata to ocr1_tdata
	/// And all the string information store into the singal link list
	/// array of pointer
	rs = readOCRComponents(ocrfname1, ocr1_hdata, ocr1_tdata);
	if(rs != Success) return rs;

	/// Read and store input OCR component and related information
	rs = readOCRComponents(ocrfname2, ocr2_hdata, ocr2_tdata);
	if(rs != Success) return rs;

	/// Read and store predefined string list
	/// read the dictionary
	/// create the hashTable for that dictionary
	rs = readPredefinedStrings(stringList);
	if(rs != Success) return rs;

	/// Set OCR driven strings
	/// merge the string
	rs = updateStingInfo();
	if(rs != Success) return rs;

	rs = discardOverlappedSting();
	if(rs != Success) return rs;

	/// Exit from function
	return rs;
}

void ComponentAnalyzer :: printBoundingRectangles
(UCharData &imageR, UCharData &imageG, UCharData &imageB)
{
	/// Print text informations
	cip.printTextRegionsDetails(tx_hdata, tx_tdata, imageR, imageG, imageB);

//	/// Print OCR1 informations
//	cip.printOCRDetails(ocr1_hdata, ocr1_tdata, imageR, _PMAX + 1, imageG, _PMAX + 1, imageB, 92);

//	/// Print OCR2 informations
//	cip.printOCRDetails(ocr2_hdata, ocr2_tdata, imageR, 92, imageG, _PMAX + 1, imageB, _PMAX + 1);
}

void ComponentAnalyzer :: printBoundingRectangles (UCharImage &image)
{
	UCharData &imageR = image.getchannelR();
	UCharData &imageG = image.getchannelG();
	UCharData &imageB = image.getchannelB();

	/// Print  information
	printBoundingRectangles (imageR, imageG, imageB);
}

int ComponentAnalyzer :: generateComponentOutput(char *fname)
{
	int rs = Success;

	/// Initialize output file
	rs = cdp->init(fname);
	if(rs != Success) return rs;

	rs = cdp->printPageDetails(pg_hdata, pg_tdata);
	if(rs != Success) return rs;

	/// Print text informations
	rs = cdp->printTextRegionDetails(tx_hdata, tx_tdata);
	if(rs != Success) return rs;

	/// Print horizontal line informations
	rs = cdp->printLineDetails(hl_hdata, hl_tdata);
	if(rs != Success) return rs;

	/// Print vertical line informations
	rs = cdp->printLineDetails(vl_hdata, vl_tdata);
	if(rs != Success) return rs;

	/// Print Image informations
	rs = cdp->printImageDetails(im_hdata, im_tdata);
	if(rs != Success) return rs;

	/// Print Noise informations
	rs = cdp->printNoiseDetails(sn_hdata, sn_tdata);
	if(rs != Success) return rs;

	/// Close output file
	rs  = cdp->close();
	if(rs != Success) return rs;

	return Success;
}
