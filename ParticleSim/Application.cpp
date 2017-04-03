#include "Stdafx.h"
#include "Application.h"
/**
 * Application class which defines the main sequences for the application (Init, Update, and Exit)
 * @file: Application.cpp
 * @author: Ryan Thorn
 * @date: 16/03/2017
 * @copyright: Copyright Ryan Thorn (c) 2017. All rights reserved.
 */

Application::Application()
{
	// Defaults
	// SDL defaults
	m_window = nullptr;
	m_renderer = nullptr;
	
	// Engine defaults
	m_running = false;

	// Timing defaults
	m_frames = 0;
	m_lastTime = 0;
	m_currentTime = 0;
	m_deltaTime = 0.0166666667f; // Default deltatime to 1/60 for first frame
	m_particleStep = 1000; // Increment/decrement by a 1000
	m_drawDebugLines = false;
	m_drawFPSProfile = true;
	// Default our function key states
	for (int i = 0; i < 12; i++)
	{
		m_functionKeys[i] = false;
	}
}

Application::~Application()
{}

// Runs the application in the Init->Update->Exit order with error checking
bool Application::Run()
{
	// Checks each run to ensure it returned true. If it didnt this will return false to main
	if (Init())
	{
		if (Update())
		{
			if (Exit())
			{
				// Returns true to main stating that everything ran okay and we can exit now.
				return true;
			}
		}
	}
	return false;
}

// Initialises the application
bool Application::Init()
{
	// Load in our settings file into the json DOM
	std::ifstream m_settingsFile("settings.json");
	rapidjson::IStreamWrapper m_settingsWrapped(m_settingsFile);
	m_settings.ParseStream(m_settingsWrapped);

	m_rngv = std::uniform_int_distribution<int>(-50, 50);
	m_rngpw = std::uniform_int_distribution<int>(1, m_settings["WindowWidth"].GetInt() - 1);
	m_rngph = std::uniform_int_distribution<int>(1, m_settings["WindowHeight"].GetInt() - 1);
	// Init SDL with video mode
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Failed to init SDL
		std::cerr << "Failed to init SDL!\n";
		return false;
	}

	if (TTF_Init() < 0)
	{
		// Failed to init ttf plugin
		std::cerr << "Failed to init TTF plugin. " << TTF_GetError() << "\n";
		return false;
	}

	// Create the SDL window
	m_window = SDL_CreateWindow(m_settings["ProgramTitle"].GetString(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		m_settings["WindowWidth"].GetInt(), m_settings["WindowHeight"].GetInt(),
		SDL_WINDOW_SHOWN);

	// Check the window was made okay
	if (m_window == NULL)
	{
		std::cerr << "Failed to create SDL window!\n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", "Failed to create SDL Window!", NULL);
		return false;
	}

	// Create the SDL renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	// Create the SDL surface from our window
	m_surface = SDL_GetWindowSurface(m_window);

	// Check the renderer was made okay
	if (m_renderer == NULL)
	{
		std::cerr << "Failed to create SDL renderer!\n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", "Failed to create SDL Renderer!", NULL);
		return false;
	}
	
	// Create our profiler
	m_profiler = new FPSProfiler("FPS_Profile/profile");

	// Create our spatial hash table
	m_sht = new SpatialHashTable(m_settings["WindowWidth"].GetInt(), m_settings["WindowHeight"].GetInt(), 32);

	// Create our particles from the count given in the settings json
	m_particles.clear();
	for (int i = 0; i < m_settings["ParticleCount"].GetInt(); i++)
	{
		m_particles.push_back(new Particle(glm::vec2(m_rngpw(m_rng), m_rngph(m_rng)), glm::vec2(m_rngv(m_rng), m_rngv(m_rng)),
			glm::vec2(0, 0), glm::vec3(rand() % 255 + 200, rand() % 255 + 200, rand() % 255 + 200), 500.0f, 1.0f));
	}
	
	// Load our text
	m_umText = new UIText("resources/fonts/ubuntumono/UbuntuMono-Bold.ttf", 16);

	// Get the last time to calculate deltatime for the first runthrough.
	/* No more code should be under this line in the init function unless its
	   timing related or enabling the update loop */
	m_lastTime = SDL_GetTicks();
	m_running = true;
	
	return true;
}

// Updates the application's runtime
bool Application::Update()
{
	// Game loop
	while (m_running)
	{
		// Poll our events
		while (SDL_PollEvent(&m_events))
		{
			// Switch event types
			switch (m_events.type)
			{
				// Quitting the program
				case SDL_QUIT:
				{
					m_running = false;
					break;
				}
				// Keyboard down events
				case SDL_KEYDOWN:
				{
					// Switch keys
					switch (m_events.key.keysym.sym)
					{
						// Escape key
						case SDLK_ESCAPE:
						{
							// Quit
							m_running = false;
							break;
						}
						// Up key
						case SDLK_UP:
						{
							// Add 1000 particles
							AddParticles(m_particleStep);
							break;
						}
						// Down key
						case SDLK_DOWN:
						{
							// Remove 1000 particles
							RemoveParticles(m_particleStep);
							break;
						}
						// F1 key
						case SDLK_F1:
						{
							// Toggle debug cell lines
							if (m_functionKeys[1] != true)
							{
								m_drawDebugLines = !m_drawDebugLines;
							}
							m_functionKeys[1] = true;
							break;
						}
						// F2 key
						case SDLK_F2:
						{
							// Toggle UI
							if (m_functionKeys[2] != true)
							{
								m_drawFPSProfile = !m_drawFPSProfile;
							}
							m_functionKeys[2] = true;
							break;
						}
					}
					break;
				}
				// Keyboard key up events
				case SDL_KEYUP:
				{
					// switch keys
					switch (m_events.key.keysym.sym)
					{
						// Set states to false
						case SDLK_F1:
						{
							m_functionKeys[1] = false;
							break;
						}
						case SDLK_F2:
						{
							m_functionKeys[2] = false;
							break;
						}
					}
					break;
				}
			}
		} // End of events

		// Calculate deltatime
		m_currentTime = SDL_GetTicks();
		m_deltaTime = (float)(m_currentTime - m_lastTime) / 1000.0f;
		m_lastTime = m_currentTime;

		// Run our FPS profiler
		m_profiler->Run(m_settings["ParticleCount"].GetInt());
		// Update scene

		// Clear our SpatialHashTable
		m_sht->Clear();

		// Loop through every particle, updating it and adding it to the spatial hash table
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i)->Update(m_deltaTime, (*m_sht));
			m_sht->AddParticle(m_particles.at(i));
		}

		// Clear our buffer
		SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
		SDL_RenderClear(m_renderer);
		// Render scene
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i)->Draw(m_renderer);
		}

		// Render UI
		if (m_drawDebugLines)
		{
			m_sht->DrawCellLines(m_renderer);
		}
		
		// Display FPS
		if (m_drawFPSProfile)
		{
			m_umText->Printf(m_renderer, glm::vec2(10, 10), { 255, 255, 255 }, "Avg. FPS: %i", (int)m_profiler->GetCurrentFPS().m_average);
			m_umText->Printf(m_renderer, glm::vec2(10, 30), { 255, 255, 255 }, "Max FPS: %i", (int)m_profiler->GetCurrentFPS().m_max);
			m_umText->Printf(m_renderer, glm::vec2(10, 50), { 255, 255, 255 }, "Min FPS: %i", (int)m_profiler->GetCurrentFPS().m_min);
			// Display particle count
			m_umText->Printf(m_renderer, glm::vec2(10, 70), { 255, 255, 255 }, "Particle Count: %i", m_settings["ParticleCount"].GetInt());
			m_umText->Print(m_renderer, glm::vec2(10, 90), { 200, 200, 255 }, "Press 'F2' to hide/unhide the UI. Press 'F1' to show gridlines of our spatial hash table.");
			m_umText->Print(m_renderer, glm::vec2(10, m_settings["WindowHeight"].GetInt() - 20), { 200, 200, 255 }, "Press 'Up Arrow' to increase particles. Press 'Down Arrow' to decrease particles.");
		}
		
		// Present the renderer buffer to the screen
		SDL_RenderPresent(m_renderer);

		// Used to limit fps but is disabled to show the full fps range of the application
		//if (m_settings["MaxFPS"].GetInt() > 0) // If the MaxFPS is set to higher than 0
		//{
		//	// Check if we have gone over the fps limit
		//	if (m_deltaTime < (1.0f / m_settings["MaxFPS"].GetInt()))
		//	{
		//		// We have gone over our fps limit so lets wait it out	
		//		SDL_Delay((unsigned int)(((1.0f / m_settings["MaxFPS"].GetInt()) - m_deltaTime) * 1000.0f));
		//	}
		//}
	}

	return true;
}

// Exit sequence for the application
bool Application::Exit()
{
	// Export our profiler data to file
	m_profiler->Export();
	// Destroy everything
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	TTF_Quit();
	SDL_Quit();

	return true;
}

/**
* Add particles to the simulation
* @param _amount int Amount of particles to add
*/
void Application::AddParticles(int _amount)
{
	// Update the particle count number
	m_settings["ParticleCount"].SetInt(m_settings["ParticleCount"].GetInt() + _amount);

	// Loop through adding the new amount of particles
	for (int i = 0; i < _amount; i++)
	{
		m_particles.push_back(new Particle(glm::vec2(m_rngpw(m_rng), m_rngph(m_rng)), glm::vec2(m_rngv(m_rng), m_rngv(m_rng)),
			glm::vec2(0, 0), glm::vec3(rand() % 255 + 200, rand() % 255 + 200, rand() % 255 + 200), 500.0f, 1.0f));
	}
}

/**
* Remove particles from the simulation
* @param _amount int Amount of particles to remove
*/
void Application::RemoveParticles(int _amount)
{
	// Check we are not going below 0. Very bad
	if (m_settings["ParticleCount"].GetInt() - _amount < 1)
	{
		// Return if we do
		return;
	}

	int m_count = 0;
	m_settings["ParticleCount"].SetInt(m_settings["ParticleCount"].GetInt() - _amount);
	// Our iterator
	std::vector<Particle*>::iterator m_iter;
	
	// Loop through until we have cleaned up enough particles
	for (m_iter = m_particles.begin(); m_iter != m_particles.end();)
	{
		// check if we have reached our amount
		if (m_count > _amount)
		{
			// We reached the limit, break out the loop
			break;
		}

		// Delete more
		delete * m_iter;
		m_iter = m_particles.erase(m_iter);
		// Increase our count
		m_count++;
	}
}

/* STATIC IMPLEMENTS */
// The static instance variable that stores the one instance of itself
Application* Application::s_instance = nullptr;

// The Instance getter method. Returns the static instance of itself (or makes one if it doesn't yet exist
// This is a singleton class structure
Application* Application::Instance()
{
	// Check if the instance is null
	if (s_instance == nullptr)
	{
		// Instance is null, we need to make one
		s_instance = new Application();
	}

	// Return the instance of the application
	return s_instance;
}