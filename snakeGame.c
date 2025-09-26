#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#include <conio.h>
#include <Windows.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

#define HEIGHT (25)
#define WIDTH (50)
#define FOODS (5)

//for keeping xy dir of food and snake body
typedef struct{
    int x,y;
}position;

#ifdef _WIN32
void usleep(__int64 usec) {
    Sleep(100); // Sleep takes milliseconds
}
void hideCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}

void showCursor() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
void gotoxy(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

#else
//get the non conanical terminal format using this
void initTerminal() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &term);
}

//get back the canonical terminal format
void resetTerminal() {
    struct termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

//maybe this is for raw data input continuous
void setNonBlockingInput() {
    int flags = fcntl(0, F_GETFL, 0);
    fcntl(0, F_SETFL, flags | O_NONBLOCK);
}
void hideCursor() {
    printf("\e[?25l");
    fflush(stdout);
}

void showCursor() {
    printf("\e[?25h");
    fflush(stdout);
}
#endif

//to get the food location from 1 to max-1 both inclusive
position getFood(){
    position f;
    f.x = (1 + rand()% (HEIGHT-1));
    f.y = (1 + rand()% (WIDTH-1)); 
    return f;
}   

//clear as the name suggests it is a gameover fn
void gameOver(int snakeLen){
    printf("\n\t\t\t\t\t  GAME OVER!\n");
    printf("\t\t\t\t\t  Score: %d\n",snakeLen-1);
    #ifndef _WIN32
        resetTerminal();
    #endif
    showCursor();
    exit(0);
}

//as the name suggests it draws or prints
void drawWindow(position snake[], position food[], int snakeLen){
    #ifdef _WIN32
        gotoxy(0,0);
    #else
        printf("\033[2J\033[H"); //linux/macOS clear screen
    #endif 

    printf("\n\tGame over on collison with self or the borders!\n");
    printf("\tUse WASD for movement and Q to exit.\n");
    printf("\tDont eat too much! xD\t\t\t\tMy HighScore-76\n");
    printf("\t\t\t\t\t\t\t~MAW11\n");

    for(int i=0;i<=HEIGHT; i++){
        for(int j=0;j<=WIDTH; j++){
            int printed=0;

            //for borders
            if(i==0 || j==0 || i==HEIGHT || j==WIDTH){
                printf("##");
                printed=1;
            }
            for(int k=0; k<snakeLen; k++){
                if(snake[k].x == i && snake[k].y == j){
                    printf("@@");
                    //double printing to match height and length of pixels in terminal output
                    printed=1;

                    //collision only works for min (3snakeLen/2score)
                    if((snake[k+1].x == snake[0].x) && (snake[k+1].y==snake[0].y)) {
                        gameOver(snakeLen);
                    }
                    break;
                }
            }
            //printing the food using the food array, checking this for every i j cell 
            for(int l=0;l<FOODS;l++){
                if(!printed && i==food[l].x && j==food[l].y) {
                printf("00");
                printed=1;
                }
            }
            //if nothing printed in the cell yet then do empty cell print
            if(!printed) printf("..");
        }
        printf("\n");
    }
    //printing the score at the bottom of the playable cell area
    printf("Score: %d\n\n", snakeLen-1);
}

int main(){
    srand(time(NULL));
    #ifdef _WIN32
    #else
    initTerminal(); //make it non canonical
    setNonBlockingInput(); //make it raw format continuous input
    #endif
    hideCursor();
    int snakeLen=1;
    position snake[HEIGHT*WIDTH];
    snake[0].x = HEIGHT/2; //initial x of snake
    snake[0].y = WIDTH/2; //initial y of snake

    position food[FOODS];
    for(int i=0;i<FOODS;i++){
        food[i] = getFood(); //getting food xy for FOODS number of food
    }

    char input;
    while(1){
        char ch;
        #ifdef _WIN32
            if(_kbhit()){
                ch = _getch();
            }
        #else 
            read(0, &ch, 1);
        #endif
            
        if(ch == 'a' ||ch == 'd'||ch == 'w'||ch == 's'){
            input = ch; //if the inputs are the following then only input is recorded
        }else if(ch == 'q') break;

        //copy data from prev snake part to new, here other body part follows the head posi
        for(int i=snakeLen-1; i>0; i--){
            snake[i] = snake[i-1];
        }

        //switch after copy becuz changes in dir hv to be noted after the entry of full curr body data
        switch(input) {
            case 'w': snake[0].x--; break;  
            case 's': snake[0].x++; break;  
            case 'a': snake[0].y--; break;  
            case 'd': snake[0].y++; break;  
        }

        //border collision detection and action
        if((snake[0].x == HEIGHT || snake[0].x == 0) || (snake[0].y == WIDTH || snake[0].y == 0)){
            gameOver(snakeLen);
        }

        //eating apple/food detection and action
        for(int i=0;i<FOODS;i++){
            if(snake[0].x==food[i].x && snake[0].y == food[i].y) {
            food[i] = getFood();
            snakeLen++;
            }
        }
        drawWindow(snake,food,snakeLen);
        //tells the sleep duration/gap to the code according to me
        usleep(120000); //this value is in microsecs, 100000 is 100ms or 10fps
    }
    #ifndef _WIN32
        resetTerminal();
    #endif
    return 0;
}