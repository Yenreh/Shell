#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

//arreglos donde se almacenan todos los comandos escritos
char* instruction[200];
char* instruction2[200];
char* pip1[200];
char* pip2[200];
char* filename;

/*Este metodo se encarga de ejecutar el comando guardado en el arreglo previamente, haciendo uso de
la llamada al sistema fork
*/
void ejecutar(){
  pid_t pid = fork();
  if(pid==0){
    if(execvp(instruction[0],instruction)==-1){
      printf("El comando no fue ejecutado felizmente\n");
    }
  }else{
    //printf("I'm a Boss\n");
    wait(NULL);
  }
}
/*Este metodo recibe un operador, y se encarga de recorrer el arreglo en busca de este, si lo encuentra retorna
1(true) y 0(false) si no lo encuentra

 */
int buscarOperador(char* op){
  char *head;
  int c=0;
  head=instruction[c];
  while (head!=NULL) {
    if(!strcmp(op,head)){
      return 1;
    }
    c++;
    head=instruction[c];
  }
  return 0;
}
/*Este metodo se encarga de almacenar en un buffer la ejecucion del primer comando siendo esta la nueva entrada del segundo
comando*/
void Pipe(){
    int buffer[2];
    pid_t pid;
    pipe(buffer);
    pid = fork();
    if (pid == 0){
      //printf("Child\n");
      dup2(buffer[1], 1);
      close(buffer[0]);
      close(buffer[1]);
      execvp(pip1[0], pip1);
    }
    else{
      //printf("Dad\n");
      dup2(buffer[0], 0);
      close(buffer[0]);
      close(buffer[1]);
      execvp(pip2[0], pip2);
    }
}
/*Este metodo se encarga de separar en subarreglos las partes entre los simbolos (|) y (>) obteniendo asi, 3 subarreglos
De los cuales se ejecutan los separados por el pipe para posteriormente redirigir su salida a un archivo cuyo nombre es
el ultimo subarreglo*/
void pipeArchivo(){
  char *head;
  int c=0;
  int k=0;
  head=instruction[c];
  while(strcmp("|",head)!=0){
    pip1[c]=head;
    c++;
    head=instruction[c];
  }
  c++;
  head=instruction[c];
  while(strcmp(">",head)!=0){
    pip2[k]=head;
    c++;
    k++;
    head=instruction[c];
  }
  c++;
  filename=instruction[c];
  pid_t pid = fork();
  if(pid==0){
    close(STDOUT_FILENO);
    open(strcat(filename,".output"),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IXOTH);
    Pipe();
  }else{
    wait(NULL);
  }
}
/* Este metodo se usa cuando hay un pipe ("|") en el arreglo, y se encarga de guardar en dos  subarreglos lo que esta
antes y despues del pipe, para ejecutar la instruccion que esta despues y  usarla como entrada del primero
*/
void ejecutarPipe(){
  char *head;
  int c=0;
  int k=0;
  head=instruction[c];
  while(strcmp("|",head)!=0){
    pip1[c]=head;
    c++;
    head=instruction[c];
  }
  c++;
  head=instruction[c];
  while(head!=NULL){
    pip2[k]=head;
    c++;
    k++;
    head=instruction[c];
  }
  pid_t pid = fork();
  if(pid==0){
    Pipe();
  }else{
    wait(NULL);
  }
}
/*Este metodo se usa si se encuentra ">" en el arrego, se encarga de guardar lo que esta antes y despues del ">" en
subarreglos, posteriormente usa estos para crear un archivo en donde el primer subarreglo(instruction2) es el cuerpo del
archivo y el segundo(filename) el nombre de este.
 */
void guardarEnArchivo(){
  char *head;
  int c=0;
  head=instruction[c];
  while(strcmp(">",head)!=0){
    instruction2[c]=head;
    c++;
    head=instruction[c];
  }
  filename=instruction[c+1];

  pid_t pid = fork();
  if(pid==0){
    close(STDOUT_FILENO);
    open(strcat(filename,".output"),O_CREAT|O_WRONLY|O_TRUNC,S_IRWXU|S_IXOTH);
    if(execvp(instruction2[0],instruction2)==-1){
      printf("El comando no fue ejecutado felizmente\n");
    }
  }else{
    wait(NULL);
  }
}
/*Este metodo se encarga de recibir lo capturado por el scanf, toma el primer elemento mediante un token, posteriormente
en un bucle por cada token se saca un elemento separado por un espacio del string.
*/
void removerEspacios(char* comando){
  const char attribute[2] = " ";
  char *head;
  int c=0;
  //head almacena el primero
  head = strtok(comando, attribute);
  instruction[c]=head;

  //se almacena el resto en el array
  while(head != NULL ) {
    c++;
    //printf("\n%i", c);
    head = strtok(NULL, attribute);
    instruction[c]=head;
  }
}

/*Este metodo inicializa todos los arreglos implementados, lo cual permite
que no queden resiudos de otros comandos en los arreglos*/
void vaciar(){
  for (int i = 0; i < 200; ++i)
  {
    instruction[i]=NULL;
    pip1[i]=NULL;
    pip2[i]=NULL;
    instruction2[i]=NULL;
  }
}
