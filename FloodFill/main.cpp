#include <GL/glut.h>
#include <stdlib.h>
#include <iostream>
#include <utility>
#include <ctime>
#include "sor.h"
#include <string>
#include <windows.h>
#include <fstream>

using namespace std;


const int DX[8] = { 0, 0, -1, 1, -1, -1, 1, 1 };
const int DY[8] = { -1, 1, 0, 0, -1, 1, -1, 1 };
const int GRID_SIZE = 24;
const float BUTTON_WIDTH = 7.0f / GRID_SIZE;
const float BUTTON_HEIGHT = 3.0f / GRID_SIZE;
char active = 'B';
int **colorMatrix;
bool **checked;
int buttonActive = 5;
int fillMode = 0;
bool floodON = false;
int newColor = 0;

Sor<pair<int, int>> Q;

// set color
void color(int nr){
    switch(nr){
        case 0: glColor3f(1.0f, 0.0f, 0.0f); break;
        case 1: glColor3f(0.0f, 1.0f, 0.0f); break;
        case 2: glColor3f(0.0f, 0.0f, 1.0f); break;
    }
}

// get color code for the colorMatrix
int colorCode(char c){
    switch(c){
        case 'R': return 0;
        case 'G': return 1;
        case 'B': return 2;
        default: return -1;
    }
}

// creates a random fill pattern in the matrix
void randomFill(){
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            colorMatrix[i][j] = rand()%3;
        }
    }
}

// load fill pattern from file
void fillFromFile(){
    string fileName;
    string path;
    cout << "Enter the filename: ";
    cin>>fileName;
    path = "patterns\\" + fileName;
    ifstream f(path);
    if(f){
        char colorLetter;
        for(int i = 0; i < GRID_SIZE; i++){
            for(int j = 0; j < GRID_SIZE; j++){
                f >> colorLetter;
                if(colorLetter != 'R' && colorLetter != 'G' && colorLetter != 'B' && colorLetter != ' '){
                    cout << "Incorrect input from file! Exiting program..." << endl;
                    exit(1);
                }
                else{
                    colorMatrix[i][j] = colorCode(colorLetter);
                }
            }
        }
    }
    else{
        cout << "File not found! Exiting program..." << endl;
        Sleep(100);
        exit(1);
    }
}

// clears checked matrix
void clearChecked(){
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            checked[i][j] = false;
        }
    }
}

// clears color matrix
void clearColorMatrix(){
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            colorMatrix[i][j] = colorCode(active);
        }
    }
}

// Draw text on the screen
void drawText(float x, float y, string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawRandomButton(float x, float y){
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y); // bottom-left
    glVertex2f(x + BUTTON_WIDTH, y); // bottom-right
    glVertex2f(x + BUTTON_WIDTH, y + BUTTON_HEIGHT); // top-right
    glVertex2f(x, y + BUTTON_HEIGHT); // top-left
    glEnd();


    if(buttonActive < 5){
        glLineWidth(4.0f);
        buttonActive++;
    }
    else {
        glLineWidth(2.0f);
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    // Draw the borders of the button
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + BUTTON_WIDTH, y);
    glVertex2f(x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    glVertex2f(x, y + BUTTON_HEIGHT);
    glEnd();


    // Draw the text on the button rectangle
    glColor3f(0.0f, 0.0f, 0.0f);
    string randomButtonText = "Randomize";
    float randomTextX = x + (BUTTON_WIDTH / (randomButtonText.size() + 2));
    float randomTextY = y + BUTTON_HEIGHT / 2.8;
    drawText(randomTextX, randomTextY, randomButtonText);
}

void drawSingleFillButton(float x, float y){
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + BUTTON_WIDTH, y);
    glVertex2f(x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    glVertex2f(x, y + BUTTON_HEIGHT);
    glEnd();

    if(fillMode == 1){
        glLineWidth(4.0f);
    }
    else {
        glLineWidth(2.0f);
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    // Draw the borders of the button
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + BUTTON_WIDTH, y);
    glVertex2f(x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    glVertex2f(x, y + BUTTON_HEIGHT);
    glEnd();


    // Draw the text on the button
    glColor3f(0.0f, 0.0f, 0.0f);
    string singleButtonText = "Single Fill";
    float singleFillTextX = x + (BUTTON_WIDTH / (singleButtonText.size() - 2.5));
    float singleFillTextY = y + BUTTON_HEIGHT / 2.8;
    drawText(singleFillTextX, singleFillTextY, singleButtonText);
}

void drawFloodFillButton(float x, float y){
    glColor3f(1.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + BUTTON_WIDTH, y);
    glVertex2f(x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    glVertex2f(x, y + BUTTON_HEIGHT);
    glEnd();

    if(fillMode == 2){
        glLineWidth(4.0f);
    }
    else {
        glLineWidth(2.0f);
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    // Draw the borders of the rectangle
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + BUTTON_WIDTH, y);
    glVertex2f(x + BUTTON_WIDTH, y + BUTTON_HEIGHT);
    glVertex2f(x, y + BUTTON_HEIGHT);
    glEnd();


    // Draw the text on the button
    glColor3f(0.0f, 0.0f, 0.0f);
    string floodButtonText = "Flood Fill";
    float floodFillTextX = x + (BUTTON_WIDTH / (floodButtonText.size() - 2.5));
    float floodFillTextY = y + BUTTON_HEIGHT / 2.8;
    drawText(floodFillTextX, floodFillTextY, floodButtonText);
}


void floodFill() {
	if (!Q.isEmpty()) {
		pair<int, int> temp = Q.peek();
		Q.pop();
		int x, y;
		for (int i = 0; i < 8; i++) {
			x = temp.first + DX[i];
			y = temp.second + DY[i];
			if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
				if (colorMatrix[x][y] == colorMatrix[temp.first][temp.second] && !checked[x][y]) {
                    checked[x][y] = true;
					Q.push(pair<int, int>(x, y));
				}
			}
		}
		colorMatrix[temp.first][temp.second] = newColor;
	}
	else{
        floodON = false;
	}
}


// Draw on the screen
void display() {
    if(floodON){
        Sleep(100);
        floodFill();    // if floodfill is running, it executes the changes that occur in one iteration
    }
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the color buffer

    glColor3f(0.0f, 0.0f, 0.0f);

    float squareSize = 1.2f / GRID_SIZE;

    // Draw the grid of rectangles
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            // current rectangle coordinates
            float x = -0.9f + j * squareSize;
            float y = 0.9f - (i + 1) * squareSize;
            // draw a rectangle
            glBegin(GL_QUADS);
            color(colorMatrix[i][j]);
            glVertex2f(x, y);
            glVertex2f(x + squareSize, y);
            glVertex2f(x + squareSize, y + squareSize);
            glVertex2f(x, y + squareSize);
            glEnd();

            // draw the border
            glColor3f(0.0f, 0.0f, 0.0f);
            glLineWidth(2.0f);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y);
            glVertex2f(x + squareSize, y);
            glVertex2f(x + squareSize, y + squareSize);
            glVertex2f(x, y + squareSize);
            glEnd();
        }
    }


    float bigSquareSize = 5.0f / GRID_SIZE;

    // RED BUTTON
    float x = -0.9f + squareSize * 32;
    float y = 0.9f - squareSize * 9;
    glBegin(GL_QUADS);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(x, y);
    glVertex2f(x + bigSquareSize, y);
    glVertex2f(x + bigSquareSize, y + bigSquareSize);
    glVertex2f(x, y + bigSquareSize);
    glEnd();

    if(active == 'R'){
        glLineWidth(4.0f);
        glColor3f(0.0f, 0.0f, 0.0f); // Change border color to black
    }
    else{
        glLineWidth(2.0f);
        glColor3f(0.5f, 0.5f, 0.5f); // Change border color to grey
    }

    // draw the border
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + bigSquareSize, y);
    glVertex2f(x + bigSquareSize, y + bigSquareSize);
    glVertex2f(x, y + bigSquareSize);
    glEnd();


     // GREEN BUTTON
    y = 0.9f - squareSize * 14;
    glBegin(GL_QUADS);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex2f(x, y);
    glVertex2f(x + bigSquareSize, y);
    glVertex2f(x + bigSquareSize, y + bigSquareSize);
    glVertex2f(x, y + bigSquareSize);
    glEnd();

    if(active == 'G'){
        glLineWidth(4.0f);
        glColor3f(0.0f, 0.0f, 0.0f); // change border color to black
    }
    else{
        glLineWidth(2.0f);
        glColor3f(0.5f, 0.5f, 0.5f); // change border color to grey
    }
    // draw the border
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + bigSquareSize, y);
    glVertex2f(x + bigSquareSize, y + bigSquareSize);
    glVertex2f(x, y + bigSquareSize);
    glEnd();


    // BLUE BUTTON
    y = 0.9f - squareSize * 19;

    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex2f(x, y);
    glVertex2f(x + bigSquareSize, y);
    glVertex2f(x + bigSquareSize, y + bigSquareSize);
    glVertex2f(x, y + bigSquareSize);
    glEnd();

    if(active == 'B'){
        glLineWidth(4.0f);
        glColor3f(0.0f, 0.0f, 0.0f); // Change border color to black
    }
    else{
        glLineWidth(2.0f);
        glColor3f(0.5f, 0.5f, 0.5f); // Change border color to grey
    }

    // draw the border
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + bigSquareSize, y);
    glVertex2f(x + bigSquareSize, y + bigSquareSize);
    glVertex2f(x, y + bigSquareSize);
    glEnd();

    // Draw the fill control buttons

    // RANDOM BUTTON
    x = -0.9f + squareSize * 8;
    y = 0.9f - squareSize * 30;
    drawRandomButton(x, y);

    // SINGLE FILL BUTTON
    x = -0.9f + squareSize * 15;
    drawSingleFillButton(x, y);

    // FLOOD FILL BUTTON
    x = -0.9f + squareSize * 22;
    drawFloodFillButton(x, y);


    glFlush(); // Flush drawing commands
}

// returns an index to the matrix based on coordinates
int getIndex(int number){
    return (number-36)/18;
}

//handles keyboard events -> changing the active color
void keyboardCallback(unsigned char key, int x, int y) {
    if (key == 'r') {
        active = 'R';
    }
    else if(key == 'g'){
        active = 'G';
    }
    else if(key == 'b'){
        active = 'B';
    }
    else if(key == 's'){
        fillMode = 1;
    }
    else if(key == 'f'){
        fillMode = 2;
    }
    else if(key == 'p'){
         cout << "Current pattern: " << endl;
         for(int i = 0; i < GRID_SIZE; i++){
            for(int j = 0; j < GRID_SIZE; j++){
                switch(colorMatrix[i][j]){
                    case 0: cout << "R "; break;
                    case 1: cout << "G "; break;
                    case 2: cout << "B "; break;
                    default: break;
                }
            }
            cout << endl;
        }
    }
    else if(key == 'c'){
        clearColorMatrix();
    }
}

// handles mouse clicks
void mouseClick(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // RGB buttons
        if(x >= 612 && x <= 688){
            if(y >= 123 && y <= 197){
                active = 'R';
            }
            else if(y >= 213 && y <= 287){
                active = 'G';
            }
            else if(y >= 303 && y <= 377){
                active = 'B';
            }
        }
        // RANDOMIZE button
        if(y >= 531 && y <= 575){
            if(x >= 180 && x <= 284 && buttonActive > 4  && !floodON){
                srand(time(NULL));
                randomFill();
                buttonActive = 0;
                fillMode = 0;
            }
            // SINGLE FILL button
            else if(x >= 307 && x <= 411){
                fillMode = 1;
            }
            // FLOOD FILL button
            else if(x >= 432 && x <= 538){
                fillMode = 2;
            }
        }

        // GRID
        // allow single fill when flood on. Disallow fill if checked.
        // create clear button with active color used as clear color
        if(x >= 36 && y >= 36 && x <= 468 && y < 468  && !floodON){
            int j = getIndex(x);
            int i = getIndex(y);
            if(fillMode == 1){
                colorMatrix[i][j] = colorCode(active);
            }
            else if(fillMode == 2){
                if(colorCode(active) != colorMatrix[i][j]){
                    floodON = true;
                    Q.clear();
                    clearChecked();
                    pair<int, int> koord(i, j);
                    Q.push(koord);
                    checked[i][j] = true;
                    newColor = colorCode(active);
                }
            }
        }
    }
}

// handles hover effect on buttons
void mouseMovement(int x, int y){
    bool onButton = false;
    // RGB buttons
    if(x >= 612 && x <= 688){
        if(y >= 123 && y <= 197){
            onButton = true;
        }
        else if(y >= 213 && y <= 287){
            onButton = true;
        }
        else if(y >= 303 && y <= 377){
            onButton = true;
        }
    }
    if(y >= 531 && y <= 575){
        // RANDOMIZE button
        if(x >= 180 && x <= 284){
            onButton = true;
        }
        // SINGLE FILL button
        else if(x >= 307 && x <= 411){
            onButton = true;
        }
        // FLOOD FILL button
        else if(x >= 432 && x <= 538){
            onButton = true;
        }

    }
    if(onButton){
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW); // set cursor to hand when hovering over the button
    }
    else{
        glutSetCursor(GLUT_CURSOR_CROSSHAIR); // set cursor to default arrow when not hovering over the button
    }
}

// set refresh rate
void timer(int value) {
    glutPostRedisplay(); // redraw the screen
    glutTimerFunc(1000 / 15, timer, 0); // timer callback request
}

int main(int argc, char** argv) {
    srand(time(NULL));

    colorMatrix = new int*[GRID_SIZE];
    checked = new bool*[GRID_SIZE];
    for(int i = 0; i < GRID_SIZE; i++){
        colorMatrix[i] = new int[GRID_SIZE];
        checked[i] = new bool[GRID_SIZE];
    }
    char c;
    cout << argv[0] << endl;
    cout << "Welcome to FloodFill!\nKeyboard controls:\nChoose color: r\\g\\b\nSet fill mode: s\\f\nClear matrix with active color: c\nPrint current pattern: p\n\nLoad pattern from file?(y/n) ";
    cin>>c;
    if(c == 'y'){
        fillFromFile();
    }
    else{
        randomFill();
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(720, 720);
    glutCreateWindow("FloodFill");
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    gluOrtho2D(-1, 1, -1, 1);

    // Set display function
    glutDisplayFunc(display);

    // Set mouse movement handler
    glutPassiveMotionFunc(mouseMovement);

    // Set mouse click handler
    glutMouseFunc(mouseClick);

    // Set keyboard event handler
    glutKeyboardFunc(keyboardCallback);

    // Set timer function
    glutTimerFunc(0, timer, 0);

    glutMainLoop(); // Enter the event processing loop

    for(int i = 0; i < GRID_SIZE; i++){
        delete[] colorMatrix[i];
        delete[] checked[i];
    }
    delete[] colorMatrix;
    delete[] checked;
    return 0;
}
