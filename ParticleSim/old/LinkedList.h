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
class Particle;
// Defines a data packet to be stored within the linked list
template <class T> class LinkedData
{
public:
	std::string m_key;
	T m_data;
	LinkedData<T> *m_next;
};

// Our linked list defined by a root node data
template <class T> class LinkedList
{
private:
	// The root node (head node) of this list
	LinkedData<T> *m_root;
	// The length of this list
	int m_length;

public:
	LinkedList();
	~LinkedList();

	/**
	 * Inserts the specified data to the back of the list (push back)
	 * @param LinkedData* _data The data to be inserted to the end of the list
	 */
	void Insert(LinkedData<T> * _data);

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
	LinkedData<T>* Find(std::string _key);

	// Getters/setters
	int GetLength() { return m_length; }
};

/**
* Constructor will make an empty list structure
*/
template <class T>
LinkedList<T>::LinkedList()
{
	// An empty root node to begin the linked list
	m_root = new LinkedData<T>;
	// Set its next item to null
	m_root->m_next = nullptr;
	// As the next item is null we have a length of 0 as the root node doesn't count towards its length
	m_length = 0;
}

/**
* Destructor will destroy the linked list's memory and reset it all to null
*/
template <class T>
LinkedList<T>::~LinkedList()
{
	// Create two temporary storages for the current item and its next item
	LinkedData<T> *m_current = m_root;
	LinkedData<T> *m_next = m_root;
	// While a next item exists
	while (m_next)
	{
		// Set the current item as this next item
		m_current = m_next;
		// Set the new next item
		m_next = m_current->m_next;
		// If the new next item exists
		if (m_next)
		{
			// Delete it
			delete m_next;
		}
	}
}

/**
* Inserts the specified data to the back of the list (push back)
* @param LinkedData* _data The data to be inserted to the end of the list
*/
template <class T>
void LinkedList<T>::Insert(LinkedData<T> * _data)
{
	// If the root doesn't have a next
	if (!m_root->m_next)
	{
		// Set the inserted item as the next item
		m_root->m_next = _data;
		// Set the inserted data's next node to null
		_data->m_next = nullptr;
		// Add up the new length
		m_length++;
		return;
	}

	// Else we need to find the end of the list
	// Temporary storage for the current node and next node
	LinkedData<T> *m_current = m_root;
	LinkedData<T> *m_next = m_root;

	// Loop through until we have no next node
	while (m_next)
	{
		// Swap our current to the next item
		m_current = m_next;
		// Get the next item from the current item
		m_next = m_current->m_next;
	}
	// Our current item now has an empty next node, so lets use it
	m_current->m_next = _data;
	// Set the inserted data's next node to null
	_data->m_next = nullptr;
	// Update the length
	m_length++;

}

/**
* Removes data from the list based on the key provided
* @param string _key The key to identify the data to be removed
* @returns bool True if successful, false if unsuccessful
*/
template <class T>
bool LinkedList<T>::Remove(std::string _key)
{
	// Check if this is an empty list
	if (!m_root->m_next)
	{
		// Its empty, no point in going further
		return false;
	}
	// Temporary storage for the current node and next node
	LinkedData<T> *m_current = m_root;
	LinkedData<T> *m_next = m_root;

	// Loop through until we have no next node
	while (m_next)
	{
		if (m_next->m_key == _key)
		{
			// Found the item, lets reconnect the next link to avoid this item
			// and then delete it
			m_current->m_next = m_next->m_next;
			// Delete it
			delete m_next;
			// Update the length
			m_length--;
			// Return true to notify of success
			return true;
		}
		// Swap to next node until found
		m_current = m_next;
		m_next = m_current->m_next;
	}

	// Didn't find it. Return false
	return false;
}

/**
* Retrieves data from the given key
* @param string _key The key to identify the data to be returned
* @returns LinkedData Returns the data if found. If not it returns a nullptr
*/
template <class T>
LinkedData<T> * LinkedList<T>::Find(std::string _key)
{
	// Temporary storage for the current node and next node
	LinkedData<T> *m_current = m_root;
	LinkedData<T> *m_next = m_root;

	// Loop through until we have no next node
	while (m_next)
	{
		// Swap our current to the next item
		m_current = m_next;

		// See if we found what we were looking for
		if ((m_current != m_root) && (m_current->m_key == _key))
		{
			// We found the item and it wasnt the root node so lets return it
			return m_current;
		}

		// Get the next item from the current item
		m_next = m_current->m_next;
	}
	// Didn't find it. Return null
	return nullptr;
}

#endif // !_LINKEDLIST_H_

