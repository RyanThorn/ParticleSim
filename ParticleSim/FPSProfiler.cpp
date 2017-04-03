#include "Stdafx.h"
#include "FPSProfiler.h"

/** 
 * Constructs a profiler which takes an output file location to output the results
 * @param _outputFile char* The output file location for the results
 */
FPSProfiler::FPSProfiler(std::string _outputFile)
{
	// Get the current time for the output file name
	time_t m_time = time(0);
	struct tm m_now;
	localtime_s(&m_now, &m_time);
	// Store the filename stream
	std::stringstream m_fileName;

	// Make the filename with date (filename-day-month-year-hour-minute-second.txt)
	m_fileName << _outputFile.c_str() << "-" << std::setfill('0') << std::setw(2) << m_now.tm_mday << "-" << std::setfill('0') << std::setw(2) << (m_now.tm_mon + 1)
		<< "-" << (m_now.tm_year + 1900) << "-" << std::setfill('0') << std::setw(2) << m_now.tm_hour << "-" << std::setfill('0') << std::setw(2) << m_now.tm_min << "-" << std::setfill('0') << std::setw(2) << m_now.tm_sec << ".txt" << std::setfill(' ');

	// Set the filename
	m_outputFile = m_fileName.str();

	// Default our current fps packet, we default the max to a very low value and our min to a very high value to ensure
	// that we get the very first result registered correctly.
	m_currentFPS = { -1000,1000,0 };

	// Default the timing variables
	memset(m_frameTimes, 0, sizeof(m_frameTimes));
	m_frameCount = 0;
	m_frameTimeLast = SDL_GetTicks();

	m_lastParticleCount = 0;
	m_collisionChecks = 0;
}

FPSProfiler::~FPSProfiler()
{
}

/**
 * Runs the profiler to monitor FPS
 * @param _particleCount int The current particle count
 */
void FPSProfiler::Run(int _particleCount)
{
	// The index on our time array
	Uint32 m_index;
	// The current time in ticks
	Uint32 m_currentTicks;
	// The current count of times in the array
	Uint32 m_count;

	// We want to reset the currents if the particle count changes
	if (m_lastParticleCount != _particleCount)
	{
		// Particle count changes
		m_lastParticleCount = _particleCount;
		m_currentFPS = { -1000, 1000, 0 };
	}

	// Calculate the index
	m_index = m_frameCount % MAX_FRAME_TIMES;
	// Get the current ticks
	m_currentTicks = SDL_GetTicks();
	// Save this value
	m_frameTimes[m_index] = m_currentTicks - m_frameTimeLast;
	// Save the last frametime
	m_frameTimeLast = m_currentTicks;
	// Move to the next frame count
	m_frameCount++;

	// Check if we have a full set of values or not
	m_count = (m_frameCount < MAX_FRAME_TIMES ? m_frameCount : MAX_FRAME_TIMES);

	// Find the mean average
	m_currentFPS.m_average = 0; // Reset the current average
	for (unsigned int i = 0; i < m_count; i++)
	{
		m_currentFPS.m_average += m_frameTimes[i];
	}
	// Divide by the count to find the mean
	m_currentFPS.m_average /= m_count;
	// Convert the mean time average into an actual fps number
	m_currentFPS.m_average = 1000.f / m_currentFPS.m_average;

	// Check if we found a new max fps, wait 1000 ticks before we define this
	if (m_currentFPS.m_average > m_currentFPS.m_max && m_currentTicks > 1000)
	{
		// Found new max, set it
		m_currentFPS.m_max = m_currentFPS.m_average;
	}

	// Check if we found a new min fps, wait 1000 ticks before we define this
	if (m_currentFPS.m_average < m_currentFPS.m_min && m_currentTicks > 1000)
	{
		// Found new min, set it
		if (m_currentFPS.m_average >= 0)
		{
			m_currentFPS.m_min = m_currentFPS.m_average;
		}
	}

	// Store the last fps for this particle count
	m_fpsMap[_particleCount] = m_currentFPS;
}

/**
* Exports the fps profile to a file
*/
void FPSProfiler::Export()
{
	// Get the current time
	time_t m_time = time(0);
	struct tm m_now;
	localtime_s(&m_now, &m_time);

	std::ofstream m_output;
	// Open our file
	m_output.open(m_outputFile, std::ios::out | std::ios::trunc);

	// check its open
	if (m_output.is_open())
	{
		// Create our output, this is the title of the file with the date and time
		m_output << "== FPS Profile -- Ryan Thorn -- " << std::setfill('0') << std::setw(2) << m_now.tm_mday << "-" << std::setfill('0') << std::setw(2) << (m_now.tm_mon + 1)
			<< "-" << (m_now.tm_year + 1900) << " " << std::setfill('0') << std::setw(2) << m_now.tm_hour << ":" << std::setfill('0') << std::setw(2) << m_now.tm_min << ":" << std::setfill('0') << std::setw(2) << m_now.tm_sec << " ==\n";
		// Output the total runtime in seconds
		m_output << "Total Runtime: " << SDL_GetTicks() / 1000.0f << " seconds\n";
		m_output << "Total Collision Calculations: " << m_collisionChecks << "\n\n";

		// Output the headers to our table
		m_output << std::setfill(' ') << std::left << std::setw(20) << "Particle Count" << std::left << std::setw(20) << "Average FPS" << std::left << std::setw(20) << "Maximum FPS" << std::left << std::setw(20) << "Minimum FPS" << "\n";
		
		// Loop through our map of fps data
		for (auto const &data : m_fpsMap)
		{
			// Output each fps data for each particle count
			m_output << std::left << std::setw(20) << data.first << std::left << std::setw(20) << data.second.m_average << std::left << std::setw(20) << data.second.m_max << std::left << std::setw(20) << data.second.m_min << "\n";
		}
		// close the file
		m_output.close();
	}
	else
	{
		// output if we failed to save the file
		std::cerr << "Failed to open output file for FPS profiler\n";
	}
}