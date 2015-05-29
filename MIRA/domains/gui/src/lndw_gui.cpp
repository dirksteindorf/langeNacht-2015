#include "lndw_gui.hpp"

extern void publishPose();

namespace lndw
{
	Gui::Gui(sf::VideoMode mode, sf::VideoMode fullscreen_mode):window(mode, "LNdW 2015 Demo", sf::Style::Close){
		offset_karte = sf::Vector2u(15,2);
		mouseWasPressed=false;
		window_mode = mode;
		this->fullscreen_mode = fullscreen_mode;
		f_elf_pressed = false;
		fullscreen_active = false;		

		addArea("LNdW 2015", sf::IntRect(115, 580, 169, 182), L"Zur Langen Nacht der Wissenschaft \n2015 präsentieren Ihnen die \nArbeitsgruppen ESS, IS und CSE \naktuelle Projekte aus dem Bereich \nder Robotik.", "res/white_square.png", "res/lndw15_start.png", 7.8, 20.4, M_PI * 0.4, "res/LNdW2015.ogg", false, true, false);
		
		createStatics();
		initStateMachine();
		
		showArea(areas.begin());
		setRobotPose(7.8, 20.4, M_PI * 0.4);
	}

	int Gui::setRobotPose(float x, float y, float theta) {
		float max_distance_square = 0.04; //[m^2]

		float delta_base_square = (x - areas.begin()->target.x) * (x - areas.begin()->target.x) + (y - areas.begin()->target.y) * (y - areas.begin()->target.y);
		float delta_target_square = (x - target.x) * (x - target.x) + (y - target.y) * (y - target.y);
		//std::cout << "base^2: " << delta_base_square << " target^2: " << delta_target_square << "\n";

		state.base = ( delta_base_square < max_distance_square ) ? true : false;
		state.target = ( delta_target_square < max_distance_square ) ? true : false;
		
		robot.pos.x = x * 20.0 * scale + offset_karte.x;
		robot.pos.y = y * 20.0 * scale + offset_karte.y;
		robot.pos.theta = theta;
		//std::cout << "robotPose: x=" << x << " y=" << y << " theta=" << theta * 180 /M_PI << "°\n";

		float r = robot.circle.getRadius();
		float c = robot.tail.getSize().y * 0.5;				
		robot.circle.setPosition( robot.pos.x - r, robot.pos.y - r);
		robot.tail.setPosition( robot.pos.x + c * sin(robot.pos.theta), robot.pos.y + c * cos(robot.pos.theta) );
		robot.tail.setRotation((robot.pos.theta + M_PI) * -180 /M_PI);

		return 0;
	}

	int Gui::createStatics(){
		background = sf::RectangleShape( sf::Vector2f(window.getSize().x, window.getSize().y) );
		background.setFillColor(sf::Color(205, 205, 205));
		
		texture_karte.loadFromFile("res/hoersaal_clean.png");
		karte = sf::Sprite(texture_karte);

		texture_blaupause.loadFromFile("res/raumskizze_blaupause.png");
		blaupause = sf::Sprite(texture_blaupause);
		blaupause.setPosition(24, 8);

		scale=((float)window.getSize().y)/480.0;
		karte.setScale(scale, scale);
		std::cout << "\nScale: " << scale << "\n";
		karte.setPosition(offset_karte.x, offset_karte.y);

		bottom_line = sf::RectangleShape( sf::Vector2f(texture_karte.getSize().x * scale, offset_karte.y + 5) );
		bottom_line.setFillColor(sf::Color(205, 205, 205));
		bottom_line.setPosition(offset_karte.x, window.getSize().y - offset_karte.y - 5);

		texture_fin.loadFromFile("res/inf_sign_web.png");
		fin = sf::Sprite(texture_fin);

		float fin_scale=70.0/((float)texture_fin.getSize().y);
		fin.setScale(fin_scale, fin_scale);
		fin.setPosition(window.getSize().x - 10 - texture_fin.getSize().x * fin_scale, window.getSize().y - 2 - texture_fin.getSize().y * fin_scale);

		int left_border = (int)(texture_karte.getSize().x*scale) + offset_karte.x + 10;
		if ( !font.loadFromFile("/usr/share/fonts/truetype/droid/DroidSansMono.ttf") ) {
			font.loadFromFile("/usr/share/fonts/truetype/ttf-dejavu/DejaVuSansMono.ttf");
		}

		header = sf::Text(areas.begin()->name, font, 70);
		header.setPosition(left_border, 10);
		header.setColor(sf::Color(0,0,0));
		
		text = sf::Text(areas.begin()->text, font, 30);
		text.setPosition(left_border, header.getGlobalBounds().top + header.getGlobalBounds().height + 10);
		text.setColor(sf::Color(0,0,0));

		int top_border = text.getGlobalBounds().top + text.getGlobalBounds().height + 10;
		bild_bereich = sf::IntRect(left_border, top_border, window.getSize().x - left_border - 20, fin.getPosition().y - top_border - 25);
		std::cout << "bildbereich: l" << bild_bereich.left << " t" << bild_bereich.top << " w" << bild_bereich.width << " h" << bild_bereich.height << "\n";

		bild = sf::Sprite();
		fitIn(bild_bereich, &(areas.begin()->texture_bild), &bild);

		goButton.area = sf::IntRect(left_border - karte.getGlobalBounds().width/4, fin.getPosition().y, fin.getPosition().x - left_border + karte.getGlobalBounds().width/4 - 30, fin.getGlobalBounds().height - 10);
		goButton.color = sf::RectangleShape( sf::Vector2f(goButton.area.width, goButton.area.height) );
		goButton.color.setPosition(goButton.area.left, goButton.area.top);
		goButton.color.setFillColor(sf::Color(29, 183, 40));
		goButton.text = sf::Text(L"Fahr da hin.", font, 30);
		goButton.text.setColor(sf::Color(0,0,0));
		goButton.color.setOutlineColor(sf::Color(0,0,0));
		goButton.color.setOutlineThickness(-2);
		goButton.text.setPosition(goButton.area.left + (goButton.color.getSize().x - goButton.text.getGlobalBounds().width)/2, goButton.area.top + (goButton.color.getSize().y - goButton.text.getGlobalBounds().height)/2 - 5);
		goButton.show = false;

		robot.circle = sf::CircleShape(10*scale);
		robot.circle.setFillColor(sf::Color(0,0,160));
		robot.tail = sf::RectangleShape( sf::Vector2f(13*scale, 8*scale) );
		robot.tail.setFillColor(sf::Color(0,0,160));
		
		target_arrow.setPointCount(7);
		target_arrow.setPoint(0, sf::Vector2f(0, -3));
		target_arrow.setPoint(1, sf::Vector2f(20, -3));
		target_arrow.setPoint(2, sf::Vector2f(20, -6));
		target_arrow.setPoint(3, sf::Vector2f(40, 0));
		target_arrow.setPoint(4, sf::Vector2f(20, 6));
		target_arrow.setPoint(5, sf::Vector2f(20, 3));
		target_arrow.setPoint(6, sf::Vector2f(0, 3));
		target_arrow.setFillColor(sf::Color(0,0,0));

		return 0;
	}

	int Gui::initStateMachine() {
		state.base = false;
		state.target = false;
		state.moving = true;
		state.saidGoodbye = false;
		state.navigation_stopped = false;
		std::cout << "volume: " << state.speech.getVolume() << "\n";
		state.timer = sf::Clock();
		for (int i=0; i<24; i++) state.personIsMaybePresent[i] = false;
		state.nextField = 0;
		state.personIsPresent = false;
		
		return 0;
	}
	
	int Gui::setTargetPose(pose2d target_pose) {
		target = target_pose;
		target_arrow.setPosition(target.x * 20.0 * scale + offset_karte.x, target.y * 20.0 * scale + offset_karte.y);
		target_arrow.setRotation(target.theta * -180 /M_PI);

		return 0;
	}

	int Gui::addArea(std::string name, sf::IntRect area, std::wstring text, std::string logo_pfad, std::string bild_pfad, float target_x, float target_y, float target_theta, std::string sprach_pfad, bool debugMsg, bool showGoButton, bool showWhiteArea){
		poi newArea;
		
		newArea.area = area;
		newArea.area.left = newArea.area.left + offset_karte.x;
		newArea.area.top = newArea.area.top + offset_karte.y;
		newArea.border = sf::RectangleShape( sf::Vector2f(newArea.area.width, newArea.area.height) );
		newArea.border.setPosition(newArea.area.left, newArea.area.top);
		/*newArea.border.setFillColor(sf::Color(0,0,0,0));
		newArea.border.setOutlineColor(sf::Color(0,0,0));
		newArea.border.setOutlineThickness(-1);*/
		if (showWhiteArea) {
			newArea.border.setFillColor(sf::Color(255,255,255));
		} else {
			newArea.border.setFillColor(sf::Color(0,0,0,0));
		}
		
		newArea.showGoButton = showGoButton;
		newArea.target.x = target_x;
		newArea.target.y = target_y;
		newArea.target.theta = target_theta;
		
		newArea.name = name;
		newArea.text = text;
		newArea.texture_bild.loadFromFile(bild_pfad);

		newArea.texture_logo.loadFromFile(logo_pfad);
		newArea.logo = sf::Sprite();

		newArea.speech = sprach_pfad;

		areas.push_back(newArea);
		
		if (debugMsg) std::cout << "Logo " << name << ":\n";
		fitInLogo(area, &areas.back().texture_logo, &areas.back().logo, debugMsg);

		for(std::vector<poi>::iterator it=areas.begin(); it != areas.end()--; it++) {
			fitInLogo(it->area, &(it->texture_logo), &(it->logo));
		}

		showArea(areas.begin());
		
		return 0;
	}

	bool Gui::isOpen(){
		return window.isOpen();
	}

	void Gui::setCurrentTargetReached() {
		state.navigation_stopped = true;
	}
	
	void Gui::setPersonPresent(bool isPresent) {
	    state.personIsMaybePresent[state.nextField] = isPresent;
	    state.nextField = (state.nextField < 24) ? state.nextField + 1 : 0;

	    int count=0;
	    for (int i=0; i<25; i++) if (state.personIsMaybePresent[i]) count++;

	    state.personIsPresent = (count > 20);

	    //std::cout << "lastMaybe: " << isPresent << " person is: " << state.personIsPresent << "\n";
	}

	int Gui::update(bool drawTargetArrowAndBorder, bool robotFollowsMouse, bool debugMsg) {
		checkEvent();
		checkMouse(robotFollowsMouse, debugMsg);
		draw(drawTargetArrowAndBorder);

		if ( state.base && state.navigation_stopped ) {
			sayHello(debugMsg);
		} else if ( state.target && state.navigation_stopped ) {
			sayGoodbye(debugMsg);
		} else {
			state.timer.restart();
			state.moving = true;
			state.saidGoodbye = false;
			if (debugMsg) std::cout << "Moving\n";
		}

		return 0;
	}

	int Gui::sayHello(bool debugMsg) {
		if ( state.personIsPresent && (state.timer.getElapsedTime().asSeconds() > 13.0 || state.moving)) {
			std::cout << "Hello Again\n";
			state.speech.openFromFile(areas.begin()->speech);
			state.speech.play();
			state.timer.restart();
			state.moving = false;
		}
		return 0;
	}

	int Gui::sayGoodbye(bool debugMsg) {
		if ( state.moving ) {
			std::cout << "New At Target\n";
			state.speech.openFromFile(state.next_speech);
			state.timer.restart();
			state.moving = false;
		} else if ( state.timer.getElapsedTime().asSeconds() > 0.5 && !state.saidGoodbye) {
			state.speech.play();
			state.saidGoodbye = true;
		} else if ( state.timer.getElapsedTime().asSeconds() > state.speech.getDuration().asSeconds() + 2.0 ) {
			showArea(areas.begin(), debugMsg);
			publishTarget();
		}
		return 0;
	}

	int Gui::publishTarget(bool debugMsg) {
		if (debugMsg) std::cout << "Ich Fahr dann mal los.\n";
		publishPose();
		state.navigation_stopped = false;
 
		return 0;
	}

	int Gui::checkEvent(){
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				window.close();
				exit(-1);
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && (window.hasFocus() || fullscreen_active) ) {
			window.close();
			exit(-1);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::C) && (window.hasFocus() || fullscreen_active) ) {
			window.close();
			exit(-1);
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && (window.hasFocus() || fullscreen_active) ) {
			f_elf_pressed = true;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && goButton.show && (window.hasFocus() || fullscreen_active)) {
			publishTarget(true);
		}

		if ( !sf::Keyboard::isKeyPressed(sf::Keyboard::F11) && f_elf_pressed) {
			f_elf_pressed = false;
			if (fullscreen_active) {
				window.create(window_mode, "LNdW 2015 Demo", sf::Style::Close );
			} else {//window_mode
				window.create(fullscreen_mode, "LNdW 2015 Demo", sf::Style::Fullscreen );
			}
			fullscreen_active = !fullscreen_active;
		}
			
		return 0;
	}

	int Gui::checkMouse(bool robotFollowsMouse, bool debugMsg){
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && (window.hasFocus() || fullscreen_active) )
		{
		    mouseWasPressed = true;
		}
		if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && mouseWasPressed && (window.hasFocus() || fullscreen_active) )
		{//mousebutton released
		    mouseWasPressed = false;
		    sf::Vector2i position = sf::Mouse::getPosition(window);

		    if (debugMsg) std::cout << "Pos: x=" << position.x << " y=" << position.y << "\n";

			for(std::vector<poi>::iterator it=areas.begin(); it != areas.end(); it++) {
				if (it->area.contains(position)) showArea(it, debugMsg);
			}

			if (goButton.area.contains(position) && goButton.show) publishTarget();
		}
		
		sf::Vector2i position = sf::Mouse::getPosition(window);
		std::ostringstream out;
		out << "LNdW 2015 Demo [x=" << position.x << " (" << (position.x - offset_karte.x) * 0.05 / scale << "m); y=" << position.y << " (" << (position.y - offset_karte.y) * 0.05 / scale << "m)]";
		window.setTitle(out.str());

		if (robotFollowsMouse) {
			float new_x = (position.x - offset_karte.x) * 0.05 /scale;
			float new_y = (position.y - offset_karte.y) * 0.05 /scale;
			float old_x = (robot.pos.x - offset_karte.x) * 0.05 /scale;
			float old_y = (robot.pos.y - offset_karte.y) * 0.05 /scale;
			setRobotPose( new_x, new_y, (new_x == old_x && new_y == old_y) ? robot.pos.theta : atan2(old_y - new_y, new_x - old_x));//-M_PI/2);//
			if (new_x == old_x && new_y == old_y) setCurrentTargetReached();
		}
		
		return 0;
	}

	int Gui::showArea(std::vector<lndw::Gui::poi>::iterator area, bool debugMsg) {
		if (debugMsg) std::cout << area->name << "\n";
		header.setString(area->name);
		text.setString(area->text);

		sf::IntRect temp_bereich = bild_bereich;
		temp_bereich.top = text.getGlobalBounds().top + text.getGlobalBounds().height + 10;
		temp_bereich.height = fin.getPosition().y - temp_bereich.top - 10;

		fitIn(bild_bereich, &(area->texture_bild), &bild, debugMsg);
		goButton.show = area->showGoButton;
		setTargetPose(area->target);
		
		state.next_speech = area->speech;

		return 0;
	}

	int Gui::fitIn(sf::IntRect borders, sf::Texture *input, sf::Sprite *frame, bool debugMsg){
		sf::Vector2u size = input->getSize();
		float scale_x = (float)borders.width / (float)size.x;
		float scale_y = (float)borders.height / (float)size.y;

		if (debugMsg) std::cout << "scale_x: " << scale_x << " scale_y: " << scale_y << "\n";

		sf::Vector2f position;
		float scale;
		
		if (scale_x < scale_y) {
			position.x = borders.left;
			position.y = borders.top + (borders.height - (float)size.y * scale_x) / 2.0;
			scale =  scale_x;
		} else {
			position.y = borders.top;
			position.x = borders.left + (borders.width - (int)((float)size.x * scale_y)) / 2;
			scale = scale_y;
		}

		if (debugMsg) std::cout << "scale: " << scale << " pos_x: " << position.x << " pos_y: " << position.y << "\n";

		frame->setPosition(position);
		frame->setScale(scale, scale);
		frame->setTexture(*input, true);

		return 0;
	}

	int Gui::fitInLogo(sf::IntRect borders, sf::Texture *input, sf::Sprite *frame, bool debugMsg){
		float factor = 0.75;

		borders = sf::IntRect(borders.left + borders.width * (1-factor)/2, borders.top + borders.height * (1-factor)/2, borders.width * factor, borders.height * factor);
		if (debugMsg) std::cout << "logo-bereich: l" << borders.left << " t" << borders.top << " w" << borders.width << " h" << borders.height << "\n";

		fitIn(borders, input, frame, debugMsg);

		return 0;
	}

	int Gui::draw(bool showTarget){
		window.clear();
		window.draw(background);
		//window.draw(karte);
		window.draw(blaupause);
		window.draw(bottom_line);
		for(std::vector<poi>::iterator it=areas.begin(); it != areas.end(); it++) {
			
			if (showTarget) window.draw(it->border);
			window.draw(it->logo);
		}

		window.draw(robot.circle);
		window.draw(robot.tail);

		if (showTarget) window.draw(target_arrow);
		
		if (goButton.show && state.base) {
			window.draw(goButton.color);
			window.draw(goButton.text);
		}
		window.draw(fin);
		window.draw(bild);
		window.draw(header);
		window.draw(text);
		window.display();

		return 0;
	}

};
