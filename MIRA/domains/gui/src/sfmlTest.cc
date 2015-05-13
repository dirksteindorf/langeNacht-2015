#include <iostream>
#include <fw/Framework.h>
#include <fw/ChannelReadWrite.h>
#include <SFML/Graphics.hpp>

using namespace mira;
using namespace std;


void checkInput(const Timer& timer)
{
	
}

int main(int argc, char** argv)
{
	Framework framework(argc, argv, true);
	Authority authority;
	authority.checkin("/", "sfmlAuthority");
	authority.start();


	mira::Channel<std::string> sfmlChannel;

	std::string hey = "Hey, it is me - SFML!";
	std::string bye = "See you later. :)";

	sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
	sf::CircleShape shape(100.f);
	shape.setFillColor(sf::Color::Green);

    
	sfmlChannel = authority.publish<std::string>("sfmlChannel");
    //mira::ChannelWrite<std::string> writeStuff = sfmlChannel.write();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
            	window.close();
            	exit(-1);
            }


            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
            {
            	sfmlChannel.post(hey);
            }

            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            {
            	sfmlChannel.post(bye);
            	window.close();
            	exit(-1);
            }
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
    return 0;
}
