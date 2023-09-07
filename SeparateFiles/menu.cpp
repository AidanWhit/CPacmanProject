#include "menu.h"
#include <FEHLCD.h>
#include <stdio.h>
#include "simulator_libraries/FEHImages.h"
#include <math.h>
#include <stdlib.h>

#define pi 3.1415926535897932384626433832795028841971693

using namespace std;

/*Creates a PacMan object and a ghost with the same class of player. Each of these objects as different parameters so they will
be drawn differently and in different locations*/
Player PacMan(10 , 10, 4, YELLOW, 95, 95);
Player Ghost(10, 10, 4, PINK, 145, 95);

/*Creates an array of 37 objects that will be used to create the maze and check collisions with the maze itself*/
Maze block[37];

/*Creates an array of 131 pellets that will be drawn to the game screen and this array of ebjects will be used to check 
collisions between the pellets and the pacman object*/
Pellet pellets[131];

/*Stores the current score of the run*/
int score;

/*index 0 stores the highest score the of the session, index 1 stores the second highest score of the session, index 2 stores the 
score of the current run*/
int highscores[3] = {0,0,0};

int PelletsLeft;
int lives_left;

/*Created by Aidan Whitlatch. Draws the aspects of the menu to the screen. More detail below*/
void CreateMenu()
{
    float x_touch, y_touch;
    int i;

    PelletsLeft = 131;
    score = 0;
    lives_left = 3;

    LCD.Clear();
    LCD.SetFontColor(WHITE);

    /*Creates the Main Menu area*/
    LCD.DrawRectangle(10,10,299,219);

    LCD.WriteAt("Start Game", 94, 111);
    LCD.WriteAt("Instructions", 84, 131);
    LCD.WriteAt("Stats", 124, 151);
    LCD.WriteAt("Credits", 114, 171);

    LCD.WriteAt("PacMan", 120, 30);

    /*Waits for a click on the screen and stores the coordinates of the click into x_touch and y_touch*/
    while(!LCD.Touch(&x_touch, &y_touch)){};
    while(LCD.Touch(&x_touch, &y_touch)){};

    /*Click on Start Game*/
    if (x_touch >= 94 && x_touch <=210 && y_touch>=91 && y_touch <= 130)
    {
        LCD.Clear();

        /*Opens the pellet data file which contains data for the position and size of each pellet*/
        FILE *input = fopen("SeparateFiles/PelletData.txt", "r");
        int x, y, r;

        /*Reads the data from each row and stores the value into a pellet object in the array of pellet objects*/
        for (i = 0; i < 131; i++)
        {
            fscanf(input, "%i%i%i", &x, &y, &r);
            pellets[i].SetValues(x, y, r);
        }
        fclose(input);

        /*Calls game screen to draw all of the graphics to the screen*/
        GameScreen();
        
        /*Draws Pacman and the Ghost to the screen*/
        PacMan.CreatePlayer();
        Ghost.CreatePlayer();
    }
    /*If instructions was clicked on*/
    else if (x_touch >= 84 && x_touch <= 210 && y_touch >= 130 && y_touch <= 150)
    {
        LCD.Clear();
        
        /*Writes the instructions on how to play the game to the screen*/
        LCD.WriteAt("Instructions", 90, 0);
        LCD.WriteAt("Click on Up, Down, Left, or", 0, 30);
        LCD.WriteAt("Right to move PacMan.", 50, 50);
        LCD.WriteAt("Avoid the Ghost and collect", 0, 70);
        LCD.WriteAt("pellets to increase your", 0, 90);
        LCD.WriteAt("score. You have 3 lives to", 0, 110);
        LCD.WriteAt("get the highest score ", 20, 130);
        LCD.WriteAt("possible.", 110, 150);
        LCD.WriteAt("Have Fun!", 110, 190);

        /*Calls the Back Button function which creates a clickable back "button"*/
        BackButton();
    }

    /*If Stats was clicked on*/
    else if (x_touch >= 124 && x_touch <= 210 && y_touch >= 151 && y_touch <= 171) //Stats
    {
        LCD.Clear();
        LCD.WriteAt("Stats", 120, 0);
        
        /*If the highscores first index is 0*/
        if (highscores[0] == 0)
        {
            LCD.WriteAt("Play to get stats!", 20, 100);
            BackButton();
        }
        /*Otherwise displays the vlaues of each index of the array to the screen*/
        else
        {
            LCD.WriteAt("Highscore: ", 60, 40);
            LCD.WriteAt(highscores[0], 220, 40);

            LCD.WriteAt("2nd highest score: ", 20, 70);
            LCD.WriteAt(highscores[1], 260, 70);

            LCD.WriteAt("Last Score: ", 60, 100);
            LCD.WriteAt(highscores[2], 220, 100);
            
            /*Calls the Back Button function which creates a clickable back "button"*/
            BackButton();
        }

    }
    /*If credits was clicked on, display the credits to the screen*/
    else if (x_touch >= 114 && x_touch <= 210 && y_touch >= 171 && y_touch <= 191) //Credits
    {
        LCD.Clear();

        LCD.WriteAt("Credits", 110, 0);
        LCD.WriteAt("Made by: ", 110, 40);
        LCD.WriteAt("Aidan Whitlatch", 65, 60);
        LCD.WriteAt("&", 150, 80);
        LCD.WriteAt("Grace Murray", 80, 100);

        /*Calls the Back Button function which creates a clickable back "button"*/
        BackButton();
    }
    /*If none of the options above were clicked, calls create menu again in order to allow the program to store the location of
    another click*/
    else
    {
        CreateMenu();
    }
}

/*Created by Grace Murray: Stores the values inputted when creating a player object into their respective variables that are public
inside of the Player class*/
Player::Player(float xv, float yv, float r, float c, float xp, float yp)
{
    x_velocity = xv;
    y_velocity = yv;
    radius = r;
    color = c;
    x_pos = xp;
    y_pos = yp;
}

/*Created by Grace Murray: Draws the Player object to the screen. Based on the color of each object, it will either draw the pacman
shape to the screen or the ghost shape*/
void Player::CreatePlayer()
{
    int i;
    LCD.SetFontColor(color);
    if (color == YELLOW)
    {
        LCD.FillCircle(x_pos, y_pos, radius);
        LCD.SetFontColor(BLACK);
        /*loops through the indexes of the x_offset array and the y_offset array to create the black triangle that is Pacman's mouth*/
        for (i = 0; i < 60; i++)
        {
            LCD.DrawLine(PacMan.x_pos, PacMan.y_pos, PacMan.x_pos + PacMan.x_offest[i], PacMan.y_pos + PacMan.y_offset[i]);
        }
    }
    /*If the color is not yellow, it will draw a ghost to the screen which is 3 circles and 2 pixels to the screen that will
    resemble a circular ghost with eyes*/
    else
    {
        LCD.FillCircle(x_pos, y_pos, radius);

        LCD.SetFontColor(WHITE);
        LCD.FillCircle(Ghost.x_pos - 2, Ghost.y_pos - 1, 1);
        LCD.FillCircle(Ghost.x_pos + 2, Ghost.y_pos - 1, 1);

        LCD.SetFontColor(BLACK);
        LCD.DrawPixel(Ghost.x_pos - 2, Ghost.y_pos - 1);
        LCD.DrawPixel(Ghost.x_pos + 2, Ghost.y_pos - 1);
    }
}

/*Created by Grace Murray: Creates a Back button in the top left of the screen, when clicked on it will call the create menu function
to allow players to choose other options on the menu*/
void BackButton()
{
    float x_touch, y_touch;

    LCD.WriteAt("Back", 0, 0);
    while(!LCD.Touch(&x_touch, &y_touch)){};
    while(LCD.Touch(&x_touch, &y_touch)){};
    
    /*If the general location of back button was pressed*/
    if (x_touch >= 0 && x_touch <= 50 && y_touch >= 0 && y_touch <= 20)
    {
        LCD.Clear();
        CreateMenu();
    }
    /*Otherwise call backbutton() again to be able to register another click*/
    else
    {
        BackButton();
    }

}

/*Created by Grace Murray: Draws the details of the game to the screen. Calling this function will draw the border of the game
the maze, the pellets, the buttons to operate the game, and the lives of the player to the screen*/
void GameScreen()
{
    int i;
    /*The variable space is used to create a gap in between each of remaining lives of the player*/
    int space = 0;
    LCD.SetFontColor(WHITE);

    LCD.DrawRectangle(0,0,311, 171);
    LCD.WriteAt("Up", 134, 180);
    LCD.WriteAt("Left", 84, 200);
    LCD.WriteAt("Right", 154,200);
    LCD.WriteAt("Down", 124, 220);

    LCD.SetFontColor(YELLOW);
    for (i = 0; i < lives_left; i++)
    {
        LCD.FillCircle(10 + space, 209, 4);
        space += 10;
    }

    CreateMaze();

    CreatePellets();
}

/*Created by Aidan Whitlatch: Creates the general logic for the game. Allows players to move the pacman object with different buttons.
Checks to see if the pacman object has collided with the maze, the outside boundary of the screen, with a pellet, or with the ghost.
More detail on how the funciton works below*/
void GameFunction()
{  
    float x_touch, y_touch;

    /*Creates variables that store the previous location of pacman before it moves. This will be used with the boundary collision
    function*/
    int prev_x = PacMan.x_pos, prev_y = PacMan.y_pos;

    int i;
    float angle;


    while(!LCD.Touch(&x_touch, &y_touch)){};
    while(LCD.Touch(&x_touch, &y_touch)){};

    /*If the up button was pressed*/
    if (x_touch >= 134 && x_touch <=164 && y_touch>=180 && y_touch <= 190)
    {
        LCD.Clear();
        GameScreen();

        PacMan.y_pos -= 10;
        
        /*Each function below is used to check collisions*/
        BoundaryCollision(prev_x, prev_y);
    
        CheckGhostCollision();

        PelletCollide();
    }

    /*If the down button was pressed*/
    else if (x_touch >= 124 && x_touch <=164 && y_touch>=220 && y_touch <= 230) //Down
    {
        LCD.Clear();
        GameScreen();

        PacMan.y_pos += 10;
        
        /*Each function below is used to check collisions*/
        BoundaryCollision(prev_x, prev_y);
        
        CheckGhostCollision();

        PelletCollide();
    }

    /*If the right button was pressed*/
    else if (x_touch >= 154 && x_touch <=199 && y_touch>=200 && y_touch <= 215) //Right
    {
        LCD.Clear();
        GameScreen();

        PacMan.x_pos += 10;

        /*Creates an initial angle*/
        angle = 30;

        /*Calculates the x and y coordinates of each point along the circlee for each respective angle. The offeset values will
        be used to draw a line from the center of the pacman to the outisde edge for each angle to create the black triangle
        for its mouth*/
        for (i = 0; i < 60; i++)
        {
            PacMan.x_offest[i] =  (PacMan.radius + 1) * cos((angle*pi)/180);
            PacMan.y_offset[i] = (PacMan.radius + 1) * sin((angle*pi)/180);
            angle--;
        }
        
        /*Each function below is used to check collisions*/
        BoundaryCollision(prev_x, prev_y);
        
        CheckGhostCollision();

        PelletCollide();
    }

    else if (x_touch >= 84 && x_touch <=124 && y_touch>=200 && y_touch <= 215) //Left
    {
        LCD.Clear();
        GameScreen();

        PacMan.x_pos -= 10;

        angle = 210;

        /*Calculates the x and y coordinates of each point along the circlee for each respective angle. The offeset values will
        be used to draw a line from the center of the pacman to the outisde edge for each angle to create the black triangle
        for its mouth*/
        for (i = 0; i < 60; i++)
        {
            PacMan.x_offest[i] =  (PacMan.radius + 1) * cos((angle*pi)/180);
            PacMan.y_offset[i] = (PacMan.radius + 1) * sin((angle*pi)/180);
            angle--;
        }

        /*Each function below is used to check collisions*/
        BoundaryCollision(prev_x, prev_y);

        CheckGhostCollision();

        PelletCollide();
    }
    
}

/*Created by Aidan Whitlatch: This function allows the Ghost object to move around the screen without being able to move through
the maze and the boundary of the game. This function also checks to see if the ghost object and the pacman object have collided.
More detail about each component of the fucntion below.*/
void CheckGhostCollision()
{
    int x_touch, y_touch, i;

    /*Direction will have a random value from 1-4 assigned to it and will move the ghost based on the value that was assigned*/
    int direction;

    bool flag = true;

    /*Ghost movement Code below*/

    /*This loop runs until the ghost object has made a succesful move without running into the maze or outside boundaries.
    Once this is happens, flag is set to false thus ending the loop*/
    while (flag)
    {
        /*Creates a random number from 1-4*/
        direction = rand() %4 + 1;


        if (direction == 1)
        {
            /*Moves the ghost right*/
            Ghost.x_pos += 5;

            /*If this movement will not result in a collsion*/
            if (GhostBoundaryCollision())
            {
                /*The ghost is drawn to the screen*/
                Ghost.CreatePlayer();
                flag = false;
            }
            /*If the ghost did collide with something*/
            else 
            {
                /*Set the position of the ghost back to its original value*/
                Ghost.x_pos -= 5;
            }
        }
        else if (direction == 2)
        {
            /*Moves the ghost to the left*/
            Ghost.x_pos -= 5;

            /*If this movement will not result in a collsion*/
            if (GhostBoundaryCollision())
            {
                /*The ghost is drawn to the screen*/
                Ghost.CreatePlayer();
                flag = false;
            }
            else 
            {
                /*Set the position of the ghost back to its original value*/
                Ghost.x_pos += 5;
            }
        }
        else if (direction == 3)
        {
            /*Moves the Ghost down*/
            Ghost.y_pos -= 5;

            /*If this movement will not result in a collsion*/
            if (GhostBoundaryCollision())
            {
                /*The ghost is drawn to the screen*/
                Ghost.CreatePlayer();
                flag = false;
            }
            else 
            {
                /*Set the position of the ghost back to its original value*/
                Ghost.y_pos += 5;
            }
        }
        else 
        {
            /*Moves the ghost up*/
            Ghost.y_pos += 5;

            /*If this movement will not result in a collsion*/
            if (GhostBoundaryCollision())
            {
                /*The ghost is drawn to the screen*/
                Ghost.CreatePlayer();
                flag = false;
            }
            else 
            {
                /*Set the position of the ghost back to its original value*/
                Ghost.y_pos -= 5;
            }
        }
    }

    /*PacMan and Ghost collision detection below*/


    int a = PacMan.x_pos - Ghost.x_pos;
    int b = PacMan.y_pos - Ghost.y_pos;
    /*Calculates the distance between the ghost and pacman objects using pythagorean's theorem*/
    float c = sqrt(pow(a,2) + pow(b,2));

    /*If the Ghosts have collided*/
    if (c < (PacMan.radius + Ghost.radius))
    {
        lives_left -= 1;
        if (lives_left == 0)
        {
            /*Creates the game over screen*/
            PacMan.x_pos = 95;
            PacMan.y_pos = 95;
            LCD.Clear();
            LCD.SetFontColor(RED);
            LCD.WriteAt("Game Over", 101, 111);
            LCD.WriteAt("Score: ", 101, 141);
            LCD.WriteAt(score, 171, 141);

            /*Stores the run's score into the highscores array*/
            if (score > highscores[0])
            {
                highscores[1] = highscores[0];
                highscores[0] = score;

                highscores[2] = score;
            }
            else if (score > highscores[1])
            {
                highscores[1] = score;
                highscores[2] = score;
            }
            else
            {
                highscores[2] = score;
            }

            LCD.WriteAt("Click to Continue", 71, 171);

            /*Draws the menu when the screen is clicked*/
            while(!LCD.Touch(&x_touch, &y_touch)){};
            while(LCD.Touch(&x_touch, &y_touch)){};
            CreateMenu();
        }

        /*If the player still has lives left*/
        else
        {
            /*Sets pacman back to its original position*/
            PacMan.x_pos = 95;
            PacMan.y_pos = 95;
            
            /*Creates a life lost screen*/
            LCD.Clear();
            LCD.SetFontColor(WHITE);
            LCD.WriteAt("Life Lost :(", 91, 111);
            LCD.WriteAt("Click to continue", 51, 141);
            
            /*Once clicked, it allows the user to play the game from where they left off*/
            while(!LCD.Touch(&x_touch, &y_touch)){};
            while(LCD.Touch(&x_touch, &y_touch)){};
            LCD.Clear();
            PacMan.CreatePlayer();
            Ghost.CreatePlayer();
            GameScreen();
            GameFunction();
        }
    }
}

/*Created by Grace Murray: A Function used to determine if the ghost has collided with the maze or the boundary. It will return true
if no collision has been detected and false if a collision has been detected. Uses the same logic that is used in the 
BoundaryCollision function. The logic will be explained in that fucntion rather than this one.*/
bool GhostBoundaryCollision()
{
    int i = 0;
    int width = 311, height = 171;

    int a = Ghost.x_pos - width/2;
    int b = Ghost.y_pos - height/2;

    /*Stores either true or false based on if there was a collision or not*/
    bool value;
    
    if (abs(a) > (width/2 -5) || abs(b) > (height/2 -5))
    {
        value = false;
    }
    else
    {
        value = true;
        
        for (i = 0; i < 37; i++)
        {
            int a2 = Ghost.x_pos - block[i].x_center;
            int b2 = Ghost.y_pos - block[i].y_center;

            if (abs(a2) < block[i].width/2 + 5 && abs(b2) < block[i].height/2 + 5)
            {
                value = false;
                break;
            }
        }
    }

    return value;
}

/*Created by Aidan Whitlatch: This funciton determines if the pacman object has collided with the maze or outside boundary. If a
collision was found, the pacman object is set back to its original position before the collision was made. Therefore, the PacMan 
object is not able to phase through the maze or the outside boundary*/
void BoundaryCollision(int prev_x, int prev_y)
{
    int i = 0;
    /*Width and height of the outside boundary*/
    int width = 311, height = 171;


    /*Outside boundary collision below*/

    /*Calculates the length between the pacman and the center of the outside bounary's rectangle in the x and y direction*/
    int a = PacMan.x_pos - width/2;
    int b = PacMan.y_pos - height/2;
    float c = sqrt(pow(a,2) + pow(b,2));

    /*If the length of a is larger than the width of the boundary/2 - 5 or if the lenght of b is larger than the height of the 
    boundary/2 - 5. Then a collision has been detected*/
    if (abs(a) > (width/2 - 5) || abs(b) > (height/2 - 5))
    {
        /*If the collision was on the right side*/
        if (a > width/2 - 5)
        {
            PacMan.x_pos = width - 6;
            PacMan.CreatePlayer();
        }

        /*If the collision was on the bottom*/
        else if (b > height/2 - 6)
        {
            PacMan.y_pos = height - 6;
            PacMan.CreatePlayer();
        }

        /*If the collision was on the top*/
        else if (b < -(height/2 - 5))
        {
            PacMan.y_pos = 5;
            PacMan.CreatePlayer();
        }

        /*If the collision was on the left*/
        else if (a < -(width/2 - 5))
        {
            PacMan.x_pos = 5;
            PacMan.CreatePlayer();
        }
    }
    
    /*if no collision between the outside boundary was detected*/
    else
    {
        PacMan.CreatePlayer();

        //Maze Collision Below

        /*Loops through every object in the block array*/
        for (i = 0 ; i < 37; i++)
        {
            int a2 = PacMan.x_pos - block[i].x_center;
            int b2 = PacMan.y_pos - block[i].y_center;
            
            /*If PacMan collides with the maze set pacman back to its original poisiton before it was moved*/
            if (abs(a2) < block[i].width/2 + 5 && abs(b2) < block[i].height/2 + 5)
            {
                LCD.Clear();
                GameScreen();
                PacMan.x_pos = prev_x;
                PacMan.y_pos = prev_y;
                PacMan.CreatePlayer();
            }
        }
    }
}

/*Created by Grace Murray: Draws the rectangles to the screen based on their inputted values*/
void Maze::Rect()
{
    LCD.SetFontColor(BLUE);
    LCD.FillRectangle(x_center-width/2, y_center-height/2, width, height);
}

/*Created by Grace Murray: Sets classes public variables equal to the values that were inputted*/
void Maze::SetValues(int x_c, int y_c, int w, int h)
{
    width = w;
    height = h;
    x_center = x_c;
    y_center = y_c;
}

/*Created by Aidan Whitlatch: This fucntions reads the data that is found in Rect.txt and sets the values found in the file
equal to the center of the rectangle in the x and y direction, the width, and the height of the rectangles. It then sets the values
read in to be equal to the object's values that are found in its class. Then the rectangle is created. This is repeated for 37
rectangles to create the entire maze*/
void CreateMaze()
{
    int i;
    int h, w, x_c, y_c;
    FILE *input;

    input = fopen("SeparateFiles/Rect.txt", "r");
    
    for (i = 0; i < 37 ; i++)
    {
        fscanf(input, "%i%i%i%i", &x_c, &y_c, &w, &h);
        block[i].SetValues(x_c, y_c, w, h);
        block[i].Rect();
    }

    fclose(input);
}

/*Created by Grace Murray: Sets the public varaibles in the pellet class equal to the inputted values*/
void Pellet::SetValues(int x, int y, int r)
{
    x_pos = x;
    y_pos = y;
    radius = r;
}

/*Created by Aidan Whitlatch: this fucntion draws the pellet object to the screen with its respective values*/
void Pellet::DrawPellet()
{
    LCD.FillCircle(x_pos, y_pos, radius);
}

/*Created by Grace Murray: This function loops through every pellet object found in the pellets array and will draw each of those
to the screen*/
void CreatePellets()
{
    int i;
    LCD.SetFontColor(WHITE);
    for (i = 0; i < 131; i++)
    {
        pellets[i].DrawPellet();
    }
    
}

/*Created by Aidan Whitlatch: This function is used to determine if Pacman has collided with a pellet. If there was a collision
the score is increased by 100 and the pellets left value is decreased by 1. If there are no pellets left, a you win screen is shown
and the score is stored into the highscores array*/
void PelletCollide()
{
    int i, x_touch, y_touch;

    /*Loops through every pellet object in the pellet array*/
    for (i = 0; i < 131; i++)
    {
        /*If Pacman and a pellet have collided*/
        if (PacMan.x_pos == pellets[i].x_pos && PacMan.y_pos == pellets[i].y_pos)
        {
            score += 100;
            
            /*Decrease the size of the pellet to a pixel and move it to the top left corner so it can never be collided with again*/
            pellets[i].radius = 0;
            pellets[i].x_pos = 0;
            pellets[i].y_pos = 0;
            PelletsLeft--;

            /*If there are no pellets left on the screen*/
            if (PelletsLeft == 0)
            {
                /*Create the you win screen*/
                LCD.Clear();
                LCD.SetFontColor(WHITE);
                LCD.WriteAt("Congrats! You Win!", 50, 80);
                LCD.WriteAt("Click to Continue", 55, 120);

                /*System to store the score into the highscores array*/
                if (score > highscores[0])
                {
                    highscores[1] = highscores[0];
                    highscores[0] = score;

                    highscores[2] = score;
                }
                else if (score > highscores[1])
                {
                    highscores[1] = score;
                    highscores[2] = score;
                }
                else
                {
                    highscores[2] = score;
                }

                /*When clicked, the menu will be created, esentially restarting the game*/
                while(!LCD.Touch(&x_touch, &y_touch)){};
                while(LCD.Touch(&x_touch, &y_touch)){};
                PacMan.x_pos = 95;
                PacMan.y_pos = 95;

                CreateMenu();
            }
            
        }
    }
}