#ifndef _FPSPROFILER_H_
#define _FPSPROFILER_H_

struct FPSPacket 
{
	int m_max;
	int m_min;
	int m_average;
};

class FPSProfiler
{
private:
	// A constant to define how many frame times to store for profiling
	static const int MAX_FRAME_TIMES = 10;
	// The output file
	std::string m_outputFile;

	// A map of fps data to particle count (Key: particle count, Data: FPSPacket)
	std::map<int, FPSPacket> m_fpsMap;

	// Live profile feeds
	FPSPacket m_currentFPS;

	// An array of all frametimes so we can make an average. Size of 10 as we store 10 last times
	Uint32 m_frameTimes[MAX_FRAME_TIMES];
	// The number of frame times available
	Uint32 m_frameCount;
	// The last time
	Uint32 m_frameTimeLast;

	// The last particle count
	int m_lastParticleCount;
	// Total number of collision checks
	int m_collisionChecks;
public:
	FPSProfiler(std::string _outputFile);
	~FPSProfiler();

	/**
	 * Runs the profiler to monitor FPS
	 * @param _particleCount int The current particle count
	 */
	void Run(int _particleCount);

	/**
	 * Exports the fps profile to a file
	 */
	void Export();

	// Getters for profile feeds
	FPSPacket GetCurrentFPS() { return m_currentFPS; }
	// Pluses the collisions by one
	void AddCollision() { m_collisionChecks++; }
};
#endif // !_FPSPROFILER_H_

