#include "iGraphics.h"
#include <string>
#include <windows.h>
#include <mmsystem.h>
#include <ctime> 
#pragma comment(lib, "winmm.lib")
#define buttonMusic mciSendString("open \"asset\\music\\button sound.wav\" type waveaudio alias buttonSound", NULL, 0, NULL);
#define bgMusic mciSendString("open \"asset\\music\\music_bg.wav\" type mpegvideo alias bgmusic", NULL, 0, NULL);

// Screen size
int screenSizeX = 1000;
int screenSizeY = 800;
int maxDepth = 4000;



int playBg_y = screenSizeY;
int playBg_x = 0;
int rod_x = 237;
int rod_y = 490;
int string_length = 10;
int hook_radius = 5;
int hook_x = rod_x;
int hook_y = rod_y-150;
int string_move_speed = 5;
int boat_speed = 5;
int screen_horizontal_speed = 7;

int cat_x = 5;
int cat_y = 365;

int screen_scroll_speed = 10;

const int MAX_FISH_NUMBER = 35;

// Background images
int menuBg, aboutUsBg, highScoreBg, playBg, catBg; // Added playBg

//Fish var
enum FishType { BLUEANGEL, HOLUD, LALTHOT, BEGUNITHOT, FREEZEORB, SHARK, HOLUDDORI, NEMO, OCTOPUS };

//
int blueAngel_left;
int blueAngel_right;

int begunithot_left;
int begunithot_right;

int holud_left;
int holud_right;

int lalthot_left;
int lalthot_right;

int shark_left;
int shark_right;

int holud_dori_left;
int holud_dori_right;

int nemo_left;
int nemo_right;

int octopus_left;
int octopus_right;

int freezeOrb;
bool isFrozen = false;
int freezeDuration = 7000;
clock_t freezeStartTime = 0;
int freeze_bg;

int ink_bg;
bool isInk = false;
int inkDuration = 5000;
clock_t inkStartTime = 0;

int three_hp_pic,two_hp_pic,one_hp_pic;

int total_money = 0;
int depth = 0;
int hp = 3;

// Button properties
struct Button {
	int x, y;
	int width, height;
	bool isHovered;
} buttons[4];

// Button labels
char* buttonLabels[] = { "Play", "High Score", "About Us", "Exit" };

// Back button properties (for all screens)
struct Button backButton = { 20, screenSizeY - 75, 200, 50, false };

// Current screen state
enum Screen { MENU, PLAY, HIGH_SCORE, ABOUT_US };
Screen currentScreen = MENU;

// Initialize button positions and sizes
void initializeButtons() {
	int buttonWidth = 200;
	int buttonHeight = 50;
	int startX = (screenSizeX - buttonWidth) / 2;
	int startY = 400;

	for (int i = 0; i < 4; i++) {
		buttons[i].x = startX;
		buttons[i].y = startY - i * (buttonHeight + 20);
		buttons[i].width = buttonWidth;
		buttons[i].height = buttonHeight;
		buttons[i].isHovered = false;
	}
}

// Back button 16jan
void drawBackButton() {
	if (currentScreen != MENU) {
		// Draw button background
		if (backButton.isHovered) {
			iSetColor(0, 102, 204);  // Hover color next page
			iFilledRectangle(backButton.x - 10, backButton.y - 5, backButton.width + 20, backButton.height + 10);
		}
		else {
			iSetColor(86, 164, 230);   // Normal color
			iFilledRectangle(backButton.x, backButton.y, backButton.width, backButton.height);
		}

		// Center the text
		iSetColor(255, 255, 255);  // White text

		int textWidth = 8 * strlen("Back"); // Approximate text width
		int textX = backButton.x + (backButton.width - textWidth) / 2;

		int textHeight = 12; // Approximate text height for HELVETICA_18
		int textY = backButton.y + (backButton.height - textHeight) / 2;

		// Draw "Back" text
		if (backButton.isHovered) {
			iText(textX, textY, "Back", GLUT_BITMAP_9_BY_15); // Larger font on hover
		}
		else {
			iText(textX, textY, "Back", GLUT_BITMAP_8_BY_13); // Smaller font when not hovered
		}
	}
}


//FISHFISH
struct Fish{
	int fish_x, fish_y; //position
	int width, height;
	FishType type;

	int speed;
	bool movingRight;
	int imageRight;
	int imageLeft;
	bool isAttached = false;

	bool isSpecial = false;
	int money = 0;
};
Fish fishArray[50];

Fish createFish() {

	Fish fish;
	int spawnRoll = rand() % 100;


	if (spawnRoll <= 60){

		int randNum = rand() % 8;
		switch (randNum)
		{
			

		case 0:
			fish.type = BLUEANGEL;
			fish.width = 45;
			fish.height = 55;
			fish.speed = 1;
			fish.imageRight = blueAngel_right;
			fish.imageLeft = blueAngel_left;
			fish.money = 200;
			
			break;

		case 1:
			fish.type = LALTHOT;
			fish.width = 45;
			fish.height = 45;
			fish.speed = 5;
			fish.imageRight = lalthot_right;
			fish.imageLeft = lalthot_left;
			fish.money = 100;
			
			break;

		case 2:
			fish.type = SHARK;
			fish.width = 250;
			fish.height = 150;
			fish.speed = 10;
			fish.imageLeft = shark_left;
			fish.imageRight = shark_right;
			break;

		case 3:
			fish.type = HOLUDDORI;
			fish.width = 50;
			fish.height = 50;
			fish.speed = 10;
			fish.imageLeft = holud_dori_left;
			fish.imageRight = holud_dori_right;
			break;
		case 4:
			fish.type = FREEZEORB;
			fish.width = 50;
			fish.height = 50;
			fish.speed = 10;
			fish.imageLeft = freezeOrb;
			fish.imageRight = fish.imageLeft;
			break;
		case 5:
			fish.type = NEMO;
			fish.width = 90;
			fish.height = 50;
			fish.speed = 10;
			fish.imageLeft = nemo_left;
			fish.imageRight = nemo_right;
			break;
		case 6:
			fish.type = OCTOPUS;
			fish.width = 90;
			fish.height = 90;
			fish.speed = 10;
			fish.imageLeft = octopus_left;
			fish.imageRight = octopus_right;
			break;

		default:
			fish.type =BEGUNITHOT;
			fish.width = 45;
			fish.height = 45;
			fish.speed = 5;
			fish.imageRight = begunithot_right;
			fish.imageLeft = begunithot_left;
			
			break;

			
		}

	}
	else{
		fish.type = HOLUD;
		fish.width = 45;
		fish.height = 45;
		fish.speed = 5;
		fish.imageRight = holud_right;
		fish.imageLeft = holud_left;
		printf("type 1");
	}
	





	// Random position and direction
	fish.fish_x = rand() % screenSizeX;
	fish.fish_y =-( rand() % (maxDepth));  //need further modification
	printf("%d", fish.fish_y);
	fish.movingRight = rand() % 2;

	return fish;
}

void spawnFish() {

	for (int i = 0; i < MAX_FISH_NUMBER; i++) {
		fishArray[i] = createFish();
	}

}

void drawFishes() {

	for (int i = 0; i < MAX_FISH_NUMBER; i++) {

		Fish &fish = fishArray[i];
		char imageRight[100], imageLeft[100];
		int fishImage = fish.movingRight ? fish.imageRight : fish.imageLeft;
		iShowImage(fish.fish_x, fish.fish_y, fish.width, fish.height, fishImage);

	}
}



void moveFish() {


	if (isFrozen) {
		// Check if 5 seconds have passed
		if ((clock() - freezeStartTime) * 1000 / CLOCKS_PER_SEC > freezeDuration) {
			isFrozen = false; // Unfreeze fish
		}
		else {
			return; // Stop fish movement
		}
	}


	for (int i = 0; i <MAX_FISH_NUMBER; i++) {
		Fish &fish = fishArray[i];
		fish.fish_x += fish.movingRight ? fish.speed : -fish.speed;

		// Wrap around screen
		if (fish.fish_x > screenSizeX) fish.fish_x = -fish.width;
		if (fish.fish_x + fish.width < 0) fish.fish_x = screenSizeX;
	}





}


void moveFishVertically(int direction){



	for (int i = 0; i < MAX_FISH_NUMBER; i++){
		Fish &fish = fishArray[i];
		fish.fish_y =fish.fish_y + (direction) * screen_scroll_speed;
	}
}


bool checkCollision(int hookX, int hookY, int hookRadius, Fish fish) {
	return !(hookX + hookRadius < fish.fish_x || hookX - hookRadius > fish.fish_x + fish.width ||
		hookY + hookRadius < fish.fish_y || hookY - hookRadius > fish.fish_y + fish.height);
}

void successfulCatch(int i){

	total_money+=fishArray[i].money;
	fishArray[i] = createFish();
	


}

void fishAttach(int i){

	

	Fish &fish = fishArray[i];

	if (fish.type == FREEZEORB){

		isFrozen = true;
		freezeStartTime = clock();
		successfulCatch(i);
		return;
	}
	else if (fish.type==SHARK)
	{
		hp--;
		successfulCatch(i);
		return;
	}
	else if (fish.type == OCTOPUS){
		isInk = true;
		inkStartTime = clock();
		successfulCatch(i);
		return;
	}

	fish.fish_y = hook_y-fish.height/2;
	fish.fish_x = hook_x-5;
	fish.isAttached = true;
	//218 530
	if (hook_x == rod_x && hook_y >= rod_y-150)
		successfulCatch(i);
		


}
void handleCollisions() {
	for (int i = 0; i < MAX_FISH_NUMBER; i++) {
	if (checkCollision(hook_x, hook_y, hook_radius, fishArray[i])) {
			fishAttach(i);
	}
	}
}






// Draw the main menu screen 16 jan
void drawMenu() {
	iClear();
	iShowImage(0, 0, screenSizeX, screenSizeY, menuBg);
	for (int i = 0; i < 4; i++) {
		Button btn = buttons[i];

		// Draw button background
		if (btn.isHovered) {
			iSetColor(0, 102, 204);  // Hover color
			iFilledRectangle(btn.x - 10, btn.y - 5, btn.width + 20, btn.height + 10);
		}
		else {
			iSetColor(86, 164, 230);   // Normal color
			iFilledRectangle(btn.x, btn.y, btn.width, btn.height);
		}

		// Draw button text
		iSetColor(255, 255, 255);  // White text

		// Calculate centered text position
		int textWidth = 8 * strlen(buttonLabels[i]); // Approximate width per character
		int textX = btn.x + (btn.width - textWidth) / 2;

		int textHeight = 12; // Approximate text height for HELVETICA_18
		int textY = btn.y + (btn.height - textHeight) / 2;

		// Add bold effect when hovered
		if (btn.isHovered) {
			iText(textX, textY, buttonLabels[i], GLUT_BITMAP_9_BY_15);
		}
		else {
			iText(textX, textY, buttonLabels[i], GLUT_BITMAP_8_BY_13);
		}
	}

}

// Draw the "About Us" screen 16 jan
void drawAboutUs() {
	iClear();
	iShowImage(0, 0, screenSizeX, screenSizeY, aboutUsBg);
	// Draw the title
	int titleWidth = 12 * strlen("About Us");
	int titleX = (screenSizeX - titleWidth) / 2;
	int titleY = screenSizeY - 100;  // Slightly below the top

	iSetColor(255, 255, 255);
	iText(titleX, titleY, "About Us", GLUT_BITMAP_9_BY_15); //next pg chng

	drawBackButton();
}

// Draw the "High Score" screen
void drawHighScore() {
	iClear();
	iShowImage(0, 0, screenSizeX, screenSizeY, highScoreBg);
	drawBackButton();
}

// Draw the "Play" screen (Game screen, could be just a placeholder for now)
void drawPlay() {
	iClear();
	iShowImage(playBg_x, -playBg_y-2400, 1600, maxDepth, playBg); // Display the play background image
	iShowImage(cat_x, cat_y, 240, 200, catBg);
	if (isFrozen)
		iShowImage(0,0, screenSizeX, screenSizeY, freeze_bg);

	


	iSetColor(155, 75, 0);
	iLine(rod_x, rod_y, hook_x, hook_y);
	iSetColor(155, 75, 0);
	iFilledCircle(hook_x, hook_y, hook_radius);
	drawFishes();
	if (isInk){
		iShowImage(0, 0, screenSizeX, screenSizeY, ink_bg);
		if ((clock() - inkStartTime) * 1000 / CLOCKS_PER_SEC > inkDuration)
			isInk = false;
	}
	handleCollisions();

	char moneyText[50];
	sprintf_s(moneyText, sizeof(moneyText), "Total Money: Tk.%d", total_money);
	iSetColor(255, 255, 0);  // Yellow color
	iText(20, screenSizeY - 40, moneyText, GLUT_BITMAP_HELVETICA_18);

	if (hp == 3){
		iShowImage(screenSizeX - 150, screenSizeY - 40, 150, 33, three_hp_pic);
	}
	else if (hp == 2){
		iShowImage(screenSizeX - 150, screenSizeY - 40, 150, 33, two_hp_pic);

	}
	else if (hp == 1){
		iShowImage(screenSizeX - 150, screenSizeY - 40, 150, 33, one_hp_pic);

	}

	char depthText[50];
	sprintf_s(depthText, sizeof(depthText), "Depth: %dm", depth);
	iSetColor(255, 255, 0);  // Yellow color
	iText(screenSizeX - 110, screenSizeY - 60, depthText, GLUT_BITMAP_HELVETICA_18);
}

// Main drawing function
void iDraw() {
	if (currentScreen == MENU) {
		drawMenu();
	}
	else if (currentScreen == ABOUT_US) {
	
		drawAboutUs();
	}
	else if (currentScreen == HIGH_SCORE) {
		drawHighScore();
	}
	else if (currentScreen == PLAY) {
		drawPlay();
	}
}

// Handle passive mouse movement to check if the cursor is over a button
void iPassiveMouseMove(int mx, int my) {
	for (int i = 0; i < 4; i++) {
		if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].width &&
			my >= buttons[i].y && my <= buttons[i].y + buttons[i].height) {
			buttons[i].isHovered = true;
		}
		else {
			buttons[i].isHovered = false;
		}
	}

	// Check if the mouse is hovering over the back button
	if (mx >= backButton.x && mx <= backButton.x + backButton.width &&
		my >= backButton.y && my <= backButton.y + backButton.height) {
		backButton.isHovered = true;
	}
	else {
		backButton.isHovered = false;
	}
}

// Handle mouse clicks for navigation 16 jan
void iMouse(int button, int state, int mx, int my) {
	buttonMusic
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = 0; i < 4; i++) {
			if (mx >= buttons[i].x && mx <= buttons[i].x + buttons[i].width &&
				my >= buttons[i].y && my <= buttons[i].y + buttons[i].height) {

				// Play sound when a button is clicked
				mciSendString("play buttonSound from 0", NULL, 0, NULL);

				// Handle button clicks to change screens
				if (i == 0) {
					currentScreen = PLAY;  // Start the game
				}
				else if (i == 1) {
					currentScreen = HIGH_SCORE;  // Show high scores
				}
				else if (i == 2) {
					currentScreen = ABOUT_US;  // Show about us information
				}
				else if (i == 3) {
					exit(0);  // Exit the game
				}
			}
		}

		// Back to Menu (for "About Us" and "High Score" screens)
		if (currentScreen == ABOUT_US || currentScreen == HIGH_SCORE || currentScreen == PLAY) {
			if (mx >= backButton.x && mx <= backButton.x + backButton.width &&
				my >= backButton.y && my <= backButton.y + backButton.height) {

				// Play sound when the back button is clicked
				mciSendString("play buttonSound from 0", NULL, 0, NULL);

				currentScreen = MENU;  // Go back to the main menu
			}
		}
	}
}

void iMouseMove(int mx, int my) {
	// Empty function to handle mouse dragging
}

void iKeyboard(unsigned char key) {
	if (key == '\r') {
		printf("Enter key pressed!\n");
	}
	if (key == 'p') {
		printf("P key pressed - Game paused!\n");
	}

	// Adjust background position based on key inputs
	if (key == 'w'){


		if (playBg_y < screenSizeY) {
			playBg_y += screen_scroll_speed; // Move up, but do not go above the top of the image
			cat_y -= screen_scroll_speed;
			rod_y -= screen_scroll_speed;
			moveFishVertically(-1);
			depth--;
		}

		if (hook_y<340){
			hook_y += string_move_speed;
			depth--;
			
		}

	}
	if (key == 's'){

		
		if (playBg_y>-2400){
			playBg_y -= screen_scroll_speed;
			cat_y += screen_scroll_speed;
			rod_y += screen_scroll_speed;// Move down continuously 
			moveFishVertically(1);
			depth++;
		}
		else{
			if (hook_y > 0){
				hook_y -= string_move_speed;
				depth++;
			}
		}

		if (hook_y > 350){
			hook_y -= string_move_speed;
			
		}
			
		
	

		
	}
	

	else if (key == 'a') { // Move the hook left
		if (hook_x > 0) {
			hook_x -= string_move_speed;
		}
	}
	else if (key == 'd') { // Move the hook right
		if (hook_x < screenSizeX) {
			hook_x += string_move_speed;
		}
	}
}



void iSpecialKeyboard(unsigned char key) {
	if (key == GLUT_KEY_RIGHT) {
		
		if (cat_x<800){
			cat_x += boat_speed;
			rod_x += 5;

		}
			
		if (playBg_x>-600)
			playBg_x -= screen_horizontal_speed;
	}
	if (key == GLUT_KEY_LEFT) {
		if (cat_x > 0){
			cat_x -= boat_speed;
			rod_x -= 5;

		}
			
		if (playBg_x<0)
			playBg_x += screen_horizontal_speed;
		
	}
	if (key == GLUT_KEY_UP) {
		printf("Up arrow key pressed!\n");
	}
	if (key == GLUT_KEY_DOWN) {
		printf("Down arrow key pressed!\n");
	}
	if (key == GLUT_KEY_HOME) {
		printf("Home key pressed!\n");
	}
}

void loadRandomBg(){

	menuBg = iLoadImage("asset\\background\\menu_bg.png");
	aboutUsBg = iLoadImage("asset\\background\\about us.png");
	highScoreBg = iLoadImage("asset\\background\\menu_bg.png");
	catBg = iLoadImage("asset\\background\\cat.png");
	int randomNum = rand() % 4;

	freeze_bg = iLoadImage("asset\\background\\freeze_bg.png");
	ink_bg = iLoadImage("asset\\background\\ink_bg.png");

	switch (randomNum)
	{
	case 0:
		playBg = iLoadImage("asset\\background\\green_hill_bg.png");
		break;
	case 1:
		playBg = iLoadImage("asset\\background\\cherry_bg.png");
		break;
	case 2:
		playBg = iLoadImage("asset\\background\\night_bg.png");
		break;
	default:
		playBg = iLoadImage("asset\\background\\morning_bg.png");
		break;
	}
}

void loadPower(){

	three_hp_pic = iLoadImage("asset\\power\\three_hp.png");
	two_hp_pic = iLoadImage("asset\\power\\two_hp.png");
	one_hp_pic = iLoadImage("asset\\power\\one_hp.png");
	freezeOrb = iLoadImage("asset\\power\\freeze_orb.png");
}
void loadFish(){

	blueAngel_left = iLoadImage("asset\\fish\\blueAngel_left.png");
	blueAngel_right = iLoadImage("asset\\fish\\blueAngel_right.png");

	lalthot_left = iLoadImage("asset\\fish\\lalthot_left.png");
	lalthot_right = iLoadImage("asset\\fish\\lalthot_right.png");

	holud_left = iLoadImage("asset\\fish\\holud_left.png");
	holud_right = iLoadImage("asset\\fish\\holud_right.png");

	begunithot_left = iLoadImage("asset\\fish\\begunithot_left.png");
	begunithot_right = iLoadImage("asset\\fish\\begunithot_right.png");

	shark_left = iLoadImage("asset\\fish\\shark_left.png");
	shark_right = iLoadImage("asset\\fish\\shark_right.png");

	holud_dori_left = iLoadImage("asset\\fish\\holud_dori_left.png");
	holud_dori_right = iLoadImage("asset\\fish\\holud_dori_right.png");

	nemo_left = iLoadImage("asset\\fish\\nemo_left.png");
	nemo_right = iLoadImage("asset\\fish\\nemo_right.png");
	
	octopus_left = iLoadImage("asset\\fish\\octopus_left.png");
	octopus_right = iLoadImage("asset\\fish\\octopus_right.png");

}
int main() {
	iInitialize(screenSizeX, screenSizeY, "Fun-tastic Catch!");

	srand(time(0));
	// Load background images
	

	//load bunch of fishes
	loadRandomBg();
	loadFish();
	loadPower();
	spawnFish();
	iSetTimer(20, moveFish);

	//Play background music in a loop, and ensure it doesn't stop when switching screens
	bgMusic
		mciSendString("play bgmusic repeat", NULL, 0, NULL);

	initializeButtons();
	iStart();
	mciSendString("stop bgmusic repeat", NULL, 0, NULL);
	mciSendString("close bgmusic repeat", NULL, 0, NULL);
	return 0;
}
