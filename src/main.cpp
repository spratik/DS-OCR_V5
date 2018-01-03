#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

#include "Magick++.h"
#include "datatype.h"
#include "enum.h"
#include "imageprocessor.h"
#include "wrapperLMDB.h"
#include <map>
using namespace std;

#define MAX_FILE_NAME 1024

using namespace Magick;

static char *readfname(char *fname)
{
	static char filename[MAX_FILE_NAME];
	char *tmp = NULL;
	int i = 0;
	int len = 0;

	int fname_len = strlen(fname);
	len = strlen(fname);
#ifdef __linux__
	for (i = len; i > 0 && fname[i] != '/'; i--);
#else
	for (i = len; i > 0 && fname[i] != '\\'; i--);
#endif
	tmp = fname + (i > 0 ? i + 1 : 0);

	len = 0;

	while(fname_len > len)
	{
		if(tmp[len] == '\0')
			break;
		if(tmp[len] == '.') {
			if ((len + 3) < (fname_len-1)) {
				if( (tmp[len+1] == 'p') &&
					(tmp[len+2] == 'n') &&
					(tmp[len+3] == 'g'))
					break;
			}

		}
		len++;
	}
	memset(filename, 0x0, MAX_FILE_NAME);
	memcpy(filename, tmp, len);

	return filename;
}

void readImagedata(Magick::Image &image, unsigned char *&databuffer) {

	Magick::ColorGray colr;
	Magick::Pixels view(image);


	Magick::PixelPacket* pixels;
	int rows = image.rows();
	int cols = image.columns();
	pixels = view.get(0, 0, cols, rows);
	int i, j, index, offset, offset1, offset2, offset3;

	/// Determine ofsets
	offset1 = 0;
	offset2 = rows * cols;
	offset3 = 2 * offset2;

	/// Read data
	for(i = 0 ; i < rows; i++){
		offset = i * cols;
		for(j = 0 ; j < cols; j++){
			/// Get pixel offset
			index = offset + j;
			colr = *(pixels + index);

			/// Get image data as RGB planes
			databuffer[offset1 + index] = (unsigned char)(colr.redQuantum()   >> 8);
			databuffer[offset2 + index] = (unsigned char)(colr.greenQuantum() >> 8);
			databuffer[offset3 + index] = (unsigned char)(colr.blueQuantum()  >> 8);
		}
	}
}

int main(int argc, char *argv[])
{
	Magick::Image inputimage;

	// Check input parameters
	if (argc < 7) {
		printf("Usage :  \n[0]%s "
						"\n[1]<pngfile> "
						"\n[2]<Image-Sieve result file1> "
						"\n[3]<input text file1> "
						"\n[4]<input text file2> "
						"\n[5]<string list file2> "
						"\n[6]<output path>\n", argv[0]);
		return TError;
	}

	// Read input file
	try {
		inputimage.read(argv[1]);
	}
	catch(Exception	&error_){
		printf("MOCR : Image read Caught exception: %s\n", error_.what());
		return TError;
	}

	// Create output directory
	char cmd[1000];
	sprintf(cmd, "mkdir %s", argv[argc - 1]);
	system(cmd);

	char *fname = readfname(argv[1]);
	char filename[200];
#ifdef __linux__
	sprintf(filename,"%s/%s.txt", argv[argc - 1], fname);
#else
	sprintf(filename,"%s\\%s.txt", argv[argc - 1], fname);
#endif
	std :: cout << std :: endl ;
	std :: cout << "filename: " << filename << std :: endl;
	unsigned char *databuffer = (unsigned char *)
		calloc(inputimage.rows() * inputimage.columns() * 3, sizeof(unsigned char));

	/// Check allocation status
	if(databuffer == NULL) {
		return MemoryOverFlow;
	}

	/// Read image data
	readImagedata(inputimage, databuffer);

	/// Allocate image processor module
	ImageProcessor *ip = new ImageProcessor();
	if(ip != NULL) {
		/// Allocate internal data structure for image processor
		if(ip->allocate(inputimage.rows(), inputimage.columns(), databuffer) == Success) {

			/// Process image data and get output in text file
			ip->processImage(argv[2], argv[3], argv[4], argv[5], filename);
		}

		/// Release internal memory for image processor
		ip->release();

		/// Release image processor data
		delete ip;
	}

	/*string fileName =  "/root/HeliosWorkspase_copy/DS-OCR_V5/out/test1";

	WrapperLMDB obj("/root/HeliosWorkspase_copy/DS-OCR_V5/out/test1", READ_ONLY);

	map <string, string> readFromLmdb;
	//map <string, string> :: iterator it;
	obj.Batch_read( readFromLmdb );

	for(auto it = readFromLmdb.begin(); it != readFromLmdb.end(); it++){
		cout << it->second << endl;
	}
*/

	/// Release input data buffer
	free(databuffer);

	return Success;
}


