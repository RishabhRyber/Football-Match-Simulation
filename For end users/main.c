#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<conio.h>


//this structure will contail info about an individual match
struct matchNode
{
	char cTeam1[100];
	char cTeam2[100];
	int iGoalCount;
	int iTimeOfGoals[15];
	int iGoalsScorer[15];
	char cWinnerTeam[100];
	struct matchNode *rlink,*llink;
};

//this structure will contail info about an individual team
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

typedef struct matchNode* MATCHNODEPTR;
typedef struct teamNode* TEAMNODEPTR;


MATCHNODEPTR firstMatch = NULL;

TEAMNODEPTR conductMatch(TEAMNODEPTR ,char [],char []);
TEAMNODEPTR sortTeams(TEAMNODEPTR ,TEAMNODEPTR );
TEAMNODEPTR copyStruct(TEAMNODEPTR ,TEAMNODEPTR );
TEAMNODEPTR findTeamById(TEAMNODEPTR ,char []);
void showLeaderBoard(TEAMNODEPTR );
void showTeams(TEAMNODEPTR );
void showTeamStats(TEAMNODEPTR ,char []);
TEAMNODEPTR fetchTeamsToNodes();
TEAMNODEPTR insertRearToTeam(TEAMNODEPTR ,struct teamNode);
//MATCHNODEPTR insertFrontToMatch(MATCHNODEPTR, struct matchNode);
void viewPrevMatches(MATCHNODEPTR firstMatch,int iMatchCount);
void exitProgram(TEAMNODEPTR firstTeam,MATCHNODEPTR firstMatch);
void intro();
TEAMNODEPTR resetAll(TEAMNODEPTR);
//MATCHNODEPTR fetchMatchToNodes();



int main(){

	TEAMNODEPTR firstTeam=fetchTeamsToNodes();
	char tempBuffer[20];
	int iTeamCount=-1,iChoice;
	char teamID1[4], teamID2[4];
	intro();
	system("cls");
//	firstMatch=fetchMatchToNodes();

	while(1){
		printf("\n");
		printf("Menu:\n\n");
		printf("1.Show the complete LeaderBoard\n");
		printf("2.Conduct a match\n");
		printf("3.Check Individual Team stats\n");
		printf("4.Show the stats of previous matches in this session\n");
		printf("5.Reset the Leaderboard\n6.Exit\n\n");
		printf("Enter your choice to proceed with:\n");
		scanf("%d",&iChoice);
		switch(iChoice){
			case 1:
					showLeaderBoard(firstTeam);
					break;
			case 2:
					showTeams(firstTeam);
					printf("Enter team id of the first team\n");
					scanf("%3s",teamID1);
					gets(tempBuffer);
					printf("Enter team id of the second team\n");
					scanf("%3s",teamID2);
					gets(tempBuffer);
					firstTeam=conductMatch(firstTeam,teamID1,teamID2);
					break;
			case 3:
					showTeams(firstTeam);
					printf("Enter team id of the team\n\n");
					scanf("%s",teamID1);
					showTeamStats(firstTeam,teamID1);
					break;
			case 4:
					printf("Enter the count of previous matches to view in this session\n");
					scanf("%d",&iTeamCount);
					viewPrevMatches(firstMatch,iTeamCount);
					break;
			case 5:
					firstTeam=resetAll(firstTeam);
					break;
			case 6:
					exitProgram(firstTeam,firstMatch);
					break;

			default: printf("Invalid Choice\n");
		}
	}
	return 0;
}



//Function used to Conduct a Match
TEAMNODEPTR conductMatch(TEAMNODEPTR firstTeam,char teamID1[],char teamID2[])
	{
		srand(time(NULL));
		MATCHNODEPTR newMatchNode=(MATCHNODEPTR)malloc(sizeof(struct matchNode));

		TEAMNODEPTR team1=findTeamById(firstTeam,teamID1);//this function will return location of node with given id
		TEAMNODEPTR team2=findTeamById(firstTeam,teamID2);//this function will return location of node with given id

		int curTime=rand()%20,team1Goal=0,team2Goal=0;

		//initialise the variables of newmatch node to zero
		newMatchNode->iGoalCount=0;
		for (int i = 0; i < 15; ++i)
		{
		newMatchNode->iGoalsScorer[i]=0;
		newMatchNode->iTimeOfGoals[i]=0;
		}
		strcpy(newMatchNode->cWinnerTeam,"DRW");
		strcpy(newMatchNode->cTeam1,team1->name);
		strcpy(newMatchNode->cTeam2,team2->name);


		while(1){

			newMatchNode->iGoalsScorer[newMatchNode->iGoalCount]=1+rand()%2; //stores random 1 or 2 to indicate the team scored
			newMatchNode->iTimeOfGoals[newMatchNode->iGoalCount]=curTime; //stores random 1 or 2 to indicate timeo of goal


			if(newMatchNode->iGoalsScorer[newMatchNode->iGoalCount]==1){
				team1Goal++;

			}else{
				team2Goal++;
			}

			(newMatchNode->iGoalCount)++;

			if(curTime>=85||newMatchNode->iGoalCount>=10)
				break;

			curTime+=rand()%(90-curTime);										//this is for generating the time for the goal

		}

		(team1->iGoalsFor)+=team1Goal;
		(team1->iGoalsAgainst)+=team2Goal;
		(team2->iGoalsFor)+=team2Goal;
		(team2->iGoalsAgainst)+=team1Goal;


		if(team1Goal>team2Goal){
			strcpy(newMatchNode->cWinnerTeam,team1->name);
			team1->iWinCount++;
			team2->iLossCount++;
			team1->iPoints+=3;

		}else if(team2Goal>team1Goal){
			strcpy(newMatchNode->cWinnerTeam,team2->name);
			team2->iWinCount++;
			team1->iLossCount++;
			team2->iPoints+=3;
		}else{
			team2->iDrawCount++;
			team1->iDrawCount++;
			team1->iPoints++;
			team2->iPoints++;
		}
		firstTeam=sortTeams(firstTeam,team1);
		firstTeam=sortTeams(firstTeam,team2);

		//Think of adding another matchNode
		newMatchNode->rlink=firstMatch;
		newMatchNode->llink=NULL;
		firstMatch=newMatchNode;
		printf("\n\nCongrats Match Successfully Conducted\n");
		viewPrevMatches(firstMatch,1);
		printf("\n\n\n");
		return firstTeam;
}

//this function is for sorting the teams in order of team points
TEAMNODEPTR sortTeams(TEAMNODEPTR firstTeam,TEAMNODEPTR team1)
{
	TEAMNODEPTR newNode1;
	newNode1=(TEAMNODEPTR)malloc(sizeof(struct teamNode));
	newNode1=copyStruct(newNode1,team1);

	TEAMNODEPTR prev,cur,temp;
	if(team1->llink!=NULL)
	{
		if(team1->iPoints > (team1->llink)->iPoints)
		{
			prev=team1->llink;
			cur=team1;
			prev->rlink=cur->rlink;
			temp=team1->rlink;

			if(temp!=NULL)
			{
				temp->llink=prev;
			}

			while(prev!=NULL && team1->iPoints > prev->iPoints )
			{
				cur=prev;
				prev=prev->llink;
			}

			if(prev==NULL)
			{
				cur->llink=newNode1;
				newNode1->rlink=cur;
				newNode1->llink=NULL;
				firstTeam=firstTeam->llink;
				free(team1);
			}
			else if(team1->iPoints <= prev->iPoints)
			{
				prev->rlink=newNode1;
				newNode1->llink=prev;
				newNode1->rlink=cur;
				cur->llink=newNode1;
				free(team1);
			}
		}
	}


	return firstTeam;
}

//this function is to copy the whole structure content to another
TEAMNODEPTR copyStruct(TEAMNODEPTR newNode,TEAMNODEPTR team)
{
	strcpy(newNode->name,team->name);
	strcpy(newNode->id,team->id);
	newNode->iPoints=team->iPoints;
	newNode->iWinCount=team->iWinCount;
	newNode->iLossCount=team->iLossCount;
	newNode->iDrawCount=team->iDrawCount;
	newNode->iGoalsFor=team->iGoalsFor;
	newNode->iGoalsAgainst=team->iGoalsAgainst;
	newNode->llink=NULL;
	newNode->rlink=NULL;
	return newNode;
}

//this function will return position of node with given id
TEAMNODEPTR findTeamById(TEAMNODEPTR firstTeam,char teamID[]){
	TEAMNODEPTR cur=firstTeam;
	while(cur!=NULL && (strcmp(cur->id,teamID)!=0) )
		cur=cur->rlink;
	if(cur!=NULL)
		return cur;
	else{
		printf("Invalid team name given\n\n\n");
		getch();
		main();
	}
	return cur;
}


//this function is to print the main Leaderboard
void showLeaderBoard(TEAMNODEPTR firstTeam){
	printf("\n\n\n\n");
	int i=1;
	TEAMNODEPTR cur=firstTeam;
	printf("    Name\tTeam ID\tPositon\tPoints\tMatches Played\tMatches Won\tMatches Loss\tMatches Draw\tGoals For\tGoals Against\t\n\n");

	while(cur!=NULL){
		printf("%-17s", cur->name);
		printf("%5s\t", cur->id);
		printf("%4d\t",i++);
		printf("%4d\t", cur->iPoints);
		printf("%7d\t\t", cur->iWinCount+cur->iDrawCount+cur->iLossCount);
		printf("%6d\t\t", cur->iWinCount);
		printf("%6d\t\t", cur->iLossCount);
		printf("%6d\t\t", cur->iDrawCount);

		printf("%4d\t\t", cur->iGoalsFor);
		printf("%6d", cur->iGoalsAgainst);
		printf("\n");
		cur=cur->rlink;
	}
	printf("\n\n\n\n");
}



//To show the stats for a particular team by its team id
void showTeamStats(TEAMNODEPTR firstTeam,char teamID[4]){

	TEAMNODEPTR cur=firstTeam;

	printf("\n\n");
	int i=1;
	while((strcmp(cur->id,teamID)!=0)&&cur!=NULL){
		cur=cur->rlink;
		i++;
	}
	if(cur == NULL)
	{
		printf("Invalid team ID entered please try again....\n");
		return;
	}
	printf("Name:\t\t\t%s\n", cur->name);
	printf("Team ID:\t\t%s\n", cur->id);
	printf("Current Position:\t%d\n",i);
	printf("Point:\t\t\t%d\n", cur->iPoints);
	printf("Matches Played:\t\t%d\n", cur->iWinCount+cur->iDrawCount+cur->iLossCount);
	printf("Matches Won:\t\t%d\n", cur->iWinCount);
	printf("Matches Loss:\t\t%d\n", cur->iLossCount);
	printf("Matches Draw:\t\t%d\n", cur->iDrawCount);

	printf("Goals For:\t\t%d\n", cur->iGoalsFor);
	printf("Goals Against:\t\t%d\n", cur->iGoalsAgainst);
	printf("\n\n");
	return;
}

//this function is to fetch the leaderboard from the saved file
TEAMNODEPTR fetchTeamsToNodes()
{
	FILE *fptr;
	int i;
	struct teamNode team;
	TEAMNODEPTR first=NULL;
	fptr=fopen("Initiate.bin","rb");
	for(i=0;i<10;i++)
	{
	    fread(&team, sizeof(struct teamNode), 1, fptr);
	    first=insertRearToTeam(first,team);
	}
	fclose(fptr);
	return first;
}

//this function is to made the linked list when the program is started again after closing
TEAMNODEPTR insertRearToTeam(TEAMNODEPTR first,struct teamNode team)
{
	TEAMNODEPTR newNode,cur;
	newNode = (TEAMNODEPTR) malloc (sizeof(struct teamNode));
	strcpy(newNode->name,team.name);
	strcpy(newNode->id,team.id);
	newNode->iPoints=team.iPoints;
	newNode->iWinCount=team.iWinCount;
	newNode->iLossCount=team.iLossCount;
	newNode->iDrawCount=team.iDrawCount;
	newNode->iGoalsFor=team.iGoalsFor;
	newNode->iGoalsAgainst=team.iGoalsAgainst;
	newNode->llink=NULL;
	newNode->rlink=NULL;

	cur = first;

	if(first == NULL)
	{
		return newNode;
	}

	while(cur->rlink != NULL)
	{
		cur = cur->rlink;
	}

	cur->rlink = newNode;
	newNode->llink = cur;
	return first;
}

/*  function is destined to fetch the nodes after starting again  (Not Working)  */
// MATCHNODEPTR fetchMatchToNodes()
// {
// 	FILE *fptr;
// 	int i;
// 	struct matchNode match;
// 	MATCHNODEPTR first=NULL;
// 	fptr=fopen("SaveMatch.bin","rb");
// 	while(!feof(fptr))
// 	{
// 		fread(&match, sizeof(struct matchNode), 1, fptr);
// 	    first=insertFrontToMatch(first,match);
// 	    printf("%s\n",match.cTeam1);
// 	}
// 	fclose(fptr);
// 	return first;
// }

// MATCHNODEPTR insertFrontToMatch(MATCHNODEPTR first, struct matchNode match)
// {
// 	int i;
// 	MATCHNODEPTR newNode;
// 	newNode = (MATCHNODEPTR) malloc(sizeof(struct matchNode));

// 	strcpy(newNode->cTeam1,match.cTeam1);

// 	strcpy(newNode->cTeam2,match.cTeam2);
// 	strcpy(newNode->cWinnerTeam,match.cWinnerTeam);
// 	newNode->iGoalCount = match.iGoalCount;
// 	for(i=0;i<15;i++)
// 	{
// 		newNode->iTimeOfGoals[i]=match.iTimeOfGoals[i];
// 		newNode->iGoalsScorer[i]=match.iGoalsScorer[i];
// 	}

// 	newNode->llink = NULL;
// 	newNode->rlink = NULL;

// 	if(first == NULL)
// 		return newNode;

// 	first->llink=newNode;
// 	newNode->rlink=first;
// 	return newNode;
// }

//this function is to view n previous martches
void viewPrevMatches(MATCHNODEPTR firstMatch,int iMatchCount){

	MATCHNODEPTR cur=firstMatch;
	int i,j,iGoalCount1,iGoalCount2;

	for(i=1;i<=iMatchCount;i++){
		if(cur!=NULL){

			iGoalCount1=0;
			iGoalCount2=0;
			printf("\n\n");
			printf("Match No\t%d\n",i);
			printf("\nTeams Played:\n\t\t\t%s\t%s\n", cur->cTeam1,cur->cTeam2);
			printf("\nMatch Summary::\n\n");

			for(j=0;j<cur->iGoalCount;j++){
				if(cur->iGoalsScorer[j]==1){
					iGoalCount1++;
				}
			}

			if (iGoalCount1==0)
				printf("\t%s didn't score any goal\n",cur->cTeam1);
			else{
				printf("\t%d Goals were scored by  %s at\t",iGoalCount1,cur->cTeam1);
				for(j=0;j<cur->iGoalCount;j++)
					if(cur->iGoalsScorer[j]==1){
						printf("%d'\t",cur->iTimeOfGoals[j]);
					}
			}
			printf("\n");

			for(j=0;j<cur->iGoalCount;j++){
				if(cur->iGoalsScorer[j]==2){
					iGoalCount2++;
				}
			}
			if (iGoalCount2==0)
				printf("\t%s didn't score any goal\n",cur->cTeam2);
			else{
				printf("\t%d Goals were scored by  %s at\t",iGoalCount2,cur->cTeam2);
				for(j=0;j<cur->iGoalCount;j++)
					if(cur->iGoalsScorer[j]==2){
						printf("%d'\t",cur->iTimeOfGoals[j]);
					}
			}

			printf("\n");


			int WinnerGoalCount,LooserGoalCount;
			if(iGoalCount1 > iGoalCount2){
				WinnerGoalCount = iGoalCount1;
				LooserGoalCount = iGoalCount2;
			}else if(iGoalCount1 < iGoalCount2){
				WinnerGoalCount = iGoalCount2;
				LooserGoalCount = iGoalCount1;
			}else{
				printf("\n\tFinally the match ended a draw with a score of (%d-%d)\n",iGoalCount2,iGoalCount2);
				cur=cur->rlink;
				continue;
			}

			printf("\n\tFinally %s won with (%d-%d) score\n", cur->cWinnerTeam,WinnerGoalCount,LooserGoalCount);
			printf("\n");

			cur=cur->rlink;
		}
		else{
			printf("\n\nNumber of matches played in this session is less than %d\n",iMatchCount);
			printf("\n\n");
			break;
		}
	}
}

//this function is to display all the teams in the leaderboard
void showTeams(TEAMNODEPTR first){
	TEAMNODEPTR cur=first;
	printf("\n\n\n");
	printf("   Name\t\t\tTeam ID\t\n");
	while(cur != NULL){
		printf("%-18s\t", cur->name);
		printf("%6s\n", cur->id);
		cur=cur->rlink;
	}
	printf("\n\n\n");

}


//this function is to save the present condition of leaderboard to a binary file
void exitProgram(TEAMNODEPTR firstTeam,MATCHNODEPTR firstMatch){
	FILE *fptr;
	int i;
	fptr=fopen("Initiate.bin","wb");
	struct teamNode team;

	for(i=0;i<10;i++)
	{
		strcpy(team.name,firstTeam->name);
		strcpy(team.id,firstTeam->id);
		team.iPoints=firstTeam->iPoints;
		team.iWinCount=firstTeam->iWinCount;
		team.iLossCount=firstTeam->iLossCount;
		team.iDrawCount=firstTeam->iDrawCount;
		team.iGoalsFor=firstTeam->iGoalsFor;
		team.iGoalsAgainst=firstTeam->iGoalsAgainst;
		team.llink=NULL;
		team.rlink=NULL;

		fwrite(&team, sizeof(struct teamNode), 1, fptr);
		firstTeam=firstTeam->rlink;
	}
	fclose(fptr);

/* this part is to save the previous matches to the file    (Not Working) */
	// fptr=fopen("SaveMatch.bin","wb");
	// struct matchNode match;
	// MATCHNODEPTR temp;
	// temp=firstMatch;

	// while(temp!=NULL)
	// {
	// 	strcpy(match.cTeam1,temp->cTeam1);
	// 	strcpy(match.cTeam2,temp->cTeam2);
	// 	match.iGoalCount=temp->iGoalCount;
	// 	for(i=0;i<15;i++)
	// 	{
	// 		match.iTimeOfGoals[i]=temp->iTimeOfGoals[i];
	// 		match.iGoalsScorer[i]=temp->iGoalsScorer[i];
	// 	}
	// 	strcpy(match.cWinnerTeam,temp->cWinnerTeam);
	// 	match.rlink=match.llink=NULL;

	// 	fwrite(&match,sizeof(struct matchNode),1,fptr);
	// 	temp=temp->rlink;
	// }
	// fclose(fptr);

	exit(0);
}

//this function is to reset the leaderboard all over to 0
TEAMNODEPTR resetAll(TEAMNODEPTR firstTeam)
{
	FILE *fptr;
	fptr=fopen("Initiate.bin","rb");
	struct teamNode team[10];
	int i,j;

	for(i=0;i<10;i++)
	{
	    fread(&team[i], sizeof(struct teamNode), 1, fptr);
		team[i].iPoints = team[i].iWinCount = team[i].iLossCount = team[i].iDrawCount = team[i].iGoalsAgainst = team[i].iGoalsFor=0;
		team[i].llink=team[i].rlink=NULL;
	}
	fclose(fptr);
	fptr=fopen("Initiate.bin","wb");
	for(i=0;i<10;i++)
	{
		fwrite(&team[i], sizeof(struct teamNode), 1, fptr);
	}
	fclose(fptr);
	firstTeam=fetchTeamsToNodes();


	// remove("SaveMatch.bin");
	// fptr=fopen("SaveMatch.bin","wb");
	// fclose(fptr);
	firstMatch=NULL;

	return firstTeam;							//initiating the previous matches linked list to NULL beacuse of reset command

}

//Inro Function
void intro(){
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t\t  Football League Simulation\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t===============================================================================\n");
	printf("\t\t\t\t\tMade By::\n");
	printf("\t\t\t\t\t\tRahul Choudhary\t1SI17CS081\n");
	printf("\t\t\t\t\t\tRishabh Kumar\t1SI17CS088\n");
	printf("\t\t\t\t\t\tPranav Bansal\t1SI17CS076\n");
	printf("\t\t\t\t\t\tPranjal Sharma\t1SI17CS077\n");
	printf("\t\t\t===============================================================================\n\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t\t  Press any key to continue\n");
	getch();
}
