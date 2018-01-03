/*
 * enum.h
 *
 *  Created on: Mar 15, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef ENUM_H_
#define ENUM_H_

enum return_type {
	Success = 0,
	TError,
	FileReadError,
	FileWriteError,
	OutOfBoundError,
	InavidParameter,
	MemoryOverFlow,
	MemoryNotAllocated,
	DBInitError

};

enum pixel_type {
	_PBG = 0,
	_PFG,
	_PMARKER,

	_PMAX = 255
};

enum _page_type {
	_Normal = 0,
	_UpSideDown,
	_LeftRotated,
	_RightRotated,

	_Horizontal,
	_Vertical
};

enum _binaryzation {
	NormalOtsu = 0,
	LowerOtsu,
	UpperOtsu,
};

enum component_type {
	Page = 0,
	TextRegion,
	TextLine,
	TextWord,
	TextCharacter,
	TextAssumedCharacter,
	BlankSpace,
	Line,
	HorizontalLine,
	VerticalLine,
	Image,
	SubImage,
	Noise,
	SmallNoise,
	LineNoise,
	ImageNoise
};

enum _style {
	Bold = 0,
	Italic,
	BoldItalic,
	Regular
};

enum _alignment {
	LeftAligned = 0,
	RightAligned,
	CenterAligned,
	Justify
};

enum _string_match {
	/// 'Image-Sieve', 'OCR-A', 'OCR-B' 'Dictionary string matching'

	/// All have single word for a bounding box and
	/// string matched in both OCR and as well as with the dictionary.
	_m1111 = 1,

	/// All have single word for a bounding box and only
	/// OCR(A) string matched with dictionary and
	/// OCR(B) string does not match with dictionary.
	_m111A, // = 2,

	/// All have single word for a bounding box and only
	/// OCR(B) string matched with dictionary and
	/// OCR(A) string does not match with dictionary.
	_m111B, // = 3,

	/// At least one OCR has multiple words for a
	/// bounding box detected by other OCR or Image-
	/// Sieve and string matched in both OCRs and as
	/// well as with the dictionary.
	//_m1MM1, // = 4,

	/// At least one OCR has multiple words for a
	/// bounding box detected by other OCR or Image-
	/// Sieve and only OCR(A) string matched with dictionary
	//_m1MMA, // = 5,

	/// At least one OCR has multiple words for a
	/// bounding box detected by other OCR or Image-
	/// Sieve and only OCR(B) string matched with dictionary
	//_m1MMB, // = 6,

	/// OCR(A) & OCR(B) stings match and they have digits in it
	_m1DD1 = 7,

	/// OCR(A) & OCR(B) stings mismatch and
	/// OCR(B) has digits in it whereas
	/// OCR(A) matches with dictionary
	_m11D1, // = 8,

	/// OCR(A) & OCR(B) stings mismatch and
	/// OCR(A) has digits in it whereas
	/// OCR(B) matches with dictionary
	_m1D11, // = 9,



	/// All have single word for a bounding box and both
	/// string matched with dictionary separately even
	/// individually strings are different.
	_m111X, // = 10,

	/// At least one OCR has multiple words for a
	/// bounding box detected by other OCR or Image-
	/// Sieve and both string matched with dictionary
	/// separately even individually strings are different.
	//_m1MMX, // = 11,

	/// OCR(A) sting present in dictionary where OCR(B)
	/// does not have any string.
	_m1101 = 12,

	/// OCR(B) sting present in dictionary where OCR(A)
	/// does not have any string.
	_m1011, // = 13,



	/// All have single word for a bounding box and
	/// string does not matched with dictionary
	_m1110, // = 14,

	/// OCR(A) sting not present in dictionary where
	/// OCR(B) does not have any string.
	_m1100, // = 15,

	/// OCR(B) sting not present in dictionary where
	/// OCR(A) does not have any string.
	_m1010, // = 16,

	/// OCR(A) has sting and digits in it
	_m1D00, // = 17,

	/// OCR(B) has sting and digits in it
	_m10D0, // = 18,

	/// OCR(A) has sting and digits in it and OCR(B) has string
	/// Both do not matched with dictionary
	_m1D10, // = 19,

	/// OCR(B) has sting and digits in it and OCR(A) has string
	/// Both do not matched with dictionary
	_m11D0, // = 20,

	/// OCR(A) & OCR(B) stings do not match and they have digits in it
	_m1DD0, // = 21,


	/// Image-sieve has bounding box but none of the OCR has string for that
	_m1000, // = 22

	/// Both or any of the OCR has multiple string for the Segment and
	/// None of them matches with dictionary
	//_m1MM0, // = 23,

};

#endif /* ENUM_H_ */
