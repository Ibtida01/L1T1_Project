#include"iGraphics.h"
#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include<time.h>
#include<unistd.h>
#define ENEMYCT 5//ENEMYCT=# of enemies to be spawned
#define screenwidth 1920
#define screenheight 1080
/*____________________________FUNCTION PROTOTYPES___________________________________________________________________*/
void set_player_values();
void  set_enemy_info_values();
void changes();
/*____________________________GLOBAL VARIABLE LIST___________________________________________________________________*/
int record_score;//it is the current highscore
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
int mode;
/*
mode=1 game
2 menu
3 gameover && taking player name input
4 show credits and highscore
*/
bool sound_on=true;
int maxscore=0;
struct enemy_info
{
    int health=100;//100 initially
    int spawn_loc_x;
    int spawn_loc_y;//spwan location (x,y)
    int tango_index;
    bool dead;//when health>0 dead=false//when dead==true then stop spawning enemy
} tango[ENEMYCT];
//int DEIS;//Dead Enemy Index Saver
//bool DEIS_saved=false;
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
int spawn_loc_ys[ENEMYCT];
int spawn_loc_xs[ENEMYCT];
char tango_BMP[2][36]={"D:\\igfx\\Assets\\BMPs\\tango1.bmp","D:\\igfx\\Assets\\BMPs\\tango2.bmp"};
char star_BMP[4][35]={"D:\\igfx\\Assets\\BMPs\\star1.bmp","D:\\igfx\\Assets\\BMPs\\star1.bmp","D:\\igfx\\Assets\\BMPs\\star1.bmp","D:\\igfx\\Assets\\BMPs\\star4.bmp"};
char str[100], str2[100],highest_scorer_name[100];
int len;

/*____________________________FUNCTIONS USER DEFINED______________________________________________________*/
void drawTextBox()
{
	iSetColor(150, 150, 150);
	iRectangle(700, 250, 250, 30);
}

void newgame()
{
    gameover=false;
    mode=1;
    set_player_values();
    set_enemy_info_values();
    iResumeTimer(timerindex);
    collision=false;
}
void Show_credits_highscores()
{
    mode=4;
}
void highscore()
{
    FILE* fpin=fopen("D:\\igfx\\HIGHESTSCORE.txt","r");
    fscanf(fpin,"%d",&record_score);
    fclose(fpin);
    if(p1.score>record_score){
    FILE* fpout=fopen("D:\\igfx\\HIGHESTSCORE.txt","w");
    if(fpout!=NULL)
    {
        fprintf(fpout,"%d\n",p1.score);
        record_score=p1.score;
    }
    fclose(fpout);
     }
}
void shoot(void)
{
   // PlaySound("blaster.wav",NULL,  SND_ASYNC);
    missile.missileno++;
    missile.missilex=x,missile.missiley=y+110;
    //shooting will start from the spaceship's position
    missile.pressed_s=true;
}
void collisiondetection(int indx)
{
    tango[indx].spawn_loc_y=spawn_loc_ys[indx];
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
    for (int i=0; i<ENEMYCT; i++)
    {
        tango[i].spawn_loc_y-=10;
        if(tango[i].spawn_loc_y<=0)
        {
            tango[i].spawn_loc_y=spawn_loc_ys[i];
            tango[i].spawn_loc_x=spawn_loc_xs[i]+rand()%100;
            ++tango[i].tango_index;
        }

        //now detecting spaceship and enemy collison

     if(abs(x-tango[i].spawn_loc_x)<=80 && abs(y-tango[i].spawn_loc_y)<=80 && mode==1)
            {
                collision=true;
                p1.health-=10;
                if(p1.health<=0)
                {
                    p1.dead=true;
                    mode=3;
                    gameover=true;
                    highscore();
                    iPauseTimer(timerindex);
                }
            }
            if(abs(missile.missilex-tango[i].spawn_loc_x)<=90 && abs(missile.missiley-tango[i].spawn_loc_y)<=90)
            {
                collisiondetection(i);
            }
        }
    //updating player data
    p1.score+=1;
    if(p1.score>record_score && mode==1)
    {
        highscore();
    }

}
void set_enemy_info_values()
{
    for(int i=0; i<ENEMYCT; i++)
    {
        tango[i].spawn_loc_y=screenheight+rand()%(screenheight/2);
        spawn_loc_ys[i]=tango[i].spawn_loc_y;
        tango[i].spawn_loc_x=spawn_loc_xs[i];
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
    if(mode==2)
    {
        iShowBMP(0,0,"D:\\igfx\\Assets\\BMPs\\Menu.bmp");
    }
    if(mode==1)
        iShowBMP2( x, y, "D:\\igfx\\Assets\\BMPs\\spcship.bmp",0);
    iSetColor(0,0,255);
    iRectangle(0, 0, screenwidth-10, screenheight-10);//this is the game border
    iSetColor(20, 200, 0);
    //iText(40, 40, "Welcome to spaceshooter");
    if(missile.pressed_s==true && mode==1)
    {
        iSetColor(255,0,0);
        iShowBMP2(missile.missilex,missile.missiley,"D:\\igfx\\Assets\\BMPs\\missile.bmp",0);
        missile.missiley+=15;
    }
    //generating stars fly by as environment
    iSetColor(255,255,255);
    for(int i=0; i<20; i++)
    {
        if(stars[i].showspaceflyby==true && mode==1)
        {
            iShowBMP2(stars[i].stars_x,stars[i].stars_y,star_BMP[i%4],0);

        }
    }
    //spawning enemy

    for(int i=0; i<ENEMYCT; i++)
    {
        iSetColor(255,0,0);
        if(tango[i].dead==false && mode==1)
        {
            iShowBMP2(tango[i].spawn_loc_x,tango[i].spawn_loc_y,tango_BMP[i%2],0);
        }

    }
    //afterburner ignition
    if(afterburner==true && mode==1)
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
    if(collision==true && mode==1)
    {
        iSetColor(255,255,255);
        iShowBMP2(x,y,"D:\\igfx\\Assets\\BMPs\\explosion.bmp",0);
    }
    if(gameover==true && p1.dead==true && mode==3)
    {
        iShowBMP2(0,0,"D:\\igfx\\Assets\\BMPs\\gameover.bmp",0);
    }
    iSetColor(0,200,100);
    if(mode==1)
    {iText(40,40,"SCORE: ",GLUT_BITMAP_TIMES_ROMAN_24);
    char s[50];
    iText(250,40,itoa(p1.score,s,10),GLUT_BITMAP_TIMES_ROMAN_24);
    iText(40,1030,"HEALTH ",GLUT_BITMAP_TIMES_ROMAN_10);
    iText(100,1030,itoa(p1.health,s,10),GLUT_BITMAP_TIMES_ROMAN_10);
    }
    if(mode==3)
    {
        iShowBMP(0,0,"D:\\igfx\\Assets\\BMPs\\gameover.bmp");
        iSetColor(0,255,200);
        char s[20];
        iText(470,372,"YOUR SCORE:",GLUT_BITMAP_TIMES_ROMAN_24);
        iText(700,372,itoa(p1.score,s,10),GLUT_BITMAP_TIMES_ROMAN_24);
        //sleep(2);
        //exit(0);
        drawTextBox();
		iSetColor(255, 255, 255);
		iText(700, 255, str);
		//iText(700, 200, "Click to activate the box, enter to finish.");
    }
    if(mode==4)//show credits and highscore
    {
        char s_temp[20];
        iShowBMP(0,0,"D:\\igfx\\Assets\\BMPs\\HIGHSCORES_CREDITS.bmp");
        iSetColor(0,255,0);
        iText(571,736,highest_scorer_name,GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(161,228,72);
        iText(330,145,itoa(record_score,s_temp,10),GLUT_BITMAP_TIMES_ROMAN_24);

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
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=494 && mx<=1064) && (my>=774 && my<=898) && mode==2)//pressed in new game region
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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=139 && mx<=286) && (my>=723 && my<=833) && mode==2)
    {
        if(sound_on==true)
        {
            sound_on=false;
            PlaySound(NULL,0,0);
        }
        else{
            sound_on=true;
            PlaySound("theme.wav",NULL, SND_LOOP | SND_ASYNC);
        }
    }
    //NOTE:same logic and code are written for keyboard key=='r' or 'R' to resume game...but that doesnt make the game laggy
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && (mx>=1383 && mx<=1481) && (my>=1040 && my<=1070) && mode==4)
    {
       pausegame();
    }
//    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
//	{
//		//place your codes here
//		if(mx >= 50 && mx <= 300 && my >= 250 && my <= 280 && mode == 3)
//		{
//			mode = 3;
//		}
//	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
	*/
void iKeyboard(unsigned char key)
{
    if (key == 'q' || key =='Q')
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
    if(key == 'm' || key == 'M')
    {
        PlaySound(NULL,0,0);
    }
    if(key=='g' || key=='G')
    {
        PlaySound("theme.wav",NULL, SND_LOOP | SND_ASYNC);
    }
    int I;
	if(mode == 3)
	{
        if(key == '\r')
		{
			strcpy(str2, str);
			printf("%s\n", str2);//FOR DEBUGGINNG PURPOSES ONLY
			FILE* fpout=fopen("HIGHSCORE_with_names.txt","a");
			fprintf(fpout,"%s %d\n",str2,p1.score);
			fclose(fpout);
			if(p1.score>record_score)
            {
                record_score=p1.score;
                strcpy(highest_scorer_name,str2);
                fpout=fopen("HIGHEST_SCORER.txt","w");
                fprintf(fpout,"%s",str2);
                fclose(fpout);
            }
			for(I = 0; I < len; I++)
				str[I] = 0;
			len = 0;
			mode=2;
		}
		else
		{
		    if(key!=8)
			{
			    str[len] = key;
                len++;
			}
			else{
                str[--len]=0;
			}
		}
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
    for(int i=0;i<ENEMYCT;i++)
    {
        spawn_loc_xs[i]=140+rand()%(screenwidth/2);
        printf("%d\n",spawn_loc_xs[i]);
    }
    len = 0;
	mode = 2;
	str[0]= 0;
	FILE* fpin=fopen("D:\\igfx\\HIGHESTSCORE.txt","r");
    fscanf(fpin,"%d",&record_score);
    fclose(fpin);
    //printf("record score:%d\n",record_score);(for debugging)
    fpin=fopen("HIGHEST_SCORER.txt","r");
    char* temp=(char* )malloc(100*sizeof(char));
    fgets(temp,100,fpin);
    puts(temp);
    strcpy(highest_scorer_name,temp);
    free(temp);
    fclose(fpin);
    if(sound_on==true)
        PlaySound("theme.wav",NULL, SND_LOOP | SND_ASYNC);
    //game starts as soon as mode is set to 1
    set_environment_values();
    //stars will spawn
    set_enemy_info_values();
    //enemies will spawn
    timerindex=iSetTimer(20,changes);
    //place your own initialization codes here.
    iInitialize(screenwidth, screenheight, "gamescreen");

    return 0;
}
