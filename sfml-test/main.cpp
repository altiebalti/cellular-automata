#include "SFML/Graphics.hpp"
#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <thread>
#include <algorithm>


#define N 50 // number of cells
#define K 20 // size of cells

template <typename T>
std::array<T, N*N> update(std::array < T, N*N >& arr) {
    std::array<T, N* N> temp_arr = arr;            
    for (int i = 0; i < N * N; i++) {
        T temp = arr[i];

        int indeces[] = { i + 1, i - 1, i + N, i - N, i + N + 1, i + N - 1, i - N - 1, i - N + 1 };

        int x = i % N;
        int y = i / N;
        int indecesx[] = { x + 1, x - 1, x, x, x + 1, x - 1, x - 1, x + 1 };
        int indecesy[] = { y, y, y + 1, y - 1, y + 1, y + 1, y - 1, y - 1 };
        int sum = 0;                   
        for (int k = 0; k < 8; k++) {
            if (indecesx[k] >= 0 && indecesx[k] < N && indecesy[k] >= 0 && indecesy[k] < N) {
                // sum += arr[indecesy[k] * N + indecesx[k]];
                sum += arr[indeces[k]];
            }
        }
        if (temp) {
            if (sum < 2) { // check for underpopulation
                temp_arr[i] = false;
            }
            else if (sum > 3) { // check for overpopulation
                temp_arr[i] = false;
            }
        }
        else {
            if (sum == 3) { // check for reproduction
                temp_arr[i] = true;
            }
        }
    }
    return temp_arr;
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(N*K, N*K), "cellular automata");
    bool a = true;
    bool game_not_started = true;
    std::array<bool, N*N> game_field = {};
    std::array<bool, N* N> next_game_field = {};
    while (window.isOpen())
    {
        sf::RectangleShape base(sf::Vector2f(0, 0));
        base.setSize(sf::Vector2f(K, K));
        base.setOutlineColor(sf::Color(255, 255, 255));
        base.setOutlineThickness(0.9);
        

        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f pos = sf::Vector2f(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
                if (sf::Mouse::Left == event.key.code && game_not_started) {
                    std::cout << "\n" << pos.x << ' ' << pos.y;
                    int calculated_y = int(pos.y) / K;
                    int calculated_x = int(pos.x) / K;
                    std::cout << "\n" << calculated_x << ' ' << calculated_y;
                    int index = calculated_y * N + calculated_x;
                    game_field[index] = !game_field[index];
                }
            }
            else if (event.type == sf::Event::KeyPressed && sf::Keyboard::Space == event.key.code) {
                game_not_started = !game_not_started;
                
            }
        }   
        
        if (!game_not_started) {
            next_game_field = update(game_field);
            game_field.swap(next_game_field);
        }
        for (int i = 0; i < N * N; i++) {
            
            base.setPosition(K * (i % N), K * (i / N)); 
            int col = game_field[i] * 255; 
            base.setFillColor(sf::Color(col, col, col)); 
            window.draw(base);
             
        }
        
        window.display();
        if (!game_not_started) {
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
        


    }

    return 0;
}


//
// 111
// 010
// 101
//
//