# -SNAKE-GAME
Company: "CODETECH IT SOLUTIONS"
Name: SHREEYA WATTAMWAR 
Intern ID:CT04DY2687 
Domain: C++ Duration :4 weeks

"YOU HAVE TO ENTER DESCRIPTION OF YOUR TASK (IN 500 WORDS)"
This task was a fantastic dive into game development fundamentals using pure C++. The goal was to build a fully functional Snake Game, which required mastering three key areas without relying on fancy external graphics libraries like SFML or SDL.

"Graphics" in the Terminal: Since we couldn't use a graphics card, the visual part of the game was handled by simulating a display in the console. The screen constantly clears and redraws itself, using characters like 'O' for the snake's head, 'o' for its body, and '@' for the food. It's a clever way to make a simple but "visually engaging" game using just text.

The Snake's Body (No Vectors!): The most technical part was managing the snake's length. Instead of using a standard container like a std::vector, I built a Manual Linked List. This structure is perfect for the snake because:

Movement: Moving is easy—you add a new piece to the head and delete the last piece from the tail.

Growth: When the snake eats food, you just add the new head piece but don't delete the tail, making the snake longer instantly.

Increasing Difficulty: The game gets faster as the snake grows! Every time the snake eats food, the movement delay (the time between steps) is slightly reduced, forcing the player to react quicker and quicker. This fulfills the increasing difficulty requirement perfectly.

In short, this project was a brilliant exercise in building a robust, self-contained application that proved technical skill in data structures, memory management, and game logic, all within the strict limitations of the console environment
