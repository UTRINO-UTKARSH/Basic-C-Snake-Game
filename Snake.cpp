#include <iostream>
#include <conio.h>
#include <vector>
#include <windows.h>
#include <cstdlib>// used for geeting the coordinates of the cursor to posistion the grid
#include <mmsystem.h>
using namespace std;
int grid_size = 20; // max area for snake to move;
class snake_desc
{
private:
    vector<pair<int, int>> snake; // body (head at index 0)
    char direction;               // 'W', 'A', 'S', 'D'
    pair<int, int> fruit;         // fruit position
    int score;

public:
    void render();                    // creates the area to play (DONE)
    void initialize();                // starting snake (done)
    void move();                      // movement logic, (done)
    void changeDirection(char input); // done
    void spawnFruit();                // random fruit(done)
    bool checkCollision();            // wall + self(done)
    void checkFruit();                // eating logic(done)
    void gameOver();                  // reset / end(done)
    void gotoxy(int x , int y);
};

// stops the flicker by re-writting the console cursor position
void snake_desc::gotoxy(int x , int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//
void snake_desc::checkFruit()
{
    if(snake[0] == fruit){
        score++;
        spawnFruit();
    }
}
//
void snake_desc::spawnFruit()
{
   while (true){
      fruit.first = rand() % (grid_size-2) +1;
      fruit.second = rand() % (grid_size-2)+1;
      bool onSnake = false;
      for(auto part:snake){
        if(part.first == fruit.first && part.second == fruit.second){
            onSnake = true;
            break;
        }
      }
      if(!onSnake){
        break;
      }
   }
   
}

void snake_desc::gameOver(){
    system("cls");
    cout << "########################\n";
    cout << "#      GAME OVER     #\n";
    cout << "#   Final Score: " << score << "   #\n";
    cout << "########################\n";
    PlaySound(TEXT("./Game.wav"), NULL, SND_SYNC | SND_FILENAME);
    Sleep(100);
    exit(0);
}

void snake_desc::render(){
    gotoxy(5,2);
    for (int y = 0; y < grid_size; y++){
        for (int x = 0; x < grid_size; x++){
            if (x == 0 || y == 0 || x == grid_size - 1 || y == grid_size - 1){
                cout << "#  ";
            }
            else if(x==fruit.first && y == fruit.second){
                cout<<"@";
            }
            else{
                bool isSnake = false;
                for (auto part : snake){
                    if (part.first == x && part.second == y){
                        isSnake = true;
                        break;
                    }
                }
                if (isSnake){
                    cout << "O";
                }
                else{
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
}

bool snake_desc::checkCollision()
{
    pair<int,int> head = snake[0];

    if(head.first < 0 || head.first >= grid_size ||
       head.second < 0 || head.second >= grid_size){
        return true;
    }

    for(int i = 1; i < snake.size(); i++){
        if(snake[i] == head){
            return true;
        }
    }
    return false;
}

void snake_desc::initialize()
{
    snake.clear();
    score = 0;
    direction = 'D';
    snake.push_back({10, 10});
    snake.push_back({10, 9});
    snake.push_back({10, 8});
    spawnFruit();
}
void snake_desc::move()
{
    pair<int, int> head = snake[0];
    pair<int, int> newHead = head;

    if (direction == 'W')
        newHead.second -= 1;
    else if (direction == 'S')
        newHead.second += 1;
    else if (direction == 'A')
        newHead.first -= 1;
    else if (direction == 'D')
        newHead.first += 1;

    snake.insert(snake.begin(), newHead);
    if(newHead!=fruit){
        snake.pop_back();
    }
}
void snake_desc::changeDirection(char input)
{

    if (direction == 'W' && input == 'S')
        return;
    if (direction == 'S' && input == 'W')
        return;
    if (direction == 'A' && input == 'D')
        return;
    if (direction == 'D' && input == 'A')
        return;

    direction = input;
}
int main()
{
    snake_desc ss;
    ss.initialize();
    cout<<"\n\n\n\n";
    while (true)
    {
        if (_kbhit())
        {
            char key = _getch();
            switch (key)
            {
            case 'w':
                ss.changeDirection(toupper(key));
                break;
            case 'a':
                ss.changeDirection(toupper(key));
                break;
            case 's':
                ss.changeDirection(toupper(key));
                break;
            case 'd':
                ss.changeDirection(toupper(key));
                break;

            default:
                break;
            }
        }
        if (ss.checkCollision()){
            ss.gameOver();
            break;
        }
        ss.gotoxy(0,0);
        ss.move();
        ss.checkFruit();
        ss.render();
        Sleep(200);
    }
}
