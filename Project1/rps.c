/*Project 1 - Part 1
  Rock, Paper, Scissors
  cs449 - m/w
  Joshua Rodstein
  ps#4121607
 */


#include <stdio.h>
#include <string.h>

int main (){
  unsigned int rand_int;
  char usr_ans[10];
  char cmp_ans[10];
  char result;
  int score[2] = {0,0};

  srand((unsigned int)time(NULL));

  printf("Welcome to Rock, Paper, Scissors\n\n");
  printf("Would you like to play? ");
  scanf("%s", &usr_ans);
  printf("\n");
  
  while (strcasecmp(usr_ans, "yes") == 0){
    printf("What is your choice? ");
    scanf("%s", &usr_ans);
    rand_int = rand() % (3 - 1 + 1) + 1;
    
    if(rand_int == 1) {
      strcpy(cmp_ans, "Rock");
    }else if (rand_int == 2) {
      strcpy(cmp_ans, "Paper");
    }else {
      strcpy(cmp_ans, "Scissors");
    }
    
    if(strcasecmp(usr_ans, cmp_ans) != 0) {
      if(strcasecmp(usr_ans, "Rock")) {
	if(strcasecmp(cmp_ans, "Paper")) {
	  score[1]++;
	  result = 'l';
	}else if(strcasecmp(cmp_ans, "Scissors")){
	  score[0]++;
	  result = 'w';
	}
      }else if(strcasecmp(usr_ans, "Paper")) {
	if(strcasecmp(cmp_ans, "Scissors")) {
	  score[1]++;
	  result = 'l';
	}else if(strcasecmp(cmp_ans, "Rock")) {
	  score[0]++;
	  result = 'w';
	}
      }else{ // usr_ans == scissors
	if(strcasecmp(cmp_ans, "Rock")) {
	  score[1]++;
	  result = 'l';
	}else if(strcasecmp(cmp_ans, "Paper")) {
	  score[0]++;
	  result = 'w';
	}
      }
    }else {
      printf("The computer chooses %s. Try again!\n\n");
      result == 'd';
    }

    if(result == 'w') {
      printf("The computer chooses %s, you win this game!\n\n", cmp_ans);
    }else if(result == 'l') {
      printf("The computer chooses %s, you lose this game!\n\n", cmp_ans);
    }

    printf("The score is now you: %d computer: %d\n\n", score[0], score[1]);

    if(score[0] == 3 ||  score[1] == 3){
      score[0] = 0;
      score[1] = 0;
      printf("Would you like to play again? ");
      scanf("%s", &usr_ans);
      printf("\n");
    } else {
      strcpy(usr_ans, "yes");
    }

  }

  return 0;
}
