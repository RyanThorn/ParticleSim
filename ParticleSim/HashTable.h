#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_
/**
 * Defines a hash table data structure composed of linked lists.
 * @file: HashTable.h
 * @author: Ryan Thorn
 * @date: 17/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */
class HashTable
{
private:
	// The linked list of data for this hash table
	LinkedList* m_data;

	// The length of the hash table
	int m_length;
	// The default length of a hash table
	const static int DEFAULT_LENGTH = 17;

	/**
	 * Generates a hash of the given key. Used to index data in the hash table
	 * @param string _key The key to identify this data
	 * @returns int The hash generated from the algorithm
	 */
	int Hash(std::string _key);
public:
	/**
	 * Makes an empty hash table with a provided length.
	 * @param int _length The length of the hash table. Prime numbers recommended for efficiency. Defaults to 17.
	 */
	HashTable(int _length = DEFAULT_LENGTH);
	~HashTable();

	/**
	 * Inserts data into the hash table
	 * @param LinkedData* _data The data to be inserted into the hash table
	 */
	void Insert(LinkedData* _data);

	/**
	 * Removes data from the hashtable based on the key provided
	 * @param string _key The key to identify the data to be removed
	 * @returns bool True if successful, false if unsuccessful
	 */
	bool Remove(std::string _key);

	/**
	 * Retrieves data from the given key
	 * @param string _key The key to identify the data to be returned
	 * @returns LinkedData Returns the data if found. If not it returns a nullptr
	 */
	LinkedData* Find(std::string _key);

	// Getters/Setters
	// Returns length of the hash table
	int GetLength() { return m_length; }
};
#endif // !_HASHTABLE_H_

