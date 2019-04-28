#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>

struct teamNode
{
	char name[100];
	char id[4];
	int iPoints;
	int iWinCount;
	int iLossCount;
	int iDrawCount;
	int iGoalsFor,iGoalsAgainst;

	struct teamNode *llink, *rlink;

};

typedef struct teamNode * TEAMNODEPTR;

int main()
{
	FILE *fptr;
	fptr=fopen("Initiate.bin","wb");
	int i,j;
	struct teamNode team;

	//Initializing all teams name and all other variables to 0
	for(i=0;i<10;i++)
	{
		printf("Enter the name of the Team:");
		gets(team.name);

		for(j=0;j<3;j++)
		{
			team.id[j]=toupper(team.name[j]);
		}
		team.id[4]='\0';

		team.iPoints = team.iWinCount = team.iLossCount = team.iDrawCount = team.iGoalsAgainst = team.iGoalsFor=0;//New File have to be made after removal of ipos
		fwrite(&team, sizeof(struct teamNode), 1, fptr);	
	}
	fclose(fptr);

	//Displaying the file contents
	fptr=fopen("Initiate.bin","rb");
	printf("name\tid\tiPoints\tiWinCount\tiLossCount\tiDrawCount\tiGoalsAgainst\tiGoalsFor\n");
	for(i=0;i<10;i++)
	{
	    fread(&team, sizeof(struct teamNode), 1, fptr); 
	    printf("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%d\n",team.name,team.id,team.iPoints,team.iWinCount,team.iLossCount,team.iDrawCount,team.iGoalsAgainst,team.iGoalsFor);
	}
	fclose(fptr);

	return 0;
}