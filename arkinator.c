#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *f;
FILE *fp;

enum nodeType {QUESTION, ANSWER};

union questions
{
  char *question;
  char *answer;
};

struct nodeStruct
{
  //set up data for if answer or question
  union questions nodeData;
  enum nodeType type;
  //which path we will take to the next node
  struct nodeStruct *yes;
  struct nodeStruct *no;
};

//Part 1
void nodePrint(struct nodeStruct *node)
{
  printf("Current node data = %s\n", node->nodeData);
  if(node->type == ANSWER)
  {
    printf("ANSWER node no yes/no branches");
  }
  else
  {
    printf("[YES BRANCH]: %s\n", node->yes->nodeData);
    printf("[NO BRANCH]: %s\n", node->no->nodeData);
  }
}

//Part 1
void treePrint(struct nodeStruct *node)
{
    if (node == NULL)
    {
    }
    else
    {
      if(node->type == QUESTION)
      {
        printf("QUESTION %s\n", node->nodeData);
      }
      else
      {
        printf("ANSWER %s\n", node->nodeData);
      }
      treePrint(node->yes);
      treePrint(node->no);
    }

}

//Part 2

void insertNode(struct nodeStruct *AboveNode, int previousAnswer, 
        char* AnswerData, char* QuestionData)
{

  struct nodeStruct *newAnswerNode = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  newAnswerNode->type = ANSWER;
  newAnswerNode->nodeData.answer = AnswerData;
  newAnswerNode->yes = NULL;
  newAnswerNode->no = NULL;
  
  struct nodeStruct *newQuestionNode = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  newQuestionNode->type = QUESTION;
  newQuestionNode->nodeData.question = QuestionData;
  newQuestionNode->yes = newAnswerNode;
  
  if (previousAnswer == 1)
  {
    //yes on our above node got us here
    newQuestionNode->no = AboveNode->yes;
    AboveNode->yes = newQuestionNode;
  }
  else
  {
    newQuestionNode->no = AboveNode->no;
    AboveNode->no = newQuestionNode;
  }
  
}

void addNewQuestion(struct nodeStruct *AboveNode, int previousAnswer)
{
    printf("What was it you were thinking of?\n");
    char *inputtedAnswer = malloc(40);
    fgets(inputtedAnswer,40,stdin);
    strtok(inputtedAnswer, "\n");
    //ask player question that is yes for new answer but no for old answer
    printf("\nPlease give a question that the answer is yes for your new question"
            " but no for my answer\n:");
    char *inputtedQuestion = malloc(40);
    fgets(inputtedQuestion,40,stdin);
    strtok(inputtedQuestion, "\n");
    
  insertNode(AboveNode, previousAnswer, inputtedAnswer ,inputtedQuestion);
  
}

void treeWrite(struct nodeStruct *node)
{
    if (node == NULL)
    {
    }
    else
    {
      if(node->type == QUESTION)
      {
        fprintf(f,"QUESTION %s\n", node->nodeData);
      }
      else
      {
        fprintf(f,"ANSWER %s\n", node->nodeData);
      }
      treeWrite(node->yes);
      treeWrite(node->no);
    }

}

struct nodeStruct* readNode(struct nodeStruct *topNode, int firstrun)
{
    char buff[255];
    fgets(buff, 80, fp);
    char QuestionOrAnswer[40];
    //int x = 0; //initilize counter for place in file
    int QorA = 0;
    char* ReadData = strchr(buff,' ');

    if(strstr(buff, "QUESTION") != NULL)
    {
      QorA = 0;
    }
    else
    {
      QorA = 1;
    }


    if(QorA == 0)
    {
      if(firstrun == 1)
      {
        topNode->yes = readNode(topNode,0);
        topNode->no = readNode(topNode,0);
        return topNode;
      }
      else
      {
        struct nodeStruct *newQuestion = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
        newQuestion->nodeData.question = ReadData;
        newQuestion->type = QUESTION;
        newQuestion->yes = readNode(topNode, 0);
        newQuestion->no = readNode(topNode, 0);
        printf("\nDebug 3A: %s", newQuestion->yes);
        printf("\nDebug 2A: %s", newQuestion->no);
        return newQuestion;
      }
      
    }
    else if(QorA == 1)
    {
      struct nodeStruct *newAnswer = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
      newAnswer->nodeData.answer = ReadData;      
      newAnswer->type = ANSWER;
      newAnswer->yes = NULL;
      newAnswer->no = NULL;
      return newAnswer;
    }
}



void freeTree(struct nodeStruct *node)
{
  //pass in the node at the top of the tree and work
  //our way down

  if(node == NULL)
  {
  }
  else
  {
    //save location of nodes further down the tree
    struct nodeStruct *nextYes;
    struct nodeStruct *nextNo;
    nextYes = node->yes;
    nextNo = node->no;
    //free the current node
    free(node);
    //recursively call this function to the
    //next nodes
    freeTree(nextYes);
    freeTree(nextNo);
  }
}


int main(int argc, char **argv)
{

  struct nodeStruct *toyAnswer = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  toyAnswer->nodeData.answer = "toy!";
  toyAnswer->type = ANSWER;
  toyAnswer->yes = NULL;
  toyAnswer->no = NULL;

  struct nodeStruct *BroadSwordAnswer = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  BroadSwordAnswer->nodeData.answer = "BROADSWORD";
  BroadSwordAnswer->type = ANSWER;
  BroadSwordAnswer->yes = NULL;
  BroadSwordAnswer->no = NULL;

  struct nodeStruct *HumanAnswer = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  HumanAnswer->nodeData.answer = "human";
  HumanAnswer->type = ANSWER;
  HumanAnswer->yes = NULL;
  HumanAnswer->no = NULL;

  struct nodeStruct *PigAnswer = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  PigAnswer->nodeData.answer = "babe pig in the city";
  PigAnswer->type = ANSWER;
  PigAnswer->yes = NULL;
  PigAnswer->no = NULL;

  struct nodeStruct *canMoveQuestion = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  canMoveQuestion->nodeData.question = "Can it move?";
  canMoveQuestion->type = QUESTION;
  canMoveQuestion->yes = toyAnswer;
  canMoveQuestion->no = BroadSwordAnswer;

  struct nodeStruct *isHumanQuestion = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  isHumanQuestion->nodeData.question = "Is it Human?";
  isHumanQuestion->type = QUESTION;
  isHumanQuestion->yes = HumanAnswer;
  isHumanQuestion->no = PigAnswer;

  struct nodeStruct *isaliveQuestion = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  isaliveQuestion->nodeData.question = "Is it alive?";
  isaliveQuestion->type = QUESTION;
  isaliveQuestion->yes = isHumanQuestion;
  isaliveQuestion->no = canMoveQuestion;
  
  //treePrint(&isaliveQuestion);
  
/*
  fp = fopen("./tree.txt","r");
  struct nodeStruct *tonothing = (struct nodeStruct*) malloc(sizeof(struct nodeStruct));
  tonothing = readNode(isaliveQuestion, 1);
  fclose(fp); 

*/
  int playagain = 1;
  while(playagain == 1)
  {
      //this while loop should do all the questioning
      struct nodeStruct *currentNode = isaliveQuestion;
      struct nodeStruct *previousNode = currentNode;
      //lastanswer is 1 if a yes got us here else its 0  
      int lastanswer = 1;
      //build a buffer to hold the data
      char *yesnobuffer = malloc(40);
      //game loop
      while (currentNode->type == QUESTION)
      {
        //write out our current question
        printf("%s\n", currentNode->nodeData);


        fgets(yesnobuffer,40,stdin);   

        //use strcmp to figure out user input
        if(strcmp(yesnobuffer,"yes\n") == 0 || strcmp(yesnobuffer,"y\n") == 0 || strcmp(yesnobuffer,"YES\n") == 0)
        {
          previousNode = currentNode;
          currentNode = currentNode->yes;
          lastanswer = 1;
        }
        else if (strcmp(yesnobuffer,"no\n") == 0 || strcmp(yesnobuffer,"n\n") == 0 || strcmp(yesnobuffer,"NO\n") == 0)
        {
          previousNode = currentNode;
          currentNode = currentNode->no;
          lastanswer = 0;
        }
        else
        {
          
          printf("unrecognised input: please enter yes or no");
        }

        printf("\n");
      }
      printf("\nIt's a %s\n", currentNode->nodeData);
      free(yesnobuffer);

      //more logic needed here to see if answer was correct
      char *correctBuffer = malloc(30);
      printf("\nWas I correct? ");
      fgets(correctBuffer, 30,stdin);
      printf("\n");

      if(strcmp(correctBuffer,"yes\n") == 0)
      {
        printf("yes I was correct\n");
      }
      else if(strcmp(correctBuffer,"no\n") == 0)
      {
        addNewQuestion(previousNode, lastanswer);
      }

      free(correctBuffer);
      
      char *playAgainBuffer = malloc(30);
      printf("\nplayAgain? ");
      fgets(playAgainBuffer, 30,stdin);
      printf("\n");
      
      if (strcmp(playAgainBuffer,"yes\n") != 0)
      {
          playagain = 0;
      }
      else
      {
          playagain = 1;
      }
      free(playAgainBuffer);
  }
/*
  f = fopen("./tree.txt", "w");
  treeWrite(isaliveQuestion);
  //close file when done writing
  fclose(f);
  
  free(tonothing);
  freeTree(isaliveQuestion); */
}
