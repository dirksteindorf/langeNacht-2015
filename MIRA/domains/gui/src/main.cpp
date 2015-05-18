#include "lndw_gui.hpp"

int main()
{
    lndw::Gui fenster(sf::VideoMode(1024, 768));//, sf::Style::Fullscreen);

    std::wstring counter = L"123456789 123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 \n123456789 123456789 123456789 ";

    fenster.addArea("oTToCAR", sf::IntRect(-12, 65, 121, 498), counter, "../res/oTToCAR_Logo.png", "../res/ottocar_CeBIT2015_03.jpg");
    fenster.addArea("robOTTO", sf::IntRect(227, 174, 135, 407), counter, "../res/logo-robotto.png", "../res/robotto.png");
    fenster.addArea("Finken Projekt", sf::IntRect(37, 1, 205, 63), counter, "../res/red_logo.png", "../res/finken.jpg");

    while (fenster.isOpen())
    {
        fenster.checkEvent();
		fenster.checkMouse();
		fenster.draw();
    }

    return 0;
}
