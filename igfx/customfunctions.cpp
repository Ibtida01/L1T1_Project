#include"iGraphics.h"
#include<windows.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#define ENEMYCT 5//ENEMYCT=# of enemies to be spawned
#define screenwidth 800
#define screenheight 600
/*____________________________GLOBAL VARIABLE LIST___________________________________________________________________*/
int i;
int x = screenwidth/2, y = screenheight/2, r = 20;//x ,y co ordinate of player,s spaceship
int enemy_to_be_spawned;
bool afterburner=false;//default...when hit arrow up,it will be true
int afterburner_x,afterburner_y;
int ran;//random value
bool gameover=false;
struct enemy_info
{
    int health=100;//100 initially
    int spawn_loc_x;
    int spawn_loc_y;//spwan location (x,y)
    int tango_index;
    bool dead;//when health>0 dead=false//when dead==true then stop spawning enemy
} tango[ENEMYCT];
struct player
{
    int health;//initially 100
    bool special_ability;//true when player got streak
    bool dead;//if true,show "gameover.bmp"
};
struct environment
{
    int stars_x;
    int stars_y;
    int stars_index;
    bool showspaceflyby;
} stars[50];
struct missileinfo{
    int missileno;//missile_no
    int missilex;
    int missiley;
    bool collision;
    bool pressed_s;//shoot button pressed or not?
} missile[3];

/*______________________________USER DEFINED FUNCTIONS___________________________*/
void shoot(void)
{
    missile[i].missileno++;
    missile[i].missilex=x,missile[i].missiley=y+110;
    //shooting will start from the spaceship's position
    missile[i].pressed_s=true;
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
    for (int i=0; i<ENEMYCT; i++)
    {
        tango[i].spawn_loc_y-=10;
        if(tango[i].spawn_loc_y<=0)
        {
            tango[i].spawn_loc_y=screenheight+rand()%(screenheight/2);
        }
        tango[i].tango_index++;
        if(tango[i].tango_index>=5)
            tango[i].tango_index=0;
    }
}
void set_enemy_info_values()
{
    for(int i=0; i<ENEMYCT; i++)
    {
        tango[i].spawn_loc_y=screenheight+rand()%(screenheight/2);
        tango[i].spawn_loc_x=20+rand()%screenwidth;
        tango[i].tango_index=rand()%10;
        tango[i].dead=false;
    }
}
void set_environment_values()
{
   for(int i=0; i<50; i++)
    {
        stars[i].stars_y=screenheight+rand()%(screenheight/2);
        stars[i].stars_x=20+rand()%screenwidth;
        stars[i].stars_index=rand()%50;
        stars[i].showspaceflyby=true;
    }
}


