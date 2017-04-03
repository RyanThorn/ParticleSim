#include "Stdafx.h"
#include "UIText.h"

/**
 * Constructs a font from a given file location and point size
 * @param _fontFile char* The font file location.
 * @param _ptSize int The size of the font in point.
 */
UIText::UIText(const char* _fontFile, int _ptSize)
{
	// Use our LoadFont function
	LoadFont(_fontFile, _ptSize);
}

/**
 * Constructs a font from a given TTF_Font* object that has previously been loaded in using the SDL_TTF plugin.
 * @param _fontObject TTF_Font* The TTF_Font* object loaded in using the SDL_TTF plugin
 */
UIText::UIText(TTF_Font* _fontObject)
{
	// Set the new font object to use for this UIText
	m_font = _fontObject;
}

UIText::~UIText()
{
	// Free the font
	TTF_CloseFont(m_font);
	// Dereference the font object
	m_font = nullptr;
}

/**
 * Loads in a given font with a font size using the SDL_TTF plugin.
 * @param _fontFile char* The file location of the font.
 * @param _ptSize int The size of the font in point size.
 * @returns TTF_Font* The loaded font in its TTF_Font object. Returns nullptr if font failed to load.
 */
TTF_Font* UIText::LoadFont(const char* _fontFile, int _ptSize)
{
	// Load in a font
	m_font = TTF_OpenFont(_fontFile, _ptSize);
	
	// Check it loaded successfully
	if (!m_font)
	{
		// Failed to load the font
		std::cerr << "Failed to load the given font. " << TTF_GetError() << "\n";
		return nullptr;
	}

	// Font loaded successfully
	return m_font;
}

void UIText::Printf(SDL_Renderer* _renderer, glm::vec2 _screenPosition, SDL_Color _colour, char* _staticText, ...)
{
	// Storage to traverse throught the static text to find format codes
	char* m_nextChar;
	// Int argument storage
	unsigned int m_argInt;
	// Float arg storage
	float m_argFloat;
	// String argument storage
	char* m_argString;
	// Final string to render
	std::stringstream m_finalString;
	// Final string (in string format)
	std::string m_outputString;

	// Init our argument list
	va_list m_argList;
	va_start(m_argList, _staticText);

	// Loop through our _staticText for the format codes
	for (m_nextChar = _staticText; *m_nextChar != '\0'; ++m_nextChar)
	{
		// If we dont see a %, just carry the text over as normal
		while (*m_nextChar != '%')
		{
			// Base case to check if we hit the null-termination of the char*
			if (*m_nextChar == '\0')
			{
				// break the loop if we did
				break;
			}
			// add the char to the final string
			m_finalString << *m_nextChar;
			// move to the next char
			m_nextChar++;
		}
		// Move to the next char
		m_nextChar++;

		// Switch through all the format codes
		switch (*m_nextChar)
		{
			case 'i': // int
			{
				// Fetch the int
				m_argInt = va_arg(m_argList, int);
				// Add the int to the final string
				m_finalString << m_argInt;
				break;
			}
			case 'd': // float
			{
				// Fetch the int
				m_argFloat = va_arg(m_argList, float);
				// Add the int to the final string
				m_finalString << m_argFloat;
				break;
			}
			case 's': // String
			{
				// Fetch the string
				m_argString = va_arg(m_argList, char*);
				// Add string to the final string
				m_finalString << m_argString;
				break;
			}
		}
	}
	// Close the variable argument list
	va_end(m_argList);

	// Set the final output
	m_outputString = m_finalString.str();

	// The text has changed so we HAVE to update the surface and texture
	SDL_Surface* m_textSurface = TTF_RenderText_Solid(m_font, m_outputString.c_str(), _colour);

	// Check we made the surface
	if (m_textSurface == nullptr)
	{
		// Failed
		std::cerr << "Failed to create the text surface from the TTF render (UIText::Printf)\n";
		return;
	}

	// Now create our texture which we will blit to the buffer
	SDL_Texture* m_textTexture = SDL_CreateTextureFromSurface(_renderer, m_textSurface);
	
	// Check we made the texture
	if (m_textTexture == nullptr)
	{
		// Failed
		std::cerr << "Failed to create the text texture from the TTF render (UIText::Printf)\n";
		return;
	}

	// Set the render rectangle
	SDL_Rect m_renderRectangle = { (int)_screenPosition.x, (int)_screenPosition.y, m_textSurface->w, m_textSurface->h };

	// Now we have the texture, we no longer need the surface
	SDL_FreeSurface(m_textSurface);
	m_textSurface = nullptr;

	// Render the texture
	SDL_RenderCopy(_renderer, m_textTexture, nullptr, &m_renderRectangle);

	// Clear the texture or bad memory leaks will happen :(
	SDL_DestroyTexture(m_textTexture);
	m_textTexture = nullptr;
}

void UIText::Print(SDL_Renderer* _renderer, glm::vec2 _screenPosition, SDL_Color _colour, char* _staticText)
{
	// Make a new texture for this text
	SDL_Surface* m_textSurface = TTF_RenderText_Solid(m_font, _staticText, _colour);

	// Check we made the surface
	if (m_textSurface == nullptr)
	{
		// Failed
		std::cerr << "Failed to create the text surface from the TTF render (UIText::Printf)\n";
		return;
	}

	// Make the texture
	SDL_Texture* m_textTexture = SDL_CreateTextureFromSurface(_renderer, m_textSurface);

	// Check we made the texture
	if (m_textTexture == nullptr)
	{
		// Failed
		std::cerr << "Failed to create the text texture from the TTF render (UIText::Printf)\n";
		return;
	}

	// Get dimensions
	SDL_Rect m_renderRectangle = { (int)_screenPosition.x, (int)_screenPosition.y, m_textSurface->w, m_textSurface->h };
		
	// Now we have the texture, we no longer need the surface
	SDL_FreeSurface(m_textSurface);
	m_textSurface = nullptr;

	// Render the texture
	SDL_RenderCopy(_renderer, m_textTexture, nullptr, &m_renderRectangle);

	// Clear the texture or bad memory leaks will happen :(
	SDL_DestroyTexture(m_textTexture);
	m_textTexture = nullptr;
}