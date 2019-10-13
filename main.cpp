#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <string>
#include <memory>
#include <set>
#include <SFML/Graphics.hpp>
#include <Windows.h>

#include "mau_tools.h"

sf::RectangleShape shape;

int main()
{
   //*
   setlocale(NULL,"ru_RU");
   
   SetConsoleOutputCP(1251);
   SetConsoleCP(1251);

   sf::RenderWindow window(sf::VideoMode(600,400), "Title");
   shape.setPosition(10, 10);
   
   call_backs["resize"] = [](VIS_EXEC* context, VEC<EXPR*>& args)->void {
      int x, y;
      y = context->num_stack.back();
      context->num_stack.pop_back();
      x = context->num_stack.back();
      context->num_stack.pop_back();
      shape.setSize(sf::Vector2f(x, y));
   };

   while (window.isOpen()) {
      sf::Event ev;
      while (window.pollEvent(ev)) {
         if (ev.type == sf::Event::Closed) {
            window.close();
         }
      }
      run_script("C:/Users/Zver/Desktop/main.mau");
      window.clear();
      window.draw(shape);
      window.display();
   }
   //*/
}


//lex_it("./file.mau");
