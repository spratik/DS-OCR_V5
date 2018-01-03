/*
 * textdata.h
 *
 *  Created on: Jul 8, 2016
 *      Author: Soumyadeep Dhar
 */

#ifndef TEXTDATA_H_
#define TEXTDATA_H_

#include <datatype.h>
#include <datatype.h>

class TextData
{
public:
	class Text
	{
	private:
		/// default string
		Tascii defaultstr;

		/// Head and Tail pointer for component list
		Tascii *bufHead, *bufTail;

		/// Strings without non alpha characters
		unsigned int *alphastr;

		/// Length of string and alpha string and
		/// character continuous match count
		int sLength, aLength, mLength;

		/// flag to identify contains number of not
		bool nPresent;

	public:
		Text() {
			/// Set default string
			defaultstr.value = '-'; defaultstr.next = NULL;

			/// Head and Tail pointer for component list
			bufHead = bufTail = NULL;

			/// Strings without non alpha characters
			alphastr = NULL;

			/// Length of string and alpha string
			sLength = aLength = mLength = 0;

			/// flag to identify contains number of not
			nPresent = false;
		}

		~Text() {}

		/// Function to check character type
		static bool checkAlpha(unsigned int value);

		/// Set a new string at the end of the list
		int setString(Tascii *head, Tascii *tail, int slen, int mlen);

		/// Set a default string at the end of the list
		int setDefaultString();

		/// Get a string
		void getString(Tascii *&h, Tascii *&t, int &length);

		/// Get string after removal of non-alpha characters
		void getAlphaString(unsigned int *&base, int &length);

		/// Get digit present flag
		bool isdigitPresent();

		/// Get string matching length
		int getMatchedLength();

		/// Get string length
		int getLength();

		/// Clear internal data
		void clear();
	};

	TextData() {
		/// Total number of string(s)
		maxCount = MAX_WORD_SUGGESTION;

		/// Total number of string already registered
		strCount = 0;

		/// All representative strings
		objects = NULL;

	};
	~TextData() {};

	int allocate(int numString);
	int release();

	/// Total number of strings
	int getStringCount();

	/// Set a new string at the end of the list
	int setString(Tascii *head, Tascii *tail, int slen, int mLen);
	int replaceStringAt(int position, Tascii *head, Tascii *tail, int slen, int mlen);

	/// Set a default string at the end of the list
	int setDefaultString();

	/// Get a string from a particular position
	int getStringAt(int position, Tascii *&h, Tascii *&t, int &length);

	/// Match length
	int getMatchedLengthAt(int position, int &length);

	/// Match length
	int getLengthAt(int position, int &length);

	/// Get string after removal of non-alpha characters
	void getAlphaStringAt(int position, unsigned int *&base, int &length);

	/// Get digit present flag
	bool isdigitPresentAtString(int position);

	/// Clear internal data
	void clearAll();

	/// Swap position the strings
	void swap(int position1, int position2);

private:
	/// Total number of string(s)
	int maxCount;

	/// Total number of string already registered
	int strCount;

	/// All representative strings
	Text *objects;
};

#endif /* TEXTDATA_H_ */
