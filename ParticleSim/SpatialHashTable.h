#ifndef _SPATIALHASHTABLE_H_
#define _SPATIALHASHTABLE_H_
class SpatialHashTable
{
private:
	// Screen width and cell size passed through in the constructor
	int m_screenWidth, m_screenHeight;
	int m_cellSize;

	// Table columns, rows and size
	int m_tableColumns, m_tableRows;
	int m_tableSize;

	// Holds a pointer array of vectors for our buckets (hash map)
	std::vector<Particle*>* m_hashTable;
public:
	// Constructor for the spatial hash table
	SpatialHashTable(int _screenWidth, int _screenHeight, int _cellSize);
	~SpatialHashTable();

	/**
	 * Clears the hash table and resets it to a fresh table
	 */
	void Clear();

	/**
	 * Adds a particle to the spatial hash table
	 * @param _particle Particle* The particle to add to the hash table
	 */
	void AddParticle(Particle* _particle);

	/** 
	 * Returns a list of cell indices from the provided particle
	 * @param _particle Particle* The particle to get the cell indices for
	 * @returns list<int> A list of cell indices that this particle is in
	 */
	std::list<int> GetCellIndices(Particle* _particle);

	/**
	 * Generates a hash (or cell position) for a coordinate position
	 * @param _position glm::vec2 The position of the particle on the screen eg:(235, 732)
	 * @returns int Returns the hash (cell position) of this screen position
	 */
	int Hash(glm::vec2 _position);

	/**
	 * Returns a vector of particles which are close to the given particle
	 * @param _particle Particle* The particle to use as the search case
	 * @returns vector<Particle*> A vector of particles near the given particle
	 */
	std::vector<Particle*> GetLocalObjects(Particle* _particle);

	/**
	 * Draws the cell boundaries for debugging purposes
	 * @param _renderer SDL_Renderer* The SDL renderer used to draw graphics
	 */
	void DrawCellLines(SDL_Renderer* _renderer);

	/** Getters **/
	std::vector<Particle*>* GetHashTable() { return m_hashTable; }
	int GetSize() { return m_tableSize; }
};
#endif // !_SPATIALHASHTABLE_H_

