#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "shell.c"

//Arreglo donde se almacenan todos los comandos escritos



int main(int argc, char* argv[]){

  printf("Ghost in the shell\n");
  while(1){
    char comando[200];

    printf("$: ");
    scanf("%[^\n]%*c",comando);//almacena un string (char*) completo

    //se comparan strings, si son iguales retorna 0
    if (!strcmp("exit", comando)){
      printf("\n");
      break;
    }
    //Modifica el arreglo arr, el cual contiene todos los comandos digitados
    removerEspacios(comando);
    ejecutarElShell();

  }
}