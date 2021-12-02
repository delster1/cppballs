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
RectangleShape q2port(Vector2f(10,200));
RectangleShape q1port(Vector2f(200,10));
RectangleShape q4port(Vector2f(10, 200));
RectangleShape q3port(Vector2f(200, 10));

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
            // shape.move(xvel,yvel);  
        }
        // bounce off bottom
        
        // bounce off right
        else if(shape.getPosition().x > (500.f-(siz*2)) and xvel > 0){
            xvel -= frict;
            xvel= -1 * fabs(xvel);
            // shape.move(xvel,yvel);
        }
        // bounce off left
        else if(shape.getPosition().x <0 and xvel < 0){
            xvel += frict;
            xvel = fabs(xvel);
            // shape.move(xvel,yvel);
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

struct portalStuff {
    int quad;
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
        
        int quad = 0;
        string toString()
        {
            string str = to_string(x) + " " + to_string(y);
            return str;
        }

        void drawPortal()
        {
            switch (quad){
                case 1:
                    q1port.setPosition(x, y);
                    window.draw(q1port);
                    cout << "x portal drawn " << endl;
                    break;
                case 2:
                    q2port.setPosition(x-10, y);
                    window.draw(q2port);
                    cout << "y portal drawn " << endl;
                    break;

                case 3:
                    q3port.setPosition(x, y-10);
                    window.draw(q3port);
                    cout << "x portal drawn " << endl;
                    break;
                case 4:
                    q4port.setPosition(x, y - 10);
                    window.draw(q4port);
                    cout << "y portal drawn " << endl;
                    break;
            }   

        }

        portalStuff getQuad(){

            portalStuff p;
            static double r[3];
            double mx = Mouse::getPosition(window).x;
            double my = Mouse::getPosition(window).y;
            double ogX = 250;
            double ogY = 250;
            double slope = (my - ogY) / (mx - ogX);
            double ypos;
            double xpos;

            double m = 1;
            double formx = Mouse::getPosition(window).x - 250;
            double formy = (Mouse::getPosition(window).y - 250) * -1;
            double formm = (0 - formy) / (0 - formx);
            if (fabs(slope) > m & my < 250)
            {
                ypos = 0;
                xpos = 500 + ((250 / formm) - 250) -100;
                quad = 1;
            }
            else if (fabs(slope) > m & my > 250)
            {
                ypos = 500;
                xpos = 500 - (250 / formm) - 250 -100;

                quad = 3;
            }

            else if (slope<m & mx> 250)
            {
                xpos = 500;
                ypos = 250 - (formm * (250)) -100;
                quad = 2;
            }
            else if (slope < m & mx < 250)
            {
                xpos = 0;
                ypos = 250 - (formm * -250) -100;
                quad = 4;
            }
            x = xpos;
            y = ypos;
            p.x = xpos;
            p.y = ypos;
            p.quad = quad;
            
            return p;
        }
        Portal()
        {
            portalStuff p = getQuad();
            quad = p.quad;
            x = p.x;
            y = p.y;
            drawPortal();
        }
};

bool inRange(unsigned low, unsigned high, unsigned x)        
{        
    return (low <= x && x <= high);         
} 

void teleportBall(int initQuad, Portal portals[2]){
    cout  << initQuad << endl;

    int newy;
    int newx;
    int og;
    int other;
    for(int i = 0; i<2;i++){
        if(portals[i].quad == initQuad){
            og = i;
            other = 1-i;
            break;
        }
    }
    if (portals[og].quad % 2 == 0 && portals[other].quad % 2 == 0 )
    {
        newy = shape.getPosition().y - portals[og].y;
        shape.setPosition(portals[other].x,portals[other].y + newy);
    }
    else if (portals[og].quad % 2 == 1 && portals[other].quad % 2 == 1){
        
        // cout<<portals[og].quad << endl;
        newx = portals[other].x + (shape.getPosition().x - portals[og].x);

        if(portals[og].y == 0){
            newy = 500;
        }else if(portals[og].y == 500){
            newy = 0;
        }
        shape.setPosition(newx,newy);
        // shape.move(xvel,yvel);
    }
}

void quadMath(int q1, int q2){
    
}

void portMove(Portal portals[2]){
    int initPort;

    bool port1Intersect;
    bool port2Intersect;

    int p1quad = portals[0].quad;
    int p2quad = portals[1].quad;

    double sposy = shape.getPosition().y;
    double sposx = shape.getPosition().x;

    double p1x = portals[0].x;
    double p1y = portals[0].y;

    double p2x = portals[1].x;
    double p2y = portals[1].y;

    yvel += grav;
    shape.move(xvel,yvel);
    shape.setFillColor(Color(171, 39, 242, 255));

    bool p1inx = inRange(p1x, p1x + 250, sposx);
    bool p1iny = inRange(p1y, p1y + 200, sposy);

    bool p2inx = inRange(p2x, p2x + 250, sposx);
    bool p2iny = inRange(p2y, p2y + 200, sposy);

    if (shape.getPosition().y <= 0 and yvel < 0 && inRange(p1x, p1x + 250, shape.getPosition().x) == false && inRange(p2x, p2x + 200, shape.getPosition().x) == false)
    {
        yvel *= -1 * fabs(25 / siz - grav);
    }
    else if (shape.getPosition().y <= 0 and yvel < 0 && (inRange(p1x, p1x + 250, shape.getPosition().x) == true || inRange(p2x, p2x + 200, shape.getPosition().x) == true))
    {   
        cout << "intersection q1" << endl;
        initPort = p2inx==true;
        
        
        teleportBall(1, portals);
    }
    

    else if (shape.getPosition().x > (500.f - (siz * 2)) and xvel > 0 && inRange(p1y, p1y + 200, shape.getPosition().y) == false && inRange(p2y, p2y + 200, shape.getPosition().y) == false)
    {

        xvel -= frict;
        xvel = -1 * fabs(xvel);
    }
    else if (shape.getPosition().x > (500.f - (siz * 2)) && xvel > 0 && (inRange(p1y, p1y + 200, shape.getPosition().y) == true || inRange(p2y, p2y + 200, shape.getPosition().y) == true))
    {
        cout << "intersection q2"  << endl;

        initPort = p2inx == true;

        teleportBall( 2, portals);
    }

    if (shape.getPosition().y >= (500.f - (siz * 2)) and fabs(yvel) > .01 && (inRange(p1x-100, p1x + 100, shape.getPosition().x) == false && inRange(p2x-100, p2x + 100, shape.getPosition().x) == false))
    {
        cout << "bounce" << endl;

        yvel *= -1 * fabs(25 / siz - grav);
        shape.move(xvel, yvel);
    }
    else if (shape.getPosition().y > (500.f - (siz * 2)) and fabs(yvel) > 0.1 && (inRange(p1x, p1x + 250, shape.getPosition().x) == true || inRange(p2x, p2x + 250, shape.getPosition().x) == true))
    {
        bool result = shape.getPosition().y > (500.f - (siz * 2));
        // cout << result << endl;
        // cout << p1x << " " << p2x << " " << endl;
        cout << "intersection q3" << endl;

        initPort = p2inx == true;

        teleportBall( 3, portals);
    }

    else if (shape.getPosition().x < 0 && xvel < 0 && inRange(p1y, p1y + 200, shape.getPosition().y) == false && inRange(p2y, p2y + 200, shape.getPosition().y) == false)
    {
        xvel += frict;
        xvel = fabs(xvel);
    }
    else if (shape.getPosition().x < 0 && xvel < 0 && (inRange(p1y, p1y + 200, shape.getPosition().y) == true || inRange(p2y, p2y + 200, shape.getPosition().y) == true))
    {
        cout << "intersection q4" << endl;

        initPort = p2inx == true;

        teleportBall( 4, portals);
    }

    //FOR PORTAL 2
    



    if(Mouse::isButtonPressed(Mouse::Button::Left)){

        yvel -= grav;
        drag();     
        dragging = true; 
    }else if(dragging == true and Mouse::isButtonPressed(Mouse::Button::Left)==false and shape.getPosition().y > 0){
        dothrow();
    }
    // cout << xvel << endl << yvel << endl;

    shape.move(xvel, yvel);
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
    int numPorts = 0;
    
    bool portalsExist;
    bool Apressed = false;
    bool Spressed = false;
    double height = 450.f;
    
    Portal portals[2];
    // cout << portals[0].x << endl;
    sf::RectangleShape rects[3];
    q1port.setPosition(700, 700);
    q2port.setPosition(700, 700);
    q3port.setPosition(700, 700);
    q4port.setPosition(700, 700);

    // rects[0] = q1port; rects[1] = q2port; rects[2] = q3port; rects[4] = q4port;
    cout << "ran" << endl;


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

                Portal p1;
                portals[0] = p1;

                // cout << p.toString() << endl;
                q1port.setFillColor(sf::Color(0, 0, 255));
                q2port.setFillColor(sf::Color(0, 0, 255));
                q3port.setFillColor(sf::Color(0, 0, 255));
                q4port.setFillColor(sf::Color(0, 0, 255));

                portals[0].drawPortal();

                numPorts += 1;
            }
            else if (Keyboard::isKeyPressed(Keyboard::S) && Spressed == false)
            {
                AKeyPress();
                Spressed = true;
                Portal p2;
 
                portals[1] = p2;
                q2port.setFillColor(sf::Color(204, 102, 0));
                q1port.setFillColor(sf::Color(204, 102, 0));
                q3port.setFillColor(sf::Color(204, 102, 0));
                q4port.setFillColor(sf::Color(204, 102, 0));

                portals[1].drawPortal();
                // window::draw
                numPorts +=1;
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
                numPorts = 0;
        
            }
            // cout << shape.getPosition().y  << ", " << shape.getPosition().x << endl;
            portalsExist = numPorts==2;

            // cout << "portal exists? " << portalsExist << endl;
            // checkIntersect(rects, shape);
            if(portalsExist == false){
                movement();
            }
            else{
                portMove(portals);
            }
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
