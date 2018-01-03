/*
 * ComponentMajorOrderFile.cpp
 *
 *  Created on: Apr 29, 2016
 *  Author: Soumyadeep
 */

#include <string.h>
#include <ctype.h>

#include "componentmajororderfile.h"
#include "enum.h"

#define OUT_BUF		16

/// Print TOKTYPE informations
#define _PRINT_TOKTYPE(a, b)												\
		{																			\
	if(fp) {																\
		fprintf(fp, "\n%d\t%s\t", a, b);									\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print TOKENSUBTYPE informations
#define _PRINT_TOKENSUBTYPE(a)												\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print COORDINATES informations
#define _PRINT_CORDS(a)														\
		{																			\
	if(fp) {																\
		fprintf(fp, "%d %d %d %d\t",										\
				a.left, a.top,												\
				(a.right - a.left + 1), (a.bottom - a.top + 1));			\
				fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print PARENTID informations
#define _PRINT_PARENTID_STRING(a)												\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print PARENTID informations
#define _PRINT_PARENTID(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%d\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print Foreground Color informations
#define _PRINT_COLOR(a, b, c)												\
		{																			\
	if(fp) {																\
		fprintf(fp, "#%02X%02X%02X\t", a, b, c);							\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print STYLE informations
#define _PRINT_STYLE(a)														\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print UNDERLINE informations
#define _PRINT_UNDERLINE(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", (a == true) ? "YES" : "NO");					\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}


/// Print STRIKETHRU informations
#define _PRINT_STRIKETHRU(a)												\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", (a == true) ? "YES" : "NO");					\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print FONTSIZE informations
#define _PRINT_FONTSIZE(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%d\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print ALIGNMENT informations
#define _PRINT_ALIGNMENT(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print LINE ABOVE informations
#define _PRINT_LINEABOVE(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", (a == true) ? "YES" : "NO");					\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print LINE BELOW informations
#define _PRINT_LINEBELOW(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", (a == true) ? "YES" : "NO");					\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print TEXT ABOVE informations
#define _PRINT_TEXTABOVE(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", (a == true) ? "YES" : "NO");					\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print text below informations
#define _PRINT_TEXTBELOW(a)													\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", (a == true) ? "YES" : "NO");					\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

/// Print scope informations
#define _PRINT_SCOPE(a)														\
		{																			\
	if(fp) {																\
		fprintf(fp, "%s\t", a);												\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

#define _PRINT_STRING(a, b, c)												\
		{																			\
	if(fp) {																\
		if(a) {																\
			for (c = a; c != b->next;  c = c->next) {						\
				fprintfUnicode(fp, c->value);								\
			}																\
		} 																	\
		fflush(fp);															\
	} else {																\
		return FileWriteError;												\
	}																		\
		}

int ComponentMajorOrderFile :: fprintfUnicode(FILE *fp, unsigned int value) {
	if((value >> 24) > 0) {
		return fprintf(fp, "%c%c%c%c",
				(value & 0xFF000000) >> 24,
				(value & 0x00FF0000) >> 16,
				(value & 0x0000FF00) >> 8,
				(value & 0x000000FF));
	} else if((value >> 16) > 0) {
		return fprintf(fp, "%c%c%c",
				(value & 0x00FF0000) >> 16,
				(value & 0x0000FF00) >> 8,
				(value & 0x000000FF));

	} else if((value >> 8) > 0) {
		return fprintf(fp, "%c%c",
				(value & 0x0000FF00) >> 8,
				(value & 0x000000FF));

	} else {
		return fprintf(fp, "%c",
				(value & 0x000000FF));
	}
}

int ComponentMajorOrderFile :: init(char *fname)
{
	/// Open file descriptor
	fp = fopen(fname,"w+");

	/// Check for valid file descriptor
	if(fp) {
		/// Print header informations
		fprintf(fp, "TOKENID\t"
				"TOKTYPE\t"
				"TOKENSUBTYPE\t"
				"CORDS\t"
				"PARENTID\t"
				"FCOLOR\t"
				"BCOLOR\t"
				"STYLE\t"
				"UNDERLINE\t"
				"STRIKETHRU\t"
				"FONT-SIZE\t"
				"ALIGNMENT\t"
				"LINE-ABOVE\t"
				"LINE-BELOW\t"
				"TEXT-ABOVE\t"
				"TEXT-BELOW\t"
				"SCOPE\t");
		for (int index = 1; index <= MAX_WORD_SUGGESTION; index++) {
			fprintf(fp, "STRING%d\t", index);
		}
		fprintf(fp, "MATCHTYPE");
		fflush(fp);
	} else {
		return FileWriteError;
	}

	return Success;

}

int ComponentMajorOrderFile :: close()
{

	/// Check for valid file descriptor
	if(fp) {
		fflush(fp);
		fclose(fp);
		fp = NULL;
	} else {
		return FileWriteError;
	}

	return Success;

}

int ComponentMajorOrderFile :: printPageDetails(CNode *head, CNode *tail)
{
	int rs = Success;

	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the page object
			PageObject *obj = (PageObject *) temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for page
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print id for page
			_PRINT_PARENTID_STRING(obj->getParentID());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}
		}
	}
	return rs;
}

int ComponentMajorOrderFile :: printTextRegionDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {

			/// Get the region object
			TextRegionObject *obj = (TextRegionObject *)temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for page
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print alignment information
			switch(obj->getAlignment()) {
			case LeftAligned :
				_PRINT_ALIGNMENT("LEFT");
				break;
			case RightAligned :
				_PRINT_ALIGNMENT("RIGHT");
				break;
			case CenterAligned :
				_PRINT_ALIGNMENT("CENTER");
				break;
			default :
				//Justify
				_PRINT_ALIGNMENT("JUSTIFIED");
				break;
			}

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			rs = printTextLineDetails(temp->hchild, temp->tchild);
			if(rs != Success) return rs;
		}
	}
	return rs;
}

int ComponentMajorOrderFile :: printTextLineDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {

			/// Get the line object
			TextLineObject *obj = (TextLineObject *)temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for page
			_PRINT_TOKENSUBTYPE(obj->getSubToken());
			if(rs != Success) return rs;

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			rs = printTextWordDetails(temp->hchild, temp->tchild);
			if(rs != Success) return rs;
		}
	}
	return rs;
}

int ComponentMajorOrderFile :: printTextWordDetails(CNode *head, CNode *tail)
{
	Tascii *wtemp = NULL;
	int rs = Success;
	Tascii *h, *t;
	int index, strCount, length;
	bool istoupper;

	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {

			/// Get the line object
			TextWordObject *obj = (TextWordObject *)temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for page
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());


			/// Print string information
			strCount = obj->getStringCount();

			/// If string present
			if(strCount > 0) {
				/// Set default value
				istoupper = false;

				/// Get the first string
				h = t = NULL; length = 0;
				obj->getStringAt(0, h, t, length);

				/// If a valid string
				if(length > 0) {
					/// Get first character case status
					istoupper = h->value < ASCII_RANGE ? isupper(h->value) : false;

					/// Print string to the file
					_PRINT_STRING(h, t, wtemp);
					fprintf(fp, "\t"); fflush(fp);
				}

				/// Get the rest strings
				for (index = 1; index < strCount; index++) {
					/// Get remaining strings
					h = t = NULL; length = 0;
					obj->getStringAt(index, h, t, length);

					/// If a valid string
					if(length > 0) {
						/// Make the first character case sensitive
						if(istoupper) {
							/// Print the first character
							fprintfUnicode(fp, toupper(h->value));

							/// Print the rest string to the file
							_PRINT_STRING(h->next, t, wtemp);
							fprintf(fp, "\t"); fflush(fp);
						} else {
							/// Print string directly to the file
							_PRINT_STRING(h, t, wtemp);
							fprintf(fp, "\t"); fflush(fp);
						}
					} else {
						fprintf(fp, "-\t"); fflush(fp);
					}
				}
			}

			/// Print remaining strings
			for (index = strCount; index < MAX_WORD_SUGGESTION; index++) {
				fprintf(fp, "-\t"); fflush(fp);
			}

			/// Print matching type information
			fprintf(fp, "%d", obj->getMatchType()); fflush(fp);

			/// Print character details
			rs = printCharacterDetails(temp->hchild, temp->tchild);
			if(rs != Success) return rs;
		}
	}
	return rs;
}

int ComponentMajorOrderFile :: printCharacterDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	char ntype[OUT_BUF];

	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {

			/// Get the line object
			CharacterObject *obj = (CharacterObject *)temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for character
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// print foreground information
			_PRINT_COLOR(obj->getFGRed(), obj->getFGGreen(), obj->getFGBlue());

			/// print background information
			_PRINT_COLOR(obj->getBGRed(), obj->getBGGreen(), obj->getBGBlue());

			/// Get STYLE informations
			switch(obj->getStyle()) {
			case Bold :
				strcpy(ntype, "BOLD");
				break;
			case Italic :
				strcpy(ntype, "ITALIC");
				break;
			case BoldItalic :
				strcpy(ntype, "BOLDITALIC");
				break;
			default :
				strcpy(ntype, "REGULAR");
				break;
			}
			/// Print STYLE informations
			_PRINT_STYLE(ntype);

			/// Print UNDERLINE informations
			_PRINT_UNDERLINE(obj->getUnderLine());

			/// Print STRIKETHRU informations
			_PRINT_STRIKETHRU(obj->getStrikeThru());

			/// Print FONTSIZE informations
			_PRINT_FONTSIZE(obj->getFontSize());

			/// Ignore alignment properties
			if(fp) {
				fprintf(fp, "-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print LINE ABOVE informations
			_PRINT_LINEABOVE(obj->getLineAbove());

			/// Print LINE BELOW informations
			_PRINT_LINEBELOW(obj->getLineBelow());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope informations
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}
		}
	}
	return rs;
}


int ComponentMajorOrderFile :: printLineDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the line object
			LineObject *obj = (LineObject *)temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for page
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print Font Size informations
			_PRINT_FONTSIZE(obj->getSize());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print TEXT ABOVE informations
			_PRINT_TEXTABOVE(obj->getTextAbove());

			/// Print TEXT BELOW informations
			_PRINT_TEXTBELOW(obj->getTextBelow());

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}
		}
	}
	return rs;
}

int ComponentMajorOrderFile :: printImageDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {
			/// Get the line object
			ImageObject *obj = (ImageObject *)temp->getData();

			/// Print token id for image
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for image
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token coordinates for image
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print sub-image details
			rs = printImageDetails(temp->hchild, temp->tchild);
			if(rs != Success) return rs;
		}
	}
	return rs;
}

int ComponentMajorOrderFile :: printNoiseDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head) {
		for (CNode *temp = head; temp != tail->next; temp = temp->next) {

			/// Get object
			NoiseObject *obj = (NoiseObject *)temp->getData();

			/// Print token id for page
			_PRINT_TOKTYPE(obj->getMarker(), obj->getToken());

			/// Print token sub type for page
			_PRINT_TOKENSUBTYPE(obj->getSubToken());

			/// Print token id for page
			_PRINT_CORDS(obj->getBrect());

			/// Print token id for page
			_PRINT_PARENTID(obj->getPmarker());

			/// Ignore rest properties
			if(fp) {
				fprintf(fp, "-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t-\t");
				fflush(fp);
			} else {
				return FileWriteError;
			}

			/// Print scope of the component
			_PRINT_SCOPE(obj->getScope());

			/// Ignore rest properties
			if(fp) {
				for (int index = 0; index < MAX_WORD_SUGGESTION; index++) {
					fprintf(fp, "-\t");
				}
				fprintf(fp, "-");
				fflush(fp);
			} else {
				return FileWriteError;
			}
		}
	}
	return rs;
}
