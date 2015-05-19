#ifndef LNDW_GUI_H_
#define LNDW_GUI_H_

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
#include <math.h>

namespace lndw
{
	class Gui
	{
		sf::RenderWindow window;
		sf::VideoMode window_mode;
		sf::VideoMode fullscreen_mode;
		bool fullscreen_active;
		sf::RectangleShape background;
		sf::RectangleShape bottom_line;
		sf::Texture texture_karte;
		sf::Sprite karte;
		sf::Texture texture_fin;
		sf::Sprite fin;
		sf::IntRect bild_bereich;
		sf::Sprite bild;
		sf::Vector2u offset_karte;
		sf::Font font;
		sf::Text header;
		sf::Text text;
		sf::ConvexShape target_arrow;
		bool mouseWasPressed;
		bool f_elf_pressed;

		struct button {
			sf::IntRect area;
			sf::Text text;
			sf::RectangleShape color;
			bool show;
		};

		button goButton;

		struct pose2d {
			float x;
			float y;
			float theta;
		};

		struct robot_struct {
			pose2d pos;
			sf::CircleShape circle;
			sf::RectangleShape tail;
		};

		robot_struct robot;

		struct poi {
			sf::IntRect area;
			sf::RectangleShape border;
			std::string name;
			std::wstring text;
			sf::Texture texture_logo;
			sf::Sprite logo;
			sf::Texture texture_bild;
			bool showGoButton;
			pose2d target;
		};

		std::vector<poi> areas;

		int createStatics();
		int setTargetPose(pose2d target_pose);
		int fitIn(sf::IntRect borders, sf::Texture *input, sf::Sprite *frame, bool debug = false);
		int fitInLogo(sf::IntRect borders, sf::Texture *input, sf::Sprite *frame, bool debug = false);

		int checkEvent();
		int checkMouse(bool debug = false);
		int draw(bool debug);

	public:
		pose2d target;
		float scale;
	
		Gui(sf::VideoMode mode, sf::VideoMode fullscreen_mode = sf::VideoMode(1366, 768) );
		virtual ~Gui(){}
		
		int addArea(std::string name, sf::IntRect area, std::wstring text, std::string logo_pfad, std::string bild_pfad, float target_x, float target_y, float target_theta, bool debug = false, bool showGoButton = true);
		int setRobotPose(float x, float y, float theta);
		bool isOpen();
		int update(bool debug = false);

		
	};
};
#endif //LNDW_GUI_H_
