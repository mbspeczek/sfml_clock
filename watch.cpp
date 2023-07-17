
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
            std::vector<std::pair<float, float>> centers;
            float rad = 3.14159265/180;
            
            // calculate dots
            for(unsigned int angle = 0; angle < 360; angle += 6){
                centers.push_back({center.first+radius*sin(angle*rad), center.second-radius*cos(angle*rad)});
            }
            
            for(unsigned int i = 0; i < centers.size(); i++){
                dots.at(i).setPosition(centers.at(i).first-5, centers.at(i).second-5);
                if(i%5 == 0){
                    dots.at(i).setFillColor(sf::Color::Red);    /*hour dot*/
                }
                else{
                    dots.at(i).setFillColor(sf::Color::White); /* minute dot*/ 
                } 
                dots.at(i).setRadius(5);
            }
            
            auto tmpHour = getDotPosition(0);
            // clock center + 5 -small dots radius
            hourArrow = {sf::Vertex(sf::Vector2f(center.first+5, center.second+5)), sf::Vertex(sf::Vector2f(tmpHour.first,tmpHour.second))};
            minArrow = {sf::Vertex(sf::Vector2f(center.first+5, center.second+5)), sf::Vertex(sf::Vector2f(tmpHour.first,tmpHour.second))};
        }
    
    
    // set watch at s - secounds, m - minures, h - hours
    void setWatch(int s, int m, int h){
        watchTime = {s, m, h};
        setArrowToPos(hourArrow, watchTime.at(2));
        setArrowToPos(minArrow, watchTime.at(1));          
    }

    void draw(){
        for(auto& d : dots){
            renderContext.draw(d);
        }
        renderContext.draw(hourArrow.data(), 2, sf::Lines);
        renderContext.draw(minArrow.data(), 2, sf::Lines);
    }

    void pushClock(){
        watchTime.at(0)++;
        if(watchTime.at(0) >= 60){
            watchTime.at(0) = 0;
            watchTime.at(1)++;
        }
        if(watchTime.at(1) >= 60){
            watchTime.at(1) = 0;
            watchTime.at(2)++;    
        }
        if(watchTime.at(2) >= 12){
            watchTime.at(2) = 0;
            watchTime.at(1) = 0;
            watchTime.at(0) = 0;
        }
        setArrowToPos(hourArrow,watchTime.at(2)*5);
        setArrowToPos(minArrow,watchTime.at(1));
    }

private:

    // dot position 0-59
    std::pair<float,float> getDotPosition(int second){
        auto Position = dots.at(second).getPosition();
        return std::pair(Position.x+5, Position.y+5);
    }
    // position 0-59
    void setArrowToPos(std::array<sf::Vertex, 2>& arrow, int hour){
        auto newPosition = getDotPosition(hour);
        arrow.at(1).position.x = newPosition.first;
        arrow.at(1).position.y = newPosition.second;
    }


    // render context
    sf::RenderWindow &renderContext;
    // clock center
    std::pair<float, float> clockCenterPos;
    float radius;
    // dots per hours
    std::array<sf::CircleShape, 60> dots;
    // arrows
    std::array<sf::Vertex, 2> hourArrow;
    std::array<sf::Vertex, 2> minArrow;
    std::array<int, 3> watchTime; // seconds, minutes, hours
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML Clock");

    window.setFramerateLimit(120);

    Clock cf{std::pair(250, 250), 200, window};
    //cf.setWatch(0,0,0);

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

        std::this_thread::sleep_for(std::chrono::nanoseconds(10));
        cf.pushClock();
                
        // draw
        window.clear();
       
        cf.draw();

        window.display();
    }

    return 0;
}