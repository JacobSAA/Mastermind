/**
 * Filename:   Mastermind.cpp
 * Project:    Mastermind
 * Authors:    Jacob Adamiec and Karm Nagi
 *
 * Semester:   Spring 2019
 * Course:     CS368
 * Lecture:    002
 */

#include <GLFW/glfw3.h>
#include <cstdlib>
#include <ctime>

//HELPER METHODS/VARIABLES
void createBoard();	//Creates board lines and color pallet
int pieces[11][4];	//Array for piece colors
void drawPieces();	//Draws all user guess pieces with a loop (using pieces[][])
//Draws individual piece given a color and rectangle parameters
void drawPiece(int color, double topX, double topY, double botX, double botY);
void drawSquare(double topX, double topY, double botX, double botY);	//Draws a square/rectanlge
int feedbackPieces[10][4];
void drawFeedbackSquares();
float y(double y);	//Converts y pixel coordinates into percentages since GLFW uses -1=left, 0=center, 1=right
float x(double x);	//Same as y() just a modified equation
void generateCode();	//Generates a random code for the user to guess
bool checkCode();	//Checks the users guess to the generated code and generates appropriate feedback

//GAME CONDITIONS
bool end = false;	//End condition
int guess = 0;	//guess counter
int piece = 0;	//piece counter
int selected = 0;	//Selected color (0-6 SEE SWITCH STATEMENT)
int code[4];	//Code to be guessed
int codeGuess[4];	//Users guess

//INPUT CONDITIONS
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);	//Mouse input listener
double mouseX;	//Mouse coordinates
double mouseY;

int width = 400;
int height = 600;

int main()
{
	GLFWwindow* window;
	srand((int)time(NULL));
	//Initialize glfw
	if (!glfwInit())
		return -1;

	//Create window and terminate if that fails
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(width, height, "MasterMind", NULL, NULL);
	
	if (!window) {
		glfwTerminate();
		return -1;
	}

	//Sets glfw's context to our window
	glfwMakeContextCurrent(window);

	//Allows for mouse button press action
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	generateCode();
	//Loops until window is closed
	while (!glfwWindowShouldClose(window))
	{
		//IDK it said to do this
		
		glClear(GL_COLOR_BUFFER_BIT);

		//DRAW GAME
		createBoard();
		drawPieces();
		drawFeedbackSquares();
		//draws cover if game's not over
		if (!end) {
			glColor3f(0.5, 0.5, 0.5);//Color = grey
			drawSquare(0.403125 * width, 0.92121 * height, 0.971875 * width, 0.987877 * height);
		}

		//Updates window
		glfwSwapBuffers(window);

		//Polls for events
		glfwPollEvents();
	}

	//Terminate glfw
	glfwTerminate();
	return 0;
}


void createBoard() {

	glBegin(GL_LINES);

	//Vertical lines
	glColor3f(1, 1, 1);//set color to white
	glVertex2f(x(0.375 * width), y(height));
	glVertex2f(x(0.375 * width), y(-1.0 * height));

	glVertex2f(x(0.1875 * width), y(height));
	glVertex2f(x(0.1875 * width), y(-1.0 * height));

	//Horizontal lines (NOTE:y() and x() are helper methods to calculate %)
	for (int i = 1; i <= 11; i++) {
		glVertex2f(x(0.1875 * width), y((0.0909 * height) * i));
		glVertex2f(x(width), y((0.0909 * height) * i));
	}
	glEnd();

	double xCor1 = 0.04375 * width;
	double xCor2 = 0.14375 * width;

	//RED
	drawPiece(1, xCor1, 0.06666 * height, xCor2, 0.13333 * height);
	//Green
	drawPiece(2, xCor1, 0.14666 * height, xCor2, 0.21333 * height);
	//BLUE
	drawPiece(3, xCor1, 0.22666 * height, xCor2, 0.29333 * height);
	//Yellow
	drawPiece(4, xCor1, 0.30666 * height, xCor2, 0.37333 * height);
	//Purple
	drawPiece(5, xCor1, 0.38666 * height, xCor2, 0.45333 * height);
	//Orange
	drawPiece(6, xCor1, 0.46666 * height, xCor2, 0.53333 * height);

	//Selected Color (CLICK ON THIS TO ENTER COLOR GUESS)
	drawPiece(selected, xCor1, 0.6 * height, xCor2, 0.66666 * height);
}

void drawPieces() {
	//Uses pieces[][] to determine color
	//Color is set when color choice is entered by user as a guess
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 4; j++) {
			//A lot of anoying math to get this equation for spacing the squares relative to i and j
			drawPiece(pieces[i][j], (0.403125 * width) + ((0.15625 * width) * j),
				(0.01212 * height) + ((0.0909 * height) * i),
				(0.503125 * width) + ((0.15625 * width) * j),
				(0.07878 * height) + ((0.0909 * height) * i));
		}
	}
}

void drawFeedbackSquares() {
	//Uses feedbackPieces[][] to determine color
	//Color is set when color choice is entered by user as a guess
	for (int i = 0; i < 10; i++) {
		//A lot of anoying math to get this equation for spacing the squares relative to i
		double x1 = (0.23125 * width); double y1 = (0.01212 * height); double yp = (0.0909 * height); 
		double x2 = (0.29125 * width); double y2 = (0.03878 * height);
		double x3 = (0.27125 * width); double y3 = (0.05212 * height); 
		double x4 = (0.33125 * width); double y4 = (0.07878 * height);

		drawPiece(feedbackPieces[i][0], x1, y1 + (yp * i), x3, y2 + (yp * i));
		drawPiece(feedbackPieces[i][1], x2, y1 + (yp * i), x4, y2 + (yp * i));
		drawPiece(feedbackPieces[i][2], x1, y3 + (yp * i), x3, y4 + (yp * i));
		drawPiece(feedbackPieces[i][3], x2, y3 + (yp * i), x4, y4 + (yp * i));
	}
}

void drawPiece(int color, double topX, double topY, double botX, double botY) {
	//Select Color
	switch (color) {
	case 0:
		glColor3f(0.5, 0.5, 0.5);//Grey (not used)
		break;
	case 1:
		glColor3f(1, 0, 0);//Red
		break;
	case 2:
		glColor3f(0, 1, 0);//Green
		break;
	case 3:
		glColor3f(0, 0, 1);//Blue
		break;
	case 4:
		glColor3f(1, 1, 0);//Yellow
		break;
	case 5:
		glColor3f(0.5, 0, 0.5);//Purple
		break;
	case 6:
		glColor3f(1, 0.5, 0);//Orange
		break;
	}
	//Draw
	drawSquare(topX, topY, botX, botY);
}

void drawSquare(double topX, double topY, double botX, double botY) {
	glBegin(GL_QUADS);
	
	glVertex2f(x(topX), y(topY));
	glVertex2f(x(topX), y(botY));
	glVertex2f(x(botX), y(botY));
	glVertex2f(x(botX), y(topY));

	glEnd();
}

float y(double y) {
	//This calulation takes pixel coordinates and converts them into glfw coordinates
	return ((float)((height - y) * (1.0f / (height/2))) - 1);
}

float x(double x) {
	//This calulation takes pixel coordinates and converts them into glfw coordinates
	return ((float)(1 - ((width - x) * (1.0f / (width/2)))));
}


void mouse_button_callback(GLFWwindow * window, int button, int action, int mods) {

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		//GET MOUSE POSITION
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//Check clicked location
		if (mouseX > (0.042 * width) && mouseX < (0.144 * width)) {
			if (mouseY > 0.06666 * height && mouseY < 0.13333 * height) {
				selected = 1;//red
			}
			else if (mouseY > 0.14666 * height && mouseY < 0.21333 * height) {
				selected = 2;//green
			}
			else if (mouseY > 0.22666 * height && mouseY < 0.29333 * height) {
				selected = 3;//blue
			}
			else if (mouseY > 0.30666 * height && mouseY < 0.37333 * height) {
				selected = 4;//yellow
			}
			else if (mouseY > 0.38666 * height && mouseY < 0.45333 * height) {
				selected = 5;//purple
			}
			else if (mouseY > 0.46666 * height && mouseY < 0.53333 * height) {
				selected = 6;//orange
			}
			else if (mouseY > 0.6 * height && mouseY < 0.66666 * height && selected != 0 && !end) {
				pieces[guess][piece] = selected;//sets color
				codeGuess[piece] = selected;//sets guess color
				piece++;//piece gets incremented
				if (piece > 3) {
					//If four pieces were input then guess is checked and incremented
					if (checkCode() == true)
						end = true;

					guess++;
					piece = 0;	//reset
				}
				if (guess > 9)
					end = true;//Ran out of guesses
			}
		}
	}
}


void generateCode() {
	//Generates four random numbers and stores them in code[]
	for (int i = 0; i < 4; i++) {
		code[i] = (rand() % 6)+1;//Random number from 1-6
		pieces[10][i] = code[i];
	}
}

bool checkCode() {
	bool win = true;	//Win condition
	//Makes it so the same colors can't be matched more than once
	bool matchedCode[4] = {false, false, false, false};
	bool matchedGuess[4] = { false, false, false, false };
	int correct = 0;	//Amount of correct colors and correct locations
	int close = 0;	//Amount of correct colors but incorrect locations

	//Checks correctness
	for (int i = 0; i < 4; i++) {
		if (code[i] == codeGuess[i]) {
			matchedCode[i] = true;
			matchedGuess[i] = true;
			correct++;
		}
		else {
			win = false;
		}
	}
	
	//Checks closeness
	for (int i = 0; i < 4; i++) {
		if (matchedCode[i] == true)
			continue;
		for (int j = 0; j < 4; j++) {
			if (matchedGuess[j] == true)
				continue;
			if (code[i] == codeGuess[j]) {
				matchedCode[i] = true;
				matchedGuess[j] = true;
				close++;
				break;
			}	
		}
	}

	//Sets colors for the feedback pieces so that the user knows what to change
	for (int i = 0; i < correct; i++) {
		feedbackPieces[guess][i] = 2;
	}
	for (int i = 0; i < close; i++) {
		feedbackPieces[guess][i+correct] = 1;
	}
	return win;
}
