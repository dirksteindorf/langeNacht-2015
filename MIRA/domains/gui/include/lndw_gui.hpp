#ifndef LNDW_GUI_H_
#define LNDW_GUI_H_

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>

namespace lndw
{
	class Gui
	{
		sf::RenderWindow window;
		sf::RectangleShape background;
		sf::Texture texture_karte;
		sf::Sprite karte;
		sf::Texture texture_fin;
		sf::Sprite fin;
		sf::IntRect bild_bereich;
		sf::Sprite bild;
		float scale;
		sf::Vector2u offset_karte;
		sf::Font font;
		sf::Text header;
		sf::Text text;
		bool mouseWasPressed;

		struct button {
			sf::IntRect area;
			sf::Text text;
			sf::RectangleShape color;
			bool show;
		};

		button goButton;

		struct poi {
			sf::IntRect area;
			std::string name;
			std::wstring text;
			sf::Texture texture_logo;
			sf::Sprite logo;
			sf::Texture texture_bild;
			bool showGoButton;
		};

		std::vector<poi> areas;

		int createStatics();
		int fitIn(sf::IntRect borders, sf::Texture *input, sf::Sprite *frame, bool debug = false);
		int fitInLogo(sf::IntRect borders, sf::Texture *input, sf::Sprite *frame, bool debug = false);

	public:
		Gui(sf::VideoMode mode, unsigned int style = sf::Style::Close);
		virtual ~Gui(){}
		
		int addArea(std::string name, sf::IntRect area, std::wstring text, std::string logo_pfad, std::string bild_pfad, bool showGoButton = true);
		bool isOpen();

		int checkEvent();
		int checkMouse();
		int draw();
	};
};
#endif //LNDW_GUI_H_
