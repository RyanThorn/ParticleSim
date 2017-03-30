#ifndef _SPATIALHASHTABLE_H_
#define _SPATIALHASHTABLE_H_
class SpatialHashTable
{
private:
	int m_screenWidth, m_screenHeight;
	int m_cellSize;

	int m_tableColumns, m_tableRows;
	int m_tableSize;

	std::vector<Particle*>* m_hashTable;
public:
	SpatialHashTable(int _screenWidth, int _screenHeight, int _cellSize);
	~SpatialHashTable();

	/**
	 * Clears the hash table and resets it to a fresh table
	 */
	void Clear();

	/**
	 * Adds a particle to the spatial hash table
	 * @param Particle* _particle The particle to add to the hash table
	 */
	void AddParticle(Particle* _particle);

	/** 
	 * Returns a list of cell indices from the provided particle
	 * @param Particle* _particle The particle to get the cell indices for
	 * @returns list<int> A list of cell indices that this particle is in
	 */
	std::list<int> GetCellIndices(Particle* _particle);

	/**
	 * Generates a hash (or cell position) for a coordinate position
	 * @param glm::vec2 _position The position of the particle on the screen eg:(235, 732)
	 * @returns int Returns the hash (cell position) of this screen position
	 */
	int Hash(glm::vec2 _position);

	/**
	 * Returns a vector of particles which are close to the given particle
	 * @param Particle* _particle The particle to use as the search case
	 * @returns vector<Particle*> A vector of particles near the given particle
	 */
	std::vector<Particle*> GetLocalObjects(Particle* _particle);
};
#endif // !_SPATIALHASHTABLE_H_

