#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include "iGraphics.h"
#define SIZE 200

int stackX[SIZE],stackY[SIZE],scoreArr[3];
char strScore[12], highScore[3][6+1],nameStr[7];
int tosX,tosY;
double foodX = (rand()*1000 )% 960 +30,foodY =(rand()*1000)% 560 +30;
int length=10,a=510,b=310,input=1,p=length,r,over,start=1,newGame,cont,rad,highestScore,pause,hs,nameLen;
int quit,setting,about,level,help,difficulty=2,score,special,holdSpecial,specialX,specialY,check=length,music=1,sound=1;

void retrieve()
{
	int i;
	FILE *fp;

	fp=fopen("values.txt","r");
	fscanf(fp,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",&length,&a,&b,&p,&r,&difficulty,&score,
		&special,&holdSpecial,&specialX,&specialY,&check,&tosX,&tosY,&foodX,&foodY);
	fclose(fp);

	fp=fopen("stackX.txt","r");
	for(i=0;i<200;i++)
	{
		fscanf(fp,"%d ",&stackX[i]);
	}
	fclose(fp);

	fp=fopen("stackY.txt","r");
	for(i=0;i<200;i++)
	{
		fscanf(fp,"%d ",&stackY[i]);
	}
	fclose(fp);
}

void write()
{
	int i;
	FILE *fp;

	fp=fopen("snake.txt","w");
	fprintf(fp,"%d %d %d",cont,music,sound);
	fclose(fp);

	if (hs)
	{
		fp=fopen("score.txt","w");
		for ( i=0; i<3 ;i++)
		{
			fprintf(fp,"%s %d ",highScore[i],scoreArr[i]);
		}
		fclose(fp);
	}

	if (!over)
	{
		fp=fopen("values.txt","w");
		fprintf(fp,"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d ",length,a,b,p,r,difficulty,score,
			special,holdSpecial,specialX,specialY,check,tosX,tosY,foodX,foodY);
		fclose(fp);

		fp=fopen("stackX.txt","w");
		for(i=0;i<200;i++)
		{
			fprintf(fp,"%d ",stackX[i]);
		}
		fclose(fp);

		fp=fopen("stackY.txt","w");
		for(i=0;i<200;i++)
		{
			fprintf(fp,"%d ",stackY[i]);
		}
		fclose(fp);
	}
}

void switchColor()
{
	switch (holdSpecial%7){
				case 0:
					iSetColor(100,0,50);
					break;
				case 1:
					iSetColor(0,0,160);
					break;
				case 2:
					iSetColor(15,135,255);
					break;
				case 4:
					iSetColor(0,150,0);
					break;
				case 5:
					iSetColor(255,255,0);
					break;
				case 6:
					iSetColor(220,0,0);
					break;
	}
}

void specialItem()
{
	if (check!=length && length%5==0)
	{
		special=1;
		specialX = (rand () *1000 ) % 920 + 40;
		specialY = (rand () *1000 ) % 520 + 40;
		check=length;
		holdSpecial=80;
	}
		
}

int foodMove()
{
	if((abs((int)(a - foodX) ) < 15) && (abs((int)(b - foodY)) < 15))
	{
		foodX = ((rand()*1000) % 960)+30;
		foodY =	((rand()*1000) % 560)+30;
		if (sound)
			PlaySound("faux",NULL,SND_ASYNC);
		length++;
		score++;
		if (length%5==0)
			specialItem();
		return 1;
	}
	return 0;
}

int isLevel()
{
	if (difficulty==1)
		return 110;
	if (difficulty==2)
		return 90;
	if (difficulty==3)
		return 70 ;
}

void restore()
{
	length=10;
	a=510;
	b=310;
	input=1;
	p=length;
	check=length;
	r=0;
	over=0;
	cont=0;
	tosX=0;
	tosY=0;
	score=0;
	special=0;
	holdSpecial=0;
}

void pushX(int x)
{
	stackX[tosX % SIZE]=x;
	tosX=(tosX+1) % SIZE;

	return;
}

void pushY(int x)
{
	stackY[tosY % SIZE] = x;
	tosY = (tosY + 1) % SIZE;

	return;
}

int popX()
{
	int a;

	a = stackX[(tosX - 1 + SIZE) % SIZE];
	stackX[(tosX - 1 + SIZE) % SIZE]= 0;
	tosX = (tosX - 1 + SIZE) % SIZE;

	return a;
}

int popY()
{
	int a;

	a = stackY[(tosY - 1 + SIZE) % SIZE];
	stackY[(tosY - 1 + SIZE) % SIZE] = 0;
	tosY = (tosY - 1 + SIZE) % SIZE;

	return a;
}

int snake(int x,int y,int c,int d,char direction)
{
	int g,i;

	for (i=0;i!=d;)
	{
		if (c%3==0)
			iSetColor(255,255,255);
		else if(c%3==1)
			iSetColor(0,0,0);
		else
			iSetColor(255,0,0);
		iFilledCircle(x,y,rad);
		c++;
		if (direction == 'V') 
		{	
			if (d>0)
			{
				y=(y-20+600)%600;
				i++;
			}
			else
			{
				y=(y+20)%600;
				i--;
			}
		}
		else
		{
			if(d>0)
			{
				x=(x-20+1000)%1000;
				i++;
			}
			else
			{	
				x=(x+20)%1000;
				i--;
			}
		}
		if ((x==a ) && (y==b))
		{
			over=1;
			break;
		}
		if (foodMove() && c+(d-i)==(length-1))
		{
			if (d>0)
				d++;
			else
				d--;
		}
		if (( abs((int)(foodX - x))>15 || abs((int)(foodY-y))>15) && !(difficulty==3 && ((foodX>192 && foodX<808 && foodY>172 && foodY<208) || 
			(foodX>192 && foodX<808 && foodY>392 && foodY<428))))
		{
			iSetColor(255,10,10);
			iFilledCircle(foodX,foodY,8);
		}
		else
		{
			foodX = ((rand()*1000) % 960)+30;
			foodY =	((rand()*1000) % 560)+30;
		}
		if (special && holdSpecial && abs(specialX - a)<25 && abs(specialY-b)<25 )
		{
			if (sound)
				PlaySound("faux",NULL,SND_ASYNC);
			special=0;
			score=score+10*holdSpecial;
		}
		if (special && holdSpecial)
		{
			if (( abs(specialX - x)>25 || abs(specialY-y)>25) && !(difficulty==3 && ((specialX>185 && specialX<815 && specialY>165 && specialY<215) || 
				(specialX>185 && specialX<815 && specialY>385 && specialY<435))))
			{
				switchColor();
				iFilledCircle(specialX,specialY,15);
			}
			else
			{
				specialX = (rand () *1000 ) % 920 + 40;
				specialY = (rand () *1000 ) % 520 + 40;
			}
			iSetColor(0,0,0);
			iFilledRectangle(520,627,2.62*holdSpecial,46);
		}
		if ((length-c)<=5)
		{
			rad--;
		}
		if (d==length || (c+d-i)==length)
		{
			if (d>0)
				d++;
			else
				d--;
		}
		if (c==length)
		{
			if (d>0)
			{
				return d-1;
			}
			else 
				return d+1;
		}
	}
			
	if (c < length) 
	{	
		if (direction == 'V')
		{
			g=snake(x,y,c,popX(),'H');
			pushX(g);
			return d;
		}
		else
		{
			g=snake(x,y,c,popY(),'V');
			pushY(g);
			return d;
		}
	}
	return 0;
}

void position()
{
	if (!start)
	{
		if (input==1 )
		{
			a=(a+20+1000)%1000;
			if (p<length)
				p++;
		}
		if (input==3)
		{
			a=(a-20+1000)%1000;
			if (p>(-length))
				p--;
		}
		if (input==0)
		{
			b=(b+20+600)%600;
			if (r<length)
				r++;
		}
		if (input==2)
		{
			b=(b-20+600)%600;
			if (r>(-length))
				r--;
		}
		if (special && holdSpecial)
			holdSpecial--;
	}
}

void userInterfase()
{
	iShowBMP(0,0,"GrassSnake2.bmp");

	iShowBMP(365,520,"03_new.bmp");
	iShowBMP(365,440,"03_difficulty.bmp");
	iShowBMP(365,360,"03_settings.bmp");
	iShowBMP(365,280,"03_help.bmp");
	iShowBMP(365,200,"03_highest_score.bmp");
	iShowBMP(365,120,"03_about.bmp");
	iShowBMP(365,40,"03_exit.bmp");

	if (cont)
	{
		iShowBMP(365,600,"03_continue.bmp");
	}
	if (newGame)
	{
		start=0;
		return ;
	}

	if (quit)
	{
		iShowBMP(0,0,"GrassSnake2.bmp");
		iShowBMP(253,254,"03_sure.bmp");
		iShowBMP(315,285,"03_no.bmp");
		iShowBMP(520,285,"03_yes.bmp");
	}
	
	if (level || help || about|| setting || highestScore)
	{
		iShowBMP(0,0,"GrassSnake2.bmp");
		iShowBMP(365,20,"03_ok.bmp");
	}
	if (help)
	{
		iShowBMP(0,0,"03_help_details.bmp");
		iShowBMP(365,20,"03_ok.bmp");
	}
	if (setting)
	{
		iShowBMP(230,300,"03_music.bmp");
		if (music)
			iShowBMP(500,300,"03_on.bmp");
		else
			iShowBMP(500,300,"03_off.bmp");
		iShowBMP(530,320,"03_mini_on.bmp");
		iShowBMP(660,320,"03_mini_off.bmp");
		iShowBMP(230,220,"03_sound.bmp");
		if (sound)
			iShowBMP(500,220,"03_on.bmp");
		else
			iShowBMP(500,220,"03_off.bmp");
		iShowBMP(530,240,"03_mini_on.bmp");
		iShowBMP(660,240,"03_mini_off.bmp");
	}
	if (about)
	{
		iShowBMP(25,90 ,"03_about_details.bmp");
	}
	if (level)
	{
		if (difficulty==1)
			iShowBMP(365,300,"03_easy_g.bmp");
		else
			iShowBMP(365,300,"03_easy.bmp");
		if (difficulty==2)
			iShowBMP(365,220,"03_normal_g.bmp");
		else
			iShowBMP(365,220,"03_normal.bmp");
		if (difficulty==3)
			iShowBMP(365,140,"03_hard_g.bmp");
		else
			iShowBMP(365,140,"03_hard.bmp");
	}
	if (highestScore)
	{
		int i;
		iShowBMP(250,300,"03_easy.bmp");
		iShowBMP(250,220,"03_normal.bmp");
		iShowBMP(250,140,"03_hard.bmp");
		iShowBMP(520,300,"03.bmp");
		iShowBMP(520,220,"03.bmp");
		iShowBMP(520,140,"03.bmp");
		iShowBMP(365,400,"03_highest_score.bmp");
		iSetColor(0,0,0);
		for (i=0;i<3;i++)
		{
			iText(560,328-i*80,highScore[i],GLUT_BITMAP_TIMES_ROMAN_24);
			sprintf(strScore,"%07d",scoreArr[i]);
			iText(640,328-i*80,strScore,GLUT_BITMAP_TIMES_ROMAN_24);
		}

	}
}

void iDraw()
{ 
	if (start)
	{
		userInterfase();
	}
	if (pause)
	{
		iPauseTimer(0);
		write();
	}
	else
		iResumeTimer(0);
	if(over)
	{
		iClear();
		iShowBMP(0,0,"GrassSnake2.bmp");
		iSetColor(0,0,0);
		
		iShowBMP(380,400,"03_game_over.bmp");
		iShowBMP(250,320,"03_score.bmp");
		iShowBMP(250,240,"03_highest_score.bmp");
		iShowBMP(520,320,"03.bmp");
		iShowBMP(520,240,"03.bmp");
		if (score > scoreArr[difficulty-1])
		{
			scoreArr[difficulty-1]=score;
			sprintf(strScore,"%07d",scoreArr[difficulty-1]);
			iText(600,268,strScore,GLUT_BITMAP_TIMES_ROMAN_24);
			hs=1;
		}
		else
		{
			sprintf(strScore,"%07d",scoreArr[difficulty-1]);
			iText(600,268,strScore,GLUT_BITMAP_TIMES_ROMAN_24);
		}
		sprintf(strScore,"%07d",score);
		iText(600,348,strScore,GLUT_BITMAP_TIMES_ROMAN_24);
		iSetColor(0,0,0);
		iShowBMP(365,20,"03_ok.bmp");
	}
	if (hs && !over)
	{
		iClear();
		iShowBMP(0,0,"footer-back4.bmp");
		iShowBMP(254,600,"03_new_high.bmp");
		iShowBMP(390,520,"03.bmp");
		sprintf(strScore,"%07d",scoreArr[difficulty-1]);
		iSetColor(0,0,0);
		iText(470,548,strScore,GLUT_BITMAP_TIMES_ROMAN_24);
		iShowBMP(254,440,"03_please.bmp");
		iShowBMP(390,360,"03.bmp");
		iText(480,388,nameStr,GLUT_BITMAP_TIMES_ROMAN_24);
	}
	if ((newGame || cont) && !over && !start)
	{
		if (p!=0 || r!=0)
		{
			//iClear();
			iShowBMP(0,0,"footer-back4.bmp");
			iShowBMP(0,600,"03_big.bmp");
			sprintf(strScore,"%07d",score);
			iShowBMP(749,615,"03_score2.bmp");
			iShowBMP(10,615,"03_back.bmp");
			iShowBMP(503,615,"03_grid.bmp");
			iSetColor(0,0,0);
			iText(860,643,strScore,GLUT_BITMAP_TIMES_ROMAN_24);
			rad=10;
			if (difficulty==3 || difficulty==2)
			{
				iShowBMP(0,0,"bricks_1000.bmp");
				iShowBMP(0,0,"bricks_600_ver.bmp");
				iShowBMP(0,580,"bricks_1000.bmp");
				iShowBMP(980,0,"bricks_600_ver.bmp");
				if (difficulty==3)
				{
					iShowBMP(200,180,"bricks_600.bmp");
					iShowBMP(200,400,"bricks_600.bmp");
				}
			}
			if ((input==1 || input==3 ) )
			{
				if ((difficulty==3 || difficulty==2) && (a>980 || a<20 || b>580 || b<20))
				{
					over=1;
				}
				else if (difficulty==3 && ((a>200 && a<800 && b>180 && b<200) || (a>200 && a<800 && b>400 && b<420)))
				{
					over=1;
				}
				else
					snake(a,b,0,p,'H');
			}
			else
			{
				if ((difficulty==3 || difficulty==2) && (a>980 || a<20 || b>580 || b<20))
					over=1;
				else if (difficulty==3 && ((a>200 && a<800 && b>180 && b<200) || (a>200 && a<800 && b>400 && b<420)))
					over=1;
				else
					snake(a,b,0,r,'V');
			}
			if (pause)
			{
				iShowBMP(360,320,"03_paused.bmp");
				iShowBMP(257,615,"03_resume.bmp");
			}
			else
				iShowBMP(257,615,"03_pause.bmp");
		}
		//write();
	}
}

void iMouseMove(int mx, int my)
{
	
}

void iMouse(int button, int state, int mx, int my)
{
	
	if ((newGame || cont) && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if ((mx > 257) && (mx < 498) && (my > 615) && (my < 686))
		{
			pause=(pause+1)%2;
		}
		if ((mx > 10) && (mx < 251) && (my > 615) && (my < 686))
		{
			start=1;
			cont=1;
			pause=1;
			newGame=0;
		}
	} 
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && start)
	{
		if (( mx > 365 ) && ( mx < 606 ) && ( my > 600 ) && ( my < 671 ) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			start=0;
			pause=1;
			retrieve();
		}
		if (( mx > 365 ) && ( mx < 606 ) && ( my > 520 ) && ( my < 591 ) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			newGame=1;
			pause=0;
			restore();
		}
		else if ((mx > 365) && (mx < 606) && (my > 440) && (my < 511) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			level = 1;
		}
		else if ((mx > 365) && (mx < 606) && (my > 360) && (my < 431) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			setting = 1;
		}
		else if ((mx > 365) && (mx < 606) && (my > 280) && (my < 351) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			help = 1;
		}
		else if ((mx > 365) && (mx < 606) && (my > 200) && (my < 271) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			highestScore = 1;
		}
		else if ((mx > 365) && (mx <606) && ( my > 120 ) && ( my < 191 ) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			about =1;
		}
		else if (( mx > 365 ) && ( mx < 606 ) && ( my > 40 ) && ( my < 111 ) && !level && !setting && !help && !about && !quit && !highestScore)
		{
			quit=1;
		}
		if (level || setting || help || about || highestScore)
		{
			if ((mx > 365) && (mx < 626) && (my > 20) && (my < 91))
			{
				level=0;
				setting=0;
				help=0;
				about = 0;
				highestScore=0;
			}
		}
		if (level)
		{
			if ((mx > 365) && (mx < 606) && (my > 300) && (my < 371))
				difficulty=1;
			if ((mx > 365) && (mx < 606) && (my > 220) && (my < 291))
				difficulty=2;
			if ((mx > 365) && (mx < 606) && (my > 140) && (my < 231))
				difficulty=3;
		}
		if (setting)
		{
			if ((mx > 530 && mx < 580 && my > 320 && my < 350) || (mx > 660 && mx < 710 && my > 320 && my < 350))
				music=(music+1)%2;
			if ((mx > 530 && mx < 580 && my > 240 && my < 270) || (mx > 660 && mx < 710 && my > 240 && my < 270)) 
				sound=(sound+1)%2;
		}
		if (quit)
		{
			if (( mx >  315) && ( mx < 460 ) && ( my > 285 ) && ( my < 328 ))
			{
				quit=0;
			}
			if (( mx > 520 ) && ( mx < 665 ) && ( my > 285 ) && ( my < 328 ))
			{
				write();
				exit(0);
			}
		}
	}
	if (over && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{
		if (hs)
		{

		}
		if (!hs && (mx > 365) && (mx < 626) && (my > 20) && (my < 91))
		{
			over=0;
			start=1;
			newGame=0;
			cont=0;
		}
		else if ((mx > 365) && (mx < 626) && (my > 20) && (my < 91))
		{
			over=0;
			newGame=0;
			cont=0;
		}
	}
}

void iKeyboard(unsigned char key)
{
	if(key == 'p' && !start)
		pause=1;
 
	if(key == 'r')
		pause=0;
	if(hs)
	{
		if(key == '\r' && nameLen<7)
		{
			nameStr[nameLen]='\0';
			strcpy(highScore[difficulty-1],nameStr);
			write();
			hs=0;
			start=1;
			for (int i=0;i<7;i++)
			{
				nameStr[i]='\0';
			}
			nameLen=0;
		}
		else if (key=='\b')
		{
			if(nameLen)
			{	
				nameStr[nameLen-1]='\0';
				nameLen--;
			}
		}
		else if(nameLen<6)
		{
			nameStr[nameLen]=key;
			nameLen++;
		}
	}
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
	if (!pause)
	{
		if (input==0 || input==2)
		{
			if(key == GLUT_KEY_RIGHT)
			{
				pushY(r);
				r=0;
				input=1;
			}
			if(key == GLUT_KEY_LEFT)
			{
				pushY(r);
				r=0;
				input=3;
			}
		}
		else
		{
			if(key == GLUT_KEY_UP)
			{
				pushX(p);
				p=0;
				input=0;
			}
			if(key == GLUT_KEY_DOWN)
			{
				pushX(p);
				p=0;
				input=2;
			}
		}
	}
	if (key==GLUT_KEY_END)
	{	
		start=1;
		newGame=0;
		pause=1;
		quit=1;
		cont=1;
	}

}

int main()
{
	int i;
	FILE *fp;
	fp=fopen("snake.txt","r");
	fscanf(fp,"%d %d %d",&cont,&music,&sound);
	fclose(fp);

	fp=fopen("score.txt","r");
	for ( i=0; i<3 ;i++)
	{
		fscanf(fp,"%s %d",highScore[i],&scoreArr[i]);
	}
	fclose(fp);

	iSetTimer(isLevel(),position);
	iInitialize(1000, 700, "Snake");
	return 0;
}	