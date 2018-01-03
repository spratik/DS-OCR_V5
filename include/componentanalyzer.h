/*
 * componentanalyzer.h
 *
 *  Created on: Mar 17, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef COMPONENT_ANALYZER_H_
#define COMPONENT_ANALYZER_H_

#include "tdata.h"
#include "timage.h"

#include "componentmajororderfile.h"
#include "componentsimageprinter.h"
#include "componentlist.h"

class ComponentAnalyzer
{
	/// Total allocated component nodes
	int compLimit, textLimit, ocrdataLimit, strNodeLimit, strLimit;

	/// Current number of component
	int compCount, textCount, ocrdataCount, strNodeCount, strCount;

	/// Marker container row, column
	int _mrow, _mcol;


	/// Head and Tail pointer for component list
	CNode *bufHead, *bufTail;

	/// Head and Tail pointer for component list
	Tascii *tbufHead, *tbufTail;

	/// Head and Tail pointer for component list
	COCR  *ocrHead, *ocrTail;

	/// Head and Tail pointer for string node list
	SNode *nstrHead, *nstrTail;

	/// Head and Tail pointer for string list
	SData *sstrHead, *sstrTail;


	/// head & tail Marker for different component
	COCR *ocr1_hdata,   *ocr1_tdata;
	COCR *ocr2_hdata,   *ocr2_tdata;

	/// all symbols are mapped to unique position
	HashMap < SNode * > stringMap;

	/// Page component
	CNode *pg_hdata, *pg_tdata;

	/// Noise component
	CNode *sn_hdata, *sn_tdata;

	/// image component
	CNode *im_hdata, *im_tdata;

	/// text region component
	CNode *tx_hdata, *tx_tdata;

	/// Horizontal line component
	CNode *hl_hdata, *hl_tdata;

	/// Vertical line component
	CNode *vl_hdata, *vl_tdata;


	/// Component details printer to file
	ComponentDetailsPrinter *cdp;

	/// Component bounding rectangle printer to image
	ComponentImagePrinter cip;

	template <typename T> int allocate(T *&head, T *&tail, int &limit);

	template <typename T>
	T* getNew(T *&bhead, T *&btail, T *&head, T *&tail, int &limit, int &count);

	/// Get a new component from component list
	Component* getNewComponent(char *token, char *subtoken);

	/// Release data to internal buffer
	template <typename T>
	void removeComponent(T *&bhead, T *&btail, T *node, int &ccount);

	void removeTextComponent(Tascii *node);
	void releaseStringList(SNode *node);

	template <typename T>
	void releaseComponentList(T *&bhead, T *&btail, T *&head, T *&tail, int &ccount);

	/// Read input component information
	int fprintfUnicode(FILE *fp, unsigned int value);
	unsigned int fscanUnicode(FILE *fp);
	int readPredefinedStrings(char *fname);
	int readOCRComponents(char *infname, COCR *&head, COCR *&tail);
	int readImageSieveComponents(char *infname);

	int getString(Component *&comp);
	int mergeOverlappedSegments(CNode *&head, CNode *&tail, const Rectangle &prect);
	int updateStingInfo();

	int removeEmptySting(CNode *&head, CNode *&tail);
	int discardOverlappedSting();

	void printDictionary(HashMap < SNode * > &map);
	int  allocateHasedElements(HashMap < SNode * > &map, const int label);
	void releaseHasedElements(HashMap < SNode * > &map);
public:
	ComponentAnalyzer() {};
	~ComponentAnalyzer() {};

	int allocate(int r, int c);
	int release();

	int analyzeComponents(char *infname, char *ocrfname1, char *ocrfname2, char *stringList, char *outfname);

	int generateComponentOutput(char *fname);

	void printBoundingRectangles(UCharData &imageR, UCharData &imageG, UCharData &imageB);
	void printBoundingRectangles(UCharImage &image);
};

#endif /* COMPONENT_ANALYZER_H_ */
