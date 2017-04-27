#include <stdio.h>
#include <conio.h>
#include "disk_lib.h"

void show_menu () {

  clrscr;
  printf("
    1 - Escrever Arquivo
    \n
    2 - Ler Arquivo
    \n
    3 - Apagar Arquivo
    \n
    4 - Mostrar Tabela FAT
    \n
    5 - Sair
    \n");
  return;
}

int main (){


  show_menu;

}