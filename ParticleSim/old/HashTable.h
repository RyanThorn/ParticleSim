#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_
/**
 * Defines a hash table data structure composed of linked lists.
 * @file: HashTable.h
 * @author: Ryan Thorn
 * @date: 17/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */
class Particle;
template <class T> class HashTable
{
private:
	// The linked list of data for this hash table
	LinkedList<T>* m_data;

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
	 * @param string _key The key to identify the data
	 * @param 
	 */

	/**
	 * Inserts data into the hash table
	 * @param LinkedData* _data The data to be inserted into the hash table
	 */
	void Insert(LinkedData<T>* _data);

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
	LinkedData<T>* Find(std::string _key);

	/**
	 * Retrieves the first data within the hashtable
	 * @returns LinkedData* Returns the first data in the hash table
	 */
	LinkedData<T>* Begin();

	/**
	 * Retrieves the last data within the hashtable
	 * @returns LinkedData* Returns the last data in the hash table
	 */
	LinkedData<T>* End();

	/**
	 * Finds an item by its order within the hashmap (used mostly for looping through data)
	 * @param int _index The index to return
	 * @returns LinkedData<T>* The data at this index
	 */
	LinkedData<T>* FindByIndex(int _index);

	// Getters/Setters
	// Returns length of the hash table
	int GetLength() { return m_length; }
};

/**
* Makes an empty hash table with a provided length.
* @param int _length The length of the hash table. Must be above 0. Prime numbers recommended for efficiency. Defaults to 17.
*/
template <class T>
HashTable<T>::HashTable(int _length)
{
	// Ensure that lengths of 0 or less are not allowed. If given set to default length
	if (_length <= 0)
	{
		_length = DEFAULT_LENGTH;
	}

	// Create our linked list array from the size given
	m_data = new LinkedList<T>[_length];
	// Set the length
	m_length = _length;
}

template <class T>
HashTable<T>::~HashTable()
{
}

/**
* Inserts data into the hash table
* @param LinkedData* _data The data to be inserted into the hash table
*/
template <class T>
void HashTable<T>::Insert(LinkedData<T> * _data)
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
template <class T>
bool HashTable<T>::Remove(std::string _key)
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
template <class T>
LinkedData<T> * HashTable<T>::Find(std::string _key)
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
template <class T>
int HashTable<T>::Hash(std::string _key)
{
	// The combined ascii value of the string key given
	int m_asciiValue = 0;

	// Calcluate the ascii value from the key given
	for (unsigned int i = 0; i < _key.length(); i++)
	{
		m_asciiValue += _key[i];
	}

	// Return the hash from the ascii value, modulus the length
	return (m_asciiValue * _key.length()) % m_length;
}

/**
 * Finds an item by its order within the hashmap (used mostly for looping through data)
 * @param int _index The index to return
 * @returns LinkedData<T>* The data at this index
 */
template <class T>
LinkedData<T>* HashTable<T>::FindByIndex(int _index)
{
	// We need to loop through all the data, omitting null items until we reach the indexd item

}

/**
 * Retrieves the first data within the hashtable ( NOT IMPLEMENTED )
 * @returns LinkedData* Returns the first data in the hash table
 */
template <class T>
LinkedData<T>* HashTable<T>::Begin()
{


}

/**
 * Retrieves the last data within the hashtable ( NOT IMPLEMENTED )
 * @returns LinkedData* Returns the last data in the hash table
 */
template <class T>
LinkedData<T>* HashTable<T>::End()
{

}
#endif // !_HASHTABLE_H_

