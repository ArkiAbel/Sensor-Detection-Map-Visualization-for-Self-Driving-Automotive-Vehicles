

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <math.h>

#define M_PI 3.14159265358979323846

//window settings

const float size_x = 1000;
const float size_y = 1000;
const float res = 200;

using precision = float;

class Robot {
public:

    int timestamp;
    std::pair<double, double> pos;
    float heading;
    float ultrasound0;
    float ultrasound1;
    float ultrasound2;
    float ultrasound3;

    //robot start settings

    Robot(int ts = 0, double px = 0, double py = 0, float h = 0, float u0 = 0, float u1 = 0, float u2 = 0, float u3 = 0)
        : timestamp(ts), pos(px, py), heading(h), ultrasound0(u0), ultrasound1(u1), ultrasound2(u2), ultrasound3(u3) {
    }
};

//READ
std::vector<Robot> ReadData(const std::string& filename) {

    std::vector<Robot> data;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "File cannot be opened!" << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line); //header

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string token;
        std::vector<std::string> tokens;
        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() >= 8) {
            int _timestamp = std::stoi(tokens[0]);
            double _pos_x = std::stod(tokens[1]);
            double _pos_y = std::stod(tokens[2]);
            float _heading = std::stod(tokens[3]);
            float _ultrasound0 = std::stod(tokens[4]);
            float _ultrasound1 = std::stod(tokens[5]);
            float _ultrasound2 = std::stod(tokens[6]);
            float _ultrasound3 = std::stod(tokens[7]);
            data.emplace_back(_timestamp, _pos_x, _pos_y, _heading, _ultrasound0, _ultrasound1, _ultrasound2, _ultrasound3);
        }
    }

    file.close();
    return data;
}

//UPDATESENSORS
void updateSensors(const Robot& robot, float scale, float sound, float x, float y, float multi,
    std::pair<float, float>& sen0, std::pair<float, float>& us0,
    std::pair<float, float>& sen1, std::pair<float, float>& us1,
    std::pair<float, float>& sen2, std::pair<float, float>& us2,
    std::pair<float, float>& sen3, std::pair<float, float>& us3) {

    //scaling parameters

    float theta = robot.heading; //+ M_PI?
    float X = robot.pos.first * scale;
    float Y = robot.pos.second * scale;
    float echo = sound / 2 / sqrt(2) * multi; //one-way distance & trigonomerty

    //US0 (+,+)

    sen0 = { Y + (x * sin(theta) + (y * cos(theta))), X + (x * cos(theta) - (y * sin(theta))) };
    us0 = { Y + (x + (robot.ultrasound0 * echo)) * sin(theta) + ((y + (robot.ultrasound0 * echo)) * cos(theta)), 
            X + (x + (robot.ultrasound0 * echo)) * cos(theta) - ((y + (robot.ultrasound0 * echo)) * sin(theta)) };

    //US1 (-,+)

    sen1 = { Y + (-x * sin(theta) + (y * cos(theta))), X + (-x * cos(theta) - (y * sin(theta))) };
    us1 = { Y + (-(x + robot.ultrasound1 * echo) * sin(theta) + ((y + robot.ultrasound1 * echo) * cos(theta))), 
            X + (-(x + robot.ultrasound1 * echo) * cos(theta) - ((y + robot.ultrasound1 * echo) * sin(theta))) };

    //US2 (-,-)

    sen2 = { Y + (-x * sin(theta) + (-y * cos(theta))), X + (-x * cos(theta) - (-y * sin(theta))) };
    us2 = { Y + (-(x + robot.ultrasound2 * echo) * sin(theta) + (-(y + robot.ultrasound2 * echo) * cos(theta))), 
            X + (-(x + robot.ultrasound2 * echo) * cos(theta) - (-(y + robot.ultrasound2 * echo) * sin(theta))) };

    //US3 (+,-)

    sen3 = { Y + (x * sin(theta) + (-y * cos(theta))), X + (x * cos(theta) - (-y * sin(theta))) };
    us3 = { Y + ((x + robot.ultrasound3 * echo) * sin(theta) + (-(y + robot.ultrasound3 * echo) * cos(theta))), 
            X + ((x + robot.ultrasound3 * echo) * cos(theta) - (-(y + robot.ultrasound3 * echo) * sin(theta))) };
}

//RENDERFRAME
void renderFrame(sf::RenderWindow& window, const std::vector<std::vector<int>>& grid, int gridRows, int gridCols,
    const std::pair<float, float>& sen0, const std::pair<float, float>& us0, 
    const std::pair<float, float>& sen1, const std::pair<float, float>& us1, 
    const std::pair<float, float>& sen2, const std::pair<float, float>& us2, 
    const std::pair<float, float>& sen3, const std::pair<float, float>& us3, 
    float dotsize, float dotsize2, float scale, float multi) {

    for (int j = 0; j < gridRows; j++) {
        for (int k = 0; k < gridCols; k++) {

            int count = grid[j][k];
            if (count == 0) continue;

            sf::RectangleShape cell(sf::Vector2f(size_x / res, size_y / res));
            int intensity = (count >= 25) ? 255 : count * 20;

            cell.setFillColor(sf::Color(intensity, intensity, intensity));
            cell.setPosition(j * (size_x / res), k * (size_y / res));
            cell.setOutlineThickness(0.f);

            window.draw(cell);
        }
    }

    //draw lines section

    std::pair<std::pair<float, float>, std::pair<float, float>> lines[] = {
        {sen0, us0}, {sen1, us1}, {sen2, us2}, {sen3, us3}
    };

    for (const auto& line : lines) {
        sf::VertexArray ray(sf::Lines, 2);
        ray[0].position = sf::Vector2f(line.first.second, line.first.first);
        ray[0].color = sf::Color(50, 50, 50);
        ray[1].position = sf::Vector2f(line.second.second, line.second.first);
        ray[1].color = sf::Color(255, 255, 255);
        window.draw(ray);
    }

    //draw dots section

    std::pair<std::pair<float, float>, sf::Color> dots[] = {
        {sen0, sf::Color::Red}, {us0, sf::Color::Red},
        {sen1, sf::Color::Green}, {us1, sf::Color::Green},
        {sen2, sf::Color::Magenta}, {us2, sf::Color::Magenta},
        {sen3, sf::Color::Yellow}, {us3, sf::Color::Yellow}
    };

    for (const auto& dot : dots) {
        sf::CircleShape circle(dotsize2);
        circle.setFillColor(dot.second);
        circle.setPosition(dot.first.second, dot.first.first);
        window.draw(circle);
    }
}

//RENDERLEGEND
void renderLegend(sf::RenderWindow& window, sf::Font& font, bool fontLoaded) {

    if (!fontLoaded) return;

    //legend variables

    const float squareSize = 20.f;
    const float margin = 10.f;

    const float legendX = window.getSize().x - squareSize - margin;
    const float legendYStart = window.getSize().y - (4 * (squareSize + margin));

    std::pair<sf::Color, std::string> legendItems[] = {
        {sf::Color::Red, "US0"},
        {sf::Color::Green, "US1"},
        {sf::Color::Magenta, "US2"},
        {sf::Color::Yellow, "US3"}
    };

    //legend list

    for (int i = 0; i < 4; i++) {

        sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
        square.setFillColor(legendItems[i].first);
        square.setPosition(legendX, legendYStart + i * (squareSize + margin));
        window.draw(square);

        sf::Text text(legendItems[i].second, font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setPosition(legendX - squareSize - margin, legendYStart + i * (squareSize + margin));
        window.draw(text);
    }
}

int main() {
    std::vector<Robot> _robot = ReadData("robot.csv");

    //ratios

    float scale = 80 * 2.f;
    float multi = 40 * 1.f; //only for scaling tests

    float sound = 343.f; //sound (m/s)
    float x = 0.20 / 2 * multi; //width (m)
    float y = 0.10 / 2 * multi; //length (m)

    //grid variables

    int gridRows = int(res);
    int gridCols = int(res);
    std::vector<std::vector<int>> _grid(gridRows, std::vector<int>(gridCols, 0));

    //dot variables

    float dotsize = 1.f;
    float dotsize2 = 3.f;

    sf::RenderWindow window(sf::VideoMode(size_x, size_y), "Occupancy Grid");

    sf::View view;
    view.setSize(size_x, -size_y);
    window.setView(view);

    //font

    sf::Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf");
    if (!fontLoaded) {
        std::cerr << "Font loading failed! Continuing without legend." << std::endl;
    }

    for (int i = 0; i < _robot.size() && window.isOpen(); i++) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear(sf::Color::Black);

        int frame = i; //i

        std::pair<float, float> sen0, us0, sen1, us1, sen2, us2, sen3, us3;

        updateSensors(_robot[frame], scale, sound, x, y, multi, sen0, us0, sen1, us1, sen2, us2, sen3, us3);

        //safe conversions

        int cell0X = static_cast<int>(us0.second / (size_x / res));
        int cell0Y = static_cast<int>(us0.first / (size_y / res));
        int cell1X = static_cast<int>(us1.second / (size_x / res));
        int cell1Y = static_cast<int>(us1.first / (size_y / res));
        int cell2X = static_cast<int>(us2.second / (size_x / res));
        int cell2Y = static_cast<int>(us2.first / (size_y / res));
        int cell3X = static_cast<int>(us3.second / (size_x / res));
        int cell3Y = static_cast<int>(us3.first / (size_y / res));

        if (cell0X >= 0 && cell0X < gridCols && cell0Y >= 0 && cell0Y < gridRows &&
            cell1X >= 0 && cell1X < gridCols && cell1Y >= 0 && cell1Y < gridRows &&
            cell2X >= 0 && cell2X < gridCols && cell2Y >= 0 && cell2Y < gridRows &&
            cell3X >= 0 && cell3X < gridCols && cell3Y >= 0 && cell3Y < gridRows) {
            _grid[cell0X][cell0Y]++;
            _grid[cell1X][cell1Y]++;
            _grid[cell2X][cell2Y]++;
            _grid[cell3X][cell3Y]++;
        }

        //draw grid

        renderFrame(window, _grid, gridRows, gridCols, sen0, us0, sen1, us1, sen2, us2, sen3, us3, dotsize, dotsize2, scale, multi);

        //draw legend

        renderLegend(window, font, fontLoaded);

        //PATH

        if (i % 10 == 0) {
            sf::CircleShape path(dotsize2);
            path.setFillColor(sf::Color(200, 200, 200));
            path.setPosition(_robot[i].pos.first * scale, _robot[i].pos.second * scale);
            window.draw(path);
        }

        sf::sleep(sf::milliseconds(5));
        if (!window.isOpen()) break;

        window.display();
    }

    //displaying only measured map

    bool animationDone = true;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear(sf::Color::Black);

        //turned off state

        renderFrame(window, _grid, gridRows, gridCols, { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }, { 0,0 }, dotsize, dotsize2, scale, multi);
        renderLegend(window, font, fontLoaded);

        window.display();
    }

    return 0;
}
