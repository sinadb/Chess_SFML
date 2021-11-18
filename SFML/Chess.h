#pragma once
#include <SFML/Graphics.hpp>
#include<vector>
#include "Board.h"
#include <iostream>
#include <algorithm>
static sf::Vector2f S(100.f, 100.f);


class Chess
{
protected:
	// rect for every piece on the board
	sf::RectangleShape m_piece;
	sf::Texture tex;
	std::string tex_path;
	std::vector<int> valid_moves;
	std::vector<std::vector<int>> Paths;
	bool first_move;
public:
	Chess() = default;
	Chess(const std::string& text_name, const sf::Vector2f pos){
		tex_path = text_name;
		tex.loadFromFile(text_name);
		tex.setSmooth(true);
		m_piece.setSize(S);
		m_piece.setTexture(&tex);
		
		m_piece.setPosition(pos);
		first_move = true;
		
		
	}
	Chess(const Chess& p) {
		m_piece = p.m_piece;
		tex.loadFromFile(p.tex_path);
		tex.setSmooth(true);
		m_piece.setSize(S);
		m_piece.setTexture(&tex);
		m_piece.setPosition(p.getPos());
	}
	bool Move(const sf::Vector2i& mousePos, Board& b, std::vector<Chess*>& drawables) {
		bool has_moved = false;
		int x = mousePos.x;
		int y = mousePos.y;
		int index = x / 100 + y / 100 * 8;
		;
		for (auto& i : valid_moves) {
			if (index == i) {
				sf::Vector2f reset_occupancy = this->getPos();
				int curr_x = reset_occupancy.x;
				int curr_y = reset_occupancy.y;
				int curr_index = curr_x / 100 + curr_y / 100 * 8;
				b.Image()[curr_index].second = false;
				this->setPos(b.getPos(index));
				this->first_move = false;
				b.Image()[index].second = true;
				sf::Vector2f new_pos = b.getPos(index);
				has_moved = true;
				
				
				for (auto& i : drawables) {
				
					if (new_pos == i->getPos() && i->getType()[1]!= this->getType()[1]) {
						if (i->getType() == "kb" || i->getType() == "kw") {
							std::cout << "gameOver";
						}
						drawables.erase(std::remove(drawables.begin(), drawables.end(), i), drawables.end());
						break;


					}
				}
				
				
			}
		}
		return has_moved;

	}

	
	void remove () { this->~Chess(); }
	void reset_moves() { valid_moves.clear(); }
	const sf::Vector2f& getPos() const{ return m_piece.getPosition(); }
	void setPos(const sf::Vector2f m) { m_piece.setPosition(m); }
	sf::RectangleShape getPiece() { return m_piece; }
	virtual const std::string getType() { return "Chess"; }
	virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables) {}
	virtual ~Chess() {}
	virtual int getIndex() {
		int x = (int)(this->getPos().x);
		int y = (int)(this->getPos().y);
		int index = x / 100 + y / 100 * 8;
		return index;
	}
	virtual void Create_Path(std::vector<Chess*> drawables) {}
	std::vector<int>& legal_moves() { return valid_moves; }
	virtual bool check_if_in_check(Board& chess, std::vector<Chess*>& drawables) { return 0; }
	const std::vector<std::vector<int>>& getPath() { return Paths; }
	virtual void reset_danger() {}
	void resetPaths() { Paths.clear(); }
	
};

class Pawn : public Chess {
private:
	
public:
	Pawn(const std::string& text_name, const sf::Vector2f pos) :Chess(text_name, pos) {}
	virtual ~Pawn() {}
	virtual const std::string getType() {

		if (tex_path == "pawnw.png") {
			return "pw";
		}
		else {
			return "pb";
		}
		
	}
	void Create_Path(std::vector<Chess*> drawables) {
		if (this->getType() == "pw") {
			std::vector<int> temp;
			temp.push_back(this->getIndex() - 9);
			temp.push_back(this->getIndex() - 7);
			Paths.push_back(temp);
		}
		else {
			std::vector<int> temp;
			temp.push_back(this->getIndex() + 9);
			temp.push_back(this->getIndex() + 7);
			Paths.push_back(temp);
		}
	}
		virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables){
			

			if ((mousePos.x - this->getPos().x > 0.f && (mousePos.x - this->getPos().x < 100.f) && (mousePos.y - this->getPos().y > 0.f && (mousePos.y - this->getPos().y < 100.f)))) {
				
				if (this->getType() == "pw" && this->first_move) {
					int x = (int)(this->getPos().x);
					int y = (int)(this->getPos().y);

					int index = x / 100 + y / 100 * 8;
					

					
					if (!chess.Image()[index - 8].second) {
						
						valid_moves.push_back(index - 8);
					}
					
					if (!chess.Image()[index - 16].second) {
							
						valid_moves.push_back(index - 16);
					}
				}

				if (this->getType() == "pw" && !this->first_move){

					int x = (int)(this->getPos().x);
					int y = (int)(this->getPos().y);

					int index = x / 100 + y / 100 * 8;

					
					if (!chess.Image()[index - 8].second) {
					
						valid_moves.push_back(index - 8);
					}
					if (chess.Image()[index - 8 + 1].second) {
						for (auto& i : drawables) {
							sf::Vector2f new_pos = chess.getPos(index - 8 + 1);
							if (i->getPos() == new_pos && i->getType()[1] != this->getType()[1] && abs(i->getPos().y - this->getPos().y) == 100) {
								
								valid_moves.push_back(index - 8 + 1);
							}
						}
					}
					if (chess.Image()[index - 8 - 1].second) {
						for (auto& i : drawables) {
							sf::Vector2f new_pos = chess.getPos(index - 8 - 1);
							if (i->getPos() == new_pos && i->getType()[1] != this->getType()[1] && abs(i->getPos().y-this->getPos().y) == 100) {
							
								valid_moves.push_back(index - 8 - 1);
							}
						}
					}

				}
				if (this->getType() == "pb" && this->first_move) {
					int x = (int)(this->getPos().x);
					int y = (int)(this->getPos().y);
					int index = x / 100 + y / 100 * 8;
					

					if (!chess.Image()[index + 8].second) {
					
						valid_moves.push_back(index + 8);
					}
					if (!chess.Image()[index + 16].second) {
						
						valid_moves.push_back(index + 16);
					}

					
				}
				if (this->getType() == "pb" && !this->first_move) {
					int x = (int)(this->getPos().x);
					int y = (int)(this->getPos().y);
					int index = x / 100 + y / 100 * 8;
					

					if (!chess.Image()[index + 8].second) {
						
						valid_moves.push_back(index +8);
					}
					if (chess.Image()[index + 8 + 1].second) {
						for (auto& i : drawables) {
							sf::Vector2f new_pos = chess.getPos(index + 8 + 1);
							if (i->getPos() == new_pos && i->getType()[1] != this->getType()[1] && abs(i->getPos().y - this->getPos().y) == 100) {
								valid_moves.push_back(index + 8 + 1);
							}
						}
					}
					if (chess.Image()[index + 8 - 1].second) {
						for (auto& i : drawables) {
							sf::Vector2f new_pos = chess.getPos(index + 8 - 1);
							if (i->getPos() == new_pos && i->getType()[1] != this->getType()[1] && abs(i->getPos().y - this->getPos().y) == 100) {
								valid_moves.push_back(index + 8 - 1);
							}
						}
					}
				}
			}
			this->Create_Path(drawables);
			
			
		}

		
		

		

};
class Castle : public Chess {
public:

Castle(const std::string& text_name, const sf::Vector2f pos) : Chess(text_name, pos) {}

	virtual const std::string getType() {
		if (tex_path == "castlew.png") {
			return "cw";
		}
		else {
			return "cb";
		}
	}
	void Create_Path(std::vector<Chess*> drawables) {
		std::vector<int> temp;
		//going down
		int i = 8;
		while (this->getIndex()+i<=63 && this->getIndex() + i >= 0)
		{
			
			for (auto j : drawables) {
				if (j->getIndex() == (this->getIndex() + i ) && j->getType()[1] != 'k') {
					temp.clear();
					break;
				}
			}
			temp.push_back(this->getIndex() + i);
			i += 8;
		}
		Paths.push_back(temp);
		temp.clear();
		// going up
		i = -8;
		while (this->getIndex() + i <= 63 && this->getIndex() + i >= 0) {
		
			for (auto j : drawables) {
				if (j->getIndex() == (this->getIndex() + i) && j->getType()[1] != 'k') {
					temp.clear();
					break;
				}
			}
			temp.push_back(this->getIndex() + i);
			i -= 8;
		}
		Paths.push_back(temp);
		temp.clear();
		sf::Vector2f curr_pos = this->getPos();
		int index = this->getIndex();
		int x = (int)curr_pos.x;
		int y = (int)curr_pos.y;
		//right path
		x += 100;
		while (x >= 0 && x < 700 && y >= 0 && y <= 700) {
			for (auto j : drawables) {
				if (j->getIndex() == index + 1 && j->getType()[1] != 'k') {
					temp.clear();
					break;
				}
			}
			temp.push_back(++index);
			x += 100;
		}
		Paths.push_back(temp);
		temp.clear();
		curr_pos = this->getPos();
		index = this->getIndex();
		x = (int)curr_pos.x;
		y = (int)curr_pos.y;
		//left path
		x -= 100;
		while (x > 0 && x <= 700 && y >= 0 && y <= 700) {
			for (auto j : drawables) {
				if (j->getIndex() == index - 1 && j->getType()[1] != 'k') {
					temp.clear();
					break;
				}
				
			}
			temp.push_back(--index);
			x -= 100;
		}
		Paths.push_back(temp);
		temp.clear();

		
		
	}
	virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables) {

		if ((mousePos.x - this->getPos().x > 0.f && (mousePos.x - this->getPos().x < 100.f) && (mousePos.y - this->getPos().y > 0.f && (mousePos.y - this->getPos().y < 100.f)))) {
			int x = (int)(this->getPos().x);
			int y = (int)(this->getPos().y);
			int index = x / 100 + y / 100 * 8;
			if (this->getType() == "cw") {
				
				for (int i = 1; i != 8; i++) {
					if (index - i * 8 >= 0) {
						if (chess.Image()[index - i * 8].second) {
							sf::Vector2f new_pos = chess.getPos(index - i * 8);
							for (auto& j : drawables) {
								if (j->getPos() == new_pos && j->getType()[1] != this->getType()[1]) {
									valid_moves.push_back(index - i * 8);
								}
							}
							break;
						}
						if (!chess.Image()[index - i * 8].second) {
							valid_moves.push_back(index - i * 8);
						}
					}

				}
			}
			if (this->getType() == "cb") {
				int x = (int)(this->getPos().x);
				int y = (int)(this->getPos().y);
				int index = x / 100 + y / 100 * 8;
				for (int i = 1; i != 8; i++) {
					if (index + i * 8  <= 63) {
						if (chess.Image()[index + i * 8].second) {
							// workout if this occupied grid has an enemy piece
							sf::Vector2f new_pos = chess.getPos(index + i * 8);
							for (auto& j : drawables) {
								if (j->getPos() == new_pos && j->getType()[1] != this->getType()[1]) {
									valid_moves.push_back(index + i * 8);
								}
							}
							break;
						}
						if (!chess.Image()[index + i * 8].second) {
							valid_moves.push_back(index + i * 8);
						}
					}
				}
			}
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			//going down
			for (int i = 0; i != 7; i++) {
				index += 8;
				y += 100;
				if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
					bool enemy = false;
					bool empty = false;
					bool friendly = false;
					sf::Vector2f position = chess.getPos(index);
					for (auto& i : drawables) {
						if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							enemy = true;
							break;
						}
						if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							friendly = true;
							break;
						}
					}
					if (!chess.Image()[index].second) {
						empty = true;
					}
					if (empty || enemy) {
						valid_moves.push_back(index);
						if (enemy) {
							break;
						}

					}
					if (friendly) {
						break;
					}

				}
			}
			//going up
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			for (int i = 0; i != 7; i++) {
				index -= 8;
				y -= 100;
				if (x >= 0 && x < 700 && y >= 0 && y <= 700) {
					bool enemy = false;
					bool empty = false;
					bool friendly = false;
					sf::Vector2f position = chess.getPos(index);
					for (auto& i : drawables) {
						if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							enemy = true;
						
							break;
						}
						if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							friendly = true;
							break;
						}
					}
					if (!chess.Image()[index].second) {
						empty = true;
				
					}
					if (empty || enemy) {
						valid_moves.push_back(index);
					
						if (enemy) {
							break;
						}

					}
					if (friendly) {
				
						break;
					}

				}
			}
			// going right 
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			for (int i = 0; i != 7; i++) {
				index += 1;
				if (x >= 0 && x < 700 && y >= 0 && y <= 700) {
					bool enemy = false;
					bool empty = false;
					bool friendly = false;
					sf::Vector2f position = chess.getPos(index);
					for (auto& i : drawables) {
						if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							enemy = true;
						
							break;
						}
						if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							friendly = true;
							break;
						}
					}
					if (!chess.Image()[index].second) {
						empty = true;

					}
					if (empty || enemy) {
						valid_moves.push_back(index);

						if (enemy) {
							break;
						}

					}
					if (friendly) {

						break;
					}

				}
			}
			// going left
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			for (int i = 0; i != 7; i++) {
				index -= 1;
				if (x > 0 && x <= 700 && y >= 0 && y <= 700) {
					bool enemy = false;
					bool empty = false;
					bool friendly = false;
					sf::Vector2f position = chess.getPos(index);
					for (auto& i : drawables) {
						if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							enemy = true;
							
							break;
						}
						if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							friendly = true;
							break;
						}
					}
					if (!chess.Image()[index].second) {
						empty = true;

					}
					if (empty || enemy) {
						valid_moves.push_back(index);

						if (enemy) {
							break;
						}

					}
					if (friendly) {

						break;
					}

				}
			}



		}
		this->Create_Path(drawables);
	}

};

class Horse : public Chess {
public:

	Horse(const std::string& text_name, const sf::Vector2f pos) : Chess(text_name, pos) {}
	
	virtual const std::string getType() {
		if (tex_path == "horsew.png") {
			return "hw";
		}
		else {
			return "hb";
		}
	}
	virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables) {
		if ((mousePos.x - this->getPos().x > 0.f && (mousePos.x - this->getPos().x < 100.f) && (mousePos.y - this->getPos().y > 0.f && (mousePos.y - this->getPos().y < 100.f)))) {

			//where mouse is or the chess piece is and find the corrsponding index for the Board class

			int x = (int)(this->getPos().x);
			int y = (int)(this->getPos().y);
			int index = x / 100 + y / 100 * 8;
			
			// horizontal to the right 
			for (int i = 0; i != 3; i++) {
				if (i <= 1) {
					index += 1;
					x += 100;
					if (!((x >= 0 && x <= 700 && y >= 0 && y <= 700))) {
						break;
					}
				}
				else {
					// one square up
					index -= 8;
					y -= 100;
					
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						// bool value to indicate valid move. valid move is either occupied by enemy piece or empty
						bool enemy = false;
						bool empty = true;
						
						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
						}
						if (chess.Image()[index].second) {
							empty = false;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}
					//one square down
					index += 16;
					y += 200;
						if((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
							bool enemy = false;
							bool empty = false;

							// test to see this is occupied by enemy 
							sf::Vector2f position = chess.getPos(index);
							for (auto& i : drawables) {
								if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
									enemy = true;
									break;
								}
							}
							if (!chess.Image()[index].second) {
								empty = true;
							}
							if (empty || enemy) {
								valid_moves.push_back(index);
							}
						}

				}
				
			}
			// horizontal to the left 
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			for (int i = 0; i != 3; i++) {
				if (i <= 1) {
					index -= 1;
					x -= 100;
				
					if (!(x>=0 && x <= 700 && y>=0 && y<=700)) {
						break;
						
					}
				}
				else {
					// one square up
					index -= 8;
					y -= 100;
					
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						bool enemy = false;
						bool empty = false;

						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}

						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}
					// one square down
					index += 16;
					y += 200;
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						bool enemy = false;
						bool empty = false;

						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}

				}

			}
			// vertical up
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			for (int i = 0; i != 3; i++) {
				if (i <= 1) {
					index -= 8;
					y -= 100;
					if (!(x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						break;

					}
				}
				else {
					// one square to the right
					index += 1;
					x += 100;
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						bool enemy = false;
						bool empty = false;

						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}
					//one square to the left
					index -= 2;
					x -= 200;
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						bool enemy = false;
						bool empty = false;

						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}
				}
				

			}
			// vertical down
			x = (int)(this->getPos().x);
			y = (int)(this->getPos().y);
			index = x / 100 + y / 100 * 8;
			for (int i = 0; i != 3; i++) {
				if (i <= 1) {
					index += 8;
					y += 100;
					if (!(x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						break;

					}
				}
				else {
					// one square to the right
					index += 1;
					x += 100;
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						bool enemy = false;
						bool empty = false;

						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}
					//one square to the left
					index -= 2;
					x -= 200;
					if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
						bool enemy = false;
						bool empty = false;

						// test to see this is occupied by enemy 
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
						}
					}
				}
				

			}


				
		}

		


	}

			

};
class Bishop : public Chess {
public:
	Bishop(const std::string& text_name, const sf::Vector2f pos) : Chess(text_name, pos) {}
	
	virtual const std::string getType() {
		if (tex_path == "bishopw.png") {
			return "bw";
		}
		else {
			return "bb";
		}
	}
	virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables) {
		if ((mousePos.x - this->getPos().x > 0.f && (mousePos.x - this->getPos().x < 100.f) && (mousePos.y - this->getPos().y > 0.f && (mousePos.y - this->getPos().y < 100.f)))) {
			int x = (int)(this->getPos().x);
			int y = (int)(this->getPos().y);
			int index = x / 100 + y / 100 * 8;
			
			// up and right movement 
			for (int i = 0; i != 7; i++) {
				index += 1;
				index -= 8;
				x += 100;
				y -= 100;
				if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
					bool enemy = false;
					bool empty = false;
					bool friendly = false;
					sf::Vector2f position = chess.getPos(index);
					for (auto& i : drawables) {
						if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							enemy = true;
							break;
						}
						if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							friendly = true;
							break;
						}
					}
					if (!chess.Image()[index].second) {
						empty = true;
					}
					if (empty || enemy) {
						valid_moves.push_back(index);
						if (enemy) {
							break;
						}
					}
					if (friendly) {
						break;
					}
					


				}
			}
			 x = (int)(this->getPos().x);
			 y = (int)(this->getPos().y);
			 index = x / 100 + y / 100 * 8;
			 // up and left
			 for (int i = 0; i != 7; i++) {
				 index -= 1;
				 index -= 8;
				 x -= 100;
				 y -= 100;
				 if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
					 bool enemy = false;
					 bool empty = false;
					 bool friendly = false;
					 sf::Vector2f position = chess.getPos(index);
					 for (auto& i : drawables) {
						 if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							 enemy = true;
							 break;
						 }
						 if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							 friendly = true;
							 break;
						 }
					 }
					 if (!chess.Image()[index].second) {
						 empty = true;
					 }
					 if (empty || enemy) {
						 valid_moves.push_back(index);
						 if (enemy) {
							 break;
						 }
						 
					 }
					 if (friendly) {
						 break;
					 }



				 }
			 }
			 x = (int)(this->getPos().x);
			 y = (int)(this->getPos().y);
			 index = x / 100 + y / 100 * 8;
			 // down and right 
			 for (int i = 0; i != 7; i++) {
				 index += 1;
				 index += 8;
				 x += 100;
				 y += 100;
				 if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
					 bool enemy = false;
					 bool empty = false;
					 bool friendly = false;
					 sf::Vector2f position = chess.getPos(index);
					 for (auto& i : drawables) {
						 if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							 enemy = true;
							 break;
						 }
						 if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							 friendly = true;
							 break;
						 }
					 }
					 if (!chess.Image()[index].second) {
						 empty = true;
					 }
					 if (empty || enemy) {
						 valid_moves.push_back(index);
						 if (enemy) {
							 break;
						 }

					 }
					 if (friendly) {
						 break;
					 }



				 }
			 }
			 x = (int)(this->getPos().x);
			 y = (int)(this->getPos().y);
			 index = x / 100 + y / 100 * 8;
			 // down and left
			 for (int i = 0; i != 7; i++) {
				 index -= 1;
				 index += 8;
				 x -= 100;
				 y += 100;
				 if ((x >= 0 && x <= 700 && y >= 0 && y <= 700)) {
					 bool enemy = false;
					 bool empty = false;
					 bool friendly = false;
					 sf::Vector2f position = chess.getPos(index);
					 for (auto& i : drawables) {
						 if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
							 enemy = true;
							 break;
						 }
						 if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
							 friendly = true;
							 break;
						 }
					 }
					 if (!chess.Image()[index].second) {
						 empty = true;
					 }
					 if (empty || enemy) {
						 valid_moves.push_back(index);
						 if (enemy) {
							 break;
						 }

					 }
					 if (friendly) {
						 break;
					 }



				 }
			 }

		}
	}
};
class Queen : public Bishop {
public:
	Queen(const std::string& text_name, const sf::Vector2f pos) : Bishop(text_name, pos) {}

	virtual const std::string getType() {
		if (tex_path == "queenw.png") {
			return "qw";
		}
		else {
			return "qb";
		}
	}
	virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables) {
		Bishop::check_move(mousePos,chess,drawables);
		if ((mousePos.x - this->getPos().x > 0.f && (mousePos.x - this->getPos().x < 100.f) && (mousePos.y - this->getPos().y > 0.f && (mousePos.y - this->getPos().y < 100.f)))) {
			int x = (int)(this->getPos().x);
			int y = (int)(this->getPos().y);
			int index = x / 100 + y / 100 * 8;
				if (this->getType() == "qw") {
					int x = (int)(this->getPos().x);
					int y = (int)(this->getPos().y);
					int index = x / 100 + y / 100 * 8;
					for (int i = 1; i != 8; i++) {
						if (index - i * 8 >= 0) {
							if (chess.Image()[index - i * 8].second) {
								sf::Vector2f new_pos = chess.getPos(index - i * 8);
								for (auto& j : drawables) {
									if (j->getPos() == new_pos && j->getType()[1] != this->getType()[1]) {
										valid_moves.push_back(index - i * 8);
									}
								}
								break;
							}
							if (!chess.Image()[index - i * 8].second) {
								valid_moves.push_back(index - i * 8);
							}
						}

					}
				}
				if (this->getType() == "qb") {
					int x = (int)(this->getPos().x);
					int y = (int)(this->getPos().y);
					int index = x / 100 + y / 100 * 8;
					for (int i = 1; i != 8; i++) {
						if (index + i * 8 <= 63) {
							if (chess.Image()[index + i * 8].second) {
								// workout if this occupied grid has an enemy piece
								sf::Vector2f new_pos = chess.getPos(index + i * 8);
								for (auto& j : drawables) {
									if (j->getPos() == new_pos && j->getType()[1] != this->getType()[1]) {
										valid_moves.push_back(index + i * 8);
									}
								}
								break;
							}
							if (!chess.Image()[index + i * 8].second) {
								valid_moves.push_back(index + i * 8);
							}
						}
					}
				}
				//horizontal to the right 
				x = (int)(this->getPos().x);
				y = (int)(this->getPos().y);
				index = x / 100 + y / 100 * 8;
				for (int i = 0; i != 7; i++) {
					index += 1;
					x += 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						bool friendly = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
							if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
								friendly = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
							if (enemy) {
								break;
							}

						}
						if (friendly) {
							break;
						}

					}
				}
				x = (int)(this->getPos().x);
				y = (int)(this->getPos().y);
				index = x / 100 + y / 100 * 8;
				for (int i = 0; i != 7; i++) {
					index -= 1;
					x -= 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						bool friendly = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
							if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
								friendly = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
							if (enemy) {
								break;
							}

						}
						if (friendly) {
							break;
						}

					}
				}
				// going down
				x = (int)(this->getPos().x);
				y = (int)(this->getPos().y);
				index = x / 100 + y / 100 * 8;
				for (int i = 0; i != 7; i++) {
					index += 8;
					y += 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						bool friendly = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
							if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
								friendly = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
							if (enemy) {
								break;
							}

						}
						if (friendly) {
							break;
						}

					}
				}
				//going up
				x = (int)(this->getPos().x);
				y = (int)(this->getPos().y);
				index = x / 100 + y / 100 * 8;
				for (int i = 0; i != 7; i++) {
					index -= 8;
					y -= 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						bool friendly = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								break;
							}
							if (i->getPos() == position && i->getType()[1] == this->getType()[1]) {
								friendly = true;
								break;
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);
							if (enemy) {
								break;
							}

						}
						if (friendly) {
							break;
						}

					}
				}


		}
		
	}

	
};
class King : public Chess {
private: 
	std::vector<int> danger_pieces;
public:

	King(const std::string& text_name, const sf::Vector2f pos) : Chess(text_name, pos) {}
	

	virtual const std::string getType() {
		if (tex_path == "kingw.png") {
			return "kw";
		}
		else {
			return "kb";
		}
	}
	bool check_if_in_check(Board& chess, std::vector<Chess*>& drawables) {
	
		bool in_check = false;
		//index of all the pieces keeping king in check
		
		sf::Vector2f kingPos = this->getPos();
		int x = (int)(kingPos.x);
		int y = (int)(kingPos.y);
		int index = x / 100 + y / 100 * 8;
		for (auto& i : drawables) {
			for (auto& j : i->legal_moves()) {
				if (j == index) {
					in_check = true;
					int curr_x = (int)(i->getPos().x);
					int curr_y = (int)(i->getPos().y);
					int curr_index = curr_x / 100 + curr_y / 100 * 8;
					danger_pieces.push_back(curr_index);
				}
			}
		}
		for (auto& i : danger_pieces) {
			chess.Image()[i].first.setFillColor(sf::Color::Blue);
		}
		return in_check;
	}
	void reset_danger() { danger_pieces.clear(); }
	virtual void check_move(const sf::Vector2i mousePos, Board& chess, std::vector<Chess*> drawables) {
		if ((mousePos.x - this->getPos().x > 0.f && (mousePos.x - this->getPos().x < 100.f) && (mousePos.y - this->getPos().y > 0.f && (mousePos.y - this->getPos().y < 100.f)))) {
			int x = (int)(this->getPos().x);
			int y = (int)(this->getPos().y);
			int index = x / 100 + y / 100 * 8;
			for (int i = 1; i != 9; i++) {
				if (i == 1) {
					// up
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index -= 8;
					y -= 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;
								
							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 2) {
					//going down
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index += 8;
					y += 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 3) {
					//going right
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index += 1;
					x += 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 4) {
					//going left
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index -= 1;
					x -= 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 5) {
					//going diagonally up and right
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index += 1;
					index -= 8;
					x += 100;
					y -= 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 6) {
					//going diagonally up and left
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index -= 1;
					index -= 8;
					x -= 100;
					y -= 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 7) {
					//going diagonally down and right
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index += 1;
					index += 8;
					x += 100;
					y += 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}
				if (i == 8) {
					//going diagonally down and left
					x = (int)(this->getPos().x);
					y = (int)(this->getPos().y);
					index = x / 100 + y / 100 * 8;
					index -= 1;
					index += 8;
					x -= 100;
					y += 100;
					if (x >= 0 && x <= 700 && y >= 0 && y <= 700) {
						bool enemy = false;
						bool empty = false;
						sf::Vector2f position = chess.getPos(index);
						for (auto& i : drawables) {
							if (i->getPos() == position && i->getType()[1] != this->getType()[1]) {
								enemy = true;

							}
						}
						if (!chess.Image()[index].second) {
							empty = true;
						}
						if (empty || enemy) {
							valid_moves.push_back(index);

						}

					}
				}

			}
		}
		// check to see these legal moves will not place king in a check position 
		for (auto& i : drawables) {
			if (i->getType()[1] != this->getType()[1]) {
				for (auto& j : i->legal_moves()) {
					for (auto& k : this->legal_moves()) {
						if (j == k) {
							this->legal_moves().erase(std::remove(this->legal_moves().begin(), this->legal_moves().end(), k), this->legal_moves().end());
							break;
						}
					}
				}
			}
		}
		
			for (auto& i : drawables) {
				if (i->getType()[1] != this->getType()[1]) {
					for (auto& j : i->getPath()) {
						for (auto& M : j) {
							for (auto& k : this->legal_moves()) {
								if (M == k) {
									this->legal_moves().erase(std::remove(this->legal_moves().begin(), this->legal_moves().end(), k), this->legal_moves().end());
									break;
								}
							}
						}
					}
				}
			}
		
	}
	
	

};