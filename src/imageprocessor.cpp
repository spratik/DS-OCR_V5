/*
 * imageprocessor.cpp
 *
 *  Created on: Mar 15, 2016
 *  Author: Soumyadeep Dhar
 */

#include <datatype.h>
#include <stdlib.h>
#include "imageprocessor.h"

#define IMAGE_PRINT_IP 	1

int ImageProcessor :: allocate
(int r, int c, unsigned char *data) {

	int rs;
	row = r;
	col = c;

	// Allocate and get the data from buffer
	// Store individual R,G and B data to respective channel_R,G,B;
	rs = this->image.allocate(r, c, data);
	if(rs != Success) return rs;

	rs = this->ca.allocate(r, c);
	if(rs != Success) return rs;

	return rs;
}
int ImageProcessor :: release() {

	this->image.release();
	this->ca.release();

	return Success;
}

int ImageProcessor :: processImage(char *infname, char *ocrfname1, char *ocrfname2, char *stringList, char *outfname) {

#if IMAGE_PRINT_IP
	char fn[200];
#endif

	/// Analyze components to get document information
	ca.analyzeComponents(infname, ocrfname1, ocrfname2, stringList, outfname);

	/// Print component details as described in APIs
	ca.generateComponentOutput(outfname);

#if IMAGE_PRINT_IP
	/// Print detected components bounding rectangles
	ca.printBoundingRectangles(image);

	/// Write as a image file
	sprintf(fn, "%s_MAP.ppm", outfname);
	image.writePPM(fn);
#endif

	return Success;
}
