/*
 * textdata.cpp
 *
 *  Created on: Jul 14, 2016
 *  Author: Soumyadeep Dhar
 */

#include <ctype.h>
#include "textdata.h"

bool TextData :: Text :: checkAlpha(unsigned int value)
{
	if((value < ASCII_RANGE && isalnum(value) == false) ||
			value == 0xC2A0 || // ' ' ||
			value == 0xC2B4 || // '´' ||
			(value >= 0xE28098 && value <= 0xE2809B) || // '‘' ||
			value == 0xC2A8   || // '¨' ||
			(value >= 0xE2809C && value <= 0xE2809F) || // '“' ||
			value == 0xC2BF || // '¿' ||
			(value >= 0xE28090 && value <= 0xE28097)) { // '‐' ||
		return false;
	}

	return true;
}



int TextData :: Text :: setDefaultString()
{
	int pos = 0;

	alphastr = (unsigned int *) calloc(2, sizeof(unsigned int));
	if(alphastr == NULL) {
		return MemoryOverFlow;
	}

	/// Head and Tail pointer for component list
	bufHead = &defaultstr;
	bufTail = &defaultstr;

	Tascii *temp = &defaultstr;

	pos = 0;
	while(temp != NULL) {
		if(checkAlpha(temp->value) == true) {
			alphastr[pos] = temp->value;
			pos ++;

			/// Set flag if number present
			if(temp->value < ASCII_RANGE && isdigit(temp->value) == true) {
				nPresent = true;
			}
		}
		temp = temp->next;
	}

	/// Append null at end
	alphastr[pos] = '\0';

	/// Length of strings
	sLength = 1;
	aLength = 1;
	mLength = 0;

	/// After successful addition return success
	return Success;
}

int TextData :: setDefaultString()
{
	int rs = Success;
	if(strCount < maxCount) {

		rs = objects[strCount].setDefaultString();
		if(rs != Success) {
			return rs;
		}

		/// Increase total number of strings
		strCount ++;

		/// After successful addition return success
		return Success;
	}

	return OutOfBoundError;
}

int TextData :: Text :: setString(Tascii *head, Tascii *tail, int slen, int mlen)
{
	int pos = 0;

	alphastr = (unsigned int *) calloc(slen + 1, sizeof(unsigned int));
	if(alphastr == NULL) {
		return MemoryOverFlow;
	}

	/// Head and Tail pointer for component list
	bufHead = head;
	bufTail = tail;

	Tascii *temp = head;

	pos = 0;
	while(temp != tail->next) {
		if(checkAlpha(temp->value) == true) {
			alphastr[pos] = temp->value;
			pos ++;

			/// Set flag if number present
			if(temp->value < ASCII_RANGE && isdigit(temp->value) == true) {
				nPresent = true;
			}
		}
		temp = temp->next;
	}

	/// Append null at end
	alphastr[pos] = '\0';

	/// Length of strings
	sLength = slen;
	aLength = pos;
	mLength = mlen;

	/// After successful addition return success
	return Success;
}

int TextData :: setString(Tascii *head, Tascii *tail, int slen, int mLen)
{
	int rs = Success;
	if(strCount < maxCount) {

		rs = objects[strCount].setString(head, tail, slen, mLen);
		if(rs != Success) {
			return rs;
		}

		/// Increase total number of strings
		strCount ++;

		/// After successful addition return success
		return Success;
	}

	return OutOfBoundError;
}

int TextData :: replaceStringAt(int position, Tascii *head, Tascii *tail, int slen, int mlen)
{
	int rs = Success;
	if(position < maxCount) {

		/// Clear previous data
		objects[position].clear();

		/// Set new object
		rs = objects[position].setString(head, tail, slen, mlen);
		if(rs != Success) {
			return rs;
		}

		/// After successful addition return success
		return Success;
	}

	return OutOfBoundError;
}

void TextData :: Text :: getAlphaString(unsigned int *&base, int &length)
{
	/// Set pointers
	base   = alphastr;
	length = aLength;
}

void TextData :: getAlphaStringAt(int position, unsigned int *&base, int &length)
{
	if(position >= 0 && position < maxCount) {
		objects[position].getAlphaString(base, length);
	} else {
		base = NULL;
		length = 0;
	}
}


void TextData :: Text :: getString(Tascii *&h, Tascii *&t, int &length)
{
	/// Set pointers
	h = bufHead;
	t = bufTail;
	length = sLength;
}

int TextData :: getStringAt(int position, Tascii *&h, Tascii *&t, int &length)
{
	int rs = Success;
	if(position >= 0 && position < maxCount) {
		objects[position].getString(h, t, length);
	} else {
		h = t = NULL;
		length = 0;
		rs = OutOfBoundError;
	}
	return rs;
}

int TextData :: Text :: getMatchedLength()
{
	/// Set pointers
		return mLength;
}

int TextData :: getMatchedLengthAt(int position, int &length)
{
	int rs = Success;
	if(position >= 0 && position < maxCount) {
		length = objects[position].getMatchedLength();
	} else {
		rs = OutOfBoundError;
	}
	return rs;
}


int TextData :: Text :: getLength()
{
	/// Set pointers
		return mLength;
}

int TextData :: getLengthAt(int position, int &length)
{
	int rs = Success;
	if(position >= 0 && position < maxCount) {
		length = objects[position].getLength();
	} else {
		rs = OutOfBoundError;
	}
	return rs;
}


bool TextData :: Text :: isdigitPresent()
{
	return nPresent;
}

bool TextData :: isdigitPresentAtString(int position)
{
	if(position >= 0 && position < maxCount) {
		return objects[position].isdigitPresent();
	} else {
		return false;
	}
}


void TextData :: Text ::  clear() {
	/// Head and Tail pointer for component list
	bufHead = bufTail = NULL;

	/// Strings without non alpha characters
	if(alphastr != NULL) {
		free(alphastr);
		alphastr = NULL;
	}

	/// Length of string and alpha string
	sLength = aLength = 0;

	/// flag to identify contains number of not
	nPresent = false;
}

void TextData :: clearAll() {
	/// Set null for all strings
	for (int index = 0; index < maxCount; index++) {
		objects[index].clear();
	}

	/// Set total internal string as 0
	strCount = 0;
}



void TextData :: swap(int position1, int position2)
{
	Text temp;
	if( position1 >= 0 && position1 < maxCount &&
		position2 >= 0 && position2 < maxCount) {
		temp = objects[position1];
		objects[position1] = objects[position2];
	    objects[position2] = temp;
	}
}


int TextData :: allocate(int numString)
{
	/// Total number of string(s)
	strCount = 0;
	maxCount = numString;

	/// Head and Tail pointer for component list
	/// which can hold pointers of strings
	objects = new Text[maxCount];
	if(objects == NULL) {
		return MemoryOverFlow;
	}

	/// Return success for successful allocation
	return Success;
}


int TextData :: release()
{
	/// Free objects
	if(objects != NULL) {
		delete [] objects;
		objects = NULL;
	}

	/// Return success for successful allocation
	return Success;

}

/// Total number of strings
int TextData :: getStringCount() {
	return strCount;
}

