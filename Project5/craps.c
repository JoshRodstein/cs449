/*********************************************
 * cs449 - Project5 -  "craps"               *
 * Author: Joshua Rodstein <jor94@pitt.edu>  *
 * Univerity of Pittsburgh - Spring 2017     *
*********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>  


int main() {
  char name[20];                        // char buffer to hole user name 19 cahr max, 20th char for null term
  char pq[10];                          // Play or quit buffer contorl intial loop entrance, and replay
  char *dice = malloc(sizeof(char)*2);  // hold initial dice roll values from read()
  int idice[2];                         // Integer array holds integer values of dice rolls for printing
  int point = 0;                        // holds the point when 2, 3, 12, 7, or 11, not initially rolled
  int roll;                             // hold sum of both dice after roll
  int d;                                // file descriptor for /dev/dice && loop exit int value

  printf("Welcome to Joshs Casino!\n");
  printf("Please enter your name: ");
  scanf("%19s", &name);                 // %19s limits user input to first 19 chars in name, 20th reserved for '\0'
 
  /* Input verification, will not proceed unless 'Play' or 'Quit' enetered
   *  NOT case sensative
   */
  do{
    printf("%s, would you like to Play or Quit? ", name);
    scanf(" %9s", &pq);                 // %9s limits buffer to 9 chars
    printf("\n");
  }while(strcasecmp(pq, "play") != 0 && strcasecmp(pq, "quit") != 0);
  
  /* While loop for craps game */
  while(strcasecmp(pq, "play")==0){
    /* open dice_driver module at '/dev/dice', assign file descriptor to 'd' 
     * read() recieves file descriptor, user supplied buffer for random bytes
     * and size of buffer. random bytes will be assigned ascending index order 
     * to dice buffer. close(d) closes "file" stream. 
     */
    d = open("/dev/dice", O_RDONLY);   
    read(d, dice, 2);
    close(d);
  
    /* store sum of rolls in roll. Make sur ein 1-7 range by adding 1 to each.
     * Store each dice value + 1 into a new integer array for printing.
     */
    roll = ((dice[1]) + 1) + ((dice[0]+1)); 
    idice[0] = dice[0] + 1;
    idice[1] = dice[1] + 1;
 
    printf("\nYou have rolled %d + %d = %d\n", idice[0], idice[1],
	   roll);
  

    /* control structure Logic to determine win or lose */
    if(point > 0){
      if(roll == 7){
	//lose
	point = 0;
	printf("You Lose!\n");
      } else if (roll == point){
	//win
	point = 0;
	printf("You Win!\n");
      } else {
	continue; // re-roll to match the point
      }
    } else if (roll == 2 || roll == 3 || roll == 12){
	// lose
      printf("You Lose!\n");
    } else if (roll == 7 || roll == 11){
      // win
      printf("You Win!\n");
    } else {
      point = roll;
      continue;  // re-roll to match the point
    }
    
    /* Final input verification to either exit or replay */
    do{
      printf("\nWould you like to play again? ");
      scanf(" %s", &pq);
      if(strcasecmp(pq, "yes") != 0 && strcasecmp(pq, "no") != 0){
	printf("\nPlease enter 'Yes' or 'No'... \n");
	d = 1;
      } else {
	d = 0;
      }
    } while(d);

    if(strcasecmp(pq, "no") == 0){
      break;
    }
    
    strcpy(pq, "play");
  }

  printf("\nGoodbye, %s!\n", name);

  /* Free malloc'd dice array */
  free(dice);
  return 0;
}
