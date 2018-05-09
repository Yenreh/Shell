#ifndef __SHELL__
#define __SHELL__
void removerEspacios(char* comando);
void ejecutar();
int buscarOperador(char* op);
void guardarEnArchivo();
void ejecutarPipe();
void Pipe(char **c1, char **c2);
void EjecutarElSell();

#endif