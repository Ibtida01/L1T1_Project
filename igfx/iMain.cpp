#include"iGraphics.h"
#include<windows.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#include"customlib.h"
#define ENEMYCT 5//ENEMYCT=# of enemies to be spawned
#define screenwidth 800
#define screenheight 600
/*____________________________FUNCTIONS USER DEFINED______________________________________________________*/


/*_____________________________________FUNCTION POINTERS_________________________________________________________________*/

//void (*enm_spwn_fnc_ptr)(void)=&enemy_spawn;
/*_________________________________________ESSENTIAL FUNCTIONS__________________________________________________________*/

void iDraw()
{
    iClear();
    //iShowBMP(0,0,"D:\\igfx\\Assets\\BMPs\\milkyway.bmp");
    iSetColor(20, 200, 200);
//    iFilledCircle(x, y, r);
    iShowBMP2( x, y, "D:\\igfx\\Assets\\BMPs\\spcship.bmp",0);
    iSetColor(0,0,255);
    iRectangle(0, 0, screenwidth-10, screenheight-10);
    iSetColor(20, 200, 0);
    iText(40, 40, "Welcome to spaceshooter");
    if(missile[i].pressed_s==true)
    {
        iSetColor(255,0,0);
        iShowBMP2(missile[i].missilex,missile[i].missiley,"D:\\igfx\\Assets\\BMPs\\missile.bmp",0);
        missile[i].missiley+=15;
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
    iSetColor(255,0,0);
    for(int i=0; i<ENEMYCT; i++)
    {
        if(tango[i].dead==false)
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
    if(gameover==true)
    {
        iShowBMP(138,20,"D:\\igfx\\Assets\\BMPs\\gameover.bmp");
    }
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my)
{
    printf("x = %d, y= %d\n",mx,my);
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
        //place your codes here
        //	printf("x = %d, y= %d\n",mx,my);
        x += 10;
        y += 10;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here
        x -= 10;
        y -= 10;
    }
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
        gameover=true;
        sleep(3);
    }
}


int main()
{
    set_environment_values();
    //stars will spawn
    set_enemy_info_values();
    //enemies will spawn
    iSetTimer(25,changes);
    //if(tango_index>ENEMYCT)
    //iPauseTimer(timer[i]);
    //place your own initialization codes here.
    iInitialize(screenwidth, screenheight, "gamescreen");
    return 0;
}
