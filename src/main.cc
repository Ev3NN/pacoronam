#include <SFML/Graphics.hpp>

#include <iostream>


int main()
{
	sf::RenderWindow renderWindow(sf::VideoMode(640, 480), "Awesome Window !");

	sf::Event event;

	sf::Font font;
	if(!font.loadFromFile("fonts/press-start-2p-v8-latin-regular.ttf")) {
		std::cout << "Error.\n";
		return 1;
	}

	sf::Text text;
	text.setFont(font);
	text.setString("You lose!");
	text.setCharacterSize(48);
	text.setFillColor(sf::Color::Red);
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);
	
	while(renderWindow.isOpen()) {
		while(renderWindow.pollEvent(event)) {
			switch(event.type) {
				case sf::Event::Closed:
					renderWindow.close();
					break;

				default:
					break;
			}
		}

		renderWindow.clear();
		renderWindow.draw(text);
		renderWindow.display();
	}


	return 0;
}