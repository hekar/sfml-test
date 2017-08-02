////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Effect.hpp"
#include <vector>
#include <cmath>

const sf::Font* Effect::s_font = NULL;

class Pixelate: public Effect {
public:

	Pixelate() :
			Effect("pixelate") {
	}

	bool onLoad() {
		// Load the texture and initialize the sprite
		if (!m_texture.loadFromFile("resources/background.png"))
			return false;
		m_sprite.setTexture(m_texture);

		// Load the shader
		if (!m_shader.loadFromFile("resources/pixelate.frag",
				sf::Shader::Fragment))
			return false;
		m_shader.setUniform("texture", sf::Shader::CurrentTexture);

		return true;
	}

	void onUpdate(float, float x, float y) {
		m_shader.setUniform("pixel_threshold", (x + y) / 30);
	}

	void onDraw(sf::RenderTarget& target, sf::RenderStates states) const {
		states.shader = &m_shader;
		target.draw(m_sprite, states);
	}

private:

	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Shader m_shader;
};

int main() {
	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "Player",
			sf::Style::Titlebar | sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	sf::Font font;
	if (!font.loadFromFile("resources/Lato-Medium.ttf"))
		return EXIT_FAILURE;
	Effect::setFont(font);

	std::vector<Effect*> effects;
	effects.push_back(new Pixelate);
	std::size_t current = 0;

	for (std::size_t i = 0; i < effects.size(); ++i)
		effects[i]->load();

	sf::Texture textBackgroundTexture;
	if (!textBackgroundTexture.loadFromFile("resources/background.png"))
		return EXIT_FAILURE;
	sf::Sprite textBackground(textBackgroundTexture);
	textBackground.setPosition(0, 512);
	textBackground.setColor(sf::Color(255, 255, 255, 200));

	sf::Texture playerTexture;
	if (!playerTexture.loadFromFile("resources/player.png"))
		return EXIT_FAILURE;
	sf::Sprite playerSprite(playerTexture);
	playerSprite.setPosition(0, 512);
	playerSprite.setColor(sf::Color(255, 255, 255, 200));

	sf::Clock clock;
	while (window.isOpen()) {
		// Process events
		sf::Event event;
		while (window.pollEvent(event)) {
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Escape:
					window.close();
					break;
				case sf::Keyboard::Left:
					break;
				case sf::Keyboard::Right:
					break;
				default:
					break;
				}
			}
		}

		// Update the current example
		float x = static_cast<float>(sf::Mouse::getPosition(window).x)
				/ window.getSize().x;
		float y = static_cast<float>(sf::Mouse::getPosition(window).y)
				/ window.getSize().y;
		effects[current]->update(clock.getElapsedTime().asSeconds(), x, y);

		window.clear(sf::Color(255, 128, 0));
		window.draw(*effects[current]);
		window.draw(textBackground);
		window.draw(playerSprite);
		window.display();
	}

	// delete the effects
	for (std::size_t i = 0; i < effects.size(); ++i)
		delete effects[i];

	return EXIT_SUCCESS;
}
