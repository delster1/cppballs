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

double size = 25.f;
CircleShape shape(size);
RectangleShape yport(Vector2f(100,10));
RectangleShape xport(Vector2f(10,100));
double vel;
double xvel = 0.f;
double yvel = 0.f;
double grav = 0.3f;
double frict = 3.4f;
bool dragging = false;
vector<Vector2i> poss;
void drag(){
    // dragging = true;
    double x = Mouse::getPosition(window).x - size;
    double y = Mouse::getPosition(window).y - size;
    yvel = 0;
    xvel = 0;
    // cout << "angle: " << angle;
    // cout<< x << ", " <<y << "\n";
    double sposx = shape.getPosition().x;
    double sposy = shape.getPosition().y - grav;

    shape.setPosition(x,y);
    // cout << "\n mouse pos: " << y << " \n obj pos: " << sposy << "\n";
    double dx = x - sposx;    
    double dy = y - sposy;



    // cout<< "y " << shape.getPosition().y;
    
    // cout << "\n dx: " << dx << "\n dy: " << dy << "\n";
    // cout <<  "distance: \n" << distance << "\n";
    // cout << "angle: " << angle << "\n"; 

    // shape.move(dx,dy);
    // xvel = dx;
    // yvel = dy;
    // cout << poss.size() << "\n";
    
}

void dothrow(){
    double x = Mouse::getPosition(window).x - size;
    double y = Mouse::getPosition(window).y - size;

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
        yvel *= -1 * fabs(25/size-grav);
    }
    if(shape.getPosition().y>=(500.f-(size*2)) and fabs(yvel) > .01){
        // yvel -= grav;
        yvel*= -1 * fabs(25/size - grav);
        // cout << shape.getPosition().y << "\n";
        // cout<<"balls";
        shape.move(xvel,yvel);
        // yvel = 0;

    }
    else if (fabs(yvel) < .01){
        cout << "stopped " << endl;
 
        yvel = 0;
        shape.move(xvel,yvel);  
    }
    // bounce off bottom
    
    // bounce off right
    else if(shape.getPosition().x > (500.f-(size*2)) and xvel > 0){
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
        // cout << dragging; 
    }else if(dragging == true and Mouse::isButtonPressed(Mouse::Button::Left)==false and shape.getPosition().y > 0){
        // cout << "thrown \n";
        dothrow();
    }
    // cout << "yvel: " << yvel << "\n";


    
    // cout << "Xvel: " << xvel << "\n";
    shape.move(xvel,yvel);
    // else 

    
    
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
            
        string toString(){
            string str = to_string(x) + " " + to_string(y);
            return str;
        }
    private:
  
        double size;
        
        
        //USE MOUSE POSITION TO DETERMINE WALL AND POSITION ON WALL

};





location getQuad(){
    
    location locale;
    // struct point locale;
    double x = Mouse::getPosition(window).x;
    double y = Mouse::getPosition(window).y;
    double ogX = 250;
    double ogY = 250;
    double slope = (y - ogY)/(x-ogX);
    double ypos;
    double xpos;
    string quad = "";
    double m = 1;
    // cout << slope << endl;
    // cout << "(" << x << ", " << y << ")" << endl;
    double formx = Mouse::getPosition(window).x -250;
    double formy =( Mouse::getPosition(window).y -250) * -1;
    double formm = (0-formy)/(0-formx);
    if(fabs(slope) > m & y<250){
        ypos = 500+ ((250/formm)-250 );
        quad = "q1";
        locale.x = ypos;
        locale.y = 0;
        // cout << quad << ", " <<  formm <<"  " << formx  <<"  " << formm * formx << endl;
        // rectangleShape.draw(locale.x,locale.y);
    }else if(fabs(slope) > m & y>250){
        ypos = 500 - (250/formm)-250;
        quad = "q3";
        locale.x = ypos;
        locale.y = 500;
    }
    else if(slope < m & x>250){
        xpos = slope*(x-ogX) + 250;
        quad = "q2";
    }
    else if(slope < m & x<250){
        xpos = slope*(x-ogX) + 250;
        quad = "q4";
        locale.x = ypos;
        locale.y = 0;
    }
    cout << "quadrant: " << quad << "    ypos: " << ypos <<endl;
    return locale;
}


void AKeyPress(){
    
    double x = Mouse::getPosition(window).x;
    double y = Mouse::getPosition(window).y;
    // location pt = getQuad(x,y);
    // cout << pt.x << ", " << pt.y << endl;
}

void drawPortal(location loc) {

} 

int main()
{
    bool Apressed = false;
    bool Spressed = false;
    double height = 450.f;
    
    Portal portals[2];

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
            
            
            movement();
            window.draw(shape);
            
            window.display();
            window.clear(); 
        }
        
        if(Keyboard::isKeyPressed(Keyboard::A) && Apressed==false){
            AKeyPress();
            Apressed = true;
     
            location points = getQuad();
            Portal p;
            p.x = points.x; p.y = points.y;
            portals[0] = p;
            
            cout << points.y << endl;
        }
        else if(Keyboard::isKeyPressed(Keyboard::S) && Spressed==false){
            AKeyPress();
            Spressed = true;
            location points = getQuad();
            Portal p;
            p.x = points.x; p.y = points.y;
            portals[1]=p;
            cout << p.toString() << endl;

            // window::draw
        }else if(Keyboard::isKeyPressed(Keyboard::Space)){
            Spressed = false;
            Apressed = false;

        }
        //error because doesnt know how to return position
        
       
    }

    return 0;
}

