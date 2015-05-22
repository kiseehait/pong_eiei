//
// PONG EIEI VERSION 1.0.A
// Created by Seehait Chockthanyawat
//

#include <iostream>
#include <unistd.h>
#include <thread>
#include <string>

using namespace std;

// game version
const string game_version = "1.0.a";

// input
char c = 'a';

// game status
bool game_start = false;

// screen
const int screen_refreshing_coodown = 30000;
char screen[24][80];

// toggle
const int toggle_cooldown = 100;
bool toggle = true;
int toggle_count = 0;

// ball
const int ball_default_x = 39, ball_default_y = 11;
int ball_x = ball_default_x, ball_y = ball_default_y;
bool ball_dir_x = true, ball_dir_y = true;

// player
const int player_default_y = 9, player_max_score = 3, player_paddle = 5;
int player1_score, player2_score;
int player1_y = player_default_y, player2_y = player_default_y;

// winner
int winner = 0;

// init terminal
void init() {
	system("stty raw");

	// hide cursor
	printf("%c[?25l", 0x1B);

	// init screen
	for (int i = 0; i < 24; i++) {
		for (int j = 0; j < 80; j++) {
			if (j == 3 && i >= player1_y && i <= player1_y + 5) screen[i][j] = ']';
			else if (j == 76 && i >= player2_y && i <= player2_y + 5) screen[i][j] = '[';
			else if (i == ball_y && j == ball_x) screen[i][j] = '*';
			else screen[i][j] = ' ';
		}
	}
}

// clear screen
void clear_screen() {
	printf("%c[1J", 0x1B);
}

// draw game title
void draw_title() {
	while (true) {
		if (!game_start) {
			printf("%c[1;1H", 0x1B);

			printf("%c[%dm", 0x1B, 32);
			printf("%c[%dm", 0x1B, 49);

			printf("                                                                                ");
			printf("                                                                                ");
			printf("                                                                                ");
			printf("             ████   ███  █   █  ███         █████ █████ █████ █████             ");
			printf("             █   █ █   █ ██  █ █            █       █   █       █               ");

			printf("             ████  █   █ █ █ █ █  ██        █████   █   █████   █               ");
			printf("             █     █   █ █  ██ █   █        █       █   █       █               ");
			printf("             █      ███  █   █  ███         █████ █████ █████ █████             ");
			printf("                                                                                ");

			printf("%c[%dm", 0x1B, 36);

			printf("                           by Seehait Chockthanyawat                            ");

			printf("                                                                                ");
			
			printf("%c[%dm", 0x1B, 31);
			if (toggle) {
				if (winner == 0) printf("                                                                                ");
				else if (winner == 1) printf("                                  Player1 WIN!                                  ");
				else if (winner == 2) printf("                                  Player2 WIN!                                  ");
			} else {
				printf("                                                                                ");
			}
			printf("%c[%dm", 0x1B, 34);

			printf("                                                                                ");
			printf("                                                                                ");
			printf("                                                                                ");

			printf("                                                                                ");
			printf("                                                                                ");
			printf("                                                                                ");
			printf("                                                                                ");
			printf("                                                                                ");

			if (toggle) printf("                                 PRESS SPACEBAR                                 ");
			else printf("                                                                                ");
			printf("                                                                                ");
			printf("                                                                                ");

			printf("%c[%dm", 0x1B, 33);
			printf("                                                                   VERSION 1.0.a");
			// if (game_start) cout << "EXIT" << endl;
			// printf("PLAYER1 SCORE: %d                                                PLAYER2 SCORE: %d", player1_score, player2_score);
			if (toggle_count > toggle_cooldown) {
				toggle = !toggle;
				toggle_count = 0;
			}
			toggle_count++;
		}
	}
	//terminate();
}

// draw current game
void draw_game() {
	printf("%c[1;1H", 0x1B);

	printf("%c[%dm", 0x1B, 37);
	printf("%c[%dm", 0x1B, 40);

	for (int i = 0; i < 23; i++) {
		for (int j = 0; j < 80; j++) cout << screen[i][j];
	}
	printf("Player1's score: %d                                            Player2's score: %d", player1_score, player2_score);
}

// move ball
void move_ball() {
	if (ball_dir_x) ball_x += 2;
	else ball_x -= 2;
	
	if (ball_dir_y) ball_y++;
	else ball_y--;

	// if (ball_x == 0 || ball_x == 7player_default_y) ball_dir_x = !ball_dir_x;
	if (ball_x == 5 && ball_y >= player1_y && ball_y <= player1_y + player_paddle) {
		ball_dir_x = !ball_dir_x;
	}
	if (ball_x == 75 && ball_y >= player2_y && ball_y <= player2_y + player_paddle) {
		ball_dir_x = !ball_dir_x;
	}

	if (ball_x >= 79) {
		ball_dir_x = true;
		ball_dir_y = true;
		ball_x = ball_default_x;
		ball_y = ball_default_y;
		player1_score++;
	}

	if (ball_x <= 0) {
		ball_dir_x = true;
		ball_dir_y = true;
		ball_x = ball_default_x;
		ball_y = ball_default_y;
		player2_score++;
	}

	if (ball_y == 0 || ball_y == 22) ball_dir_y = !ball_dir_y;

	if (player1_score > player_max_score - 1 || player2_score > player_max_score - 1) {
		winner = player1_score > player2_score ? 1 : 2;
		player1_score = 0;
		player2_score = 0;
		game_start = false;
	}
}

// input detector
void get_input() {
	while (true) {
		c = getchar();
		if (c == 'w' && player1_y > 0) player1_y--;
		if (c == 's' && player1_y < 22 - player_paddle) player1_y++;
		if (c == 'i' && player2_y > 0) player2_y--;
		if (c == 'k' && player2_y < 22 - player_paddle) player2_y++;
		
		if (c == ' ') {
			game_start = true;
		}

	}
}

// manage game screen
void screen_manager() {
	while(true) {
		if (game_start) {
			draw_game();
			usleep(screen_refreshing_coodown);
			move_ball();
			init();
		} else {
			player1_y = player_default_y;
			player2_y = player_default_y;
		}
	}
}

// set title bar
void set_title() {
	char esc_start[] = { 0x1b, ']', '0', ';', 0 };
    char esc_end[] = { 0x07, 0 };
    cout << esc_start << "Pong Eiei " << game_version << esc_end;
}

// main
int main()
{
	// terminal init
	init();
	set_title();

	// threads init
	thread tt(draw_title);
	thread sm(screen_manager);
	thread ip(get_input);

	// threads start
	ip.join();
	tt.join();
	sm.join();
}