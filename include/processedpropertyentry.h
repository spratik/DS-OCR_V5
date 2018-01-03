/*
 * processedpropertyentry.h
 *
 *  Created on: Jul 20, 2016
 *  Author: Cornelious
 */

#ifndef PROCESSEDPROPERTYENTRY_H
#define PROCESSEDPROPERTYENTRY_H

#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <sstream>


class ProcessedPropertyEntry
{

public:

	ProcessedPropertyEntry( const std::string & tokId
			, const std::string & tokType
			, const std::string & tokSubType
			, const std::string & cords
			, const std::string & parentId
			, const std::string & fColor
			, const std::string & bColor
			, const std::string & style
			, const std::string & underline
			, const std::string & strikethrough
			, const std::string & fontSize
			, const std::string & alignment
			, const std::string & lineAbove
			, const std::string & lineBelow
			, const std::string & textAbove
			, const std::string & textBelow
			, const std::string & scope
			, const std::string & str1
			, const std::string & str2
			, const std::string & matchType )
	: TOKENID( tokId )
	, TOKTYPE( tokType )
	, TOKENSUBTYPE( tokSubType )
	, CORDS( cords )
	, PARENTID( parentId )
	, FCOLOR( fColor )
	, BCOLOR( bColor )
	, STYLE( style )
	, UNDERLINE( underline )
	, STRIKETHRU( strikethrough )
	, FONT_SIZE( fontSize )
	, ALIGNMENT ( alignment )
	, LINE_ABOVE( lineAbove )
	, LINE_BELOW( lineBelow )
	, TEXT_ABOVE( textAbove )
	, TEXT_BELOW( textBelow)
	, SCOPE ( scope )
	, STRING1( str1 )
	, STRING2( str2 )
	, MATCHTYPE ( matchType )
	{

	}

	void PrintBuffer( std::vector<char> & input , int offset )
	{
		for (unsigned int i = 0 ; i < input.size() ; i++ )
			printf("%x", input[i]);
		std::cout << std::endl;
	}


	//void AddToBuffer( std::string & str , std::vector<char> & buffer , int & offset )
	void AddToBuffer( std::string & str , std::stringstream & buffer , int & offset )
	{
		/*int sz = str.size();
    memcpy( &buffer[0] + offset , &sz , sizeof(int));
    offset += sizeof(int);
    memcpy( &buffer[0] + offset , str.c_str() , sz + 1 );
    offset += sz + 1;
		 */

		if ( buffer.str().empty())
			buffer << str ;
		else
			buffer << "|" << str ;
	}



	std::string Serialize()
	{
		/*int counter = 20;
    std::vector<char> buffer;
    buffer.resize(2048);

    int offset = 0 ;
    memcpy( &buffer[0], &counter , sizeof(int)); 
    offset += sizeof(int);
		 */
		int offset = 0 ;
		std::stringstream buffer;

		AddToBuffer( TOKENID      , buffer , offset );
		AddToBuffer( TOKTYPE      , buffer , offset );
		AddToBuffer( TOKENSUBTYPE , buffer , offset );
		AddToBuffer( CORDS        , buffer , offset );
		AddToBuffer( PARENTID     , buffer , offset );
		AddToBuffer( FCOLOR       , buffer , offset );
		AddToBuffer( BCOLOR       , buffer , offset );
		AddToBuffer( STYLE        , buffer , offset );
		AddToBuffer( UNDERLINE    , buffer , offset );
		AddToBuffer( STRIKETHRU   , buffer , offset );
		AddToBuffer( FONT_SIZE    , buffer , offset );
		AddToBuffer( ALIGNMENT    , buffer , offset );
		AddToBuffer( LINE_ABOVE   , buffer , offset );
		AddToBuffer( LINE_BELOW   , buffer , offset );
		AddToBuffer( TEXT_ABOVE   , buffer , offset );
		AddToBuffer( TEXT_BELOW   , buffer , offset );
		AddToBuffer( SCOPE        , buffer , offset );
		AddToBuffer( STRING1      , buffer , offset );
		AddToBuffer( STRING2      , buffer , offset );
		AddToBuffer( MATCHTYPE    , buffer , offset );

		//buffer.resize(offset);
		//std::string str(buffer.begin(), buffer.end());

		//PrintBuffer( buffer , offset);

		// return str;
		return buffer.str();
	}

	ProcessedPropertyEntry&  DeSerialize( std::string & input )
	{

		return *this;
	}



	std::string TOKENID;
	std::string TOKTYPE;
	std::string TOKENSUBTYPE;
	std::string CORDS;
	std::string PARENTID;
	std::string FCOLOR;
	std::string BCOLOR;
	std::string STYLE;
	std::string UNDERLINE;
	std::string STRIKETHRU;
	std::string FONT_SIZE;
	std::string ALIGNMENT;
	std::string LINE_ABOVE;
	std::string LINE_BELOW;
	std::string TEXT_ABOVE;
	std::string TEXT_BELOW;
	std::string SCOPE;
	std::string STRING1;
	std::string STRING2;
	std::string MATCHTYPE;
};

#endif /* PROCESSEDPROPERTYENTRY_H */
