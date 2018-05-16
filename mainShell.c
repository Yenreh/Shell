/*
  UNIVERSIDAD DEL VALLE
  PROYECTO SHELL

  Carlos Esteban Murillo
  Juan Camilo Sanchez
  Herney Quintero

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "shell.h"

int main(int argc, char* argv[]){
  printf("Ghost in the shell\n");
while(1){
    char comando[200];
    printf("$_: ");
    scanf("%[^\n]%*c",comando);
    //almacena un string (char*) completo

    //se comparan strings, si son iguales retorna 0
    if (!strcmp("exit", comando)){
      printf("\n");
      break;
    }
    //Modifica el arreglo arr, el cual contiene todos los comandos digitados
    removerEspacios(comando);
    if(buscarOperador(">") && buscarOperador("|")){
      pipeArchivo();
      vaciar();
    }else if(buscarOperador(">")){
      guardarEnArchivo();
      vaciar();
    }else if(buscarOperador("|")){
      ejecutarPipe();
      vaciar();
    }else{
      ejecutar();
      vaciar();
    }
  }
}
