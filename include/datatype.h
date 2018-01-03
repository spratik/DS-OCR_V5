/*
 * datatype.h
 *
 *  Created on: Mar 15, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_

#include "enum.h"
#include "hashmap.h"

typedef struct _rectangle_ {

	/// Top row of rectangle
	int top;

	/// Bottom row of rectangle
	int bottom;

	/// Left column of rectangle
	int left;

	/// Right column of rectangle
	int right;

	_rectangle_& operator = (int val) {
		top = bottom = left = right = val;
		return *this;
	}

	_rectangle_& operator = (_rectangle_ rect) {
		top = rect.top;
		bottom = rect.bottom;
		left = rect.left;
		right = rect.right;
		return *this;
	}
} Rectangle;

typedef struct _point2D_ {

	/// 'x' in image coordinate starts from top, left (0, 0)
	int row;

	/// 'y' in image coordinate starts from top, left (0, 0)
	int col;

	_point2D_& operator = (int val) {
		row = col = val;
		return *this;
	}
} Point2D;


typedef struct _boundary_ {

	/// Top, left of boundary
	Point2D topleft;

	/// Top, right of boundary
	Point2D topright;

	/// Bottom, left of boundary
	Point2D bottomleft;

	/// Bottom, Right, bottom of boundary
	Point2D bottomright;

	_boundary_& operator = (int val) {
		topleft = topright = bottomleft = bottomright = 0;
		return *this;
	}

} Boundary;

//typedef struct properties {
//	/// Component foreground color
//	int fr, fg, fb;
//
//	/// Component background color
//	int br, bg, bb;
//
//	/// Component Style information
//	int style;
//
//	/// Component underline information
//	bool isUnderLine;
//
//	/// Component strike through information
//	bool isStrikeThru;
//
//	/// Component Font size
//	int fontSize;
//
//	/// Component alignment information
//	int alignment;
//
//	/// LineAbove information
//	bool isLineAbove;
//
//	/// LineBelow information
//	bool isLineBelow;
//
//	/// TextAbove information
//	bool isTextAbove;
//
//	/// TextBelow information
//	bool isTextBelow;
//
//	properties& operator = (int val) {
//		/// Component foreground color
//		fr = _PBG;
//		fg = _PBG;
//		fb = _PBG;
//
//		/// Component background color
//		br = _PMAX;
//		bg = _PMAX;
//		bb = _PMAX;
//
//
//		/// Component Style information
//		style = Regular;
//
//		/// Component underline information set default
//		isUnderLine = false;
//
//		/// Component strike through information set default
//		isStrikeThru = false;
//
//		/// Component Font size
//		fontSize = val;
//
//		/// Component alignment information
//		alignment = Justify;
//
//		/// LineAbove information
//		isLineAbove = false;
//
//		/// LineBelow information
//		isLineBelow = false;
//
//		/// TextAbove information
//		isTextAbove  = false;
//
//		/// TextBelow information
//		isTextBelow  = false;
//
//		return *this;
//	}
//
//} Properties;

typedef struct text_ascii {

	/// Value of the text character in unicode
	unsigned int value;

	/// Next character
	struct text_ascii *next;

	text_ascii& operator = (int val) {
		/// Set default value as zero
		value = val;

		/// Set NUll for next element
		next  = NULL;

		return *this;
	}
} Tascii;

typedef struct ocr_data {
	/// Bounding rectangle of the component
	Rectangle brect;

	/// Component Boundary
	Boundary boundary;

	/// Component id
	int marker;

	/// Height of the component
	int cheight;

	/// Width of the component
	int cwidth;

	/// Total character count
	int characterCount;

	/// characters within bounding rectangle child(s)
	Tascii *txhchild, *txtchild;

	ocr_data& operator = (int val) {
		/// Bounding rectangle of the component
		brect = 0;

		/// Component Boundary
		boundary = 0;

		/// Component id
		marker = 0;

		/// Height of the component
		cheight = 0;

		/// Width of the component
		cwidth = 0;

		/// Total character count
		characterCount = 0;

		/// characters within bounding rectangle child(s)
		txhchild = txtchild = NULL;

		return *this;
	}

} OCRData;

//typedef struct component {
//	/// Component is valid or not
//	int matchType;
//
//	/// Classified component_type
//	int ctype;
//
//	/// Area of the component
//	int area;
//
//	/// Bounding rectangle of the component
//	Rectangle brect;
//
//	/// Component Boundary
//	Boundary boundary;
//
//	/// Component parent id
//	int pmarker;
//
//	/// Component id
//	int marker;
//
//	/// Center row of the component
//	int crow;
//
//	/// Center column of the component
//	int ccol;
//
//	/// Height of the component
//	int cheight;
//
//	/// Width of the component
//	int cwidth;
//
//	/// Average height density of the component
//	int avgheight;
//
//	/// Average width density of the component
//	int avgwidth;
//
//	/// Density of the component
//	int cdensity;
//
//	/// Component properties
//	Properties pdata;
//
//	/// Total character count
//	int strCount;
//
//	/// Total character count
//	int characterCount;
//
//	/// characters within bounding rectangle child(s)
//	Tascii *txhchild, *txtchild;
//
//	component& operator = (int val) {
//		/// Component is valid or not
//		matchType = 0;
//
//		/// Classified component_type
//		ctype  = 0;
//
//		/// Area of the component
//		area = 0;
//
//		/// Bounding rectangle of the component
//		brect = 0;
//
//		/// Component Boundary
//		boundary = 0;
//
//		/// Component parent id
//		pmarker = 0;
//
//		/// Component id
//		marker = 0;
//
//		/// Center row of the component
//		crow = 0;
//
//		/// Center column of the component
//		ccol = 0;
//
//		/// Height of the component
//		cheight = 0;
//
//		/// Width of the component
//		cwidth = 0;
//
//		/// Average height density of the component
//		avgheight = 0;
//
//		/// Average width density of the component
//		avgwidth = 0;
//
//		/// Density of the component
//		cdensity = 0;
//
//		/// Component properties
//		pdata = 0;
//
//		/// Total number of strings
//		strCount = 0;
//
//		/// Total character count
//		characterCount = 0;
//
//		/// characters within bounding rectangle child(s)
//		txhchild = txtchild = NULL;
//
//		return *this;
//	}
//
//} Component;

typedef struct string_data {
	/// Head and Tail pointer for text list
	Tascii *tbufHead, *tbufTail;

	/// Length of the string;
	int strlenght;

	/// Next pointer
	struct string_data *next;

	string_data& operator = (int val) {
		/// Head and Tail pointer for text list
		tbufHead = tbufTail = NULL;

		/// Length of the string;
		strlenght = 0;

		/// Next pointer
		next = NULL;

		return *this;
	}
} SData;

template <typename T>
struct component_node {
	/// Data of the component
	T data;

	/// Check assigned or not
	bool isAssigned;

	/// characters within bounding rectangle child(s)
	struct component_node *hchild, *tchild;

	/// Next pointer
	struct component_node *next;

	component_node& operator = (int val) {
		/// Data of the component
		data = 0;

		/// Check assigned or not
		isAssigned = false;

		/// characters within bounding rectangle child(s)
		hchild = tchild = NULL;

		/// Next pointer
		next = NULL;

		return *this;
	}
};

template <typename T>
struct string_node {

	/// Depth of mapping
	int label;

	/// Check assigned or not
	bool ishashed;

	/// all symbols are mapped to unique position
	HashMap < string_node <T> * > stringMap;

	/// Number of strings within this category
	int stringCount;

	/// string with the same literal at the given label index
	T *hchild, *tchild;

	/// Next pointer
	struct string_node *next;

	string_node& operator = (int val) {
		/// Depth of mapping
		label = val;

		/// Check assigned or not
		ishashed = false;

		/// all symbols are mapped to unique position
		stringMap.clearHash();

		/// Number of strings within this category
		stringCount = 0;

		/// string with the same literal at the given label index
		hchild = tchild = (T*)NULL;

		/// Next pointer
		next = NULL;

		return *this;
	}
};

/// Structure to hold ocr data list
typedef struct component_node < OCRData     > COCR;

/// Structure to hold predefined string list
typedef struct string_node    < SData       > SNode;

#endif /* DATATYPE_H_ */
