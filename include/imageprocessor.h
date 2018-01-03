/*
 * imageprocessor.h
 *
 *  Created on: Mar 16, 2016
 *      Author: root
 */

#ifndef IMAGEPROCESSOR_H_
#define IMAGEPROCESSOR_H_

#include "tdata.h"
#include "timage.h"

#include "componentanalyzer.h"

#define REQUIRED_INTDATA_BUF	6

class ImageProcessor
{
	int row;
	int col;

	UCharImage image;

	ComponentAnalyzer ca;

public:
	ImageProcessor() {};
	~ImageProcessor() {};

	int allocate(int r, int c, unsigned char *data);
	int release();
	int processImage(char *infname, char *ocrfname1, char *ocrfname2, char *stringList, char *outfname);
};

#endif /* IMAGEPROCESSOR_H_ */
