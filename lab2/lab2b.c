#include <stdio.h>

int main() {
	int weight;

	printf("Please enter the weight you'd like to convert: ");
	scanf("%d", &weight);

	printf("\nHere is the weight on other planets: \n\n"
	"Mercury   %d lbs\n"
	"Venus         %d lbs\n"
	"Mars	   %d lbs\n"
	"Jupiter   %d lbs\n"
	"Saturn	       %d lbs\n"  	
	"Uranus	       %d lbs\n"				
	"Neptune   %d lbs\n",
	(38*weight)/100,
	(91*weight)/100,
	(38*weight)/100,
	(254*weight)/100,
	(108*weight)/100,
	(91*weight)/100,
	(119*weight)/100);


	return 0;

}
