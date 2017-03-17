#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
/**
 * Defines a linked list of data. Used in conjunction with the hash table data structure to 
 * process hash collisions
 * @file: LinkedList.h
 * @author: Ryan Thorn
 * @date: 17/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */

// Defines a data packet to be stored within the linked list
struct LinkedData
{
	std::string m_key;
	LinkedData *m_next;
};

// Our linked list defined by a root node data
class LinkedList
{
private:
	// The root node (head node) of this list
	LinkedData *m_root;
	// The length of this list
	int m_length;

public:
	LinkedList();
	~LinkedList();

	/**
	 * Inserts the specified data to the back of the list (push back)
	 * @param LinkedData* _data The data to be inserted to the end of the list
	 */
	void Insert(LinkedData * _data);

	/**
	 * Removes data from the list based on the key provided
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

	// Getters/setters
	int GetLength() { return m_length; }
};
#endif // !_LINKEDLIST_H_

