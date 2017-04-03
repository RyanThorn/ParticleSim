#ifndef _APPLICATION_H_
#define _APPLICATION_H_
/**
 * Application class which defines the main sequences for the application (Init, Update, and Exit)
 * @file: Application.h
 * @author: Ryan Thorn
 * @date: 16/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */
class Application
{
private:
	// SDL Variables
	SDL_Window* m_window; // SDL window
	SDL_Renderer* m_renderer; // SDL renderer
	SDL_Surface* m_surface; // SDL window surface
	SDL_Event m_events; // The current event being parsed from SDL

	// Engine Variables
	bool m_running; // Defines if the update loop is running or not
	bool m_functionKeys[12]; // A storage defining the state of the 12 function keys
	bool m_drawDebugLines; // Draws the cell lines when true
	bool m_drawFPSProfile; // Draws the fps profile when true

	// Timing Variables
	unsigned int m_lastTime; // The last frames time
	unsigned int m_currentTime; // The current frames time
	float m_deltaTime; // The delta between the last frame and current frame times
	unsigned int m_frames; // A frame counter
	float m_fps;  // current fps

	// Game storage
	std::vector<Particle*> m_particles; // Vector of all particles in the game. Used for iteration through ALL particles
	SpatialHashTable* m_sht;// Spatial hashtable for collision detection
	UIText* m_umText; // Ubuntu Mono Text
	FPSProfiler* m_profiler; // Our profiler

	int m_particleStep; // The amount of particles to increase or decrease when the buttons are pressed

	// Random number distributers
	std::default_random_engine m_rng;
	std::uniform_int_distribution<int> m_rngv;
	std::uniform_int_distribution<int> m_rngpw;
	std::uniform_int_distribution<int> m_rngph;

	// Json Inputs
	rapidjson::Document m_settings; // The settings json data from the settings.json file

	/* STATIC MEMBERS */
	static Application* s_instance;
public:
	Application();
	~Application();

	// Initialises the application
	bool Init();
	// Updates the application's runtime
	bool Update();
	// Exit sequence for the application
	bool Exit();
	// Runs the application in the Init->Update->Exit order with error checking
	bool Run();

	/**
	 * Add particles to the simulation
	 * @param _amount int Amount of particles to add
	 */
	void AddParticles(int _amount);

	/**
	* Remove particles from the simulation
	* @param _amount int Amount of particles to remove
	*/
	void RemoveParticles(int _amount);

	// Getters
	glm::vec2 GetWindowSizes() { return glm::vec2(m_settings["WindowWidth"].GetInt(), m_settings["WindowHeight"].GetInt()); }
	FPSProfiler* GetProfiler() { return m_profiler; }

	/* STATIC METHODS*/
	static Application* Instance();
};

#endif // !_APPLICATION_H_

