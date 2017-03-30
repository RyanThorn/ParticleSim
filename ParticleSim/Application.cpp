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

	std::default_random_engine m_rng;
	std::uniform_int_distribution<int> m_rngv(-50, 50);
	std::uniform_int_distribution<int> m_rngpw((m_settings["WindowWidth"].GetInt() / 2) - 100, (m_settings["WindowWidth"].GetInt() / 2) + 100);
	std::uniform_int_distribution<int> m_rngph((m_settings["WindowHeight"].GetInt() / 2) - 100, (m_settings["WindowHeight"].GetInt() / 2) + 100);
	// Init SDL with video mode
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		// Failed to init SDL
		std::cerr << "Failed to init SDL!\n";
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
	m_renderer = SDL_CreateRenderer(m_window, -1, 0);
	// Create the SDL surface from our window
	m_surface = SDL_GetWindowSurface(m_window);

	// Check the renderer was made okay
	if (m_renderer == NULL)
	{
		std::cerr << "Failed to create SDL renderer!\n";
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Error", "Failed to create SDL Renderer!", NULL);
		return false;
	}

	// Create our spatial hash table
	m_sht = new SpatialHashTable(m_settings["WindowWidth"].GetInt(), m_settings["WindowHeight"].GetInt(), 64);

	// Create our particles from the count given in the settings json
	m_particles.clear();
	for (int i = 0; i < m_settings["ParticleCount"].GetInt(); i++)
	{
		m_particles.push_back(new Particle(glm::vec2(m_rngpw(m_rng), m_rngph(m_rng)), glm::vec2(m_rngv(m_rng), m_rngv(m_rng)),
			glm::vec2(0, 0), glm::vec3(255, 255, 255), 500.0f, 2.0f));
	}
	//glm::vec3(rand() % 255 + 100, rand() % 255 + 100, rand() % 255 + 100)
	m_particles[1]->Radius(5.0f);
	// Get the last time to calculate deltatime for the first runthrough.
	/* No more code should be under this line in the init function unless its
	   timing related or enabling the update loopffg */
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
			switch (m_events.type)
			{
				case SDL_QUIT:
				{
					m_running = false;
					break;
				}
				case SDL_KEYDOWN:
				{
					switch (m_events.key.keysym.sym)
					{
						case SDLK_ESCAPE:
						{
							m_running = false;
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
		// Update scene

		// Clear our SpatialHashTable
		m_sht->Clear();

		// Loop through every particle, updating it and adding it to the spatial hash table
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i)->Update(m_deltaTime);
			m_sht->AddParticle(m_particles.at(i));
		}
		m_localGroup = m_sht->GetLocalObjects(m_particles[1]);
		unsigned int m_localGroupSize = m_localGroup.size();
		for (unsigned int i = 0; i < m_localGroupSize; i++)
		{
			m_localGroup[i]->Colour(glm::vec3(255, 0, 0));
		}

		m_particles[1]->Colour(glm::vec3(0, 255, 0));
		SDL_SetRenderDrawColor(m_renderer, 25, 25, 25, 255);
		SDL_RenderClear(m_renderer);
		// Render scene
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i)->Draw(m_renderer);
		}
		
		SDL_RenderPresent(m_renderer);
		if (m_settings["MaxFPS"].GetInt() > 0)
		{
			// If the MaxFPS is set to higher than 0
			if (m_deltaTime < (1.0f / m_settings["MaxFPS"].GetInt()))
			{
				SDL_Delay((unsigned int)(((1.0f / m_settings["MaxFPS"].GetInt()) - m_deltaTime) * 1000.0f));
			}
		}
	}

	return true;
}

// Exit sequence for the application
bool Application::Exit()
{
	// Destroy everything
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);

	SDL_Quit();

	return true;
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