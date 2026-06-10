#include <SFML/Graphics.hpp>
#include <iomanip>
#include <sstream>
#include <string>

// ==========================================
// 1. User-Made Counter Component Class
// ==========================================
class Counter {
private:
    int m_count;
    const int m_maxLimit = 99999;

public:
    // Explicit Default Constructor
    Counter() : m_count(0) {}

    void increment() {
        if (m_count >= m_maxLimit) {
            m_count = 0; // Reset loop like original Poketch
        } else {
            m_count++;
        }
    }

    void reset() {
        m_count = 0;
    }

    // Constructor/Helper logic to get the formatted string
    std::string getFormattedValue() const {
        std::stringstream ss;
        if (m_count <= 999) {
            // Maintains retro 3-digit grid alignment (e.g., "005")
            ss << std::setw(3) << std::setfill('0') << m_count;
        } else {
            ss << m_count;
        }
        return ss.str();
    }
};

// ==========================================
// 2. User-Made UI Button Component Class
// ==========================================
class PoketchButton {
private:
    sf::RectangleShape m_shape;
    sf::Text m_text;

public:
    // Custom Parameterized Constructor for Button creation
    PoketchButton(sf::Vector2f position, sf::Vector2f size, const std::string& label, 
                  sf::Color buttonColor, sf::Color textColor, const sf::Font& font, unsigned int fontSize) {
        
        m_shape.setPosition(position);
        m_shape.setSize(size);
        m_shape.setFillColor(buttonColor);
        m_shape.setOutlineThickness(3.f);
        m_shape.setOutlineColor(sf::Color::Black);

        m_text.setFont(font);
        m_text.setString(label);
        m_text.setCharacterSize(fontSize);
        m_text.setFillColor(textColor);
        m_text.setStyle(sf::Text::Bold);
        
        // Center text geometry inside button
        sf::FloatRect textRect = m_text.getLocalBounds();
        m_text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
        m_text.setPosition(position.x + size.x / 2.0f, position.y + size.y / 2.0f);
    }

    bool isClicked(sf::Vector2i mousePos) const {
        return m_shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }

    void draw(sf::RenderWindow& window) {
        window.draw(m_shape);
        window.draw(m_text);
    }
};

// ==========================================
// 3. User-Made Grid LCD Screen Component Class
// ==========================================
class PoketchScreen {
private:
    sf::RectangleShape m_background;
    sf::Color m_gridColor;
    float m_width;
    float m_height;
    float m_posX;
    float m_posY;

public:
    // Custom Parameterized Constructor for the Retro Screen Screen
    PoketchScreen(sf::Vector2f position, sf::Vector2f size, sf::Color bgColor, sf::Color gridColor)
        : m_gridColor(gridColor), m_width(size.x), m_height(size.y), m_posX(position.x), m_posY(position.y) {
        
        m_background.setPosition(position);
        m_background.setSize(size);
        m_background.setFillColor(bgColor);
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(m_background);

        // Draw the custom sub-pixel matrix LCD line grid
        const int gridSpacing = 4;
        
        // Vertical lines
        for (float x = m_posX; x < m_posX + m_width; x += gridSpacing) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(x, m_posY), m_gridColor),
                sf::Vertex(sf::Vector2f(x, m_posY + m_height), m_gridColor)
            };
            window.draw(line, 2, sf::Lines);
        }
        // Horizontal lines
        for (float y = m_posY; y < m_posY + m_height; y += gridSpacing) {
            sf::Vertex line[] = {
                sf::Vertex(sf::Vector2f(m_posX, y), m_gridColor),
                sf::Vertex(sf::Vector2f(m_posX + m_width, y), m_gridColor)
            };
            window.draw(line, 2, sf::Lines);
        }
    }
};

// ==========================================
// 4. Main App Controller Class
// ==========================================
class PoketchApp {
private:
    sf::RenderWindow m_window;
    sf::Font m_font;
    sf::Text m_displayText;
    sf::Text m_footerLabel;

    // Component instances managed via pointer initialization in Constructor
    PoketchScreen* m_screen;
    PoketchButton* m_plusButton;
    PoketchButton* m_clearButton;
    Counter m_counter; // Uses its implicit default constructor automatically

    // Palette Engine Colors
    const sf::Color dsCasing = sf::Color(0x38, 0x38, 0x38);
    const sf::Color lcdBg    = sf::Color(0x88, 0xA0, 0x70);
    const sf::Color lcdGrid  = sf::Color(0x7F, 0x96, 0x68);
    const sf::Color lcdText  = sf::Color(0x18, 0x20, 0x10);
    const sf::Color greyBtn  = sf::Color(0x58, 0x60, 0x58);
    const sf::Color redBtn   = sf::Color(0xA8, 0x38, 0x30);

public:
    // Main Application Engine Constructor
    PoketchApp() : m_window(sf::VideoMode(320, 400), "Poketch - Counter Engine") {
        m_window.setFramerateLimit(60);

        // Load Font Assets
        if (!m_font.loadFromFile("CourierNew.ttf")) {
            // Error handling fallback if file isn't present
        }

        // Initialize user-made sub-components through constructors
        m_screen      = new PoketchScreen(sf::Vector2f(20.f, 20.f), sf::Vector2f(280.f, 200.f), lcdBg, lcdGrid);
        m_plusButton  = new PoketchButton(sf::Vector2f(40.f, 250.f), sf::Vector2f(110.f, 90.f), "+", greyBtn, sf::Color::White, m_font, 50);
        m_clearButton = new PoketchButton(sf::Vector2f(220.f, 275.f), sf::Vector2f(55.f, 45.f), "C", redBtn, sf::Color::White, m_font, 22);

        // Setup HUD Texts
        m_displayText.setFont(m_font);
        m_displayText.setCharacterSize(56);
        m_displayText.setFillColor(lcdText);
        m_displayText.setStyle(sf::Text::Bold);
        m_displayText.setPosition(45.f, 80.f);

        m_footerLabel.setFont(m_font);
        m_footerLabel.setString("Poketch App 05");
        m_footerLabel.setCharacterSize(11);
        m_footerLabel.setFillColor(sf::Color(0x80, 0x80, 0x80));
        m_footerLabel.setPosition(200.f, 380.f);

        updateHUD();
    }

    // Destructor to clean up dynamically allocated object constructors
    ~PoketchApp() {
        delete m_screen;
        delete m_plusButton;
        delete m_clearButton;
    }

    void updateHUD() {
        m_displayText.setString(m_counter.getFormattedValue());
    }

    void handleEvents() {
        sf::Event event;
        while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                m_window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);

                if (m_plusButton->isClicked(mousePos)) {
                    m_counter.increment();
                    updateHUD();
                }
                else if (m_clearButton->isClicked(mousePos)) {
                    m_counter.reset();
                    updateHUD();
                }
            }
        }
    }

    void render() {
        m_window.clear(dsCasing);

        // Render modular structures
        m_screen->draw(m_window);
        m_window.draw(m_displayText);
        m_plusButton->draw(m_window);
        m_clearButton->draw(m_window);
        m_window.draw(m_footerLabel);

        m_window.display();
    }

    void run() {
        while (m_window.isOpen()) {
            handleEvents();
            render();
        }
    }
};

// ==========================================
// Entry point
// ==========================================
int main() {
    PoketchApp application; // Invokes PoketchApp Constructor
    application.run();
    return 0;
}

