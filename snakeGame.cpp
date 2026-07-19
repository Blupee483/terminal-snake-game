#include <iostream>
#include <vector>
#include <cctype>
#include <random>

const short gridColumns = 20;
const short gridRows = 10;

std::vector<std::vector<char>> grid(gridRows, std::vector<char>(gridColumns, 0));
struct SnakeCell{
    short x;
    short y;
    int life;
};
std::vector<SnakeCell> snakeBody = {};
short snakeDirX = 0;
short snakeDirY = 1;
short snakePosX = 0;
short snakePosY = 0;
short applePosX = gridColumns/2;
short applePosY = gridRows/2;
short snakeLength = 5;

bool gameOver = false;

void InitGrid(){ //initializes the grid with dashes as empty squares
    for(int i = 0; i < gridRows; i++){
        for(int j = 0; j < gridColumns; j++){
            grid[i][j] = '-';
        }
    }
}

void AppleToRandPos(){ //sends the apple to a random grid coord
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(0, gridColumns-1);
    applePosX = distrib(gen);

    std::random_device rd2;
    std::mt19937 gen2(rd2());
    std::uniform_int_distribution<int> distrib2(0, gridRows-1);
    applePosY = distrib2(gen2);
}

void IncreaseSnakeLength(){
    //adds a life to each existing body cell
    for(int i = 0; i < snakeBody.size(); i++){
        snakeBody[i].life ++;
    }

    //finds the position of a new tail cell
    short tailX = snakeBody.back().x;
    short tailY = snakeBody.back().y;
    short dirX = tailX - snakeBody[snakeLength-2].x;
    short dirY = tailY - snakeBody[snakeLength-2].y;

    tailX += dirX;
    if(tailX >= gridColumns) tailX = 0;
    if(tailX < 0) tailX = gridColumns - 1;

    tailY += dirY;
    if(tailY >= gridRows) tailY = 0;
    if(tailY < 0) tailY = gridRows - 1;

    SnakeCell newPart = {tailX, tailY, 0};
    snakeBody.insert(snakeBody.begin(), newPart);

    //increase the length variable by 1
    snakeLength ++;
}

void UpdateFunction(short snakeDirX, short snakeDirY, short* snakePosX, short* snakePosY){
    short prevX = *snakePosX;
    short prevY = *snakePosY;

    //update the snake's head's position
    //loops the snake's position if at an edge
    *snakePosX = prevX + snakeDirX;
    if(*snakePosX >= gridColumns) *snakePosX = 0;
    if(*snakePosX < 0) *snakePosX = gridColumns-1;

    *snakePosY = prevY + snakeDirY;
    if(*snakePosY >= gridRows) *snakePosY = 0;
    if(*snakePosY < 0) *snakePosY = gridRows-1;

    //checks if the snake ate the apple
    if(*snakePosY == applePosY && *snakePosX == applePosX){
        //snake ate the apple
        IncreaseSnakeLength();
        AppleToRandPos();
    }

    //adds the new head position to the body
    SnakeCell headCell = {*snakePosX, *snakePosY, snakeLength};
    snakeBody.push_back(headCell);

    //loops through the body to see if a cell is alive and check for head-body collision
    for(int i = 0; i < snakeBody.size(); i++){
        snakeBody[i].life--;

        //check if the head collided with any cell
        if(snakeBody[i].life >= 0 && i != snakeBody.size()-1){
            short x = snakeBody[i].x;
            short y = snakeBody[i].y;
            if(x == *snakePosX && y == *snakePosY){
                gameOver = true;
            }
        }
    }
    std::erase_if(snakeBody, [](const SnakeCell& cell) {
        return cell.life < 0;
    });
};

int main(){
    //main loop
    while(true){
        //resets the grid
        InitGrid();
        //updates the snake to move
        UpdateFunction(snakeDirX, snakeDirY, &snakePosX, &snakePosY);

        //updates the grid to show the snake
        for(int i = 0; i < snakeBody.size(); i++){
            short x = snakeBody[i].x;
            short y = snakeBody[i].y;
            grid[y][x] = '#';
        }

        //updates the grid to show the apple
        grid[applePosY][applePosX] = '@';

        //prints the grid
        for(int i = 0; i < gridRows; i++){
            for(int j = 0; j < gridColumns; j++){
                std::cout<<grid[i][j]<<" ";
            }
            std::cout<<"\n";
        }

        snakeBody.shrink_to_fit();

        //checks if game over
        if(gameOver) {
            std::cout<<"\n"<<"Game Over!";
            break;
        }

        //get user imput
        char input;
        std::cout<<"Input an action (z to turn counterclockwise, x to turn clockwise, c to not turn, or e to exit): ";
        while(std::cin>>input){
            input = std::toupper(input);

            if(input == 'Z' || input == 'X' || input == 'C' || input == 'E'){
                break;
            }

            std::cout<<"\n"<<"Invalid input. Please input z, x, or c: ";
        }

        //turns the snake based on the given input
        if(input == 'Z'){
            short temp = snakeDirX;
            snakeDirX = snakeDirY;
            snakeDirY = -temp;
        }
        else if(input == 'X'){
            short temp = snakeDirX;
            snakeDirX = -snakeDirY;
            snakeDirY = temp;
        }
        else if(input == 'E') {
            break;
        }
    }

    std::cout<<"\n"<<"Thanks for playing! Press enter to end.";
    
    return 0;
}