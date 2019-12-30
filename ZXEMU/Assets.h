#pragma once
#include <SFML/Graphics.hpp>

class Assets
{
public:
	sf::Font font;
public:
	static Assets& Instance()
	{
		static Assets s;
		return s;
	}
	void Load();
private:
	Assets() {};
	~Assets() {};
	Assets(Assets const&) = delete;
	Assets& operator= (Assets const&) = delete;
};
