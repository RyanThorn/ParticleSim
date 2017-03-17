#include "Stdafx.h"
#include "HashTable.h"
/**
 * Defines a hash table data structure composed of linked lists.
 * @file: HashTable.cpp
 * @author: Ryan Thorn
 * @date: 17/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */

 /**
 * Makes an empty hash table with a provided length.
 * @param int _length The length of the hash table. Must be above 0. Prime numbers recommended for efficiency. Defaults to 17.
 */
HashTable::HashTable(int _length)
{
	// Ensure that lengths of 0 or less are not allowed. If given set to default length
	if (_length <= 0)
	{
		_length = DEFAULT_LENGTH;
	}

	// Create our linked list array from the size given
	m_data = new LinkedList[_length];
	// Set the length
	m_length = _length;
}

HashTable::~HashTable()
{
}

/**
 * Inserts data into the hash table
 * @param LinkedData* _data The data to be inserted into the hash table
 */
void HashTable::Insert(LinkedData * _data)
{
	// Create our index from the hashed version of the data's key
	int m_index = this->Hash(_data->m_key);
	// Insert it into this indexes linked list
	m_data[m_index].Insert(_data);
}

/**
 * Removes data from the hashtable based on the key provided
 * @param string _key The key to identify the data to be removed
 * @returns bool True if successful, false if unsuccessful
 */
bool HashTable::Remove(std::string _key)
{
	// Create our index from the hashed version of the data's key
	int m_index = this->Hash(_key);

	// Return the result from the LinkedList remove method (true if successful)
	return m_data[m_index].Remove(_key);
}

/**
 * Retrieves data from the given key
 * @param string _key The key to identify the data to be returned
 * @returns LinkedData Returns the data if found. If not it returns a nullptr
 */
LinkedData * HashTable::Find(std::string _key)
{
	// Create our index from the hashed version of the data's key
	int m_index = this->Hash(_key);

	// Find the item using the linked lists find method from this index position
	return m_data[m_index].Find(_key);
}

/**
 * Generates a hash of the given key. Used to index data in the hash table
 * @param string _key The key to identify this data
 * @returns int The hash generated from the algorithm
 */
int HashTable::Hash(std::string _key)
{
	// The combined ascii value of the string key given
	int m_asciiValue = 0;

	// Calcluate the ascii value from the key given
	for (int i = 0; i < _key.length(); i++)
	{
		m_asciiValue += _key[i];
	}

	// Return the hash from the ascii value, modulus the length
	return (m_asciiValue * _key.length()) % m_length;
}
