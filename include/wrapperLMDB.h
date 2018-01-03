
/*
 * wrapperLMDB
 *
 *  Created on: Apr 29, 2016
 *  Author: Cornelious
 */

#ifndef WRAPPERLMDB_H
#define WRAPPERLMDB_H

#pragma once 

#include<vector>
#include"lmdb++.h"
#include<iostream>
#include<string>
#include<map>
#include<ctime>
#include<sys/stat.h>

using namespace std;

enum DBMODE
{
	READ_ONLY,
	READ_AND_WRITE
};

class WrapperLMDB
{
private:
	lmdb::env  env;
	lmdb::txn  transaction;
	lmdb::dbi dbi;

	DBMODE dbmode;

public:


	WrapperLMDB( const std::string & evnName ,DBMODE dbmode_to_open_database): env(nullptr), transaction(nullptr), dbi(0)
	{
		env = lmdb::env::create();

		env.set_mapsize(1UL*1024UL*1024UL*1024UL);

		mkdir(evnName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);

		if(dbmode_to_open_database ==    READ_ONLY )
		{
			env.open(evnName.c_str(),MDB_RDONLY|MDB_NOLOCK,0664);
			transaction = lmdb::txn::begin(env,nullptr,MDB_RDONLY);
		}

		else
		{

			if(dbmode_to_open_database ==    READ_AND_WRITE )
			{
				env.open(evnName.c_str(),0,0664);
				transaction = lmdb::txn::begin(env,nullptr,0);
			}

		}


		dbi = lmdb::dbi::open( transaction , nullptr);
	}

	~WrapperLMDB() {}


	int Read_all_keys( map<string,string> & Result)
	{

		int error_code;

		string key,value;

		lmdb::cursor cur = lmdb::cursor::open( transaction , dbi);

		while((error_code  =  cur.get( key ,  value, MDB_NEXT)))
		{
			Result[key] = value;
			cout<<"key :"<<key<<"\t"<<"value:"<<value<<endl;
		}

		return error_code;

	}

	int Write( string key , string  value,bool z=true)
	{

		int error_code;

		error_code  =  dbi.put(transaction,key.c_str(), value.c_str());

		if(z)transaction.commit();

		return error_code;
	}




	bool Find_key(string key_to_search,string & res)
	{

		int r=0;
		char *gt;
		MDB_val key,data;

		key.mv_size = key_to_search.length();
		key.mv_data = new char[key.mv_size+1];
		strcpy((char*)key.mv_data,key_to_search.c_str());

		lmdb::cursor cursor = lmdb::cursor::open( transaction , dbi);

		while ((r = mdb_cursor_get(cursor , &key, &data, MDB_SET)) == 0 )
		{

			gt = new char[data.mv_size+1];
			strcpy(gt,(char*)data.mv_data);

			res = string(gt);


			cursor.close();

			return true;

		}

		res = "";

		cursor.close();
		return false;
	}


	int Batch_write( map<string,string> & map_to_write_at_database)
	{
		int error_code;
		map<string,string>::iterator loop_var_ptr = map_to_write_at_database.begin();

		// int str_length = loop_var_ptr->first.length();

		while(loop_var_ptr != map_to_write_at_database.end())
		{
			if((error_code  =  dbi.put(transaction , loop_var_ptr->first.c_str() , loop_var_ptr->second.c_str())))
			{
				++loop_var_ptr;
			}
			else
			{
				return error_code;
			}
		}

		transaction.commit();

		return 0;
	}


	void Batch_read(map<string,string> & map_to_read_at_database)
	{

		map<string,string>::iterator loop_var_ptr = map_to_read_at_database.begin();


		lmdb::cursor cur = lmdb::cursor::open( transaction , dbi);

		string key  ,  value;

		while(cur.get( key ,  value, MDB_NEXT))
		{
			map_to_read_at_database.insert(loop_var_ptr  ,  pair<string,string>(key  ,  value));

			++loop_var_ptr;
		}

	}

	int Del( string key_2,bool z=true)
	{

		string res;
		int r=0;
		MDB_val key,data;
		key.mv_data = new char[key_2.length()+1];
		strcpy((char*)key.mv_data,key_2.c_str());

		lmdb::cursor cursor = lmdb::cursor::open( transaction , dbi);

		while ((mdb_cursor_get(cursor , &key, &data, MDB_SET)))
		{

			mdb_cursor_del(cursor,0);

			break;

		}

		if(z)transaction.commit();
		return r;
	}

	void commit_transaction()
	{
		transaction.commit();
	}

	void Resize_DB(unsigned long  mapsize)
	{
		env.set_mapsize(mapsize);
	}

	void envclose()
	{

		env.close();
	}
};

#endif /* WRAPPERLMDB_H */
