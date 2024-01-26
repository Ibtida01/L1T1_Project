#include"iGraphics.h"
#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#define ENEMYCT 15//ENEMYCT=# of enemies to be spawned
#define screenwidth 1920
#define screenheight 1080
/*____________________________GLOBAL VARIABLE LIST___________________________________________________________________*/
int timerindex=1;
int index=0;//for enemy count
int mcounter=0,ecounter=0;//counter variables for missile and enemy
int missile_no[5];//serial no of missiles shot
int enemy_no[10];//serial no of enemy engaged
int i=0;//missile counter
int x = screenwidth/2, y = screenheight/2, r = 20;//x ,y co ordinate of player,s spaceship
int enemy_to_be_spawned;
bool afterburner=false;//default...when hit arrow up,it will be true
int afterburner_x,afterburner_y;
int ran;//random value
bool gameover=false;
bool collision=false;
int j,k;
int mode=1;
/*
mode=1 game
2 menu
3 gameover
4 show credits and highscore
*/
int maxscore=0;
struct enemy_info
{
    int health=100;//100 initially
    int spawn_loc_x;
    int spawn_loc_y;//spwan location (x,y)
    int tango_index;
    bool dead;//when health>0 dead=false//when dead==true then stop spawning enemy
} tango[ENEMYCT];
int DEIS;//Dead Enemy Index Saver
bool DEIS_saved=false;
struct player
{
    int health=100;//initially 100
    bool special_ability;//true when player got streak
    bool dead;//if true,show "gameover.bmp"
    int score;
} p1;
struct environment
{
    int stars_x;
    int stars_y;
    int stars_index;
    bool showspaceflyby;
} stars[50];
struct missileinfo
{
    int missileno;//missile_no
    int missilex;
    int missiley;
    bool show;
    bool pressed_s;//shoot button pressed or not?
} missile;

/*
	function iDraw() is called again and again by the system.

	*/
/*____________________________FUNCTIONS USER DEFINED______________________________________________________*/

void newgame()
{
    mode=1;
    p1.score=0;
    p1.health=100;
    p1.dead=false;
}
void Show_credits_highscores()
{
    mode=4;
}
void highscore()
{
    FILE* fp=fopen("D:\\igfx\\score.txt","a");
    if(fp!=NULL)
    {
        int maxscore;
        //fscanf("score.txt","%d",&maxscore);
        if(p1.score>maxscore)
            fprintf(fp,"highscore=%d\n",p1.score);
    }
    fclose(fp);
}
void shoot(void)
{

    missile.missileno++;
    missile.missilex=x,missile.missiley=y+110;
    //shooting will start from the spaceship's position
    missile.pressed_s=true;
}
void collisiondetection(int indx)
{
    tango[indx].spawn_loc_y=screenheight+rand()%(screenheight/2);
    for(int i=0;i<ENEMYCT;i++)
    {
        if(tango[i].tango_index==DEIS)
            tango[i].dead=true;
    }
    int x=++tango[indx].tango_index;
    for(int i=0;i<ENEMYCT;i++)
    {
        if(tango[i].tango_index==x)
            tango[i].health=true;
    }
    p1.score+=100;
}
void changes()
{
    for (int i=0; i<50; i++)
    {
        stars[i].stars_y-=20;
        if(stars[i].stars_y<=0)
        {
            stars[i].stars_y=screenheight+rand()%(screenheight/2);
        }
        stars[i].stars_index++;
        if(stars[i].stars_index>=50)
            stars[i].stars_index=0;
    }
    for (int i=0; i<ENEMYCT/2; i++)
    {
        tango[i].spawn_loc_y-=10;
        if(tango[i].spawn_loc_y<=0)
        {
            tango[i].spawn_loc_y=screenheight+rand()%(screenheight/2);
            tango[i].spawn_loc_x=
            ++tango[i].tango_index;
        }
        printf("tango[%d].x=%d ,y=%d, index=%d\n",i,tango[i].spawn_loc_x,tango[i].spawn_loc_y,tango[i].tango_index);
        //ei[index++]=tango[i].tango_index;
        //if(tango[i].tango_index>=ENEMYCT)
        //tango[i].tango_index=0;
        //now detecting spaceship and enemy collison
        for(int j=0; j<ENEMYCT; j++)
        {
            if(abs(x-tango[j].spawn_loc_x)<=128 && abs(y-tango[j].spawn_loc_y)<=128 && tango[j].tango_index>=0 )
            {
                collision=true;
                p1.health-=10;
                if(p1.health<=0)
                {
                    p1.dead=true;
                    mode=3;
                }
                // PlaySound("explosion.wav",NULL,SND_ASYNC);
            }
            if(abs(missile.missilex-tango[j].spawn_loc_x)<=64 && abs(missile.missiley-tango[j].spawn_loc_y)<=64)
            {
                if(DEIS_saved==false)
                    DEIS=tango[i].tango_index;
                collisiondetection(i);
            }
        }
    }
    //updating player data
    p1.score+=1;
    if(p1.score>maxscore && mode==1)
    {
        highscore();
    }

}
void set_enemy_info_values()
{
    for(int i=0; i<ENEMYCT; i++)
    {
        tango[i].spawn_loc_y=screenheight+rand()%(screenheight/2);
        tango[i].spawn_loc_x=20+rand()%screenwidth;
        tango[i].tango_index=i;//if ENEMYCT=5 then tango index will be between 0 & 4 inclusive
        tango[i].dead=false;
    }
}
void set_environment_values()
{
    for(int i=0; i<50; i++)
    {
        stars[i].stars_y=screenheight+rand()%(screenheight/2);
        stars[i].stars_x=50+rand()%screenwidth;
        stars[i].stars_index=i;
        stars[i].showspaceflyby=true;
    }
}

void set_player_values()
{
    p1.score=0;
    p1.dead=false;
    p1.health=100;
}

/*_____________________________________FUNCTION POINTERS_________________________________________________________________*/

//void (*enm_spwn_fnc_ptr)(void)=&enemy_spawn;
/*_________________________________________ESSENTIAL FUNCTIONS__________________________________________________________*/

void pausegame()
{
    mode=2;
    iPauseTimer(timerindex);

}
void iDraw()
{
    iClear();
    iSetColor(20, 200, 200);
//    iFilledCircle(x, y, r);
    iShowBMP2( x, y, "D:\\igfx\\Assets\\BMPs\\spcship.bmp",0);
    iSetColor(0,0,255);
    iRectangle(0, 0, screenwidth-10, screenheight-10);//this is the game border
    iSetColor(20, 200, 0);
    //iText(40, 40, "Welcome to spaceshooter");
    if(missile.pressed_s==true)
    {
        iSetColor(255,0,0);
        iShowBMP2(missile.missilex,missile.missiley,"D:\\igfx\\Assets\\BMPs\\missile.bmp",0);
        missile.missiley+=15;
    }
    //generating stars fly by as environment
    iSetColor(255,255,255);
    for(int i=0; i<50; i++)
    {
        if(stars[i].showspaceflyby==true)
        {
            iFilledCircle(stars[i].stars_x,stars[i].stars_y,10,25);
        }
    }
    //spawning enemy

    for(int i=0; i<ENEMYCT; i++)
    {
        iSetColor(255,0,0);
        if(tango[i].dead==false && tango[i].tango_index!=DEIS)
        {
            iFilledCircle(tango[i].spawn_loc_x,tango[i].spawn_loc_y,25,100);
        }

    }
    //iFilledCircle(tango[tango_index++].spawn_loc_x,tango[tango_index++].spawn_loc_y,25,100);
    //afterburner ignition
    if(afterburner==true)
    {
        afterburner_x=x,afterburner_y=y-160;
        iShowBMP2(afterburner_x,afterburner_y,"D:\\igfx\\Assets\\BMPs\\afterburner_of_spcship.bmp",0);
        // afterburner=false;
    }
    /* if(missile.collision==true)
     {
         iShowBMP2(missile.missilex,missile.missiley,"D:\\igfx\\Assets\\BMPs\\explosion.bmp",0);
     }
     */
    if(collision==true)
    {
        iSetColor(255,255,255);
        iShowBMP2(x,y,"D:\\igfx\\Assets\\BMPs\\explosion.bmp",0);
    }
    if(gameover==true && p1.dead==true)
    {
        iShowBMP2(0,0,"D:\\igfx\\Assets\\BMPs\\gameover.bmp",0);
    }
    iSetColor(0,200,100);
    iText(40,40,"SCORE: ",GLUT_BITMAP_TIMES_ROMAN_24);
    char s[50];
    iText(250,40,itoa(p1.score,s,10),GLUT_BITMAP_TIMES_ROMAN_24);
    iText(40,1030,"HEALTH ",GLUT_BITMAP_TIMES_ROMAN_10);
    iText(100,1030,itoa(p1.health,s,10),GLUT_BITMAP_TIMES_ROMAN_10);
    if(mode==3)
    {
        iShowBMP2(0,0,"D:\\igfx\\Assets\\BMPs\\gameover.bmp",0);
        //sleep(2);
        //exit(0);
    }
    if(mode==2)
    {
        iShowBMP(0,0,"D:\\igfx\\Assets\\BMPs\\Menu.bmp");
    }
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my)
{
    //printf("x = %d, y= %d\n",mx,my);
    //place your codes here
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouse(int button, int state, int mx, int my)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        printf("x = %d, y= %d\n",mx,my);
    }
    //when in menu
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=494 && mx<=1064) && (my>=779 && my<=772) && mode==2)//pressed in new game region
        newgame();
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=494 && mx<=1064) && (my>=596 && my<=716) && mode==2)//pressed in Credits & high score game region
        Show_credits_highscores();
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=494 && mx<=1064) && (my>=400 && my<=530) && mode==2)//pressed in exit region
        exit(0);
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=1138 && mx<=1304) && (my>=778 && my<=900) && mode==2)//pressed in continue game region
    {
        mode=1;
        iResumeTimer(timerindex);
    }
    //NOTE:same logic and code are written for keyboard key=='r' or 'R' to resume game...but that doesnt make the game laggy
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
    if (key == 'q')
    {
        exit(0);
    }
    if( key =='s'|| key == 'S' )
    {
        shoot();
    }
    if(key == 'p' || key == 'P')
        pausegame();
    if((key == 'r' || key =='R') && mode==2)
    {
        mode=1;
        iResumeTimer(timerindex);
    }
    //place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key)
{

    if (key == GLUT_KEY_END)
    {
        exit(0);
    }
    if (key == GLUT_KEY_DOWN)
    {
        //	printf("x = %d, y= %d\n",mx,my);

        y -= 10;
        afterburner=false;
    }
    if (key == GLUT_KEY_UP)
    {
        //	printf("x = %d, y= %d\n",mx,my);
        y += 10;
        afterburner=true;
    }
    if (key == GLUT_KEY_RIGHT)
    {
        //	printf("x = %d, y= %d\n",mx,my);
        x += 10;
    }
    if (key == GLUT_KEY_LEFT)
    {
        //place your codes here
        //	printf("x = %d, y= %d\n",mx,my);
        x -= 10;
    }
    //printf("(%d,%d)",x,y);
    if( x==0 || y==0 || x==screenwidth-10 || y==screenheight-10)
    {
        collision=true;
        //sleep(3);
    }
}


int main()
{
    srand(time(NULL));
    PlaySound("theme.wav",NULL, SND_LOOP | SND_ASYNC);
    mode=1;
    //game starts
    set_environment_values();
    //stars will spawn
    set_enemy_info_values();
    //enemies will spawn
    timerindex=iSetTimer(20,changes);
    //if(tango_index>ENEMYCT)
    //iPauseTimer(timer[i]);
    //place your own initialization codes here.
    iInitialize(screenwidth, screenheight, "gamescreen");

    return 0;
}
