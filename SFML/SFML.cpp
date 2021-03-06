#include <iostream>
#include "SFML/Config.hpp"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/OpenGL.hpp"
#include "Chess.h"



int mousePos_to_grid(const sf::Vector2i mousePos) {
    int x = (int)(mousePos.x);
    int y = (int)(mousePos.y);
    int index = x / 100 + y / 100 * 8;
    return index;

}








int main()
{

    // bool value to ensure mouse was pressed true is for white player
    bool turn = true;
    sf::Mouse m;
    std::vector<Chess*> drawables;

    for (int i = 0; i != 8; i++) {
        Pawn* p = new Pawn("pawnw.png", sf::Vector2f(i * 100.f, 600.f));
        drawables.push_back(p);

    }
    for (int i = 0; i != 8; i++) {
        Pawn* p = new Pawn("pawnb.png", sf::Vector2f(i * 100.f, 100.f));
        drawables.push_back(p);

    }

    for (int i = 0; i != 2; i++) {
        Castle* p = new Castle("castlew.png", sf::Vector2f(i * 700.f, 700.f));
        Castle* p1 = new Castle("castleb.png", sf::Vector2f(i * 700.f, 0));
        drawables.push_back(p);
        drawables.push_back(p1);
    }

    for (int i = 0; i != 2; i++) {
        Horse* p = new Horse("horsew.png", sf::Vector2f(100.f + i * 500.f, 700.f));
        Horse* p1 = new Horse("horseb.png", sf::Vector2f(100.f + i * 500, 0));
        drawables.push_back(p);
        drawables.push_back(p1);

    }
    for (int i = 0; i != 2; i++) {
        Bishop* p = new Bishop("bishopw.png", sf::Vector2f(200.f + i * 300.f, 700.f));
        Bishop* p1 = new Bishop("bishopb.png", sf::Vector2f(200.f + i * 300, 0));
        drawables.push_back(p);
        drawables.push_back(p1);

    }
    Queen* p = new Queen("queenw.png", sf::Vector2f(300.f, 700.f));
    Queen* p1 = new Queen("queenb.png", sf::Vector2f(300.f, 0.f));
    drawables.push_back(p);
    drawables.push_back(p1);

    King* k = new King("kingw.png", sf::Vector2f(400.f, 700.f));
    King* k1 = new King("kingb.png", sf::Vector2f(400.f, 0.f));
    drawables.push_back(k);
    drawables.push_back(k1);


    sf::RenderWindow window(sf::VideoMode(800, 800), "chess");
    window.setPosition(sf::Vector2i(0, 0));
    Board chess;
    Chess* temp_piece = new Chess();
    // if king is in check
    bool in_check_w = false;
    bool in_check_b = false;
    Chess* piece = new Chess();
    bool show = true;
    int counter = 0;


    while (window.isOpen())
    {
        for (auto& i : drawables) {
            i->check_move(sf::Vector2i(i->getPos().x + 50, i->getPos().y + 50), chess, drawables);
        }
        for (auto& i : drawables) {
            if (i->getType() == "kw") {
                in_check_w = i->check_if_in_check(chess, drawables);
            }
            if (i->getType() == "kb") {
                in_check_b = i->check_if_in_check(chess, drawables);

            }

        }
        int king_index;
        Chess* king_pointer = new Chess();
        Chess* temp_pointer = new Chess();
        std::vector<int> index_of_danger;
        std::vector<int> check_path;
        std::vector<int> new_legal_moves;
        check_path.clear();
        index_of_danger.clear();
        // bool if true don't delete
        bool del = false;
        std::vector<int> to_be_deleted;
        if (in_check_b) {
            show = true;
            for (auto& i : drawables) {
                if (i->getType() == "kb") {
                    king_index = i->getIndex();
                    king_pointer = i;
                }
            }
            //find check paths for each piece
            index_of_danger = king_pointer->get_danger_pieces();
            for (auto& i : index_of_danger) {
                temp_pointer = get_Pointer(i, drawables);

                check_path = temp_pointer->danger_path(i, king_index);
                check_path.insert(check_path.begin(), i);
                if (show) {
                    for (auto& i : check_path) {
                        std::cout << i << "\t";
                    }
                    std::cout << "\n";
                }
                show = false;
            }
        }       if (in_check_b) {
            for (auto i : drawables) {
                if (i->getType() != "kb" && i->getType()[1] == 'b') {
                    for (auto j : i->legal_moves()) {
                        for (auto k : check_path) {
                            if (j == k) {
                                del = true;
                                break;
                            }
                        }
                        if (!del) {
                            to_be_deleted.push_back(j);
                        }
                        del = false;
                    }

                }
                for (auto j : to_be_deleted) {
                    i->legal_moves().erase(std::remove(i->legal_moves().begin(), i->legal_moves().end(), j), i->legal_moves().end());
                }
                to_be_deleted.clear();
                del = false;
            }
        }
        if (in_check_w) {
            show = true;
            for (auto& i : drawables) {
                if (i->getType() == "kw") {
                    king_index = i->getIndex();
                    king_pointer = i;
                }
            }
            //find check paths for each piece
            index_of_danger = king_pointer->get_danger_pieces();
            for (auto& i : index_of_danger) {
                temp_pointer = get_Pointer(i, drawables);

                check_path = temp_pointer->danger_path(i, king_index);
                check_path.insert(check_path.begin(), i);
                if (show) {
                    for (auto& i : check_path) {
                        std::cout << i << "\t";
                    }
                    std::cout << "\n";
                }
                show = false;
            }
        }       if (in_check_w) {
            for (auto i : drawables) {
                if (i->getType() != "kw" && i->getType()[1]=='w') {
                    for (auto j : i->legal_moves()) {
                        for (auto k : check_path) {
                            if (j == k) {
                                del = true;
                                break;
                            }
                        }
                        if (!del) {
                            to_be_deleted.push_back(j);
                        }
                        del = false;
                    }

                }
                for (auto j : to_be_deleted) {
                    i->legal_moves().erase(std::remove(i->legal_moves().begin(), i->legal_moves().end(), j), i->legal_moves().end());
                }
                to_be_deleted.clear();
                del = false;
            }
        }
            
        


        //to keep a track of the chesspiece the mouse clicked on

        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Close the window if a key is pressed or if requested
            if (event.type == sf::Event::Closed) window.close();

            // If escape is pressed, close the application
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) window.close();
            if (event.type == sf::Event::MouseButtonPressed) {

                for (auto i : drawables) {
                    // make sure we only allow the correct player to move
                    if (turn) {
                        if (i->getType()[1] == 'w') {
                            int index = mousePos_to_grid(m.getPosition());
                            if (chess.getPos(index) == i->getPos()) {
                                if (i != temp_piece) {
                                    chess.reset_colour();
                                }
                                temp_piece = i;
                                chess.Image()[index].first.setFillColor(sf::Color::Blue);
                                for (auto& j : i->legal_moves()) {
                                    chess.Image()[j].first.setFillColor(sf::Color::Blue);
                                }
                            }

                        }
                       /* if (in_check_w) {
                            int index = mousePos_to_grid(m.getPosition());
                            if (chess.getPos(index) == i->getPos() && i->getType() == "kw") {
                                if (i != temp_piece) {
                                    chess.reset_colour();
                                }
                                temp_piece = i;
                                chess.Image()[index].first.setFillColor(sf::Color::Blue);
                                for (auto& j : i->legal_moves()) {
                                    chess.Image()[j].first.setFillColor(sf::Color::Blue);
                                }
                            }
                        }*/

                    }
                    if (!turn) {
                        if (i->getType()[1] == 'b') {
                            int index = mousePos_to_grid(m.getPosition());
                            if (chess.getPos(index) == i->getPos()) {
                                if (i != temp_piece) {
                                    chess.reset_colour();
                                }
                                temp_piece = i;
                                chess.Image()[index].first.setFillColor(sf::Color::Blue);
                                for (auto& j : i->legal_moves()) {
                                    chess.Image()[j].first.setFillColor(sf::Color::Blue);
                                }
                            }
                        }

                       /* if (in_check_b) {
                            int index = mousePos_to_grid(m.getPosition());
                            if (chess.getPos(index) == i->getPos() && i->getType() == "kb") {
                                if (i != temp_piece) {
                                    chess.reset_colour();
                                }
                                temp_piece = i;
                                chess.Image()[index].first.setFillColor(sf::Color::Blue);
                                for (auto& j : i->legal_moves()) {
                                    chess.Image()[j].first.setFillColor(sf::Color::Blue);
                                }
                            }
                        }*/

                    }



                }


            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (temp_piece->Move(m.getPosition(), chess, drawables)) {
                    if (turn) {
                        turn = false;
                        chess.reset_colour();
                    }
                    else {
                        turn = true;
                        chess.reset_colour();
                    }
                }


                for (auto i : drawables) {
                    i->reset_moves();
                    i->resetPaths();
                    if (i->getType()[0] == 'k') {
                        i->reset_danger();
                    }
                }
            }
        }




        // Clear the window and apply grey background
        window.clear(sf::Color::Black);
        for (auto& i : chess.Image()) {
            window.draw(i.first);
        }

        for (auto& i : drawables) {
            window.draw(i->getPiece());
        }


        // Update display and wait for vsync
        window.display();

    }
    in_check_b = false;
    in_check_w = false;
    return 0;
}
