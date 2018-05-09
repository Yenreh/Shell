#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

//Arreglo donde se almacenan todos los comandos escritos

char* arr[200];
char* arr2[200];
char* pip1[200];
char* pip2[200];
char* filename;

void ejecutar(){
  pid_t pid = fork();
  if(pid==0){
    //printf("I'm a child\n");
    //se ejecuta el comando

    if(execvp(arr[0],arr)==-1){
      printf("El comando no fue ejecutado felizmente\n");
    }
  }else{
    //printf("I'm a Boss\n");
    wait(NULL);
  }
}

int buscarOperador(char* op){
  char *head;
  int c=0;
  head=arr[c];
  while (head!=NULL) {
    if(!strcmp(op,head)){
      return 1;
    }
    c++;
    head=arr[c];
  }
  return 0;
}

void Pipe(char **c1, char **c2){
    int fd[2];
    pid_t pid;
    if (pipe(fd) != 0){
      printf("Error al crear pipe");
    }
    pid = fork();
    if(pid==-1){
      printf("failed to fork");
    }
    if (pid == 0){
      //printf("Child\n");
      dup2(fd[1], 1);
      close(fd[0]);
      close(fd[1]);
      execvp(c1[0], c1);
    }
    else{
      //printf("Dad\n");
      dup2(fd[0], 0);
      close(fd[0]);
      close(fd[1]);
      execvp(c2[0], c2);
    }
}

void ejecutarPipe(){
  char *head;
  int c=0;
  int k=0;
  head=arr[c];
  while(strcmp("|",head)!=0){
    pip1[c]=head;
    c++;
    head=arr[c];
  }
  c++;
  head=arr[c];
  while(head!=NULL){
    pip2[k]=head;
    c++;
    k++;
    head=arr[c];
  }
  pid_t pid = fork();
  if(pid==0){
    Pipe(pip1,pip2);
  }else{
    wait(NULL);
  }

}



void guardarEnArchivo(){
  char *head;
  int c=0;
  head=arr[c];
  while(strcmp(">",head)!=0){
    arr2[c]=head;
    c++;
    head=arr[c];
  }
  filename=arr[c+1];

  pid_t pid = fork();
  if(pid==0){
    close(STDOUT_FILENO);
    open(strcat(filename,".output"),O_CREAT|O_WRONLY|O_TRUNC|S_IRWXU|S_IXOTH);
    if(execvp(arr2[0],arr2)==-1){
      printf("El comando no fue ejecutado felizmente\n");
    }
  }else{
    wait(NULL);
  }
}

void removerEspacios(char* comando){
  const char attribute[2] = " ";
  char *head;
  int c=0;
  //char* arr[200];

  //head almacena el primero
  head = strtok(comando, attribute);
  arr[c]=head;

  //se almacena el resto en el array
  while(head != NULL ) {
    c++;
    //printf("\n%i", c);
    head = strtok(NULL, attribute);
    arr[c]=head;
  }
}

void ejecutarElShell(){

  if(buscarOperador(">")){
      guardarEnArchivo();
    }else if(buscarOperador("|")){
      ejecutarPipe();
    }else{
      ejecutar();
    }
}
