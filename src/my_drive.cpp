#include <stdio.h>
#include <conio.h>
#include "disk_lib.h"

void show_menu () { // simples função que limpa a tela e mostra o menu.

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

void menu_selection (int menu_option) { // função que direciona o programa para as funções dependendo da opção escolhida.

  switch (menu_option){
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    default:
      break;
  }
  return;
} 

int main (){
  int menu_option=0;

  show_menu;
  scanf("%d", &menu_option);
  while ((menu_option!=1)
      &&(menu_option!=2)
      &&(menu_option!=3)
      &&(menu_option!=4)
      &&(menu_option!=5)) { // garantia de entrada correta;
    show_menu;
    printf("Escolha invalida!\nEntre novamente.\n");
    scanf("%d", &menu_option);
  }
  menu_selection;


}