#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//for stack
typedef struct stack{
    int stk[1000][2];
    int top;
} STACK;

//for marker
typedef struct Marker{
    int x;
    int y;
    struct Marker * next;
}Marker;

//for pop return values
typedef struct values
{
    int x;
    int y;
} values;

void file_parser(FILE * fp, char * file_name);
void routeFinder(char ** maze, int length, int breadth);
void push(int x_pos, int y_pos);
values pop();
bool isEmpty();

bool checkVal(int x, int y, Marker * theList);
Marker * createMarker(int x, int y, Marker * theList);

void printMaze(char ** maze, int length, int breadth);
    
//stack decleration
STACK s;

int main(int argc, char const *argv[])
{
    if(argc != 2)
    {
        printf("not enough arguments\n");
        exit(1);
    }
    char file_name[100] = {0};

    strcpy(file_name,argv[1]);

    FILE * fp = fopen(file_name, "r");

    if(fp == NULL)
    {
        printf("file does not exist\n");
        exit(1);
    }

    file_parser(fp, file_name);

    return 0;
}


void file_parser(FILE * fp, char * file_name)
{
    // bool f_first = false;
    // bool f_second = false; //not using them right now
    int t = 0;
    int m_LoopCounter = 0;
    int breadth = 0;
    int length = 0;
    char data[1000] = {0};
    char token[100] = {0};

    //counters for storing characters in the array in c.

    int l_Count = 0;
    int b_Count = 0;
    while(fgets(data, 1000, fp) != NULL)
    {
        t = 0;
        for(int i = 0; i < strlen(data); i++)
        {
            //printf("%c %d\n", data[i], i);
            if(m_LoopCounter == 0)
            {
                token[t] = data[i];
                t++;

                if(data[i+1] == ' ')
                {
                    t = 0;
                    length = atoi(token);
                    memset(token, 0, strlen(token));
                }

                if(data[i+1] == '\n')
                {
                    t = 0;
                    breadth = atoi(token);
                    memset(token, 0, strlen(token));
                }
            }  
        }
        m_LoopCounter++;
    }

    printf("length: %d breath: %d\n", length, breadth);
    fclose(fp);



    m_LoopCounter = 0;
    char ** maze= malloc(sizeof(char *) * (length));
    if(maze == NULL) {
        printf("Maze malloc did not work\n");
        exit(1);
    }

    for(int i = 0; i < length; i++) {
        maze[i] = malloc(sizeof(char) * breadth);
    }

    FILE * inFP = fopen(file_name, "r");
    if(inFP == NULL)
    {
        printf("file does not exist\n");
        exit(1);
    }


    char values[100] = {0};
    while(fgets(values, 1000, inFP) != NULL)
    {
        for(int i = 0; i < strlen(values); i++)
        {
            if(m_LoopCounter != 0)
            {
                if(values[i] != '\n')
                    {
                        maze[l_Count][b_Count] = values[i];
                        b_Count++;



                        if(b_Count == breadth)
                        {
                            b_Count = 0;
                            l_Count++;
                        }

                        if(l_Count == length)
                        {
                            printf("all the values inside maze are filled\n");
                        }  
                    }
                else
                {
                    printf("new_line\n");
                }
            }
        }
        m_LoopCounter++;
    }


     for(int i = 0; i < length; i++)
     {
         for(int j = 0; j < breadth; j++)
         {
             printf("[%c,%d,%d]\n", maze[i][j], i, j);
         }
     }

    printf("length = %d\n", length);
    printf("breadth = %d\n", breadth);
    fclose(inFP);
    routeFinder(maze, length, breadth);
}

void routeFinder(char ** maze, int length, int breadth)
{
    //stack decleration
    s.top = -1;

    Marker * theList = NULL;

    int x_cord = 0; //along the length - starting point
    int y_cord = 0; //along the breadth - starting point


    int x_end = 0;
    int y_end = 0;

    for(int i = 0; i < length; i++) {
        for(int j = 0; j < breadth; j++) {
            if(maze[i][j] == 'o'){
                x_cord = i;
                y_cord = j;
            }

            if(maze[i][j] == 'e')
            {
                x_end = i;
                y_end = j;
            }
        }
    }
    printf("starting position in this maze is: %d %d\n", x_cord,y_cord);
    printf("value at starting position: %c\n", maze[x_cord][y_cord]);
    printf("value at the end position: %d %d\n", x_end, y_end);


    //pushed the starting point on stack
    push(x_cord, y_cord);

    values current_cell;

    while(isEmpty() != true)
    {
        current_cell = pop();
        if(current_cell.x == x_end && current_cell.y == y_end)
        {
            printf("maze traversed okay\n");
            printf("End of maze location: %d %d\n", current_cell.x, current_cell.y);
            maze[x_end][y_end] = 'e';
            maze[x_cord][y_cord] = 'o';
            printMaze(maze, length, breadth);
            s.top = -1;
        }

        else
        { 
            if(maze[current_cell.x][current_cell.y] != '*') {

                int x_A = current_cell.x;
                int y_A = current_cell.y;

                if(checkVal(x_A, y_A, theList) == false)
                {
                    maze[current_cell.x][current_cell.y] = 'V';
                    theList = createMarker(x_A, y_A, theList);

                    push(current_cell.x-1, current_cell.y);//west
                    push(current_cell.x+1, current_cell.y);//east
                    push(current_cell.x, current_cell.y-1); //north
                    push(current_cell.x, current_cell.y+1);//south

                    //push the cell on the east
                    //push the cell on west
                    //push the cell on north
                    //push the cell on south
                }
            }
        }
    }


    // values r_val = pop();
    // printf("values popped are: %d %d\n", r_val.x, r_val.y);

}


void push(int x_pos, int y_pos)
{
    if(s.top == 1000 - 1)
    {
        printf("stack is full\n");
        return;
    }

    else
    {
        //printf("coordinates to be pushed: %d %d\n", x_pos, y_pos);
        s.top = s.top + 1;
        s.stk[s.top][0] = x_pos;
        s.stk[s.top][1] = y_pos;
        //printf("s.top is: %d\n", s.top);
    }

    return;
}

values pop()
{
    values val;
    if(s.top == -1) {
        printf("stack is empty\n");
        val.x = -1;
        val.y = -1;
    }

    else {
         val.x = s.stk[s.top][0];
         val.y = s.stk[s.top][1];
         //printf("s: %d %d\n",s.stk[s.top][0], s.stk[s.top][1] );
         //printf("%d %d\n", val.x, val.y);
         s.top = s.top - 1;
         
    }
    return val;
}


bool isEmpty()
{
    if(s.top == -1)
    {
        return true;
    }

    else
    {
        return false;
    }
}

bool checkVal(int x, int y, Marker * theList)
{
    Marker * temp = NULL;
    temp = theList;

    if(theList == NULL)
    {
        return false;
    }

    while(temp != NULL)
    {
        if(temp->x == x && temp->y == y)
        {
            return true;
        }
        temp = temp->next;
    }

    return false;

}

//this will create the marker and add it to the front of the list
Marker * createMarker(int x, int y, Marker * theList)
{
    Marker * temp = malloc(sizeof(Marker));

    temp->x = x;
    temp->y = y;
    temp->next = NULL;

    if(theList == NULL) {
        theList = temp;
        return theList;
    }

    while(theList->next != NULL)
    {
        theList = theList->next;
    }
    theList->next = temp;
    return theList;

}


void printMaze(char ** maze, int length, int breadth)
{
    int m_counter = 0;
    int mul_num = 0;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < breadth; j++)
        {
            printf("%c", maze[i][j]);
            

            if(m_counter == mul_num + (length - 1))
            {
                mul_num = (length) + mul_num;
                //printf("mul_num: %d\n", mul_num);
                printf("\n");
            }
            m_counter++;
        }
    }

}




