#ifndef CUSTOMLIB_H_INCLUDED
#define CUSTOMLIB_H_INCLUDED
#define ENEMYCT 5//ENEMYCT=# of enemies to be spawned
#define screenwidth 1920
#define screenheight 1080

/*GLOBAL VARIABLES DECLARATIONS AS EXTERN*/
extern int i;
extern int x = screenwidth/2, y = screenheight/2, r = 20;//x ,y co ordinate of player,s spaceship
extern int enemy_to_be_spawned;
extern bool afterburner=false;//default...when hit arrow up,it will be true
extern int afterburner_x,afterburner_y;
extern int ran;//random value
extern bool gameover=false;
extern struct enemy_info
{
    int health=100;//100 initially
    int spawn_loc_x;
    int spawn_loc_y;//spwan location (x,y)
    int tango_index;
    bool dead;//when health>0 dead=false//when dead==true then stop spawning enemy
} tango[ENEMYCT];
extern struct player
{
    int health;//initially 100
    bool special_ability;//true when player got streak
    bool dead;//if true,show "gameover.bmp"
} p1;
extern struct environment
{
    int stars_x;
    int stars_y;
    int stars_index;
    bool showspaceflyby;
} stars[50];
extern struct missileinfo{
    int missileno;//missile_no
    int missilex;
    int missiley;
    bool collision;
    bool pressed_s;//shoot button pressed or not?
} missile[3];

void shoot(void);
void changes();
void set_enemy_info_values();
void set_environment_values();



#endif // CUSTOMLIB_H_INCLUDED
