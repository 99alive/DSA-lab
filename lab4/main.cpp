// main.cpp
// Lab 4 - Memory Matching Card Game using an Unsorted Linked List
//
// The cards are kept in a singly linked list. The game treats the list
// like a 4x4 grid: position 0 is (0,0), position 15 is (3,3), so
// index = row * cols + col.
//
// There are three ways to play:
//   terminal - plain cout, type row and col           (Part 1)
//   ncurses  - arrow keys and space in the terminal    (Part 2)
//   sfml     - a window, click the cards               (Part 1 and 2)
//
// Build (Linux):
//   g++ main.cpp -o game -lncurses -lsfml-graphics -lsfml-window -lsfml-system
// Or use the CMakeLists.txt in this folder.

#include <iostream>
#include <vector>
#include <algorithm>   // for shuffle
#include <random>      // for random_device
#include <string>
#include <ncurses.h>
#include <SFML/Graphics.hpp>

// ---------- Step 1: the card ----------

struct Card {
    char value;      // e.g., 'A', 'B', etc.
    bool isFaceUp;   // True if flipped up
    bool isMatched;  // True if matched and removed from play

    Card(char val) : value(val), isFaceUp(false), isMatched(false) {}
};

// ---------- Step 2: the unsorted linked list ----------

struct Node {
    Card data;
    Node* next;
    Node(Card c) : data(c), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;
    int length;

public:
    LinkedList() : head(nullptr), length(0) {}

    ~LinkedList() {
        Node* current = head;
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    // add to the end of the list
    void insert(Card card) {
        Node* newNode = new Node(card);
        if (!head) {
            head = newNode;
        } else {
            Node* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        length++;
    }

    // walk from the head to the node at index and return its card
    Card& getAt(int index) {
        if (index < 0 || index >= length) throw std::out_of_range("Invalid index");
        Node* temp = head;
        for (int i = 0; i < index; i++) temp = temp->next;
        return temp->data;
    }

    void removeAt(int index) {
        if (index < 0 || index >= length) return;
        if (index == 0) {
            Node* temp = head;
            head = head->next;
            delete temp;
        } else {
            Node* prev = head;
            for (int i = 0; i < index - 1; i++) prev = prev->next;
            Node* temp = prev->next;
            prev->next = temp->next;
            delete temp;
        }
        length--;
    }

    int size() const { return length; }

    // just for debugging - prints every card value in order
    void traverse() {
        Node* temp = head;
        while (temp) {
            std::cout << temp->data.value << " ";
            temp = temp->next;
        }
        std::cout << "\n";
    }
};

// ---------- Step 3: the game ----------

class MemoryGame {
private:
    LinkedList cards;
    int rows, cols;
    int firstFlipIndex = -1, secondFlipIndex = -1;

    void initializeDeck() {
        char values[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
        for (char val : values) {
            cards.insert(Card(val));
            cards.insert(Card(val));
        }
    }

    // you cannot shuffle a linked list directly, so copy the cards into a
    // vector, shuffle that, then empty the list and put them back in
    void shuffle() {
        std::vector<Card> temp;
        for (int i = 0; i < cards.size(); i++) {
            temp.push_back(cards.getAt(i));
        }
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(temp.begin(), temp.end(), g);
        // Clear list (manually, since no clear method)
        while (cards.size() > 0) cards.removeAt(0);
        for (const auto& c : temp) cards.insert(c);
    }

public:
    MemoryGame(int r = 4, int c = 4) : rows(r), cols(c) {
        initializeDeck();
        shuffle();
    }

    // flips the card at (row, col). Returns true when this was the
    // second card of a pair, meaning it is time to call checkMatch()
    bool flipCard(int row, int col) {
        int index = row * cols + col;
        if (row < 0 || row >= rows || col < 0 || col >= cols) return false;
        if (cards.getAt(index).isFaceUp || cards.getAt(index).isMatched) return false;
        cards.getAt(index).isFaceUp = true;

        if (firstFlipIndex == -1) {
            firstFlipIndex = index;
        } else {
            secondFlipIndex = index;
            return true;   // Pair flipped, check match next
        }
        return false;
    }

    // compares the two flipped cards. Returns true if they matched.
    bool checkMatch() {
        if (firstFlipIndex == -1 || secondFlipIndex == -1) return false;
        Card& c1 = cards.getAt(firstFlipIndex);
        Card& c2 = cards.getAt(secondFlipIndex);
        bool matched = (c1.value == c2.value);
        if (matched) {
            c1.isMatched = true;
            c2.isMatched = true;
            // we mark matched cards instead of removing them from the
            // list, so the grid positions stay the same
        } else {
            c1.isFaceUp = false;
            c2.isFaceUp = false;
        }
        firstFlipIndex = secondFlipIndex = -1;
        return matched;
    }

    bool allMatched() {
        for (int i = 0; i < cards.size(); i++) {
            if (!cards.getAt(i).isMatched) return false;
        }
        return true;
    }

    // ---------- Step 4.1 (Part 1): plain terminal display ----------

    void displayTerminal() {
        std::cout << "Memory Game Grid:\n";
        std::cout << "    ";
        for (int c = 0; c < cols; c++) std::cout << " " << c << "  ";
        std::cout << "\n";
        for (int r = 0; r < rows; r++) {
            std::cout << " " << r << "  ";
            for (int c = 0; c < cols; c++) {
                int idx = r * cols + c;
                Card& card = cards.getAt(idx);
                if (card.isMatched) {
                    std::cout << "[ ] ";
                } else if (card.isFaceUp) {
                    std::cout << "[" << card.value << "] ";
                } else {
                    std::cout << "[*] ";
                }
            }
            std::cout << "\n";
        }
    }

    // the game loop for the plain terminal version
    void playTerminal() {
        int turns = 0;
        while (!allMatched()) {
            displayTerminal();
            int row, col;
            std::cout << "Enter row col: ";
            if (!(std::cin >> row >> col)) {
                std::cout << "\nBye.\n";
                return;
            }
            if (flipCard(row, col)) {
                // show the grid with both cards face up BEFORE checking,
                // otherwise a wrong pair flips back before you ever see it
                displayTerminal();
                turns++;
                if (checkMatch())
                    std::cout << "Match!\n\n";
                else
                    std::cout << "No match, try again.\n\n";
            }
        }
        displayTerminal();
        std::cout << "You win! It took you " << turns << " turns.\n";
    }

    // ---------- Step 4.1 (Part 2): ncurses terminal display ----------

    void drawGridNcurses(int cursorRow, int cursorCol) {
        clear();
        for (int r = 0; r < rows; r++) {
            for (int c = 0; c < cols; c++) {
                int idx = r * cols + c;
                Card& card = cards.getAt(idx);
                int x = c * 4;   // Spacing: 4 chars per card
                int y = r * 2;   // 2 lines per row for card visuals

                // Highlight if cursor is here
                if (r == cursorRow && c == cursorCol) {
                    attron(A_BOLD | A_REVERSE);
                }

                if (card.isMatched) {
                    mvprintw(y, x, "[ ]");
                } else if (card.isFaceUp) {
                    if (has_colors()) attron(COLOR_PAIR(2));
                    mvprintw(y, x, "[%c]", card.value);
                    if (has_colors()) attroff(COLOR_PAIR(2));
                } else {
                    if (has_colors()) attron(COLOR_PAIR(1));
                    mvprintw(y, x, "[*]");
                    if (has_colors()) attroff(COLOR_PAIR(1));
                }

                attroff(A_BOLD | A_REVERSE);
            }
        }
        mvprintw(rows * 2 + 1, 0, "Use arrows to move, SPACE to flip. Q to quit.");
        refresh();
    }

    void displayTerminalNcurses() {
        initscr();              // Start ncurses mode
        noecho();               // Don't echo input
        keypad(stdscr, TRUE);   // Enable arrow keys
        curs_set(0);            // Hide cursor
        if (has_colors()) {     // Optional: Colors for better visuals
            start_color();
            init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Hidden cards: White on blue
            init_pair(2, COLOR_BLACK, COLOR_WHITE);  // Face-up: Black on white
            init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Matched: Green text
        }

        int cursorRow = 0, cursorCol = 0;   // Track selected position

        while (!allMatched()) {
            drawGridNcurses(cursorRow, cursorCol);

            // Handle input
            int ch = getch();
            switch (ch) {
                case KEY_UP:    if (cursorRow > 0) cursorRow--; break;
                case KEY_DOWN:  if (cursorRow < rows - 1) cursorRow++; break;
                case KEY_LEFT:  if (cursorCol > 0) cursorCol--; break;
                case KEY_RIGHT: if (cursorCol < cols - 1) cursorCol++; break;
                case ' ':       // Space to flip
                case '\n':      // Or enter
                    if (flipCard(cursorRow, cursorCol)) {
                        // redraw first so the second card is visible,
                        // then wait a second, then check the match
                        drawGridNcurses(cursorRow, cursorCol);
                        napms(1000);
                        checkMatch();
                    }
                    break;
                case 'q':
                case 'Q': endwin(); return;
            }
        }

        // Win message
        clear();
        mvprintw(rows, cols * 2 - 5, "You Win!");
        mvprintw(rows + 2, 0, "Press any key to exit.");
        refresh();
        getch();   // Wait for key press
        endwin();  // End ncurses mode
    }

    // ---------- Step 4.2: SFML window ----------

    void displaySFML() {
        sf::RenderWindow window(sf::VideoMode(400, 400), "Memory Game");
        window.setFramerateLimit(30);   // no need to redraw thousands of times a second
        sf::Font font;
        bool haveFont = font.loadFromFile("arial.ttf");
        if (!haveFont) {
            std::cout << "Could not load arial.ttf - cards will be drawn without letters.\n";
        }

        bool needCheck = false;   // true after the second card is flipped

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) window.close();
                if (event.type == sf::Event::MouseButtonPressed && !needCheck) {
                    int x = event.mouseButton.x / 100;   // Assuming 100px per card
                    int y = event.mouseButton.y / 100;
                    if (flipCard(y, x)) needCheck = true;   // y=row, x=col
                }
            }

            window.clear();
            for (int r = 0; r < rows; r++) {
                for (int c = 0; c < cols; c++) {
                    int idx = r * cols + c;
                    Card& card = cards.getAt(idx);

                    sf::RectangleShape rect(sf::Vector2f(90, 90));
                    rect.setPosition(c * 100 + 5, r * 100 + 5);
                    rect.setFillColor(card.isMatched ? sf::Color::Transparent
                                      : (card.isFaceUp ? sf::Color::White : sf::Color::Blue));
                    if (!card.isMatched) window.draw(rect);

                    if (card.isFaceUp && !card.isMatched && haveFont) {
                        sf::Text text(std::string(1, card.value), font, 50);
                        text.setPosition(c * 100 + 30, r * 100 + 20);
                        text.setFillColor(sf::Color::Black);
                        window.draw(text);
                    }
                }
            }
            window.display();

            // both cards are drawn now, so give the player a second to see
            // them before a wrong pair gets flipped back
            if (needCheck) {
                sf::sleep(sf::seconds(1));
                checkMatch();
                needCheck = false;
            }

            if (allMatched()) {
                // Draw win message
                window.clear();
                if (haveFont) {
                    sf::Text winText("You Win!", font, 50);
                    winText.setPosition(100, 180);
                    window.draw(winText);
                }
                window.display();
                sf::sleep(sf::seconds(3));
                window.close();
            }
        }
    }
};

// ---------- Step 5: main ----------

int main(int argc, char* argv[]) {
    std::string mode;

    if (argc > 1) {
        mode = argv[1];                 // e.g.  ./game ncurses
    } else {
        std::cout << "Memory Matching Game\n";
        std::cout << "  1 - terminal (type row and col)\n";
        std::cout << "  2 - ncurses  (arrow keys)\n";
        std::cout << "  3 - sfml     (window, click the cards)\n";
        std::cout << "Pick a mode: ";
        int choice;
        std::cin >> choice;
        if (choice == 1) mode = "terminal";
        else if (choice == 2) mode = "ncurses";
        else if (choice == 3) mode = "sfml";
    }

    MemoryGame game;

    if (mode == "terminal") {
        game.playTerminal();
    } else if (mode == "ncurses") {
        game.displayTerminalNcurses();
    } else if (mode == "sfml") {
        game.displaySFML();
    } else {
        std::cout << "Usage: game [terminal|ncurses|sfml]\n";
        return 1;
    }

    return 0;
}
