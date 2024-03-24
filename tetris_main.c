/* Librarys */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <Windows.h> // WARNING: this code uses windows sleep function

/* Defines */

#define RES_X 40
#define RES_Y 23
#define PLAYGROUND_START_X 5
#define PLAYGROUND_END_X 34
#define CLEAR_COMMAND "cls" // WARNING; we used windows commands in changing color part too
#define TRUE 1
#define COLOR_SIZE 9

/* Functions */

int random(int n)
{
    // Seed the random number generator with the current time
    srand(time(NULL));

    int randomNumber = rand() % n; // rand() % n gives a number between 0 and n-1

    return randomNumber;
}

int miss_data(int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int last_view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2])
{
    /* check we lost data or not by cheacking data after and before any changing */
    int sum_data = 0;
    int last_sum_data = 0;

    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            sum_data += view_layer[j][i]; // sum of data should be stable after changing
            if (view_layer[j][i] > 1)     // we should have only 0 and 1 datas
            {
                return 1;
            }
        }
    }

    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            last_sum_data += last_view_layer[j][i];
        }
    }

    if (sum_data == last_sum_data)
    {
        return 0;
    }

    return 1;
}

void fall_a_line(int *object_layer_j_position, int *fall_time, int *round_fall_time)
{
    if (*fall_time < 25)
    {
        *fall_time = 25; // set a limit for fall time it shouldn't be zero
    }
    if (*round_fall_time < 50)
    {
        *round_fall_time = 50;
    }
    Sleep(*fall_time);                                       // wait untill fall time is reached
    *object_layer_j_position = *object_layer_j_position + 1; // move object 1 line down
}

void create_background_layer(char background_layer[RES_Y + 1][RES_X])
{
    // GUIDE: Need to declear background_layer first!
    // EX: char background_layer[RES_Y + 1][RES_X];
    for (int j = 0; j < RES_Y + 1; j++)
    {
        for (int i = 0; i < RES_X; i++)
        {
            background_layer[j][i] = ' '; // empty space for setting playground inside it
        }
    }

    for (int j = 2; j < RES_Y + 1; j++)
    {
        int i = PLAYGROUND_START_X;
        background_layer[j][i] = '|'; // lines in the left and right of playground

        i = PLAYGROUND_END_X;
        background_layer[j][i] = '|';
    }

    for (int i = PLAYGROUND_START_X + 1; i < PLAYGROUND_END_X; i++)
    {
        int j = RES_Y;
        background_layer[j][i] = '_'; // line under playground
    }
}

void show_background_layer(char background_layer[RES_Y + 1][RES_X], int score, int fall_time, int round_fall_time, int next_object_layer[4][4][4], int next_shape_rotation)
{
    for (int j = 0; j < RES_Y + 1; j++)
    {
        for (int i = 0; i < RES_X; i++)
        {
            printf("%c", background_layer[j][i]);
        }
        /*output texts*/
        switch (j)
        {
        case 1:
            printf("   score: %d", score); // showing score in line j=1
            break;
        case 5:
            printf("   next shape : "); // creating the next shape line by line in background from j=5 to j=9
            break;
        case 6:
            printf("                   ");
            for (int i = 0; i < 4; i++)
            {
                if (next_object_layer[next_shape_rotation][0][i])
                {
                    printf("[]");
                }
                else
                {
                    printf("  ");
                }
            }
            break;
        case 7:
            printf("                   ");
            for (int i = 0; i < 4; i++)
            {
                if (next_object_layer[next_shape_rotation][1][i])
                {
                    printf("[]");
                }
                else
                {
                    printf("  ");
                }
            }
            break;
        case 8:
            printf("                   ");
            for (int i = 0; i < 4; i++)
            {
                if (next_object_layer[next_shape_rotation][2][i])
                {
                    printf("[]");
                }
                else
                {
                    printf("  ");
                }
            }
            break;
        case 9:
            printf("                   ");
            for (int i = 0; i < 4; i++)
            {
                if (next_object_layer[next_shape_rotation][3][i])
                {
                    printf("[]");
                }
                else
                {
                    printf("  ");
                }
            }
            break;
        case 11:
            printf("   round fall time: %d", round_fall_time); // showning fall time
            break;
        case 13:
            printf("   current fall time: %d", fall_time);
            break;
        case 16:
            printf("   GUIDE: "); // introducing keybinds
            break;
        case 17:
            printf("-W- : switch shape ");
            break;
        case 18:
            printf("-S- : increase falling speed ");
            break;
        case 19:
            printf("-A- : move left ");
            break;
        case 20:
            printf("-D- : move right ");
            break;
        case 21:
            printf("-P- : pause the game (you can change number of rotation per press here)");
            break;
        case 22:
            printf("-SPACE BAR- : rotate");
            break;
        }

        printf("\n");
    }
}

void copy_background_layer(char cpy_background_layer[RES_Y + 1][RES_X], char background_layer[RES_Y + 1][RES_X])
{
    for (int j = 0; j < RES_Y + 1; j++)
    {
        for (int i = 0; i < RES_X; i++)
        {
            cpy_background_layer[j][i] = background_layer[j][i]; // copy by object
        }
    }
}

void create_playground_layer(int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2])
{
    // GUIDE: Need to declear playground_layer first!
    // EX: int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1)/2];
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            playground_layer[j][i] = 0; // creating a base playground in 0 and 1
        }
    }
}

void show_playground_layer(int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2])
{
    printf("--------------------------------\n");
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < (PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2; i++)
        {
            printf("%d", playground_layer[j][i]); // show as number matris
        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

void copy_playground_layer(int cpy_playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2])
{
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            cpy_playground_layer[j][i] = playground_layer[j][i]; // copy by object
        }
    }
}

void create_object_layer(int object_layer[4][4][4], int shape_code)
{
    /* create all rotatons of a shape by its code */
    // GUIDE: Need to declear object_layer first!
    // EX: int object_layer[4][4][4];
    // GUIDE: Dont forget for movement you need declear object_layer_i_pos and object_layer_j_pos
    // EX: int object_layer_i_pos = 0; //---------------------------------------------------------------should apply randomize to this variable
    // EX: int object_layer_j_pos = 0;
    // GUIDE: Objects should defined like this
    // EX: {{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},{{0,0,0,0},{0,0,1,0},{0,0,0,0},{0,0,0,0}},{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}},{{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}}}

    if (shape_code == 0)
    {
        // virtual form
        //  [][][][]

        // all rotations matris form
        //  0000    0100    0000    0100
        //  1111    0100    1111    0100
        //  0000    0100    0000    0100
        //  0000    0100    0000    0100

        int tmp_object_layer[4][4][4] = {{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}, {{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
    else if (shape_code == 1)
    {
        // [][]
        // [][]

        // 0110    0110    0110    0110
        // 0110    0110    0110    0110
        // 0000    0000    0000    0000
        // 0000    0000    0000    0000

        int tmp_object_layer[4][4][4] = {{{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 1, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
    else if (shape_code == 2)
    {
        //   []
        // [][][]

        // 0100    1000    1110    0100
        // 1110    1100    0100    1100
        // 0000    1000    0000    0100
        // 0000    0000    0000    0000

        int tmp_object_layer[4][4][4] = {{{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 0, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
    else if (shape_code == 3)
    {
        //   [][]
        // [][]

        // 0110    1000    0110    1000
        // 1100    1100    1100    1100
        // 0000    0100    0000    0100
        // 0000    0000    0000    0000

        int tmp_object_layer[4][4][4] = {{{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
    else if (shape_code == 4)
    {
        // [][]
        //   [][]

        // 1100    0100    1100    0100
        // 0110    1100    0110    1100
        // 0000    1000    0000    1000
        // 0000    0000    0000    0000

        int tmp_object_layer[4][4][4] = {{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 0, 0}, {1, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
    else if (shape_code == 5)
    {
        // []
        // [][][]

        // 1000    1100    1110    0100
        // 1110    1000    0010    0100
        // 0000    1000    0000    1100
        // 0000    0000    0000    0000

        int tmp_object_layer[4][4][4] = {{{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 1, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{0, 1, 0, 0}, {0, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
    else if (shape_code == 6)
    {
        // [][][]
        // []

        // 1110    1100    0010    1000
        // 1000    0100    1110    1000
        // 0000    0100    0000    1100
        // 0000    0000    0000    0000

        int tmp_object_layer[4][4][4] = {{{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}}, {{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, {{1, 0, 0, 0}, {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}}};
        for (int shape_rotation = 0; shape_rotation < 4; shape_rotation++)
        {
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    object_layer[shape_rotation][j][i] = tmp_object_layer[shape_rotation][j][i];
                }
            }
        }
    }
}

void show_object_layer(int object_layer[4][4][4], int shape_rotation)
{
    printf("--------------------------------\n");
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            printf("%d", object_layer[shape_rotation][j][i]); // show a rotation of matris by shape_rotation
        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

void copy_object_layer(int cpy_object_layer[4][4][4], int object_layer[4][4][4], int shape_rotation)
{
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            cpy_object_layer[shape_rotation][j][i] = object_layer[shape_rotation][j][i]; // copy by object
        }
    }
}

void show_object_layer_ij_position(int object_layer_i_position, int object_layer_j_position)
{
    printf("--------------------------------\n");
    printf("i position for object layer is : %d\n", object_layer_i_position); // for development goals
    printf("j position for object layer is : %d\n", object_layer_j_position);
    printf("--------------------------------\n");
}

void create_view_layer(int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int object_layer[4][4][4], int object_layer_i_position, int object_layer_j_position, int shape_rotation)
{
    /* create a view layer by concatting object and playground */
    // GUIDE: Need to declear view_layer first!
    // EX: int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1)/2];
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            if ((((object_layer_j_position <= j) && (j < (object_layer_j_position + 4))) && (((object_layer_i_position <= i) && (i < (object_layer_i_position + 4))))))
            {
                view_layer[j][i] = (playground_layer[j][i] + object_layer[shape_rotation][j - object_layer_j_position][i - object_layer_i_position]); // if we are in object zone view layer should be object + playground
            }
            else
            {
                view_layer[j][i] = playground_layer[j][i]; // if we arent view layer is just playground
            }
        }
    }
}

void show_view_layer(int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2])
{
    printf("--------------------------------\n");
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < (PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2; i++)
        {
            printf("%d", view_layer[j][i]);
        }
        printf("\n");
    }
    printf("--------------------------------\n");
}

void cls()
{
    system(CLEAR_COMMAND);
}

void rotate_object_layer(int *shape_rotation, int num_rotation)
{
    *shape_rotation += num_rotation; // if we add one to its rotation we recive next rotation but be careful shape rotation is  0 to 3
    *shape_rotation %= 4;
}

void switch_shape(int *shape_rotation, int *next_shape_rotation, int *shape_code, int *next_shape_code)
{
    /* object layer created by two code shape code and its rotation if we replace that two and create object layer again we replace the shape */
    // GUIDE: Need to create object layers again after switching
    int tmp_shape_code = *shape_code;
    *shape_code = *next_shape_code;
    *next_shape_code = tmp_shape_code;

    int tmp_shape_rotation = *shape_rotation;
    *shape_rotation = *next_shape_rotation;
    *next_shape_rotation = tmp_shape_rotation;
}

void input_movement(int *object_layer_i_position, int *object_layer_j_position, int *fall_time, int object_layer[4][4][4], int *shape_rotation, int *next_shape_rotation, int *shape_code, int *next_shape_code, int next_object_layer[4][4][4], int *num_rotation)
{
    char player_direction;
    int trash;

    player_direction = getch(); // use getch for keyboard inputs without needs to press ENTER or anything else to import that

    switch (player_direction)
    {
    case 119: // ASCII value for 'W' key
    case 87:
        switch_shape(shape_rotation, next_shape_rotation, shape_code, next_shape_code);
        create_object_layer(object_layer, *shape_code);           // for switch_shape function
        create_object_layer(next_object_layer, *next_shape_code); // for switch_shape function
        break;
    case 115: // ASCII value for 'S' key
    case 83:
        if (*fall_time > 200)
        {
            *fall_time -= 100; // fall faster
        }
        else if (*fall_time > 100)
        {
            *fall_time -= 50;
        }
        else if (*fall_time > 50)
        {
            *fall_time -= 25;
        }

        break;
    case 97: // ASCII value for 'A' key
    case 65:
        *object_layer_i_position -= 1; // move left
        break;
    case 100: // ASCII value for 'D' key
    case 68:
        *object_layer_i_position += 1; // move right
        break;
    case 112: // ASCII value for 'P' key
    case 80:
        printf("\nPress -4- to continue game without any changes...");
        printf("\nif you want change rotation enter number between 0 to 3");
        printf("\n0 -> no rotation , 1 -> 90 degress , 2 -> 180 degress , 3 -> 270 degress\n");
        scanf("%d", &trash); // stop code until press ENTER
        switch (trash)
        {
        case 0:
        case 1:
        case 2:
        case 3:
            *num_rotation = trash;
            break;

        default:
            break;
        }
        break;
    case 32: // ASCII value for 'SPACE' key
        rotate_object_layer(shape_rotation, *num_rotation);
        break;
    default:
        printf("\nInvalid key pressed.");
    }
}

void append_object_playground(int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int object_layer[4][4][4], int object_layer_i_position, int object_layer_j_position, int shape_rotation)
{
    /* after an object touch the ground we need a function to append that object to playground and next object come in next round */
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            if ((((object_layer_j_position <= j) && (j < (object_layer_j_position + 4))) && (((object_layer_i_position <= i) && (i < (object_layer_i_position + 4))))))
            {
                playground_layer[j][i] = (playground_layer[j][i] + object_layer[shape_rotation][j - object_layer_j_position][i - object_layer_i_position]); // in object zone playground is sum of object and playground
            }
        }
    }
}

void delete_line(int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int last_playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int j)
{
    /* delete a line and replace it with  previous*/
    // WARNING: this function change playground_layer and last_playground_layer at the same time
    for (j; j >= 0; j--) // we should go from down to up it means if we have more than one completed lines first delete lowest compleated line then go up it
    {
        if (j == 0)
        {
            for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
            {
                playground_layer[j][i] = 0; // if its first line after deleting it will be just zeroes
            }
        }
        else
        {
            for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
            {
                playground_layer[j][i] = last_playground_layer[j - 1][i]; // if its  not the first line dealeating mean replacing it with previous line
            }
        }
    }
    copy_playground_layer(last_playground_layer, playground_layer); // delete line in playground_layer and last_playground_layer
}

void fullline_score_check(int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int last_playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int *score, int *round_fall_time, char flasher_point_color_1[COLOR_SIZE], char flasher_point_color_2[COLOR_SIZE], char defult_color[COLOR_SIZE])
{
    /* cheack if we have full line give score to player */
    int line_status = 1;
    int number_full_lines = 0;
    for (int j = 0; j < (RES_Y); j++)
    {
        line_status = 1;
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            line_status = line_status && playground_layer[j][i]; // anding objects in a line for cheack full line
        }
        if (line_status)
        {
            number_full_lines += 1;
            delete_line(playground_layer, last_playground_layer, j); // if its a full line delete it and give the score
        }
    }
    *score += ((number_full_lines) * 100); // add line score
    if (number_full_lines > 0)
    {
        *round_fall_time -= 10;        // increace falling speed to make game harder after compleating full line
        system(flasher_point_color_1); /* flasher effects */
        Sleep(50);
        system(flasher_point_color_2);
        Sleep(50);
        system(defult_color);
    }
}

int maxline_check(int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2])
{
    int j = 3; // line 3 is deadline
    for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
    {
        // if (view_layer[j][i] > 2)
        // {
        //     return 1;
        // }
        if (playground_layer[j][i] > 0) // if we have a value in playground layer at deadline game is over
        {
            return 1;
        }
    }
    return 0;
}

void append_view_background(int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2], char background_layer[RES_Y + 1][RES_X])
{
    for (int j = 0; j < RES_Y; j++)
    {
        for (int i = 0; i < ((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2); i++)
        {
            if (view_layer[j][i])
            {
                background_layer[j][(2 * i) + 1 + PLAYGROUND_START_X] = '['; // each 1 in playground is "[]"
                background_layer[j][(2 * i) + 2 + PLAYGROUND_START_X] = ']';
            }
            else
            {
                background_layer[j][(2 * i) + 1 + PLAYGROUND_START_X] = ' '; // each 0 in playground is "  "
                background_layer[j][(2 * i) + 2 + PLAYGROUND_START_X] = ' ';
            }
        }
    }
}

void gameover_outro(char lunch_color[COLOR_SIZE])
{
    cls();
    system(lunch_color);

    printf("                                                                       :                                                                                     \n");
    printf("                                                         ,;           t#,                     ,;                                                             \n");
    printf("          .Gt                                          f#i           ;##W.                  f#i j.                                                           \n");
    printf("         j#W:            ..           ..       :     .E#t           :#L:WE                .E#t  EW,                                                          \n");
    printf("       ;K#f             ;W,          ,W,     .Et    i#W,           .KG  ,#D  t      .DD. i#W,   E##j                                                         \n");
    printf("     .G#D.             j##,         t##,    ,W#t   L#D.            EE    ;#f EK:   ,WK. L#D.    E###D.                                                       \n");
    printf("    j#K;              G###,        L###,   j###t :K#Wfff;         f#.     t#iE#t  i#D :K#Wfff;  E#jG#W;                                                      \n");
    printf("  ,K#f   ,GD;       :E####,      .E#j##,  G#fE#t i##WLLLLt        :#G     GK E#t j#f  i##WLLLLt E#t t##f                                                     \n");
    printf("   j#Wi   E#t      ;W#DG##,     ;WW; ##,:K#i E#t  .E#L             ;#L   LW. E#tL#i    .E#L     E#t  :K#E:                                                   \n");
    printf("    .G#D: E#t     j###DW##,    j#E.  ##f#W,  E#t    f#E:            t#f f#:  E#WW,       f#E:   E#KDDDD###i                                                  \n");
    printf("      ,K#fK#t    G##i,,G##,  .D#L    ###K:   E#t     ,WW;            f#D#;   E#K:         ,WW;  E#f,t#Wi,,,                                                  \n");
    printf("        j###t  :K#K:   L##, :K#t     ##D.    E#t      .D#;            G#t    ED.           .D#; E#t  ;#W:                                                    \n");
    printf("         .G#t ;##D.    L##, ...      #G      ..         tt             t     t               tt DWi   ,KK:                                                   \n");

    Sleep(5000);
    cls();
}
void intro(char lunch_color[COLOR_SIZE], char default_color[COLOR_SIZE])
{
    cls();
    system(lunch_color);

    printf("sdSS_SSSSSSbs    sSSs  sdSS_SSSSSSbs   .S_sSSs     .S    sSSs  \n");
    printf("YSSS~S#SSSSSP   d##SP  YSSS~S#SSSSSP  .SS~YS##b   .SS   d##SP  \n");
    printf("     S#S       d#S'         S#S       S#S   `S#b  S#S  d#S'    \n");
    printf("     S#S       S#S          S#S       S#S    S#S  S#S  S#|     \n");
    printf("     S&S       S&S          S&S       S#S    d*S  S&S  S&S     \n");
    printf("     S&S       S&S_Ss       S&S       S&S   .S*S  S&S  Y&Ss    \n");
    printf("     S&S       S&S~SP       S&S       S&S_sdSSS   S&S  `S&&S   \n");
    printf("     S&S       S&S          S&S       S&S~YSY#b   S&S    `S*S  \n");
    printf("     S*S       S*b          S*S       S*S   `S#b  S*S     l*S  \n");
    printf("     S*S       S*S.         S*S       S*S    S#S  S*S    .S*P  \n");
    printf("     S*S        SSSbs       S*S       S*S    S&S  S*S  sSS*S   \n");
    printf("     S*S         YSSP       S*S       S*S    SSS  S*S  YSS'    \n");
    printf("     SP                     SP        SP          SP           \n");
    printf("     Y                      Y         Y           Y            \n");
    printf("                                                               \n");
    printf("                                                               \n");

    Sleep(2000);
    cls();
    system(default_color);
}

void color_score_setter(char default_color[COLOR_SIZE], char flasher_point_color_1[COLOR_SIZE], char flasher_point_color_2[COLOR_SIZE], int score)
{
    /* set color by score in 5 ranges */
    if (score < 1000)
    {
        char tmp_default_color[COLOR_SIZE] = "color 07";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            default_color[i] = tmp_default_color[i];
        }
        char tmp_flasher_point_color_1[COLOR_SIZE] = "color 0b";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            flasher_point_color_1[i] = tmp_flasher_point_color_1[i];
        }
        char tmp_flasher_point_color_2[COLOR_SIZE] = "color 0d";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            flasher_point_color_2[i] = tmp_flasher_point_color_2[i];
        }
    }
    else if (score < 10000)
    {

        char tmp_default_color[COLOR_SIZE] = "color 80";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            default_color[i] = tmp_default_color[i];
        }
        char tmp_flasher_point_color_1[COLOR_SIZE] = "color 8b";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            flasher_point_color_1[i] = tmp_flasher_point_color_1[i];
        }
        char tmp_flasher_point_color_2[COLOR_SIZE] = "color 8d";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            flasher_point_color_2[i] = tmp_flasher_point_color_2[i];
        }
    }
    else
    {

        char tmp_default_color[COLOR_SIZE] = "color 0a";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            default_color[i] = tmp_default_color[i];
        }
        char tmp_flasher_point_color_1[COLOR_SIZE] = "color 0b";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            flasher_point_color_1[i] = tmp_flasher_point_color_1[i];
        }
        char tmp_flasher_point_color_2[COLOR_SIZE] = "color 0d";
        for (int i = 0; i < COLOR_SIZE; i++)
        {
            flasher_point_color_2[i] = tmp_flasher_point_color_2[i];
        }
    }
    system(default_color);
}

/* Main */

int main()
{
    /* initialize colors */
    char lunch_color[COLOR_SIZE] = {"color e4"};
    char default_color[COLOR_SIZE] = {"color e0"};
    char flasher_point_color_1[COLOR_SIZE] = {"color eb"};
    char flasher_point_color_2[COLOR_SIZE] = {"color ed"};
    /* play intro */
    intro(lunch_color, default_color);
    /* SETUP GAME and initialize valuses*/
    char background_layer[RES_Y + 1][RES_X];
    create_background_layer(background_layer); // show_background_layer(background_layer);
    int shape_rotation = 0;
    int num_rotation = 1;
    int last_shape_rotation = 0;
    int next_shape_rotation = 0;

    int playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2];
    create_playground_layer(playground_layer); // show_playground_layer (playground_layer);

    int object_layer_i_position = 0;
    int object_layer_j_position = 0; // show_object_layer_ij_position (object_layer_i_position , object_layer_j_position);
    int object_layer[4][4][4];
    int next_object_layer[4][4][4];
    int shape_code = random(7);
    int next_shape_code = random(7);
    create_object_layer(object_layer, shape_code);           // show_object_layer (object_layer);
    create_object_layer(next_object_layer, next_shape_code); // show_object_layer (object_layer);

    int view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2];
    create_view_layer(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation); // show_view_layer (view_layer);

    int sum_data = 0;

    int last_playground_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2];
    copy_playground_layer(last_playground_layer, playground_layer);
    int last_object_layer[4][4][4];
    copy_object_layer(last_object_layer, object_layer, shape_rotation);
    int last_object_layer_i_position = object_layer_i_position;
    int last_object_layer_j_position = object_layer_j_position;
    int last_view_layer[RES_Y][(PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2];
    create_view_layer(last_view_layer, last_playground_layer, last_object_layer, last_object_layer_i_position, last_object_layer_j_position, shape_rotation);

    int fall_time = 500;       // its milliseconds
    int round_fall_time = 500; // its milliseconds
    int score = 0;

    /* PLAY GAME */
    show_background_layer(background_layer, score, fall_time, round_fall_time, next_object_layer, next_shape_rotation);
    /* each object loop */
    while (TRUE)
    {
        /* set color */
        color_score_setter(default_color, flasher_point_color_1, flasher_point_color_2, score);

        fall_time = round_fall_time;
        /* reset object position */
        object_layer_i_position = (((PLAYGROUND_END_X - PLAYGROUND_START_X - 1) / 2) / 2);
        last_object_layer_i_position = 0;
        object_layer_j_position = 0;
        last_object_layer_j_position = 0;
        copy_playground_layer(last_playground_layer, playground_layer);
        copy_object_layer(last_object_layer, object_layer, shape_rotation);

        /* create new object */
        shape_code = next_shape_code;
        shape_rotation = next_shape_rotation;
        next_shape_code = random(7);
        create_object_layer(object_layer, shape_code);
        create_object_layer(next_object_layer, next_shape_code);

        create_view_layer(last_view_layer, last_playground_layer, last_object_layer, last_object_layer_i_position, last_object_layer_j_position, shape_rotation);
        /* each object falling loop */
        while (TRUE)
        {
            /* last move datas */
            copy_playground_layer(last_playground_layer, playground_layer);
            for (int i = 0; i < 4; i++)
                copy_object_layer(last_object_layer, object_layer, i);
            last_object_layer_i_position = object_layer_i_position;
            last_object_layer_j_position = object_layer_j_position;
            last_shape_rotation = shape_rotation;
            if (kbhit())
            {
                /* give input */
                input_movement(&object_layer_i_position, &object_layer_j_position, &fall_time, object_layer, &shape_rotation, &next_shape_rotation, &shape_code, &next_shape_code, next_object_layer, &num_rotation);
                create_view_layer(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation); /* miss data need this variabels be updated */
                if (miss_data(view_layer, last_view_layer))                                                                                      /* if its not correct step back */
                {
                    /* go to before move */
                    copy_playground_layer(playground_layer, last_playground_layer);
                    for (int i = 0; i < 4; i++)
                        copy_object_layer(last_object_layer, object_layer, i);
                    object_layer_i_position = last_object_layer_i_position;
                    object_layer_j_position = last_object_layer_j_position;
                    shape_rotation = last_shape_rotation;
                }
                /* show movement */
                cls();
                create_view_layer(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation);
                append_view_background(view_layer, background_layer);
                show_background_layer(background_layer, score, fall_time, round_fall_time, next_object_layer, next_shape_rotation);
            }
            else
            {
                /* time fall */
                create_view_layer(last_view_layer, last_playground_layer, last_object_layer, last_object_layer_i_position, last_object_layer_j_position, last_shape_rotation);
                fall_a_line(&object_layer_j_position, &fall_time, &round_fall_time);
                create_view_layer(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation);
                if (miss_data(view_layer, last_view_layer)) /* if we cant fall we should append object to playground */
                {
                    /* go to before fall */
                    object_layer_i_position = last_object_layer_i_position;
                    object_layer_j_position = last_object_layer_j_position;
                    create_view_layer(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation);
                    append_object_playground(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation);
                    break;
                }
                cls();
                // show_view_layer (view_layer);
                append_view_background(view_layer, background_layer);
                show_background_layer(background_layer, score, fall_time, round_fall_time, next_object_layer, next_shape_rotation);
            }
            /* check score */
            copy_playground_layer(last_playground_layer, playground_layer);
            fullline_score_check(playground_layer, last_playground_layer, &score, &round_fall_time, flasher_point_color_1, flasher_point_color_2, default_color);
            create_view_layer(view_layer, playground_layer, object_layer, object_layer_i_position, object_layer_j_position, shape_rotation);
            create_view_layer(last_view_layer, last_playground_layer, last_object_layer, last_object_layer_i_position, last_object_layer_j_position, shape_rotation);
            /* check game over */
            if (maxline_check(view_layer, playground_layer))
            {
                gameover_outro(lunch_color);
            }
        }
    }

    scanf("%c");
    return 0;
}
