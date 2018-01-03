/*
 * textdata.cpp
 *
 *  Created on: Jul 14, 2016
 *  Author: Soumyadeep Dhar
 */

#include <ctype.h>
#include "datatype.h"
#include "component.h"

#define DEBUG_PRINT_C	0

int TextWordObject :: setString(Tascii *&txhchild, Tascii *&txtchild,
		int &charCount, bool &strPresent, Tascii *head, int count) {
	Tascii *temp = NULL;
	if(count > 0) {
		/// Allocate head node
		txhchild = (Tascii *) calloc(1, sizeof(Tascii *));
		if(!txhchild) {
			return MemoryOverFlow;
		}

		/// Set null to Next
		txhchild->next = NULL;

		/// Set the head node as tail node
		txtchild = txhchild;

		/// As not allocated append new nodes to buffer
		for (int index = 0; index < count - 1; index++) {
			txtchild->next = (Tascii *) calloc(1, sizeof(Tascii *));
			if(!txtchild->next) {
				return MemoryOverFlow;
			}
			/// Move to next node
			txtchild = txtchild->next;

			/// Assign null to next of last element
			txtchild->next = NULL;
		}

		/// Get the head pointer
		temp = txhchild;
		for(int index = 0; index < count; index++) {
			/// Set the value
			temp->value = head->value;

			/// Move to next value
			temp = temp->next;
			head = head->next;
		}

		/// Set character count
		charCount = count;

		/// Set string presence flag
		strPresent = true;
	}
	return Success;
}

int TextWordObject :: appendString(Tascii *&txhchild, Tascii *&txtchild,
		int &charCount, bool &strPresent, Tascii *head, int count) {
	/// Set the current tail pointer
	Tascii *temp = txtchild;
	if(count > 0) {
		/// If allocated append new buffer
		for (int index = 0; index < count; index++) {
			txtchild->next = (Tascii *) calloc(1, sizeof(Tascii *));
			if(!txtchild->next) {
				return MemoryOverFlow;
			}

			/// Move to next node
			txtchild = txtchild->next;

			/// Assign null to next of last element
			txtchild->next = NULL;
		}

		/// Set string
		for(int index = 0; index < count; index++) {
			/// Move to next newly created element value
			temp = temp->next;

			/// Set the value
			temp->value = head->value;

			/// Move to next value
			head = head->next;
		}

		/// Set character count
		charCount += count;

		/// Set string presence flag
		strPresent = true;
	}
	return Success;
}

void TextWordObject :: printStrings() {
	Tascii *temp;

	/// If string 1 present
	if(strPresent1 == true) {
		printf("STR1 :: "); fflush(stdout);
		/// Set the current tail pointer
		temp = txhchild1;

		/// Set string
		for(int index = 0; index < charCount1; index++) {
			/// Print the value
			if((temp->value >> 24) > 0) {
				printf("%c%c%c%c",
						(temp->value & 0xFF000000) >> 24,
						(temp->value & 0x00FF0000) >> 16,
						(temp->value & 0x0000FF00) >> 8,
						(temp->value & 0x000000FF));
			} else if((temp->value >> 16) > 0) {
				printf("%c%c%c",
						(temp->value & 0x00FF0000) >> 16,
						(temp->value & 0x0000FF00) >> 8,
						(temp->value & 0x000000FF));

			} else if((temp->value >> 8) > 0) {
				printf("%c%c",
						(temp->value & 0x0000FF00) >> 8,
						(temp->value & 0x000000FF));

			} else {
				printf("%c",
						(temp->value & 0x000000FF));
			}

			/// Move to next value
			temp = temp->next;
		}
	}


	/// If string 1 present
	if(strPresent2 == true) {
		printf("    STR2 :: "); fflush(stdout);
		/// Set the current tail pointer
		temp = txhchild2;

		/// Set string
		for(int index = 0; index < charCount2; index++) {
			/// Print the value
			if((temp->value >> 24) > 0) {
				printf("%c%c%c%c",
						(temp->value & 0xFF000000) >> 24,
						(temp->value & 0x00FF0000) >> 16,
						(temp->value & 0x0000FF00) >> 8,
						(temp->value & 0x000000FF));
			} else if((temp->value >> 16) > 0) {
				printf("%c%c%c",
						(temp->value & 0x00FF0000) >> 16,
						(temp->value & 0x0000FF00) >> 8,
						(temp->value & 0x000000FF));

			} else if((temp->value >> 8) > 0) {
				printf("%c%c",
						(temp->value & 0x0000FF00) >> 8,
						(temp->value & 0x000000FF));

			} else {
				printf("%c",
						(temp->value & 0x000000FF));
			}

			/// Move to next value
			temp = temp->next;
		}

		printf("\n"); fflush(stdout);
	}
}

void TextWordObject :: clearStrings()
{
	Tascii *temp = NULL;
	/// Remove string 1
	if(strPresent1 == true) {
		/// Clear text information if text(s) exists
		while(txhchild1) {
			temp = txhchild1;
			txhchild1 = txhchild1->next;
			free(temp);
		}
		txhchild1 = txtchild1 = NULL;
	}
	charCount1 = 0;
	strPresent1 = false;

	/// Remove string 2
	if(strPresent2 == true) {
		/// Clear text information if text(s) exists
		while(txhchild2) {
			temp = txhchild2;
			txhchild2 = txhchild2->next;
			free(temp);
		}
		txhchild2 = txtchild2 = NULL;
	}
	charCount2 = 0;
	strPresent2 = false;

	/// Clear all suggested strings
	text.clearAll();

	/// Set default match type
	matchType = _m1000;
}

/// Total number of strings
int TextWordObject :: getStringCount() {
	return text.getStringCount();
}

/// Get a string from a particular position
int TextWordObject :: getStringAt(int position, Tascii *&h, Tascii *&t, int &length){
	return text.getStringAt(position, h, t, length);
}

int TextWordObject :: getMatchType() const {
	return matchType;
}

void TextWordObject :: setMatchType(int _matchType) {
	matchType = _matchType;
}

bool TextWordObject :: isPresentString1() const {
	return strPresent1;
}

bool TextWordObject :: isPresentString2() const {
	return strPresent2;
}

int TextWordObject :: setString1(Tascii *head, int count) {
	return setString(txhchild1, txtchild1, charCount1, strPresent1, head, count);
}

int TextWordObject :: appendString1(Tascii *head, int count) {
	return appendString(txhchild1, txtchild1, charCount1, strPresent1, head, count);
}

int TextWordObject :: setString2(Tascii *head, int count) {
	return setString(txhchild2, txtchild2, charCount2, strPresent2, head, count);
}

int TextWordObject :: appendString2(Tascii *head, int count) {
	return appendString(txhchild2, txtchild2, charCount2, strPresent2, head, count);
}

bool TextWordObject :: isDictionaryString(HashMap < SNode * > &map, int label, unsigned int *str, int length)
{
	SNode *mslist;
	SData *temp  = NULL;
	unsigned int *txt;
	int moffset, lcount;

	/// Get hash character position
	lcount = MIN(label, (length - 1));

	/// initialize variables
	txt = str;

	/// Get string list offset
	while(txt - str < lcount) {
		/// Move to next character
		txt ++;
	}

	if(txt != '\0') {
		/// Find list offset from map
		moffset = map.getPosition(tolower(*txt));
	} else {
		moffset = INVALID_INDEX;
	}

	/// For valid list
	if(moffset != INVALID_INDEX) {

		/// get list pointer
		mslist = map.getElementAt(moffset);

		/// If internal hash present
		if(mslist->ishashed == true) {
			return isDictionaryString(mslist->stringMap, mslist->label + 1, str, length);
		} else {
			/// Search for the string
			for (temp = mslist->hchild; temp !=  mslist->tchild->next; temp = temp->next) {
#if DEBUG_PRINT_C
				if(temp->strlenght <= 2 && length <= 2) {
					for(Tascii *t = temp->tbufHead; t != temp->tbufTail->next; t=t->next) {
						if((t->value >> 24) > 0) {
							printf("%c%c%c%c",
									(t->value & 0xFF000000) >> 24,
									(t->value & 0x00FF0000) >> 16,
									(t->value & 0x0000FF00) >> 8,
									(t->value & 0x000000FF));
						} else if((t->value >> 16) > 0) {
							printf("%c%c%c",
									(t->value & 0x00FF0000) >> 16,
									(t->value & 0x0000FF00) >> 8,
									(t->value & 0x000000FF));

						} else if((t->value >> 8) > 0) {
							printf("%c%c",
									(t->value & 0x0000FF00) >> 8,
									(t->value & 0x000000FF));

						} else {
							printf("%c",
									(t->value & 0x000000FF));
						}
					}
					printf("\t");

					for(unsigned int *t = str; *t != '\0'; t++) {
						if((*t >> 24) > 0) {
							printf("%c%c%c%c",
									(*t & 0xFF000000) >> 24,
									(*t & 0x00FF0000) >> 16,
									(*t & 0x0000FF00) >> 8,
									(*t & 0x000000FF));
						} else if((*t >> 16) > 0) {
							printf("%c%c%c",
									(*t & 0x00FF0000) >> 16,
									(*t & 0x0000FF00) >> 8,
									(*t & 0x000000FF));

						} else if((*t >> 8) > 0) {
							printf("%c%c",
									(*t & 0x0000FF00) >> 8,
									(*t & 0x000000FF));

						} else {
							printf("%c",
									(*t & 0x000000FF));
						}
					}

					printf(" (%d) (%d)\n", temp->strlenght, length);

					fflush(stdout);
				}

#endif
				/// Compare strings
				if(compare(temp->tbufHead, str, temp->strlenght, length) == 0) {
					return true;
				}
			}
		}
	}
	return false;
}


int TextWordObject :: getSuggestionList(HashMap < SNode * > &map, int label, unsigned int *str, int length)
{
	SNode *mslist;
	SData *temp  = NULL;
	Tascii *character;
	unsigned int *txt;
	int index, moffset, strCount, lcount, mlength, slength, value, rs;

	/// Get present string string information
	strCount = this->getStringCount();

	/// Get hash character position
	lcount = MIN(label, length);

	/// initialize variables
	txt = str;

	/// Get string list offset
	while(txt - str < lcount) {
		/// Move to next character
		txt ++;
	}

	if(txt != '\0') {
		/// Find list offset from map
		moffset = map.getPosition(tolower(*txt));
	} else {
		moffset = INVALID_INDEX;
	}

	/// For valid list
	if(moffset != INVALID_INDEX) {

		/// get list pointer
		mslist = map.getElementAt(moffset);

		/// If internal hash present
		if(mslist->ishashed == true) {
			return getSuggestionList(mslist->stringMap, mslist->label + 1, str, length);
		} else {
			/// Search for the string
			for (temp = mslist->hchild; temp !=  mslist->tchild->next; temp = temp->next) {
				value = -1;
				character = temp->tbufHead;
				txt = str;
				while (character != NULL && *txt != '\0') {

					/// Compare characters
					value = tolower(character->value) - tolower(*txt);
					if(value != 0) {
						break;
					}

					/// Move to next character
					character = character->next;
					txt ++;
				}

				/// Add data to string list
				rs = text.setString(temp->tbufHead, temp->tbufTail, temp->strlenght, txt - str);

				/// If list is already full
				if(rs == OutOfBoundError) {

					/// Set default return status
					rs = Success;

					/// Set string if proper position found
					for(index = strCount; index < MAX_WORD_SUGGESTION; index ++) {
						/// Get previously matched string length
						mlength = 0;
						rs = text.getMatchedLengthAt(index, mlength);
						if(rs != Success) return rs;

						rs = text.getMatchedLengthAt(index, slength);
						if(rs != Success) return rs;

						/// If previous length is shorter
						if(mlength < (txt - str) && ABS(temp->strlenght - length) <= ABS(slength - length)) {
							rs = text.replaceStringAt(index, temp->tbufHead, temp->tbufTail, temp->strlenght, txt - str);
							break;
						}
					}
				}

				/// If string set not not successful
				if(rs != Success) {
					return rs;
				}
			}
		}
	}
	return Success;
}

int TextWordObject :: getWordSuggestion(HashMap < SNode * > &map)
{
	int rs = Success;
	unsigned int *baseSTR;;
	int strCount, length;

	/// Get present string string information
	strCount = this->getStringCount();

	/// If valid string present and it does not contain digits
	if(strCount > 0 && text.isdigitPresentAtString(0) == false) {

		/// Get String first string
		baseSTR = NULL;
		text.getAlphaStringAt(0, baseSTR, length);

		if(baseSTR != NULL) {
			rs = getSuggestionList(map, 0, baseSTR, length);
			if(rs != Success) return rs;
		}
	}
	return rs;
}

// Function returns at the occurrence of first dissimilarity
// If first string contains lower utf-8 character then the second string
// in same position then it returns a negative value
// Again if first string contains higher utf-8 character then the second string
// in same position the it returns a positive value else 0
int TextWordObject :: compare(Tascii *a, unsigned int *b, int alength, int blength)
{
	int value = -1;
	while (a != NULL && *b != '\0') {

		/// Compare characters
		value = tolower(a->value) - tolower(*b);
		if(value != 0) {
			return value;
		}

		/// Move to next character
		a = a->next;
		b ++;
	}

	/// Not any valid alpha number present in left string
	if(a == NULL && *b == '\0') {
		return value;
	} else if (a == NULL) {
		return -1;
	} else if (*b == '\0') {
		return 1;
	} else {
		return 0;
	}

	return value;
}

int TextWordObject :: compare(HashMap < SNode * > &map)
{
	int rs;
	bool misMatchFound, tp1, tp2;
	int lengthSTR1, lengthSTR2;
	unsigned int *alphaSTR1, *alphaSTR2;
	Tascii *str1, *str2;

	/// OCR-A has exactly 1 result but OCR-B none
	if(strPresent1 == true && strPresent2 == false) {

		/// Add data to string list
		rs = text.setString(txhchild1, txtchild1, charCount1, charCount1);
		if(rs != Success) { return rs; }

		/// Get alpha string
		alphaSTR1 = NULL; lengthSTR1 = 0;
		text.getAlphaStringAt(0, alphaSTR1, lengthSTR1);

		/// Add second data to string list
		rs = text.setDefaultString();
		if(rs != Success) { return rs; }

		/// Check numeric character present in the string or not
		if(text.isdigitPresentAtString(0) == true) {
			/// Mention digit present at first string
			/// And second OCR string not present
			matchType = _m1D00;
		} else {
			/// If string present in dictionary
			if(isDictionaryString(map, 0, alphaSTR1, lengthSTR1)) {
				/// string matches so assign below type
				matchType = _m1101;
			} else {
				/// string not matches so assign below type
				matchType = _m1100;
			}
		}
	}

	/// OCR-B has exactly 1 result but OCR-A none
	else if(strPresent1 == false && strPresent2 == true) {

		/// Add data to string list
		rs = text.setString(txhchild2, txtchild2, charCount2, charCount2);
		if(rs != Success) { return rs; }

		/// Get alpha string
		alphaSTR2 = NULL; lengthSTR2 = 0;
		text.getAlphaStringAt(0, alphaSTR2, lengthSTR2);

		/// Add second data to string list
		rs = text.setDefaultString();
		if(rs != Success) { return rs; }

		/// Check numeric character present in the string or not
		if(text.isdigitPresentAtString(0) == true) {
			/// Mention digit present at first string
			/// And second OCR string not present
			matchType = _m10D0;
		} else {
			/// If string present in dictionary
			if(isDictionaryString(map, 0, alphaSTR2, lengthSTR2)) {
				/// string matches so assign below type
				matchType = _m1011;
			} else {
				/// string not matches so assign below type
				matchType = _m1010;
			}
		}
	}

	/// Both OCR has result
	else if(strPresent1 == true && strPresent2 == true) {

		/// Add first data to string list
		rs = text.setString(txhchild1, txtchild1, charCount1, charCount1);
		if(rs != Success) { return rs; }

		/// Get alpha string
		alphaSTR1 = NULL; lengthSTR1 = 0;
		text.getAlphaStringAt(0, alphaSTR1, lengthSTR1);

		/// Add second data to string list
		rs = text.setString(txhchild2, txtchild2, charCount2, charCount2);
		if(rs != Success) { return rs; }

		/// Get second alpha string
		alphaSTR2 = NULL; lengthSTR2 = 0;
		text.getAlphaStringAt(1, alphaSTR2, lengthSTR2);

		/// Set matching flag
		misMatchFound = false;

		/// Compare both OCR result
		if(charCount1 == charCount2) {
			/// For all character
			for(str1  = txhchild1,	str2  = txhchild2;
				str1 != txtchild1->next && str2 != txtchild2->next;
				str1  = str1->next,	str2 = str2->next) {

				/// Check for individual character equality
				if(str1->value != str2->value) {

					/// Set mismatch found
					misMatchFound = true;
					break;
				}
			}
		} else {
			/// Set mismatch found
			misMatchFound = true;
		}

		/// If both strings are similar
		if(misMatchFound == false) {
			/// Check numeric character present in the string or not
			if(text.isdigitPresentAtString(0) == true) {
				matchType = _m1DD1;
			} else {
				/// If string present in dictionary
				if(isDictionaryString(map, 0, alphaSTR1, lengthSTR1)) {
					/// string matches so assign below type
					matchType = _m1111;
				} else {
					/// string not matches so assign below type
					matchType = _m1110;
				}
			}
		} else {
			/// Set default values
			tp1 = tp2 = false;

			/// Check numeric character present in the string or not
			if(text.isdigitPresentAtString(0) == false) {
				tp1 = isDictionaryString(map, 0, alphaSTR1, lengthSTR1);
			}
			/// Check numeric character present in the string or not
			if(text.isdigitPresentAtString(1) == false) {
				tp2 = isDictionaryString(map, 0, alphaSTR2, lengthSTR2);
			}

			if(text.isdigitPresentAtString(0) == true && text.isdigitPresentAtString(1) == true) {
				/// Both contains digits
				matchType = _m1DD0;
			} else if(tp1 == true && tp2 == true) {
				/// both present in the dictionary
				matchType = _m111X;
			}	else if(tp1 == true && tp2 == false) {
				if(text.isdigitPresentAtString(1) == true) {
					matchType = _m11D0;
				} else {
					matchType = _m111A;
				}
			} else if(tp1 == false && tp2 == true) {
				if(text.isdigitPresentAtString(0) == true) {
					matchType = _m1D10;
				} else {
					matchType = _m111B;
				}
			} else {
				if(text.isdigitPresentAtString(0) == true) {
					matchType = _m1D10;
				} else if(text.isdigitPresentAtString(1) == true) {
					matchType = _m11D0;
				} else {
					matchType = _m1110;
				}
			}
		}
	}

	/// If no string present
	else /* (strPresent1 == false && strPresent2 == false) */ {
		/// Add first data to string list
		rs = text.setDefaultString();
		if(rs != Success) { return rs; }

		/// Add second data to string list
		rs = text.setDefaultString();
		if(rs != Success) { return rs; }

		/// Image-sieve has bounding box but none of the OCR has string for that
		/// Do nothing as set match not found
		matchType = _m1000;
	}

	/// Based on matched type find word suggestion(s)
	switch(matchType)
	{

	/// 'Image-Sieve', 'OCR-A', 'OCR-B' 'Dictionary string matching'

	/// All have single word for a bounding box and
	/// string matched in both OCR and as well as with the dictionary.
	case _m1111 : // = 1,
		break;
		/// All have single word for a bounding box and only
		/// OCR(A) string matched with dictionary and
		/// OCR(B) string does not match with dictionary.
	case _m111A : // = 2,
		break;
		/// All have single word for a bounding box and only
		/// OCR(B) string matched with dictionary and
		/// OCR(A) string does not match with dictionary.
	case _m111B : // = 3,
		/// Get accurate word at top
		text.swap(0, 1);
		break;
		/// At least one OCR has multiple words for a
		/// bounding box detected by other OCR or Image-
		/// Sieve and string matched in both OCRs and as
		/// well as with the dictionary.
		//case _m1MM1 : // = 4,

		/// At least one OCR has multiple words for a
		/// bounding box detected by other OCR or Image-
		/// Sieve and only OCR(A) string matched with dictionary
		//case _m1MMA : // = 5,

		/// At least one OCR has multiple words for a
		/// bounding box detected by other OCR or Image-
		/// Sieve and only OCR(B) string matched with dictionary
		//case _m1MMB : // = 6,

		/// OCR(A) & OCR(B) stings match and they have digits in it
	case _m1DD1 : // = 7,
		break;
		/// OCR(A) & OCR(B) stings mismatch and
		/// OCR(B) has digits in it whereas
		/// OCR(A) matches with dictionary
	case _m11D1 : // = 8,
		break;
		/// OCR(A) & OCR(B) stings mismatch and
		/// OCR(A) has digits in it whereas
		/// OCR(B) matches with dictionary
	case _m1D11 : // = 9,
		break;




		/// All have single word for a bounding box and both
		/// string matched with dictionary separately even
		/// individually strings are different.
	case _m111X : // = 10,
		break;
		/// At least one OCR has multiple words for a
		/// bounding box detected by other OCR or Image-
		/// Sieve and both string matched with dictionary
		/// separately even individually strings are different.
		//case _m1MMX : // = 11,


		/// OCR(A) sting present in dictionary where OCR(B)
		/// does not have any string.
	case _m1101 : // = 12,
		break;
		/// OCR(B) sting present in dictionary where OCR(A)
		/// does not have any string.
	case _m1011 : // = 13,
		break;



		/// All have single word for a bounding box and
		/// string does not matched with dictionary
	case _m1110 : // = 14,

		/// OCR(A) sting not present in dictionary where
		/// OCR(B) does not have any string.
	case _m1100 : // = 15,

		/// OCR(B) sting not present in dictionary where
		/// OCR(A) does not have any string.
	case _m1010 : // = 16,

		/// Add similar word(s) to suggestion list
		getWordSuggestion(map);
		break;

		/// OCR(A) has sting and digits in it
	case _m1D00 : // = 17,
		break;
		/// OCR(B) has sting and digits in it
	case _m10D0 : // = 18,
		break;
		/// OCR(A) has sting and digits in it and OCR(B) has string
		/// Both do not matched with dictionary
	case _m1D10 : // = 19,

		/// OCR(B) has sting and digits in it and OCR(A) has string
		/// Both do not matched with dictionary
	case _m11D0 : // = 20,
		if(text.isdigitPresentAtString(0) == true) {
			text.swap(0, 1);
		}
		/// Get alpha string
		alphaSTR1 = NULL; lengthSTR1 = 0;
		text.getAlphaStringAt(0, alphaSTR1, lengthSTR1);

		/// Add similar word(s) to suggestion list
		if(isDictionaryString(map, 0, alphaSTR1, lengthSTR1) == false) {
			getWordSuggestion(map);
		}

		/// OCR(A) & OCR(B) stings do not match and they have digits in it
	case _m1DD0 : // = 21,
		break;


		/// Image-sieve has bounding box but none of the OCR has string for that
	case _m1000 : // = 23
		break;
		/// Both or any of the OCR has multiple string for the Segment and
		/// None of them matches with dictionary
		//case _m1MM0 : // = 22,

	default:
		break;
	}

	return Success;
}
