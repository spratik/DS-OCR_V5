/*
 * component.h
 *
 *  Created on: Jul 12, 2016
 *      Author: root
 */

#ifndef COMPONENT_H_
#define COMPONENT_H_

#include "datatype.h"
#include "textdata.h"
#include "hashmap.h"

class Component
{
	/// Token for the object type
	const char *token;

	/// Sub token for the object type
	const char *subToken;

	/// Scope for the object type
	const char *scope;

	/// Classified component_type
	int ctype;

	/// Area of the component
	int area;

	/// Bounding rectangle of the component
	Rectangle brect;

	/// Component Boundary
	Boundary boundary;

	/// Component parent id
	int pmarker;

	/// Component id
	int marker;

	/// Center row of the component
	int crow;

	/// Center column of the component
	int ccol;

	/// Height of the component
	int cheight;

	/// Width of the component
	int cwidth;

	/// Average height density of the component
	int avgheight;

	/// Average width density of the component
	int avgwidth;

	/// Density of the component
	int cdensity;

public:
	Component(const char *_token, const char *_subToken, const char *_scope) {
		/// Token for the object type
		token = _token;

		/// Sub token for the object type
		subToken = _subToken;

		/// Scope for the object type
		scope = _scope;
	}

	virtual ~Component() {}

	Component& operator = (int val) {
		/// Classified component_type
		ctype  = 0;

		/// Area of the component
		area = 0;

		/// Bounding rectangle of the component
		brect = 0;

		/// Component Boundary
		boundary = 0;

		/// Component parent id
		pmarker = 0;

		/// Component id
		marker = 0;

		/// Center row of the component
		crow = 0;

		/// Center column of the component
		ccol = 0;

		/// Height of the component
		cheight = 0;

		/// Width of the component
		cwidth = 0;

		/// Average height density of the component
		avgheight = 0;

		/// Average width density of the component
		avgwidth = 0;

		/// Density of the component
		cdensity = 0;

		return *this;
	}

	const char *getToken() const {
		return token;
	}

	const char *getSubToken() const {
		return subToken;
	}

	const char *getScope() const {
		return scope;
	}

	int getArea() const {
		return area;
	}

	int getAvgheight() const {
		return avgheight;
	}

	int getAvgwidth() const {
		return avgwidth;
	}

	Boundary getBoundary() const {
		return boundary;
	}

	Rectangle getBrect() const {
		return brect;
	}

	int getCcol() const {
		return ccol;
	}

	int getCdensity() const {
		return cdensity;
	}

	int getCheight() const {
		return cheight;
	}

	int getCrow() const {
		return crow;
	}

	int getCtype() const {
		return ctype;
	}

	int getCwidth() const {
		return cwidth;
	}

	int getMarker() const {
		return marker;
	}

	int getPmarker() const {
		return pmarker;
	}

	void setArea(int _area) {
		area = _area;
	}

	void setAvgheight(int _avgheight) {
		avgheight = _avgheight;
	}

	void setAvgwidth(int _avgwidth) {
		avgwidth = _avgwidth;
	}


	void setBrect(Rectangle _brect) {
		/// Set rectangle
		brect = _brect;

		/// Set boundary information
		boundary.topleft.row  	 = brect.top;
		boundary.topleft.col  	 = brect.left;
		boundary.topright.row 	 = brect.top;
		boundary.topright.col 	 = brect.right;
		boundary.bottomleft.row  = brect.bottom;
		boundary.bottomleft.col  = brect.left;
		boundary.bottomright.row = brect.bottom;
		boundary.bottomright.col = brect.right;

		/// Set other bounding rectangle information
		crow = (brect.top + brect.bottom + 1) >> 1;
		ccol = (brect.left + brect.right + 1) >> 1;

		cheight = brect.bottom - brect.top + 1;
		cwidth  = brect.right - brect.left + 1;
	}

	void setCdensity(int _cdensity) {
		cdensity = _cdensity;
	}

	void setCtype(int _ctype) {
		ctype = _ctype;
	}

	void setMarker(int _marker) {
		marker = _marker;
	}

	void setPmarker(int _pmarker) {
		pmarker = _pmarker;
	}
};


class PageObject : public Component {

	/// Parent ID for the object type
	const char *pid;

public:
	PageObject() : Component("PAGE", "HORIZONTAL", "PAGE") {
		/// Parent ID for the object type
		pid = "NULL";
	}

	virtual ~PageObject() {}

	const char *getParentID() const {
		return pid;
	}
};

class TextRegionObject : public Component {

	/// Parent ID for the object type
	int pid;

	/// Component alignment information
	int alignment;

public:
	TextRegionObject() : Component("CHUNK", "TEXT-REGION", "PAGE") {
		/// Parent ID for the object type
		pid = 0;

		/// Component alignment information
		alignment = Justify;
	}
	virtual ~TextRegionObject() {}
	TextRegionObject& operator = (int val) {

		/// Component alignment information
		alignment = Justify;

		/// return this pointer
		return *this;
	}

	int getParentID() const {
		return pid;
	}

	void setParentID(int _pid){
		pid = _pid;
	}

	int getAlignment() const {
		return alignment;
	}

	const char *getAlignmentName() const {
		switch(alignment )
		{
		case LeftAligned : return "LEFT";
		case RightAligned : return "RIGHT";
		case CenterAligned : return "CENTER";
		default : return "JUSTIFIED";
		}
	}

	void setAlignment(int _alignment){
		alignment = _alignment;
	}
};

class TextLineObject : public Component {
public:
	TextLineObject() : Component("CHUNK", "TEXT-LINE", "CHUNK") {}
	virtual ~TextLineObject() {}
};

class TextWordObject : public Component {

	/// Matching type of dictionary
	int matchType;

	/// Text information
	TextData text;

	int setString(Tascii *&txhchild, Tascii *&txtchild, int &charCount, bool &strPresent, Tascii *head, int count);

	int appendString(Tascii *&txhchild, Tascii *&txtchild, int &charCount, bool &strPresent, Tascii *head, int count);

	int compare(Tascii *a, unsigned int *b, int alength, int blength);
	bool isDictionaryString(HashMap < SNode * > &map, int label, unsigned int *str, int length);
	int getSuggestionList(HashMap < SNode * > &map, int label, unsigned int *str, int length);
public:
	/// OCR result matching flag
	bool strPresent1, strPresent2;

	/// Total character count
	int charCount1, charCount2;

	/// characters within bounding rectangle child(s)
	Tascii *txhchild1, *txtchild1;
	Tascii *txhchild2, *txtchild2;

	TextWordObject() : Component("CHUNK", "TEXT-WORD", "CHUNK") {
		/// Text information
		text.allocate(MAX_WORD_SUGGESTION);

		/// Matching type of dictionary
		matchType = _m1000;
	}
	virtual ~TextWordObject() {
		/// Clear internal data
		clearStrings();

		/// release text information
		text.release();
	}

	TextWordObject& operator = (int val) {
		/// clear OCR results
		clearStrings();

		return *this;
	}

	int getMatchType() const;

	void setMatchType(int _matchType);

	bool isPresentString1() const;

	bool isPresentString2() const;

	int setString1(Tascii *head, int count);

	int appendString1(Tascii *head, int count);

	int setString2(Tascii *head, int count);

	int appendString2(Tascii *head, int count);

	int compare(HashMap < SNode * > &map);

	void clearStrings();

	void printStrings();

	/// Total number of strings
	int getStringCount();

	/// Get a string from a particular position
	int getStringAt(int position, Tascii *&h, Tascii *&t, int &length);

	/// Suggestive words for non matching OCR text
	int getWordSuggestion(HashMap < SNode * > &map);
};

class CharacterObject : public Component {
	/// Component foreground color
	int fr, fg, fb;

	/// Component background color
	int br, bg, bb;

	/// Component Style information
	int style;

	/// Component underline information
	bool underLine;

	/// Component strike through information
	bool strikeThru;

	/// Component Font size
	int fontSize;

	/// LineAbove information
	bool lineAbove;

	/// LineBelow information
	bool lineBelow;

public:
	CharacterObject(const char *a, const char *b, const char *c) : Component(a, b, c) {}
	virtual ~CharacterObject() {}
	CharacterObject& operator = (int val) {
		/// Component foreground color
		fr = _PBG;
		fg = _PBG;
		fb = _PBG;

		/// Component background color
		br = _PMAX;
		bg = _PMAX;
		bb = _PMAX;


		/// Component Style information
		style = Regular;

		/// Component underline information set default
		underLine = false;

		/// Component strike through information set default
		strikeThru = false;

		/// Component Font size
		fontSize = val;

		/// LineAbove information
		lineAbove = false;

		/// LineBelow information
		lineBelow = false;

		return *this;
	}


	int getFGRed() const {
		return fr;
	}

	int getFGGreen() const {
		return fg;
	}

	int getFGBlue() const {
		return fb;
	}

	int getBGRed() const {
		return br;
	}

	int getBGGreen() const {
		return bg;
	}

	int getBGBlue() const {
		return bb;
	}

	void setFGRed(int _r) {
		fr = _r;
	}
	void setFGGreen(int _g) {
		fg = _g;
	}

	void setFGBlue(int _b) {
		fb = _b;
	}

	void setBGRed(int _r) {
		br = _r;
	}

	void setBGGreen(int _g) {
		bg = _g;
	}

	void setBGBlue(int _b) {
		bb = _b;
	}

	int getFontSize() const {
		return fontSize;
	}

	bool getLineAbove() const {
		return lineAbove;
	}

	bool getLineBelow() const {
		return lineBelow;
	}

	bool getStrikeThru() const {
		return strikeThru;
	}

	bool getUnderLine() const {
		return underLine;
	}

	int getStyle() const {
		return style;
	}

	const char *getStyleName() const {
		/// Get STYLE informations
		switch(style)
		{
			case Bold : return "BOLD";
			case Italic : return "ITALIC";
			case BoldItalic : return "BOLDITALIC";
			default : return "REGULAR";
		}
	}

	void setFontSize(int _fontSize){
		fontSize = _fontSize;
	}

	void setIsLineAbove(bool _lineAbove) {
		lineAbove = _lineAbove;
	}

	void setIsLineBelow(bool _lineBelow){
		lineBelow = _lineBelow;
	}

	void setIsStrikeThru(bool _strikeThru) {
		strikeThru = _strikeThru;
	}

	void setIsUnderLine(bool _underLine) {
		underLine = _underLine;
	}
	void setStyle(int _style) {
		style = _style;
	}
};

class TextCharacterObject : public CharacterObject {
public:
	TextCharacterObject() : CharacterObject("CHARACTER", "TEXT", "CHUNK") {}
	virtual ~TextCharacterObject() {}

};

class TextAssumedCharacterObject : public CharacterObject {
public:
	TextAssumedCharacterObject() : CharacterObject("CHARACTER", "ASSUMED-TEXT", "CHUNK") {}
	virtual ~TextAssumedCharacterObject() {}
};

class BlankSpaceObject : public Component {
public:
	BlankSpaceObject() : Component("NULL", "NULL", "NULL") {}
	virtual ~BlankSpaceObject() {}
};



class LineObject : public Component {
	/// Component Font size
	int size;

	/// TextAbove information
	bool textAbove;

	/// TextBelow information
	bool textBelow;

public:
	LineObject()  : Component("LINE", "LINE", "PAGE") {}
	LineObject(const char *a, const char *b, const char *c) : Component(a, b, c) {}
	virtual ~LineObject() {}
	LineObject& operator = (int val) {
		/// Component Font size
		size = val;

		/// TextAbove information
		textAbove  = false;

		/// TextBelow information
		textBelow  = false;

		return *this;
	}

	bool getTextAbove() const
	{
		return textAbove;
	}

	bool getTextBelow() const
	{
		return textBelow;
	}

	int getSize() const
	{
		return size;
	}

	void setTextAbove(bool _textAbove)
	{
		textAbove = _textAbove;
	}

	void setTextBelow(bool _textBelow)
	{
		textBelow = _textBelow;
	}

	void setSize(int _size)
	{
		size = _size;
	}

};


class HorizontalLineObject : public LineObject {
public:
	HorizontalLineObject()  : LineObject("LINE", "HORIZONTAL", "PAGE") {}
	virtual ~HorizontalLineObject() {}
};

class VerticalLineObject : public LineObject {
public:
	VerticalLineObject() : LineObject("LINE", "VERTICAL", "PAGE") {}
	virtual ~VerticalLineObject() {}
};




class ImageObject : public Component {
public:
	ImageObject() : Component("IMAGE", "IMAGE", "PAGE") {}
	ImageObject(const char *a, const char *b, const char *c) : Component(a, b, c) {}
	virtual ~ImageObject() {}
};

class SubImageObject : public ImageObject {
public:
	SubImageObject() : ImageObject("IMAGE", "SUB-IMAGE", "PAGE") {}
	virtual ~SubImageObject() {}
};




class NoiseObject : public Component {
public:
	NoiseObject() : Component("NOISE", "NOISE", "PAGE") {}
	NoiseObject(const char *a, const char *b, const char *c) : Component(a, b, c) {}
	virtual ~NoiseObject() {}
};

class SmallNoiseObject : public NoiseObject {
public:
	SmallNoiseObject() : NoiseObject("NOISE", "SMALL-NOISE", "PAGE") {}
	virtual ~SmallNoiseObject() {}
};

class LineNoiseObject : public NoiseObject {
public:
	LineNoiseObject() : NoiseObject("NOISE", "LINE-NOISE", "PAGE") {}
	virtual ~LineNoiseObject() {}
};

class ImageNoiseObject : public NoiseObject {
public:
	ImageNoiseObject() : NoiseObject("NOISE", "IMAGE-NOISE", "PAGE") {}
	virtual ~ImageNoiseObject() {}
};


#endif /* COMPONENT_H_ */
