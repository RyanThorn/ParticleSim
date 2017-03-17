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

	// Timing Variables
	unsigned int m_lastTime; // The last frames time
	unsigned int m_currentTime; // The current frames time
	float m_deltaTime; // The delta between the last frame and current frame times
	unsigned int m_frames; // A frame counter

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

	/* STATIC METHODS*/
	static Application* Instance();
};

#endif // !_APPLICATION_H_

