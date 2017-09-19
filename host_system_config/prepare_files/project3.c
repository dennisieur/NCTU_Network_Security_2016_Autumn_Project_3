/*
    How to compile this file :
        gcc-4.8 -g -w -Wall -fno-stack-protector -z execstack -o project project3.c
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LENGTH 64
#define TRUE 1

FILE *pFile;
char PASSWD[BUFFER_LENGTH];
char FLAG[BUFFER_LENGTH];
char *this_is_a_complete_shellcode="\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\xb0\x0b\xcd\x80";
int x_location[5] = {9,8,7,6,5};
int y_location[5] = {10,10,10,10,10};

void PUZZLE3_SHELLCODE( void )
{
	char buf[BUFFER_LENGTH];
	printf("Exploit buffer overflow to jump to shellcode !! (address of shellcode: %p)\n", this_is_a_complete_shellcode);
    fflush(stdout);
	fgets(buf, 256, stdin);
}

void NEED_NOT_TO_LOOK__print_message_between_PUZZLE2_PUZZLE3_and_catflag2( void )
{
    pFile = fopen("flag2_file", "r");
    fgets(FLAG, BUFFER_LENGTH, pFile);

	printf("GREAT!!\n");
	printf("%s", FLAG);
    fflush(stdout);
}

void NEED_NOT_TO_LOOK__snake_move( void )
{
	int i;
	char move;

	scanf("%c", &move);

	if ( move=='w' || move=='W')
	{
		for( i=4; i>0; i=i-1){
			x_location[i] = x_location[i-1];
			y_location[i] = y_location[i-1];
		}
		y_location[0] = y_location[0]-1;
	}
	else if ( move=='s' || move=='S')
	{
		for( i=4; i>0; i=i-1){
			x_location[i] = x_location[i-1];
			y_location[i] = y_location[i-1];
		}
		y_location[0] = y_location[0]+1;
	}
	else if ( move=='a' || move=='A')
	{
		for( i=4; i>0; i=i-1){
			x_location[i] = x_location[i-1];
			y_location[i] = y_location[i-1];
		}
                x_location[0] = x_location[0]-1;
	}
	else if ( move=='d' || move=='D')
	{
		for( i=4; i>0; i=i-1){
			x_location[i] = x_location[i-1];
			y_location[i] = y_location[i-1];
		}
		x_location[0] = x_location[0]+1;
	}

	printf("\n");
    fflush(stdout);

}

void NEED_NOT_TO_LOOK__show_snake_message_and_graph( int x_goal, int y_goal )
{
	int i,j;
	system("clear");
	for( j=0; j<39; j=j+1){
		for( i=0; i<39 ; i=i+1 )
		{
			if( i==x_goal && j==y_goal )
				printf("O");
			else if( i==x_location[0] && j==y_location[0] )
				printf("U");
			else if( i==x_location[1] && j==y_location[1] )
				printf("@");
			else if( i==x_location[2] && j==y_location[2] )
				printf("@");
			else if( i==x_location[3] && j==y_location[3] )
				printf("@");
			else if( i==x_location[4] && j==y_location[4] )
				printf("@");
			else if( i==0 && j<=20 )
				printf("*");
			else if( i==30 && j<=20 )
				printf("*");
			else if( j==0 && i<=30 )
				printf("*");
			else if( j==20 && i<=30 )
				printf("*");
			else	printf(" ");
		}
		printf("\n");
	}

	printf("Are you hungry ??\nGet the food 'O' !!!! And you will get the flag !!!!\n");
	printf("['W'=up][ 'A'=left ][ 'S'=down ][ 'D'=right ]\tYour MOVE : ");
    fflush(stdout);
}

void PUZZLE2_BUFFER_OVERFLOW()
{
	char name[BUFFER_LENGTH];
	int width = 30;
	int high = 20;
	int x_goal = 21;
	int y_goal = 31;

	printf("So, let me know who you are, input your STUDENT ID : ");
    fflush(stdout);
	fgets(name, 128, stdin);

	while( TRUE )
	{
		NEED_NOT_TO_LOOK__show_snake_message_and_graph( x_goal, y_goal );
		NEED_NOT_TO_LOOK__snake_move();
		if( x_location[0]==0 || x_location[0]==width || y_location[0]==0 || y_location[0]==high )
		{
			printf("\n[DEAD]\n");
            fflush(stdout);
			exit(0);
		}
		if( x_location[0]==x_goal && y_location[0]==y_goal )
		{
			printf("\n!!!! Congratulations !!!!\n");
            fflush(stdout);
			break;
		}
	}
}

void NEED_NOT_TO_LOOK__print_message_between_PUZZLE1_PUZZLE2_and_catflag1( void )
{
    pFile = fopen("flag1_file", "r");
    fgets(FLAG, BUFFER_LENGTH, pFile);

	printf("GREAT!! You are our student!!\n");
	printf("%s", FLAG);
    printf("TA wants to play a game with you!!\n");
    fflush(stdout);
}

void check_is_admin(char *passwd){

    char buf[BUFFER_LENGTH];

    while( TRUE ){
        printf("Are you STUDENT? Input YOUR password : ");
        fflush(stdout);
        fgets(buf, BUFFER_LENGTH, stdin);
        printf("Your Input : ");
        fflush(stdout);
        printf(buf);
        fflush(stdout);
        printf("\n");
        fflush(stdout);

        if ( strncmp(buf, passwd, 18) == 0 )
            break;
    }
}

void PUZZLE1_FORMAT_STRING( void )
{
	char buf[BUFFER_LENGTH];

    pFile = fopen("passwd_file", "r");
    fgets(PASSWD, BUFFER_LENGTH, pFile);

    strcpy(buf, PASSWD);

    check_is_admin(buf);
}

int main( void ){

	PUZZLE1_FORMAT_STRING();

	NEED_NOT_TO_LOOK__print_message_between_PUZZLE1_PUZZLE2_and_catflag1();

	PUZZLE2_BUFFER_OVERFLOW();

	NEED_NOT_TO_LOOK__print_message_between_PUZZLE2_PUZZLE3_and_catflag2();

	PUZZLE3_SHELLCODE();

	fclose(pFile);
	return 0;
}
