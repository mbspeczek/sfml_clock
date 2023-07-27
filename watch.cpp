
#include <array>
#include <vector>
#include <thread>
#include <cmath>
#include <SFML/Graphics.hpp>

class Clock
{
public:
    Clock(std::pair<float,float> center, float radius, sf::RenderWindow &context)
        : 
        renderContext{context},
        radius{radius},
        clockCenterPos{center},
        watchTime{{0, 0, 0}}
        {    
            float rad = 3.14159265/180;
            
            // calculate centers border=1radius, minute=0.8radius, hour=0.5radius
            // 60 centers each
            for(unsigned int angle = 0; angle < 360; angle += 6){
                borderCenters.push_back({center.first+radius*sin(angle*rad), center.second-radius*cos(angle*rad)});
                minuteCenters.push_back({center.first+(radius*0.8)*sin(angle*rad), center.second-(radius*0.8)*cos(angle*rad)});
                hourCenters.push_back({center.first+(radius/2)*sin(angle*rad), center.second-(radius/2)*cos(angle*rad)});
            }
            
            // border dots
            for(unsigned int i = 0; i < borderCenters.size(); i++){
                dots.at(i).setPosition(borderCenters.at(i).first-dotRadius, borderCenters.at(i).second-dotRadius);
                if(i%5 == 0){
                    dots.at(i).setFillColor(sf::Color::Red);    /*hour dot*/
                }
                else{
                    dots.at(i).setFillColor(sf::Color::White); /* minute dot*/ 
                } 
                dots.at(i).setRadius(dotRadius);
            }
            

            // clock center + dotRadius -small dots radius
            hourArrow = {   sf::Vertex(sf::Vector2f(center.first, center.second)), 
                            sf::Vertex(sf::Vector2f(hourCenters.at(0).first, hourCenters.at(0).second))};
            
            minArrow = {   sf::Vertex(sf::Vector2f(center.first, center.second)), 
                            sf::Vertex(sf::Vector2f(minuteCenters.at(0).first, minuteCenters.at(0).second))};
            
            secArrow =  {   sf::Vertex(sf::Vector2f(center.first, center.second)), 
                            sf::Vertex(sf::Vector2f(borderCenters.at(0).first, borderCenters.at(0).second))};
        }
    
    
    // set watch at s - secounds, m - minures, h - hours
    void setWatch(int s, int m, int h){
        setArrowToPosition(s, secArrow, borderCenters);
        setArrowToPosition(m, minArrow, minuteCenters);
        setArrowToPosition(h, hourArrow, hourCenters);
        watchTime = {s,m,h};
    }

    void draw(){
        for(auto& d : dots){
            renderContext.draw(d);
        }
        renderContext.draw(hourArrow.data(), 2, sf::Lines);
        renderContext.draw(minArrow.data(), 2, sf::Lines);
        renderContext.draw(secArrow.data(), 2, sf::Lines);
    }

    void pushClock(){
       
    }

private:

    void setArrowToPosition(int position, std::array<sf::Vertex, 2>& arrow, std::vector<std::pair<float, float>>& arrowCenters){
        auto _positon = arrowCenters.at(position);
        arrow.at(1).position.x = _positon.first;
        arrow.at(1).position.y = _positon.second;        
    }
 
    // render context
    sf::RenderWindow &renderContext;
    // clock center
    std::pair<float, float> clockCenterPos;

    float radius;
    const int dotRadius = 5;
    // dots per hours
    std::array<sf::CircleShape, 60> dots;
    std::vector<std::pair<float, float>> borderCenters;
    std::vector<std::pair<float, float>> minuteCenters;
    std::vector<std::pair<float, float>> hourCenters;
    // arrows
    std::array<sf::Vertex, 2> hourArrow;
    std::array<sf::Vertex, 2> minArrow;
    std::array<sf::Vertex, 2> secArrow;

    std::array<int, 3> watchTime; // seconds, minutes, hours
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Clock");

    window.setFramerateLimit(240);

    Clock cf{std::pair(250, 250), 200, window};
    cf.setWatch(0,0,0);

    while(window.isOpen())
    {
        // events
        sf::Event event;
        while(window.pollEvent(event))
        {   
            // close clock wich keypress event
            if(event.type == sf::Event::KeyPressed){
                window.close();                
            }
            if(event.type == sf::Event::Closed)
                window.close();
        }

        // logic

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        cf.pushClock();
                
        // draw
        window.clear();
       
        cf.draw();

        window.display();
    }

    return 0;
}