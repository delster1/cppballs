#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <stdlib.h> // for
#include <SFML/Window/Mouse.hpp>
#include <vector>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <direct.h>
#include <windows.h>
#include <conio.h>
#include <string.h>
#include <thread>

using namespace sf;
using namespace std;

RenderWindow window(VideoMode(500, 500), "SFML works!");    

double siz = 25.f;
CircleShape shape(siz);
RectangleShape rectangles[3];
RectangleShape q2port(Vector2f(200,10));
RectangleShape q1port(Vector2f(10,200));
RectangleShape q4port(Vector2f(200, 10));
RectangleShape q3port(Vector2f(10, 200));

double vel;
double xvel = 0.f;
double yvel = 0.f;
double grav = 0.3f;
double frict = 3.4f;
bool dragging = false;

vector<Vector2i> poss;

void drag(){
    // dragging = true;
    double x = Mouse::getPosition(window).x - siz;
    double y = Mouse::getPosition(window).y - siz;
    yvel = 0;
    xvel = 0;
    double sposx = shape.getPosition().x;
    double sposy = shape.getPosition().y - grav;

    shape.setPosition(x,y);


    double dx = x - sposx;    
    double dy = y - sposy;

}

void dothrow(){
    double x = Mouse::getPosition(window).x - siz;
    double y = Mouse::getPosition(window).y - siz;

    double sposx = shape.getPosition().x;
    double sposy = shape.getPosition().y - grav;

    double dx = x - sposx;    
    double dy = y - sposy;

    double distance = (sqrt((dx*dx)+(dy*dy)))/25;        

    xvel = dx * distance;
    yvel = dy * distance;

    dragging = false;
}


void movement(){


    shape.setFillColor(Color(171,39,242,255));

            yvel+=grav; 
            shape.move(xvel,yvel);
        if(shape.getPosition().y <= 0 and yvel < 0){

            yvel *= -1 * fabs(25/siz-grav);
        }
        if(shape.getPosition().y>=(500.f-(siz*2)) and fabs(yvel) > .01){

            yvel*= -1 * fabs(25/siz - grav);
            shape.move(xvel,yvel);
        }
        else if (fabs(yvel) < .01){
    
            yvel = 0;
            shape.move(xvel,yvel);  
        }
        // bounce off bottom
        
        // bounce off right
        else if(shape.getPosition().x > (500.f-(siz*2)) and xvel > 0){
            xvel -= frict;
            xvel= -1 * fabs(xvel);
            shape.move(xvel,yvel);
        }
        // bounce off left
        else if(shape.getPosition().x <0 and xvel < 0){
            xvel += frict;
            xvel = fabs(xvel);
            shape.move(xvel,yvel);
        }
        if(Mouse::isButtonPressed(Mouse::Button::Left)){

            yvel -= grav;
            drag();     
            dragging = true; 
        }else if(dragging == true and Mouse::isButtonPressed(Mouse::Button::Left)==false and shape.getPosition().y > 0){
            dothrow();
        }

        shape.move(xvel,yvel);    // else 
}


struct location{
    double x;
    double y;
};

class Portal{
    // SEE IF MAKING PORTAL FROM SLOPE IS BETTER TO KEEP PORTAL PARAMATERS IN CLASS AND
    // ADD DRAWING PORTALS ON WALLS AND TESTING IF THERE ARE TWO PORTALS


    public:
        double x;
        double y;

        bool complete;

        bool horizontal;
        bool vertical;

        

        string toString()
        {
            string str = to_string(x) + " " + to_string(y);
            return str;
        }

        void drawPortal()
        {
            if (x == 0)
            {
                q1port.setPosition(x, y);
                window.draw(q4port);
                cout << "x portal drawn " << endl;
                horizontal = true;
                vertical = false;
            }
            else if (y == 0)
            {
                q2port.setPosition(x, y);
                window.draw(q1port);
                cout << "y portal drawn " << endl;
                vertical = true;
                horizontal = false;
            }
            else if (x == 500)
            {
                q3port.setPosition(x - 10, y);
                window.draw(q2port);
                cout << "x portal drawn " << endl;
                horizontal = true;
                vertical = false;
            }
            else if (y == 500)
            {
                q4port.setPosition(x, y - 10);
                window.draw(q3port);
                cout << "y portal drawn " << endl;
                vertical = true;
                horizontal = false;
            }
        }
        Portal()
        {
            double mx = Mouse::getPosition(window).x;
            double my = Mouse::getPosition(window).y;
            double ogX = 250;
            double ogY = 250;
            double slope = (my - ogY) / (mx - ogX);
            double ypos;
            double xpos;
            string quad = "";
            double m = 1;
            double formx = Mouse::getPosition(window).x - 250;
            double formy = (Mouse::getPosition(window).y - 250) * -1;
            double formm = (0 - formy) / (0 - formx);
            if (fabs(slope) > m & my < 250)
            {
                ypos = 0;
                xpos = 500 + ((250 / formm) - 250);
                quad = "q1";
            }
            else if (fabs(slope) > m & my > 250)
            {
                ypos = 500;
                xpos = 500 - (250 / formm) - 250;

                quad = "q3";
            }

            else if (slope<m & mx> 250)
            {
                xpos = 500;
                ypos = 250 - (formm * (250));
                quad = "q2";
            }
            else if (slope < m & mx < 250)
            {
                xpos = 0;
                ypos = 250 - (formm * -250);
                quad = "q4";
            }
            x = xpos;
            y = ypos;
            cout << x << " " << y << endl;
        }
};

bool checkIntersect(RectangleShape rects[], CircleShape shape){
    //METHOD TO CHECK IF PORTALS ARE INTERSECTING
    bool result;
    for (int i = 0; i <3; i++){

        if(rects[i].getPosition().x == shape.getPosition().x && rects[i].getPosition().y == shape.getPosition().y){
           return false;
        }
        else{
            result = true;
        }
    }
    cout << result << endl;
    return result;

}






void AKeyPress(){
    
    double x = Mouse::getPosition(window).x;
    double y = Mouse::getPosition(window).y;
    // location pt = getQuad(x,y);wwww

}

void drawPortal(location loc) {
    
} 

int main()
{
    
    bool Apressed = false;
    bool Spressed = false;
    double height = 450.f;
    
    Portal portals[2];
    sf::RectangleShape rects[3];
    q1port.setPosition(700, 700);
    q2port.setPosition(700, 700);
    q3port.setPosition(700, 700);
    q4port.setPosition(700, 700);

    rects[0] = q1port; rects[1] = q2port; rects[2] = q3port; rects[4] = q4port;
    Time timePerFrame = seconds(1/60.0f);
    Clock deltaClock;
    Time timeSinceLastUpdate = Time::Zero;
    while (window.isOpen())
    {
        Event event;
        Time deltaTime = deltaClock.restart();
        timeSinceLastUpdate += deltaTime;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        while(timeSinceLastUpdate >= timePerFrame){
            
            timeSinceLastUpdate -= timePerFrame;
            
            
            
            window.draw(shape);
            window.draw(q1port);
            window.draw(q2port);
            window.draw(q3port);
            window.draw(q4port);

            if (Keyboard::isKeyPressed(Keyboard::A) && Apressed == false)
            {
                AKeyPress();
                Apressed = true;

                Portal p;
                portals[0] = p;

                cout << p.toString() << endl;
                q1port.setFillColor(sf::Color(255, 255, 255));
                q2port.setFillColor(sf::Color(255, 255, 255));
                q3port.setFillColor(sf::Color(255, 255, 255));
                q4port.setFillColor(sf::Color(255, 255, 255));

                portals[0].drawPortal();
            }
            else if (Keyboard::isKeyPressed(Keyboard::S) && Spressed == false)
            {
                AKeyPress();
                Spressed = true;
                Portal p;
 
                portals[1] = p;
                q1port.setFillColor(sf::Color(255, 255, 255));
                q2port.setFillColor(sf::Color(255, 255, 255));
                q3port.setFillColor(sf::Color(255, 255, 255));
                q4port.setFillColor(sf::Color(255, 255, 255));
                cout << p.toString() << endl;
                // p.drawPortal();
                portals[1].drawPortal();
                // window::draw
            }
            else if (Keyboard::isKeyPressed(Keyboard::Space))
            {
                q1port.setFillColor(sf::Color(255,255,255,0));
                q2port.setFillColor(sf::Color(255, 255, 255, 0));
                q3port.setFillColor(sf::Color(255, 255, 255, 0));
                q4port.setFillColor(sf::Color(255, 255, 255, 0));

                q1port.setPosition(700, 700);
                q2port.setPosition(700, 700);
                q3port.setPosition(700, 700);
                q4port.setPosition(700,700);

                Spressed = false;
                Apressed = false;
            }
            checkIntersect(rects, shape);
            movement();
            // cout << portals[0].checkIntersect(shape.getPosition().x, shape.getPosition().y) << endl;
            // cout << portals[1].checkIntersect(shape.getPosition().x, shape.getPosition().y) << endl;

            // portals[1].drawPortal();
            // window.draw(xport);
            window.display();

            window.clear(); 
        }
        
        
        //error because doesnt know how to return position
        
       
    }

    return 0;
}

