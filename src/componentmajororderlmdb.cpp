/*
 * ComponentMajorOrderlmdb.cpp
 *
 *  Created on: Jul 20, 2016
 *  Author: Soumyadeep, Cornelious
 */

#include <sstream>
#include <vector>
#include <string.h>
#include<sys/stat.h>

#include "processedpropertyentry.h"
#include "componentmajororderlmdb.h"
#include "enum.h"

int ComponentMajorOrderLMDB :: init(char *fname)
{
	std::string fpath(fname);
	std::string path = fpath.substr(0, fpath.size()-4 );

	/// Create out folder
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH );

	/// Get db handler
	lmdbHandle = new WrapperLMDB( path , READ_AND_WRITE );

	if(lmdbHandle == NULL) {
		return DBInitError;
	}

	return Success;
}

int ComponentMajorOrderLMDB :: close()
{
	if(lmdbHandle != NULL) {
		lmdbHandle->commit_transaction();
		lmdbHandle->envclose();

		delete lmdbHandle;
		lmdbHandle = NULL;
	}

	return Success;
}

int ComponentMajorOrderLMDB :: printPageDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head)
	{
		for ( CNode *temp = head; temp != tail->next; temp = temp->next)
		{
			/// Get the page object
			PageObject *obj = (PageObject *) temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())
			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::string(obj->getParentID())
			, dummy, dummy,  dummy, dummy
			, dummy, dummy,  dummy, dummy
			, dummy, dummy,  dummy
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);


			rs = lmdbHandle->Write( std::to_string(obj->getMarker()) , ppe.Serialize() ,false);

			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;

			rs = Success;
		}
	}
	return rs;
}

int ComponentMajorOrderLMDB :: printTextRegionDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if (head)
	{
		for ( CNode *temp = head ; temp != tail->next; temp = temp->next)
		{
			/// Get the region object
			TextRegionObject *obj = (TextRegionObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())

			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, dummy, dummy,  dummy, dummy
			, dummy, dummy
			, std::string(obj->getAlignmentName())
			, dummy , dummy, dummy,  dummy
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);
			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;

			rs = printTextLineDetails(temp->hchild, temp->tchild);
			// if(rs != Success) return rs;

			rs = Success;
		}
	}
	return rs;
}

int ComponentMajorOrderLMDB :: printTextLineDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if (head)
	{
		for ( CNode *temp = head ; temp != tail->next; temp = temp->next)
		{
			/// Get the line object
			TextLineObject *obj = (TextLineObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())
			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, dummy, dummy,  dummy, dummy, dummy
			, dummy, dummy, dummy , dummy , dummy
			, dummy
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);
			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;

			rs = printTextWordDetails(temp->hchild, temp->tchild);
			// if(rs != Success) return rs;

			rs = Success;
		}
	}
	return rs;
}

std::string  getUnicodeStr( Tascii *start , Tascii *end )
{
   char buf[COMPONENT_BUF];
   int offset = 0 ;
   for ( Tascii *temp = start ; temp != end->next;  temp = temp->next )
   {
        if((temp->value >> 24) > 0)
        {
                 sprintf(buf, "%c%c%c%c",
                        (temp->value & 0xFF000000) >> 24,
                        (temp->value & 0x00FF0000) >> 16,
                        (temp->value & 0x0000FF00) >> 8,
                        (temp->value & 0x000000FF));
           offset += 4;
        }
        else if((temp->value >> 16) > 0)
        {
                 sprintf(buf + offset, "%c%c%c",
                        (temp->value & 0x00FF0000) >> 16,
                        (temp->value & 0x0000FF00) >> 8,
                        (temp->value & 0x000000FF));

           offset += 3;
        } else if((temp->value >> 8) > 0) {
                 sprintf(buf + offset, "%c%c",
                        (temp->value & 0x0000FF00) >> 8,
                        (temp->value & 0x000000FF));

           offset += 2;
        } else {
                sprintf(buf + offset, "%c",
                        (temp->value & 0x000000FF));
           offset += 1;
        }
  }

   if(offset > 0) {
	   return std::string( buf, buf + offset);
   } else {
	   return std::string("-");
   }
}


std::string  getUnicodeStr( unsigned int value )
{
   char buf[COMPONENT_BUF];
   int offset = 0 ;
	if((value >> 24) > 0)
	{
			 sprintf(buf, "%c%c%c%c",
					(value & 0xFF000000) >> 24,
					(value & 0x00FF0000) >> 16,
					(value & 0x0000FF00) >> 8,
					(value & 0x000000FF));
	   offset += 4;
	}
	else if((value >> 16) > 0)
	{
			 sprintf(buf + offset, "%c%c%c",
					(value & 0x00FF0000) >> 16,
					(value & 0x0000FF00) >> 8,
					(value & 0x000000FF));

	   offset += 3;
	} else if((value >> 8) > 0) {
			 sprintf(buf+offset, "%c%c",
					(value & 0x0000FF00) >> 8,
					(value & 0x000000FF));

	   offset += 2;
	} else {
			sprintf(buf + offset, "%c",
					(value & 0x000000FF));
	   offset += 1;
	}

   if(offset > 0) {
	   return std::string( buf, buf + offset);
   } else {
	   return std::string("-");
   }
}

int ComponentMajorOrderLMDB :: printTextWordDetails(CNode *head, CNode *tail)
{
	int index, strCount;
	int rs = Success;
	Tascii *h, *t;
	int length, size;
	unsigned int value;
	bool istoupper = false;

	if (head)
	{
		for ( CNode *temp = head ; temp != tail->next; temp = temp->next)
		{
			/// Get the word object
			TextWordObject *obj = (TextWordObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			/// Print string information
			std::string ocrStr1 = "";

			/// Get string count
			strCount = obj->getStringCount();

			/// If string present
			if(strCount > 0) {
				istoupper = false;
				/// Get the first string
				h = t = NULL; length = 0;
				obj->getStringAt(0, h, t, length);
				if(length > 0) {
					ocrStr1 = getUnicodeStr(h, t);
					istoupper = h->value < ASCII_RANGE ? isupper(h->value) : false;
				}

				/// Get the rest strings
				for (index = 1; index < strCount; index++) {
					h = t = NULL; length = 0;
					obj->getStringAt(index, h, t, length);
					if(length > 0) {
						/// Make the first character case sensitive
						if(istoupper) {
							ocrStr1 += (" " + getUnicodeStr(toupper(h->value)) + getUnicodeStr(h->next, t));
							//cout << ocrStr1 << endl;
						} else {
							ocrStr1 += (" " + getUnicodeStr(h, t));
						}
					}
				}
			}

			/// Get HexStream
			size = (int)ocrStr1.length();
			std::stringstream ocrStream1;
			if(size > 0) {
				for (index = 0; index < size; index++) {
					value = (unsigned int)ocrStr1[index];
					value &= 0x000000FF;
					ocrStream1 << std::hex << value;
				}
			}

			/// Get String2
			/// Get HexStream
			std::stringstream ocrStream2;

			/// Set matching type
			std::string mType = std::to_string(obj->getMatchType());


			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())

			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, dummy, dummy,  dummy, dummy , dummy
			, dummy , dummy , dummy , dummy, dummy,  dummy
			, std::string(obj->getScope())
			, ocrStream1.str()
			, ocrStream2.str()
			, mType
			);

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);
			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;

			rs = printCharacterDetails(temp->hchild, temp->tchild );
			// if(rs != Success) return rs;

			rs = Success;
		}
	}
	return rs;
}

int ComponentMajorOrderLMDB :: printCharacterDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if ( head )
	{
		for ( CNode *temp = head ; temp != tail->next; temp = temp->next)
		{
			/// Get the line object
			CharacterObject *obj = (CharacterObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			std::stringstream cfss, cbss;
			cfss << std::hex << obj->getFGRed() << obj->getFGGreen() <<  obj->getFGBlue();
			cbss << std::hex << obj->getBGRed() << obj->getBGGreen() <<  obj->getBGBlue();

			std::string underline      = obj->getUnderLine()  ? "True" : "False";
			std::string strikeThrough  = obj->getStrikeThru() ? "True" : "False";

			std::string isLineAbove = obj->getLineAbove() ?  "YES" : "NO";
			std::string isLineBelow = obj->getLineBelow() ?  "YES" : "NO";

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())
			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, cfss.str()
			, cbss.str()
			, std::string(obj->getStyleName())
			, underline
			, strikeThrough
			, std::to_string(obj->getFontSize())
			, dummy
			, isLineAbove
			, isLineBelow
			, dummy,  dummy
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);

			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;
			rs = Success;
		}
	}
		return rs;
}


int ComponentMajorOrderLMDB :: printLineDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if (head)
	{
		for ( CNode *temp = head; temp != tail->next; temp = temp->next)
		{
			/// Get the line object
			LineObject *obj = (LineObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );


			std::string isTextAbove = obj->getTextAbove() ?  "YES" : "NO";
			std::string isTextBelow = obj->getTextBelow() ?  "YES" : "NO";

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())
			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, dummy, dummy,  dummy, dummy , dummy
			, std::to_string( obj->getSize())
			, dummy , dummy, dummy
			, isTextAbove
			, isTextBelow
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);
			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;
			rs = Success;
		}
	}

	return rs;
}

int ComponentMajorOrderLMDB :: printImageDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if(head)
	{
		for ( CNode *temp = head; temp != tail->next; temp = temp->next)
		{
			/// Get the line object
			SubImageObject *obj = (SubImageObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())
			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, dummy, dummy,  dummy, dummy , dummy
			, dummy, dummy,  dummy, dummy , dummy
			, dummy
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);

			rs = printImageDetails(temp->hchild, temp->tchild );
			// if(rs != Success) return rs;

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);
			cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
			// if(rs != Success) return rs;

			rs = Success;
		}
	}
	return rs;
}


int ComponentMajorOrderLMDB :: printNoiseDetails(CNode *head, CNode *tail)
{
	int rs = Success;
	if (head)
	{
		for ( CNode *temp = head; temp != tail->next; temp = temp->next)
		{
			/// Get object
			NoiseObject *obj = (NoiseObject *)temp->getData();

			Rectangle rect = obj->getBrect();
			std::stringstream ss;
			ss << rect.left << " ";
			ss << rect.top  << " ";
			ss << ( rect.right - rect.left ) << " ";
			ss << ( rect.bottom - rect.top );

			std::string dummy = "";
			ProcessedPropertyEntry ppe( std::to_string(obj->getMarker())
			, std::string(obj->getToken())
			, std::string(obj->getSubToken())
			, ss.str()
			, std::to_string(obj->getPmarker())
			, dummy, dummy,  dummy, dummy , dummy
			, dummy, dummy,  dummy, dummy , dummy
			, dummy
			, std::string(obj->getScope())
			, dummy, dummy,  dummy
			);

			rs = lmdbHandle->Write(std::to_string(obj->getMarker()) , ppe.Serialize() ,false);
		    if(rs != Success) return rs;
		    cout << obj->getMarker()  << " " << ppe.Serialize()  << endl;
		}
	}
	return rs;
}
