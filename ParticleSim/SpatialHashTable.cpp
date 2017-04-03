#include "Stdafx.h"
#include "SpatialHashTable.h"

SpatialHashTable::SpatialHashTable(int _screenWidth, int _screenHeight, int _cellSize)
{
	// Setup the SHT parameters
	m_screenWidth = _screenWidth;
	m_screenHeight = _screenHeight;
	m_cellSize = _cellSize;

	m_tableColumns = _screenWidth / _cellSize;
	m_tableRows = _screenHeight / _cellSize;

	m_tableSize = m_tableColumns * m_tableRows;

	// Create our hashtable and run our Clear function to make sure its initialised
	m_hashTable = new std::vector<Particle*>[m_tableSize];
	
	Clear();
}

SpatialHashTable::~SpatialHashTable()
{
}

/**
 * Clears the hash table and resets it to a fresh table
 */
void SpatialHashTable::Clear()
{
	// Clear the hash table and reset it to a new one
	for (int i = 0; i < m_tableSize; i++)
	{
		m_hashTable[i].clear();
	}
}

/**
 * Adds a particle to the spatial hash table
 * @param _particle Particle* The particle to add to the hash table
 */
void SpatialHashTable::AddParticle(Particle* _particle)
{
	// Get the list of cell indicies that this particle falls into
	std::list<int> m_cellIndices = GetCellIndices(_particle);
	// Store an iterator of a list of ints
	std::list<int>::iterator m_iter;

	// Iterate over the list adding the particle to each cell (bucket) that it is part of
	for (m_iter = m_cellIndices.begin(); m_iter != m_cellIndices.end(); ++m_iter)
	{
		if (*m_iter >= 0 && *m_iter < m_tableSize)
		{
			m_hashTable[*m_iter].push_back(_particle);
		}
	}
}

/**
* Returns a list of cell indices from the provided particle
* @param _particle Particle* The particle to get the cell indices for
* @returns list<int> A list of cell indices that this particle is in
*/
std::list<int> SpatialHashTable::GetCellIndices(Particle* _particle)
{
	// Stores the cell indicies for the given particle (should range between 1 cell index to 4 indices depending
	// on where the particle is
	std::list<int> m_cellIndices;

	// Get the bounding box of the particle
	glm::vec2 m_boundMin = glm::vec2(_particle->Position().x - _particle->Radius(),
		_particle->Position().y - _particle->Radius());
	glm::vec2 m_boundMax = glm::vec2(_particle->Position().x + _particle->Radius(),
		_particle->Position().y + _particle->Radius());

	// Storage for the cell index
	int m_currentCellIndex = -1;

	// Now check the 4 corners of the bounding box to see what cell its in
	// Top left
	m_cellIndices.push_back(Hash(m_boundMin));
	// Top right
	m_cellIndices.push_back(Hash(glm::vec2(m_boundMax.x, m_boundMin.y)));
	// Bottom left
	m_cellIndices.push_back(Hash(glm::vec2(m_boundMin.x, m_boundMax.y)));
	// Bottom Right
	m_cellIndices.push_back(Hash(m_boundMax));

	// Check that we do not have duplicate entries by running the unique function
	m_cellIndices.unique();

	// Return the indices
	return m_cellIndices;
}

/**
 * Generates a hash (or cell position) for a coordinate position
 * Hashes based on (floor(x / cell_size)) + (floor(y / cell_size)) * (screen_width / cell_size)
 * @param _position glm::vec2 The position of the particle on the screen eg:(235, 732)
 * @returns int Returns the hash (cell position) of this screen position. Also returns -1 if the hash is out of range of the table size
 */
int SpatialHashTable::Hash(glm::vec2 _position)
{
	// Should return a number which is between 0 and (table_columns * table_rows) - 1
	int m_hash = (int)((floor(_position.x / m_cellSize)) + (floor(_position.y / m_cellSize)) * (m_screenWidth / m_cellSize));

	// force any outliers to just be in cell 1 for now (this isnt efficient but need to think of a better way around it)
	if (m_hash > (m_tableSize - 1) || m_hash < 0)
	{
		m_hash = -1;
	}

	return m_hash;
}

/**
 * Returns a vector of particles which are close to the given particle
 * @param _particle Particle* The particle to use as the search case
 * @returns vector<Particle*> A vector of particles near the given particle
 */
std::vector<Particle*> SpatialHashTable::GetLocalObjects(Particle* _particle)
{
	// Get the list of cell indicies that this particle falls into
	std::list<int> m_cellIndices = GetCellIndices(_particle);
	// Store an iterator of a list of ints
	std::list<int>::iterator m_iter;

	// The return vector of particles
	std::vector<Particle*> m_return;

	// Iterate over the list finding any objects that are also in the given buckets and adding them to the return
	// vector
	for (m_iter = m_cellIndices.begin(); m_iter != m_cellIndices.end(); ++m_iter)
	{
		// Insert the buckets found onto the end of the return vector
		if (*m_iter >= 0 && *m_iter < m_tableSize)
		{
			m_return.insert(m_return.end(), m_hashTable[*m_iter].begin(),
				m_hashTable[*m_iter].end());
		}
	}

	// return the vector
	return m_return;
}

/**
* Draws the cell boundaries for debugging purposes
* @param _renderer SDL_Renderer* The SDL renderer used to draw graphics
*/
void SpatialHashTable::DrawCellLines(SDL_Renderer* _renderer)
{
	SDL_SetRenderDrawColor(_renderer, 43, 206, 239, 255);
	// Draw column lines
	for (int i = 0; i < m_tableColumns; i++)
	{
		SDL_RenderDrawLine(_renderer, i * m_cellSize, 0, i * m_cellSize, m_screenHeight);
	}

	// Draw row lines
	for (int i = 0; i < m_tableRows; i++)
	{
		SDL_RenderDrawLine(_renderer, 0, i * m_cellSize, m_screenWidth, i * m_cellSize);
	}	
}
