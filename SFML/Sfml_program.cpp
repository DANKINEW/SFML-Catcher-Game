#include <iostream>
#include <random>
#include <chrono>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

sf::Color HSVToRGB(float h, float s, float v) {
	float hPrime = h / 60.0f;
	unsigned int hIndex = unsigned int(hPrime) % 6;
	float chroma = s * v;
	float min = (v - chroma);
	float x = chroma * (1.0f - abs(fmod(hPrime, 2.0f) - 1.0f));
	float outRGB[6][3] = {

		{chroma, x, 0.0f},
		{x, chroma, 0.0f},
		{0.0f, chroma, x},
		{0.0f, x, chroma},
		{x, 0.0f, chroma},
		{chroma, 0.0f, x}

	};
	float rF = (outRGB[hIndex][0] + min);
	float gF = (outRGB[hIndex][1] + min);
	float bF = (outRGB[hIndex][2] + min);
	rF *= 255;
	gF *= 255;
	bF *= 255;
	std::uint8_t rI = std::uint8_t(rF);
	std::uint8_t gI = std::uint8_t(gF);
	std::uint8_t bI = std::uint8_t(bF);
	return sf::Color(rI, gI, bI);
}
void PollEvents(sf::RenderWindow& window) {
	while (const std::optional event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
		else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
				window.close();
			}
		}
	}
}

int main() {
	unsigned int width = 640;
	unsigned int height = 320;
	sf::Vector2u windowSize = { width, height };
	sf::VideoMode videomode = sf::VideoMode(windowSize);
	std::string title = "SFMLL";
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ width, height }), title);
	window.setFramerateLimit(60);


	sf::Clock clock;

	std::default_random_engine randomEngine;
	randomEngine.seed(std::chrono::steady_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<unsigned int> randomColor;
	std::uniform_real_distribution<float> randomSize;

	randomColor = std::uniform_int_distribution<unsigned int>(0, 255);
	randomSize = std::uniform_real_distribution<float>(0, 15);
	sf::Vector2f obssize = { 5.0f, 5.0f };
	std::uniform_real_distribution<float> randomX(0.0f, float(width) - obssize.x);
	std::uniform_real_distribution<float> randomY(-200.0f, 0.0f);



	const unsigned int obsAmount = 100;

	float rectSpeed = 150.0f;
	sf::RectangleShape rectangle;
	sf::Vector2f rectSize = { 100.0f, 20.0f };
	rectangle.setSize(rectSize);
	rectangle.setFillColor(sf::Color(108, 43, 43));
	rectangle.setOutlineThickness(3.0f);
	rectangle.setOutlineColor(sf::Color(206, 147, 216));
	rectangle.setPosition({ width / 2.0f, height / 1.2f });
	

	sf::RectangleShape obstacle[obsAmount];
	float obspeed = 100.0f;


	for (int i = 0; i < obsAmount; i++) {

		sf::Vector2f obssize = { 5.0f, 5.0f };
		std::uniform_real_distribution<float> randomX(0.0f, float(width) - obssize.x);
		std::uniform_real_distribution<float> randomY(-2000.0f, 0.0f);
		obstacle[i].setSize(obssize);
		obstacle[i].setFillColor(sf::Color(randomColor(randomEngine), randomColor(randomEngine), randomColor(randomEngine)));

		obstacle[i].setPosition({ randomX(randomEngine), randomY(randomEngine) });
	}
	unsigned int score = 0;
	while (window.isOpen()) {


		//Updating

		PollEvents(window);

		float dt = clock.restart().asSeconds();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::A)) {

			rectangle.move({ -rectSpeed * dt, 0.0f });

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::D)) {

			rectangle.move({ rectSpeed * dt, 0.0f });

		}
		if (rectangle.getPosition().x < 0.0f) {

			rectangle.setPosition({ 2.0f, rectangle.getPosition().y });

		}
		if (rectangle.getPosition().x + rectSize.x > float(width)) {

			rectangle.setPosition({ float(width) + - rectSize.x - 2.0f, rectangle.getPosition().y });

		}
		for (int i = 0; i < obsAmount; i++) {

			obstacle[i].move({ 0.0f, obspeed * dt });
			if (obstacle[i].getPosition().y > float(height)) {

				continue;
			}
			if (rectangle.getGlobalBounds().findIntersection(obstacle[i].getGlobalBounds())) {

				obstacle[i].setPosition({ randomX(randomEngine), randomY(randomEngine) });
				score++;
				std::cerr << score << '\n';

			}

		}


		//Render

		window.clear(sf::Color::Black);

		// Draw
		window.draw(rectangle);
		for (int i = 0; i < obsAmount; i++) {

			window.draw(obstacle[i]);

		}
		// Display screen

		window.display();
	}
	return 0;
}