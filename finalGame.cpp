#include<GLUT/GLUT.h>
#include "bits/stdc++.h"
#include <random>
#include <ctime>
#include<iostream>
using namespace std;

float low = -1.0, high = 1.0, ballX, ballY;
int score=0;
float platformX=0,platformY=-1.8,velocityX=-0.01,velocityY = -0.01,changePLatformX = 0.3;
int highScoreInt;
char highScoreChar[100];
bool once=true,displayedHighScore=false,paused=false;
vector<pair<float,float>> objPos;
FILE* fp;

// ****************************************************************************************************************************************************************

// This function is responsible for initializing various aspects of the graphics rendering pipeline and setting up the initial state of the OpenGL context. 

void init()
{
    glClearColor(0,0,0,1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-2,2,-2,2,-2,2);
    glMatrixMode(GL_MODELVIEW);
}


// ****************************************************************************************************************************************************************

// This function is used to opent the file and get the high score of previous games

void openFetchScore()
{
    fp = fopen("bestScore.txt", "r");
    if(fp == NULL)
    {
        cout<<"Failed to open"<<endl;
    }
    fgets(highScoreChar, 100, fp);
    // if(highScoreChar == NULL)
    //  highScoreChar[0] = 0;
    fclose(fp);
}


// This function is used to display the high score of the previous games om the screen

void displayHighScore()
{
    
    string high =  highScoreChar;
    highScoreInt = stoi(high);
    if(score>=highScoreInt)
    {
     highScoreInt = score;
     high = to_string(highScoreInt);
    }
    string toDisplay = "High Score: " + high;
    const char *finalHighScore = toDisplay.c_str();
    glColor3f(1,1,1);
    glRasterPos3f(0.75,1.8,0);
   for(const char *c = finalHighScore;*c !='\0';  c++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);   
    
}

// This function is used to update the high scorew by checking if current score is greater than previous high score

void updateHighScore()
{
    
    if(score>=highScoreInt)
    {
        cout<<"in\n";
        cout<<score<<" "<<highScoreInt<<endl;
        FILE* fp = fopen("bestScore.txt", "w");
        if(fp==NULL)
        {
            cout<<"cant open file\n";
            return;
        }
        string s = to_string(score);
        for(int i=0; i<s.length(); i++)
         fprintf(fp,"%c",s[i]);
        fclose(fp);
    }
}


// This function is used to display the score of the current player

void displayScore()
{
    string s;
    s = "Score: ";
    string s1 = to_string(score);
    s = s+s1;
    char const* str= s.c_str();

    glColor3f(1,1,1);

    glRasterPos3f(-1.9,1.8,0);
   for(const char *c = str;*c !='\0';  c++)
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);


}

// ***************************************************************************************************************************************************************  

// This function is used to create the list of objects .i.e., obstacles to be placed on screen and store in vector pairs.

void createList()
{
    float startx=-1.7, starty = 1.5;
    for(int i=0; i<3; i++)
    {
        objPos.push_back(make_pair(startx, starty));
        if(startx+0.2>1.9)
        {
            starty-=0.5;
            startx=-1.7;
        }
        else
            startx+=0.5;

    }
}

// Tis function  helps with creation of ball.

void createBall()
{
    glColor3f(1,0,0);
    glutSolidSphere(0.07,25,25);

}

// Tis function  helps with creation of platform.

void createPLatform()
{
    glColor3f(0,1,0);
    glutSolidCube(0.5);    
}

// Tis function  helps with creation of Targets.

void createObject()
{
    glColor3f(0,1,1);
    glutSolidCube(0.3);
}

// ***************************************************************************************************************************************************************  

// This function draws the Targets by iteration the vector pairs where the coordinates are stored.

void drawObject()
{
         for(int i=0; i<objPos.size(); i++)
    {
        glPushMatrix();
        glTranslatef(objPos[i].first,objPos[i].second,0);
        createObject();
        glPopMatrix();

    }    
}


// This function is used to define the boundry position.

void drawBoundary()
{
    glColor3f(1,0,0);
    glBegin(GL_LINES);
        glVertex2f(-2, 1.75);
        glVertex2f(2, 1.75);
    glEnd();
    glFlush();

}


// ***************************************************************************************************************************************************************  

// This function is used to make changes in the position of the platform.

void platformMovement( int key,int x,int y)
{
   
    if(key == GLUT_KEY_LEFT)
    {
        if(platformX-changePLatformX>-1.7)
        platformX -= changePLatformX; 
    }
    if(key == GLUT_KEY_RIGHT)
    {
        if(platformX+changePLatformX<1.7)
        platformX += changePLatformX; 
    }

}

// This function traces the keyboard keys pressed used for pausing,quiting etc.

void checkKeyboard(unsigned char key,int x,int y)
{
   if((int)(key) == 27)
    {
        paused = !paused;
        glutPostRedisplay();
    }
    if(key == 'e')
    {
        updateHighScore();
        exit(0);
    }
}

// ***************************************************************************************************************************************************************  


void display()
{
    if(!paused)
    {
        glLoadIdentity();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // cout<<score<<" "<<highScoreInt<<endl;

    if((ballY +velocityY>= platformY-0.1 and ballY+velocityY <= platformY+0.1) and 
        (platformX-0.5<= ballX+velocityX and ballX+velocityX<=platformX+0.5))
        velocityY = -1*velocityY;

    

    for(int i=0; i<objPos.size(); i++)
    {
        
        if(ballX+velocityX>objPos[i].first-0.2 and
            ballX+velocityX<objPos[i].first+0.2 and
            ballY>objPos[i].second-0.2 and
            ballY<objPos[i].second+0.2)
            {
                velocityX  = -1*velocityX;
                objPos.erase(objPos.begin()+i);
                score++;
            }

        if(ballX>objPos[i].first -0.2 and
            ballX<objPos[i].first +0.2 and
            ballY+velocityY>objPos[i].second-0.2 and
            ballY+velocityY<objPos[i].second+0.2)
            {

                velocityY = -1*velocityY;
                objPos.erase(objPos.begin()+i);

                score++;
            }

    }

    if(ballY<=-1.9)
    {
     updateHighScore();
     return;
    }
    else
    if(ballY>=1.7)
     velocityY = -1*velocityY;
    if(ballX>=1.9 or ballX<=-1.9)
     velocityX = -1*velocityX;
    

    ballY += velocityY;
    ballX += velocityX;

    glPushMatrix();
    glTranslatef(ballX,ballY,0);
    createBall();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(platformX,platformY,0);
    glScalef(2,0.2,0);
    createPLatform();
    glPopMatrix();

    drawObject();

    glPushMatrix();
    displayScore();
    glPopMatrix();

    glPushMatrix();
    displayHighScore();
    glPopMatrix();


  
    if(objPos.size()==0)
    {
        createList();
        if(velocityX<0)
            velocityX-=0.01;
        else
            velocityX+=0.01;
        if(velocityY<0)
            velocityY-=0.01;
        else
            velocityY+=0.01;
    }
    
    glutSwapBuffers();
    glutPostRedisplay();
    }
}

// ***************************************************************************************************************************************************************  


int main(int argc,char **argv)
{
    createList();
    srand(time(0));
    ballX=low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
    ballY=low + static_cast<float>(rand()) * static_cast<float>(high - low) / RAND_MAX;
    openFetchScore();
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Game");
    init();
    glutDisplayFunc(display);
    glutSpecialFunc(platformMovement);
    glutKeyboardFunc(checkKeyboard);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}