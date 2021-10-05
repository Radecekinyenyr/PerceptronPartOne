#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <Windows.h>
#include <ctime>
#include "Collision/Collision.h"
#include <string>
#include <cstring>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;



float map(float value, float start1, float stop1, float start2, float stop2)
{
	float outgoing = start2 + (stop2 - start2) * ((value - start1) / (stop1 - start1));
	return outgoing;
}

int sign(float sum)
{
	if (sum >= 0)
		return 1;
	else {
		return -1;
	}
}

class Randomer {
	// random seed by default
	std::mt19937 gen_;
	std::uniform_real_distribution<float> dist_;

public:
	/*  ... some convenient ctors ... */

	Randomer(float min, float max, float seed = std::random_device{}())
		: gen_{ seed }, dist_{ min, max } {
	}

	// if you want predictable numbers
	void SetSeed(float seed) {
		gen_.seed(seed);
	}

	float operator()() {
		return dist_(gen_);
	}
};

float f(float x)
{
	return 0.3 * x + 0.2;
}

class Perceptron
{
private:

	float* weights;
	float learningRate = 0.1;
	Randomer randomer1{ -1,1 };

public:
	Perceptron(int n)
	{
		weights = new float[n];

		for (int i = 0; i <= sizeof(*weights) / sizeof(weights[0]) + 1; ++i)
		{
			weights[i] = randomer1();  // first minimum and after maximum
			std::cout << "random hodnota perceptronu je: " << weights[i] << std::endl;
		}
	}

	int guess(float inputs[])
	{
		float sum = 0;
		for (int i = 0; i <= sizeof(*weights) / sizeof(weights[0]) + 1; ++i)
		{
			sum += inputs[i] * weights[i];

		}
		int output = sign(sum);
		return output;
	}

	float guessY(float x)
	{
		return -(weights[2] / weights[1]) - (weights[0] / weights[1]) * x;
	}

	void train(float inputs[], int target)
	{
		int guess = this->guess(inputs);
		int error = target - guess;

		for (int i = 0; i <= sizeof(*weights) / sizeof(weights[0]) + 1; ++i)
		{
			weights[i] += error * inputs[i] * learningRate;

		}
	}


};

class Point 
{
private:


	sf::Vector2f points;
	int label = 0;
	float px1 = 0;
    float py1 = 0;
	float lineY = 0;
	float bias = 1;
public:
	sf::CircleShape circle;
	
	Point(float x_, float y_)
	{
		points.x = x_;
		points.y = y_;

	}
	Point()
	{
		
		
		Randomer randomer2{ -1,1 };

		points.x = randomer2();
		points.y = randomer2();
		px1 = pixelX();
		py1 = pixelY();
		lineY = f(points.x);
		std::cout << "souradnice:  " << px1 << " a " << py1 << std::endl;

		

		if (points.y > lineY)
		{
			label = 1;
		}
		else {
			label = -1;
		}
		

	}

	float pixelX()
	{
		return map(points.x, -1, 1, SCREEN_WIDTH, 0);
	}

	float pixelY()
	{
		return map(points.y, -1, 1, 0, SCREEN_HEIGHT);
	}


	void show()
	{
		if (label == 1)
		{
			circle.setRadius(2);
			circle.setPosition(px1, py1);
			circle.setOutlineColor(sf::Color(255, 0, 0));
			circle.setOutlineThickness(3);
			// set circle if position x bigger than position y

		}
		else {

			circle.setRadius(5);
			circle.setPosition(px1, py1);
			circle.setFillColor(sf::Color(0, 255, 0));
		
		}
		
		
		circle.setPosition(px1, py1);
		std::cout << "nove souradnice jsou : " << px1 << " a " << py1 << std::endl;
	}

	float Xposition()
	{
		return points.x;
	}

	float Bias()
	{
		return bias;
	}

	float Yposition()
	{
		return points.y;
	}

	int labelShow()
	{
		return label;
	}
};

int main()
{
	Point * point = new Point[100];

    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Perceptron");
	
	Perceptron* perceptron = new Perceptron(3);
	//float inputs[] = { -1, 1 };
	//int guess = 0;
	//guess = perceptron->guess(inputs);
	//std::cout << guess;
	
	Point* p1 = new Point(-1,f(-1));
	Point* p2 = new Point(1, f(1));

	sf::Vertex line2[2];
	line2[0] = sf::Vertex(sf::Vector2f(p1->pixelX(),p1->pixelY()));
	line2[1] = sf::Vertex(sf::Vector2f(p2->pixelX(), p2->pixelY()));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
		for (int i = 0; i < 100; ++i)
			window.draw(point[i].circle);

		Point* p3 = new Point(-1, perceptron->guessY(-1));
		Point* p4 = new Point(1, perceptron->guessY(1));

		sf::Vertex line1[2];
		line1[0] = sf::Vertex(sf::Vector2f(p3->pixelX(), p3->pixelY()));
		line1[1] = sf::Vertex(sf::Vector2f(p4->pixelX(), p4->pixelY()));

		for (int i = 0; i < 100; ++i)
		{
			point[i].show();
			float inputs[] = { point[i].Xposition(), point[i].Yposition(), point[i].Bias()};
			int target = point[i].labelShow();
			perceptron->train(inputs, target);

			
			int guess = perceptron->guess(inputs);
			std::cout << "odhad perceptronu je: " << guess << " A " << target << std::endl;
			if (guess == target)
			{
				point[i].circle.setFillColor(sf::Color(0, 0, 255));

			}
			else {
				
				point[i].circle.setFillColor(sf::Color(0, 255, 0));

				
			}
			window.draw(point[i].circle);
			window.draw(line2, 2, sf::Lines);
			window.draw(line1, 2, sf::Lines);
		}
        window.display();
    }
	//delete[] perceptron;
    return 0;
}
