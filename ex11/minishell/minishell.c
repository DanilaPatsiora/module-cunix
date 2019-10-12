#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "hash.c"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int lsh_cd(char **args);
int lsh_exit(char **args);
int shell_echo(char **args);
int shell_pwd(char **args);
char *builtin_str[] = {
  "cd",
  "pwd",
  "echo",
  "exit"
};

int (*builtin_func[]) (char **) = {
  &lsh_cd,
  &shell_pwd,
  &shell_echo,
  &lsh_exit
};
int lsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}
int shell_pwd (char **args)
{
  if (args[1])
  {
    printf("pwd: too many arguments\n");
  }
  else
  {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    printf("Current working dir: %s\n", cwd);
    printf("PATH : %s\n", getenv("PATH"));
    return 1;
  }
  return 0;
}

int shell_echo (char**args)
{
  int i=1;
  while(args[i]!=NULL)
  {
    printf("%s ",args[i]);
    i++;
  }
  printf("\n");
  return 1;
}

int lsh_cd(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "cd: expected argument to \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("cd");
    }
  }
  return 1;
}


int lsh_exit(char **args)
{
  return 0;
}

int lsh_launch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0){ 
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("lsh");
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}



int shell_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  for (i = 0; i < lsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return lsh_launch(args);
}

char *shell_read_command(void)
{
  char *line = NULL;
  ssize_t bufsize = 0; // have getline allocate a buffer for us
  getline(&line, &bufsize, stdin);
  return line;
}
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **shell_split_args(char *line)
{
  int bufsize = LSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, LSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += LSH_TOK_BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, LSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}
void shell_loop(void)
{
  char* command;
  char** args ;
  int status;
  do
  {
    printf("$>");
    command=shell_read_command();
    args=shell_split_args(command);
    status=shell_execute(args);
    free(command);
    free(args);
  }
  while(status);
}

int main()
{
  shell_loop();
  return EXIT_SUCCESS;
}
