#ifndef _DEFINES_H_
#define _DEFINES_H_
#include <cmath>

#define INVALID_INDEX			-1

#define ROTATION_TOLLERENCE		0.0005F
#define COMPONENT_BUF			2048
#define MAX_STRING_COUNT		16384
#define MAX_HASH_LABLE			3
#define STRING_BUF				64
#define MAX_WORD_SUGGESTION		7


#define HASH_INITIAL_LEN 		128
#define ASCII_RANGE				128

#define MAX_NOISE_AREA	 16
#define MAX_NOISE_HEIGHT 4
#define MAX_NOISE_WIDTH  4

#define MIN_NIMG_HEIGHT_FACTOR 0.05
#define MIN_NIMG_WIDTH_FACTOR  0.05

#define MIN_CHARECTER_DIMENSION	5
#define MIN_CHARECTER_HEIGHT 5
#define MIN_CHARECTER_WIDTH  5

#define MAX_CHARECTER_DENSITY	50

#ifndef TROUND
#define TROUND(x) floor(x + 0.5F)
#endif

#ifndef BOUND
#define BOUND(x, l, h) 			((x) < (l) ? (l) : ((x) > (h) ? (h) : (x)))
#endif

#ifndef ABS
#define ABS(x) 			((x) < 0 ? -(x) : (x))
#endif

#ifndef MAX
#define MAX(a,b) 		((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a,b) 		((a) < (b) ? (a) : (b))
#endif

#ifndef MAX3
#define MAX3(a,b,c)		MAX(MAX((a), (b)), (c))
#endif

#ifndef MIN3
#define MIN3(a,b,c) 	MIN(MIN((a), (b)), (c))
#endif

#ifndef MAX4
#define MAX4(a,b,c,d) 	MAX((MAX((a), (b))), (MAX((c), (d))))
#endif

#ifndef MIN4
#define MIN4(a,b,c,d) 	MIN((MIN((a), (b))), (MIN((c), (d))))
#endif

#endif /* End of _DEFINES_H_ */
