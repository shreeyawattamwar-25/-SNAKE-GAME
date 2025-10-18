#include <iostream>
#include <cstdlib>   // For rand() and system()
#include <ctime>     // For time() to seed random
#include <thread>    // For std::this_thread::sleep_for
#include <chrono>    // For std::chrono::milliseconds
#include <cmath>     // For abs()

// --- Constants ---
const int GRID_WIDTH = 30;
const int GRID_HEIGHT = 15;
const int INITIAL_SPEED_MS = 200;

// --- Data Structures (Manual Linked List) ---

struct Segment {
    int x; 
    int y;
    Segment* next; 

    Segment(int gridX, int gridY) : x(gridX), y(gridY), next(nullptr) {}
};

struct Food {
    int x;
    int y;
};

// --- Snake Class ---

class Snake {
private:
    Segment* head;
    Segment* tail;
    int dirX;
    int dirY;
    int length;
    bool needsGrowth;

public:
    Snake() : dirX(1), dirY(0), length(1), needsGrowth(false) {
        head = new Segment(GRID_WIDTH / 2, GRID_HEIGHT / 2);
        tail = head;
    }

    // Essential: Implement Destructor
    ~Snake() {
        Segment* current = head;
        while (current != nullptr) {
            Segment* next = current->next;
            delete current;
            current = next;
        }
    }

    void setDirection(int x, int y) {
        // Prevent 180-degree turns
        if (x != -dirX || y != -dirY) {
            dirX = x;
            dirY = y;
        }
    }

    void grow() { needsGrowth = true; }

    void move() {
        // 1. Create a new segment for the new head position
        int newX = head->x + dirX;
        int newY = head->y + dirY;

        Segment* newHead = new Segment(newX, newY);
        newHead->next = head;
        head = newHead;

        // 2. Handle Tail (Movement or Growth)
        if (needsGrowth) {
            length++;
            needsGrowth = false;
        } else {
            // Normal movement: delete the last segment (the tail)

            // Find the segment *before* the current tail
            Segment* current = head;
            if (head == tail) return; // Case for length 1

            while (current->next != tail) {
                current = current->next;
            }

            delete tail;
            tail = current;
            tail->next = nullptr;
        }
    }

    Segment* getHead() const { return head; }
    int getLength() const { return length; }

    bool checkSelfCollision() const {
        Segment* current = head->next;
        while (current != nullptr) {
            if (head->x == current->x && head->y == current->y) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    bool checkWallCollision() const {
        return head->x < 0 || head->x >= GRID_WIDTH || head->y < 0 || head->y >= GRID_HEIGHT;
    }
};

// --- Game Logic Functions ---

void placeFood(Food& food, const Snake& snake) {
    bool placed = false;
    while (!placed) {
        // Simple random generation
        food.x = rand() % GRID_WIDTH;
        food.y = rand() % GRID_HEIGHT;

        placed = true;
        // Check collision with snake body
        const Segment* current = snake.getHead();
        while (current != nullptr) {
            if (food.x == current->x && food.y == current->y) {
                placed = false; 
                break;
            }
            current = current->next;
        }
    }
}

bool checkFoodCollision(const Snake& snake, const Food& food) {
    return snake.getHead()->x == food.x && snake.getHead()->y == food.y;
}

void draw(const Snake& snake, const Food& food, int score, int speed) {
    // Clears the screen (Works on most terminals)
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    std::cout << "Score: " << score << " | Speed: " << speed << "\n";
    
    // Draw Top Border
    for (int i = 0; i < GRID_WIDTH + 2; ++i) std::cout << "#";
    std::cout << "\n";

    // Draw Grid Content
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        std::cout << "#"; // Left Border
        for (int x = 0; x < GRID_WIDTH; ++x) {
            char cell = ' ';
            
            // Check for Food
            if (x == food.x && y == food.y) {
                cell = '@';
            }
            
            // Check for Snake
            const Segment* current = snake.getHead();
            bool isSnake = false;
            while (current != nullptr) {
                if (x == current->x && y == current->y) {
                    cell = (current == snake.getHead()) ? 'O' : 'o'; // 'O' for head, 'o' for body
                    isSnake = true;
                    break;
                }
                current = current->next;
            }
            
            std::cout << cell;
        }
        std::cout << "#\n"; // Right Border
    }

    // Draw Bottom Border
    for (int i = 0; i < GRID_WIDTH + 2; ++i) std::cout << "#";
    std::cout << "\n";
    std::cout << "Controls: WASD or Arrows (if supported by terminal).\n";
}

// Function to check key presses (limited and platform-dependent)
// For a simple demo, we will rely on timed input processing
int get_key_input(int default_key) {
    // This is highly platform-dependent and requires non-blocking input setup 
    // (e.g., using curses/conio.h) which we cannot use here.
    // For a minimal example, we skip real-time input and rely on the main loop's logic.
    return default_key; 
}


// --- Main Function ---

int main() {
    srand(time(0)); // Seed random generator

    Snake snake;
    Food food;
    placeFood(food, snake);
    bool isGameOver = false;
    
    // Timing and Difficulty
    int moveDelayMs = INITIAL_SPEED_MS; // Milliseconds
    auto lastMoveTime = std::chrono::steady_clock::now();

    // Directions for basic key input simulation (W=1, S=2, A=3, D=4)
    int currentInput = 4; 

    while (!isGameOver) {
        
        // 1. Draw
        draw(snake, food, snake.getLength() - 1, (int)(1000.0f / moveDelayMs));

        // 2. Input Handling (Simple, block and ask for direction)
        std::cout << "Enter move (w/a/s/d, then Enter): ";
        char input;
        if (std::cin >> input) {
            switch (input) {
                case 'w': case 'W': snake.setDirection(0, -1); break;
                case 's': case 'S': snake.setDirection(0, 1); break;
                case 'a': case 'A': snake.setDirection(-1, 0); break;
                case 'd': case 'D': snake.setDirection(1, 0); break;
            }
        }
        
        // 3. Update (Logic)
        auto currentTime = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastMoveTime).count() >= moveDelayMs) {
            snake.move();
            lastMoveTime = currentTime;

            // Check for Game Over conditions
            if (snake.checkWallCollision() || snake.checkSelfCollision()) {
                isGameOver = true;
                break;
            }
            
            // Check for Food Collision
            if (checkFoodCollision(snake, food)) {
                snake.grow();
                placeFood(food, snake);

                // Difficulty Increase
                if (moveDelayMs > 50) { 
                    moveDelayMs -= 10; 
                }
            }
        } else {
             // If move time hasn't passed, just wait a bit to prevent heavy CPU load.
             std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    }

    // Final Draw and Game Over Message
    draw(snake, food, snake.getLength() - 1, (int)(1000.0f / moveDelayMs));
    std::cout << "\n=========================================\n";
    std::cout << "           G A M E   O V E R!           \n";
    std::cout << "          Final Score: " << snake.getLength() - 1 << "\n";
    std::cout << "=========================================\n";
    
    return 0;
}
