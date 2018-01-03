/*
 * hashmap.h
 *
 *  Created on: Jun 13, 2016
 *  Author: Soumyadeep Dhar
 */

#ifndef _HASHMAP_
#define _HASHMAP_

#include <stdlib.h>
#include <stdio.h>
#include "defines.h"
#include "enum.h"

#define PRINT_HASH_MAP 1

template<typename T>
struct HashNode
{
	// Tracks the next hash node position having with same key
    unsigned int next;

    // Stores the key value
    unsigned int key;

    // Indicates that the node is empty or not
    bool empty;

    // Stores the data value
    T data;
};

/******************************************************************************
Class       : HashMap
Description : This class is used to store elements in a hash table. It
              provides some utility functions to operate on the hash table
              such as adding an element, updating an element's data value,
              deleting an element, changing an existing key by new key value,
              clearing the hash table etc. It is built as a template class
              to handles more than one data types including pointers and
              other user defined data types.
*******************************************************************************/
template <typename T>
class HashMap
{
	private:

	HashNode <T> *hashTable;
    int size;
    int noOfElement;
    int emptyIndex;
    int initialLength;

    // Hash function that generates the hash value for a specific key.
    // It takes only integer key values.
    inline int hash(unsigned int key) {
        return (key % initialLength);
    }

public:

    /// Public constructor
    HashMap() {
        noOfElement 	= 0;
         size 			= 0;;
         emptyIndex 	= 0;
         initialLength 	= 0;

         /// Create hash table
         hashTable = NULL;
    }

    ~HashMap() {
    	release();
    }

    int initializeHashMap(int length)
    {
    	int i;
        noOfElement 	= 0;
        size 			= length;
        emptyIndex 		= size;
        initialLength 	= size;

        /// Create hash table
        hashTable = (HashNode <T> *)calloc(size, sizeof(HashNode <T>));

        if(hashTable == NULL) {
#if PRINT_HASH_MAP
        	printf("InitializeHashMap(int %d)\n"
				   "Memory not Found To allocate Hash Map.", length);
        	fflush(stdout);
#endif
        	return MemoryOverFlow;
        }
        // Traverse each of the hash element
        for(i = 0; i < size; i++) {
            // Initializes all the hash element by NULL
            hashTable[i].next  = 0;
            hashTable[i].empty = true;
            hashTable[i].data = (T)0;
        }
        return Success;
    }

    void release() {
    	if(hashTable != NULL) {
    		this->clearHash();
    		free(hashTable);
    		hashTable = NULL;
    	}
    }

    int addElement(unsigned int key, const T &data)
    {
    	int index = hash(key);
    	int oldIndex;

    	/// If no collision occurs
    	if(hashTable[index].empty)
    	{
    		/// Set new element in the hash table
    		hashTable[index].key = key;
    		hashTable[index].data = data;
    		hashTable[index].empty = false;

    		/// Increase total number of element
    		++ noOfElement;
    		return Success;
    	}

    	/// If collision occurs
    	else
    	{
    		do
    		{   /// If an element with that same key already exists
    			if(hashTable[index].key == key)
    			{
    				/// Update the data value
    				hashTable[index].data = data;
    				return Success;
    			}

    			oldIndex = index;
    			index = hashTable[index].next;

    		} while(index != 0);

    		/// If reallocation of the table required
    		if(emptyIndex == size) {
    			int oldSize = size;

    			/// Increase the table size
    			size += initialLength;

    			/// reallocate the memory as required
    			HashNode <T> *oldHashTabPtr = hashTable;
    			hashTable = (HashNode<T> *)realloc(oldHashTabPtr, size * sizeof(HashNode<T>));

    			/// If memory reallocation is not successful
    			if(!hashTable) {
#if PRINT_HASH_MAP
				printf("addElement :: Memory not Found To allocate Hash Map.");
				fflush(stdout);
#endif
        			return MemoryOverFlow;
    			}

    			/// memory reallocation is successful
    			else
    			{
    				/// Traverse each of the new hash node
    				for(int i = oldSize; i < size; i++)
    				{
    					/// Initialize as empty node
    					hashTable[i].next = 0;
    					hashTable[i].empty = true;
    				}
    			}
    		}

    		/// store the element at the end and make the node as non empty
    		hashTable[emptyIndex].key 	= key;
    		hashTable[emptyIndex].data 	= data;
    		hashTable[emptyIndex].empty = false;
    		hashTable[oldIndex].next 	= emptyIndex;
    		++	noOfElement;
    		emptyIndex	++;

    		// find the first node position that is empty
    		while(emptyIndex < size)
    		{
    			if(hashTable[emptyIndex].empty) {
    				break;
    			}
    			emptyIndex++;
    		}

    		// printHashTableContents();
    	}

    	return Success;
    }

    int setElement(unsigned int key, const T& data)
    {
        int index = hash(key);

        /// For each element having same hash value as for that key
        do
        {
            /// Check if an element with that key exists
            if(hashTable[index].key == key && !hashTable[index].empty)
            {
                /// Update the data value
            	hashTable[index].data = data;
                return Success;
            }
            index = hashTable[index].next;
        } while(index != 0);

        return InavidParameter;
    }


    const T getElement(unsigned int key)
    {
        int index = hash(key);

        /// For each element having same hash value as for that key
        do
        {
            /// Check if an element with that key exists
            if(hashTable[index].key == key && !hashTable[index].empty)
            {
            	T tempData = hashTable[index].data;

            	/// return the data value
                return tempData;
            }
            index = hashTable[index].next;
        } while(index != 0);

        /// If no element with that key already exists
        return (T)0;
    }

    const T getElementAt(int position)
    {
    	/// Get corresponding element
    	if(position < size && !hashTable[position].empty) {
    		return hashTable[position].data;
    	}

        /// If no element with that key already exists
        return (T)0;
    }

    int getPosition(unsigned int key)
    {
        int index = hash(key);

        /// For each element having same hash value as for that key
        do
        {
            /// Check if an element with that key exists
            if(hashTable[index].key == key && !hashTable[index].empty)
            {
                /// return the data value
                return index;
            }
            index = hashTable[index].next;
        }while(index != 0);

        /// If no element with that key already exists then
        /// return size as invalid position
        return INVALID_INDEX;
    }

    int deleteElement(unsigned int key)
    {
        int index    = hash(key);
        int oldIndex = index;

        /// For each element having same hash value as for that key
        do
        {
            /// Check if an element with that key exists
            if(hashTable[index].key == key && !hashTable[index].empty)
            {
            	hashTable[index].empty = true;
                -- noOfElement;

                if(index >= initialLength)
                {
                    if(index < emptyIndex)
                    {
                        emptyIndex = index;
                    }

                    hashTable[oldIndex].next = hashTable[index].next;
                    hashTable[index].next = 0;
                }
                return Success;
            }

            oldIndex = index;
            index = hashTable[index].next;
        } while(index != 0);

        return InavidParameter;
    }

    inline int getCount() const {
        return noOfElement;
    }

    inline int getSize() const {
        return size;
    }

    inline bool isEmpty() const {
        /// checks whether any element exists or not
    	return (noOfElement == 0) ? true : false;
    }

    /// Removes  all the elements from the hash table
    void clearHash()
    {
        for(int i = 0; i < size; i++)
        {
            // Initializes all the hash element
        	hashTable[i].next = 0;
        	hashTable[i].empty = true;
        }
        noOfElement = 0;
        emptyIndex  = initialLength;
    }


#if PRINT_HASH_MAP
    void printHashTableContents()
    {
        printf("\n***********\tHash Table size = %hd\t************\n", size);
        printf(  "*******\tNo. of Hash Table element = %hd\t********\n", getCount());
        printf("\n==================  Hash Table Contents  ==================\n");
        for(int i = 0; i < size; i++)
        {
            printf("##  Index = %3d    ", i);
            printf("Key = %4u(%c)    Next = %3u    ", hashTable[i].key, hashTable[i].key != 0 ? hashTable[i].key : '~', hashTable[i].next);

            if(hashTable[i].empty) {
                printf("Empty = True\n");
            } else {
                printf("Empty = False\n");
            }
        }
        printf(  "-----------------------------------------------------------\n");
        fflush(stdout);
    }
#endif
};

#endif
