#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include<math.h>
typedef std::pair<sf::RectangleShape, bool> square;
typedef std::vector<square> Grid;

class Board
{
private:
	Grid chess_board;
public:
	Board() {
		sf::Color c = sf::Color::Green;
		int k = 0;
		// 8 by 8 chessboard
		chess_board.resize(8 * 8);
		for (int i = 0; i != 8; i++) {
			for (int j = 0; j != 8; j++) {
				chess_board[k].first.setFillColor(c);
				chess_board[k].first.setSize(sf::Vector2f(100.f, 100.f));
				chess_board[k].first.setPosition(j * 100.f, i * 100.f);
				if (i == 0 || i == 1 || i == 6 || i == 7) {
					chess_board[k].second = true;
				}
				else {
					chess_board[k].second = false;
				}
				k++;
				
				if (k % 8!=0) {
					if (c == sf::Color::Green) {
						c = sf::Color::Red;
					}
					else {
						c = sf::Color::Green;
					}
				}
			}
		}

	}
	const sf::Vector2f getPos(int index) const { return chess_board[index].first.getPosition(); }
	Grid& Image() { return chess_board; }
	void reset_colour() {
		sf::Color c = sf::Color::Green;
		int k = 0;
		for (int i = 0; i != 8; i++) {
			for (int j = 0; j != 8; j++) {
				chess_board[k].first.setFillColor(c);
				k++;
				if (k % 8 != 0) {
					if (c == sf::Color::Green) {
						c = sf::Color::Red;
					}
					else {
						c = sf::Color::Green;
					}
				}
			}
		}

	}

};

