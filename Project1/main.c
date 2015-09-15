#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LINE 80 

struct listItem{
  struct listItem * next;
  char input [];
};

typedef struct listItem node;
int historyCount = 0;

node* historyListHead = NULL;

void printHistory() 
{
  node* cur = historyListHead;

  for (int j = 0, i = historyCount; j < 10 && cur != NULL; j += 1) {
    printf("%d %s", i, cur->input); 
    cur = cur->next;
    i -= 1;
  }
}

void addToHistory(char input [])
{
  historyCount += 1;
  node* new = (node *) malloc(sizeof(node));

  strcpy(new->input, input);

  if (historyListHead == NULL) {
    historyListHead = new;
  } else {
    new->next = historyListHead;
    historyListHead = new;
  }
}

char * getHistoryItem(int n)
{
  if (historyListHead == NULL) {
    return NULL;
  }

  node* cur = historyListHead;
  int j;
  for (j = historyCount; j > n && cur != NULL; j -= 1) {
    cur = cur->next;
  }

  if (j == n) {
    return cur->input;
  } else {
    return NULL;
  }
}

int main(void)
{
  char *args [MAX_LINE/2 + 1]; //command line arguments
  int should_run = 1; //flag to determine when to exit program
  pid_t pid;

  while(should_run) {
    printf("osh> ");
    fflush(stdout);

    int should_background = 0; 
    char input [MAX_LINE + 1]; // +1 for terminating null
    char input_copy [MAX_LINE +1];
    char *args_list = malloc(MAX_LINE + 1);

    fgets(input, MAX_LINE + 1, stdin);
    strcpy(input_copy, input);


    int i = -1;
    do {
      i += 1;
      if (0 == i) {
        args_list = strtok(input, " \n");
      } else {
        args_list = strtok(NULL, " \n");
      }

      if (args_list != NULL && strncmp("&", args_list, 1) == 0) {
        should_background = 1;
        args_list = NULL;
      }

      args[i] = args_list;
    } while (args_list != NULL);

    if (i == 0) {
      continue;
    }

    int should_wait = 1;

    if (strncmp("history", args[0], strlen(args[0])) == 0) {
        char* history = getHistoryItem(historyCount);
        if (history == NULL) 
        {
            fprintf(stderr, "No commands in history.\n");
            continue;
            }
        else
        {
            printHistory();
            continue;
            }       
        }       
    else if (strncmp("!!", args[0], strlen(args[0])) == 0) 
    {
        char* history = getHistoryItem(historyCount);
        if (history == NULL) {
            fprintf(stderr, "No commands in history.\n");
            continue;
            } 
        else 
        {
            strcpy(input, history);
            printf("%s", history);
            } 
        } 
    else if (strncmp("!", args[0], 1) == 0) 
    {
        int n;
        sscanf(args[0], "!%d", &n);
        
        char* history = getHistoryItem(n);
        if (history == NULL) 
        {
            fprintf(stderr, "No commands in history.\n");
            continue;
            } 
        else 
        {
            strcpy(input, history);
            printf("%s", history);
            }
        } 
    else if (strncmp("exit", args[0], strlen(args[0])) == 0) 
    {
        printf("Goodbye!\n");
        return 0;
        } 
    else 
    {
        strcpy(input, input_copy);
        }   

    addToHistory(input);

    i = -1;
    do {
      i += 1;
      if (0 == i) {
        args_list = strtok(input, " \n");
      } else {
        args_list = strtok(NULL, " \n");
      }

      if (args_list != NULL && strncmp("&", args_list, 1) == 0) {
        should_background = 1;
        args_list = NULL;
      }

      args[i] = args_list;
    } while (args_list != NULL);

    pid = fork();

    if (0 == pid) {
      execvp(args[0], args);
      fprintf(stderr, "Could not execute command.\n");
      return 0;
    }

    if (0 < pid) {
      if (!should_background) {
        wait(NULL);
      }
    }

  }

  return 0;
}
