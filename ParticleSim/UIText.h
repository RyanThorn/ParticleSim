#ifndef _UITEXT_H_
#define _UITEXT_H_
class UIText
{
private:
	// The font object
	TTF_Font* m_font;

	// Last text rendererd with an updating printf
	std::string m_lastText;
	// Current text to render
	std::string m_currentText;

public:
	/**
	 * Constructs a font from a given file location and point size
	 * @param _fontFile char* The font file location.
	 * @param _ptSize int The size of the font in point.
	 */
	UIText(const char* _fontFile, int _ptSize);
	
	/**
	 * Constructs a font from a given TTF_Font* object that has previously been loaded in using the SDL_TTF plugin.
	 * @param _fontObject TTF_Font* The TTF_Font* object loaded in using the SDL_TTF plugin
	 */
	UIText(TTF_Font* _fontObject);
	~UIText();

	/**
	 * Draws text with updating parameters
	 * @param _renderer SDL_Renderer* The SDL renderer to draw
	 * @param _screenPosition glm::vec2 The screen position for this text
	 * @param _colour SDL_Color The colour of the text
	 * @param _staticText char* The text and format of the text to draw (%s for string variables and %i for integer variables)
	 * @param ... Any data type to be rendered in the text with the given format
	 */
	void Printf(SDL_Renderer* _renderer, glm::vec2 _screenPosition, SDL_Color _colour, char* _staticText, ...);

	/**
	* Draws text statically with no updating sections
	* @param _renderer SDL_Renderer* The SDL renderer to draw
	* @param _screenPosition glm::vec2 The screen position for this text
	* @param _colour SDL_Color The colour of the text
	* @param _staticText char* The text to render for this text element
	*/
	void Print(SDL_Renderer* _renderer, glm::vec2 _screenPosition, SDL_Color _colour, char* _staticText);

	// Getter for the font object
	TTF_Font* GetFont() { return m_font; }

	/**
	 * Loads in a given font with a font size using the SDL_TTF plugin.
	 * @param _fontFile char* The file location of the font.
	 * @param _ptSize int The size of the font in point size.
	 * @returns TTF_Font* The loaded font in its TTF_Font object. Returns nullptr if font failed to load.
	 */
	TTF_Font* LoadFont(const char* _fontFile, int _ptSize);
};
#endif // !_UITEXT_H_

