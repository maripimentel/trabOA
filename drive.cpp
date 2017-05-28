#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>

typedef struct block {
	unsigned char bytes_s[512];
} block;

typedef struct  sector_array {
	block sector[60];
} sector_array;

typedef struct track_array {
	sector_array track[5];
} track_array;

typedef struct  fatent_s {
	unsigned int used;
	unsigned int eof;
	unsigned int next;
} fatent;

typedef struct fatlist_s {
	char file_name[100];
	unsigned int first_sector;
	struct fatlist_s *next_file;
} fatlist;

track_array cylinder[10];
fatent_s fatlist_sectors[3000];
fatlist_s *fatlist_files_initial=NULL;

void show_menu ();

void menu_selection (int menu_option);

int write_file();

int find_fat_sector(int j, int t, int s);


int main()
{
	int menu_option = -1;
	show_menu();
	scanf("%d", &menu_option);
	menu_selection (menu_option);
}

void show_menu()
{
	system("clear");
	printf("1 - Escrever Arquivo \n2 - Ler Arquivo\n3 - Apagar Arquivo\n4 - Mostrar Tabela Fat\n5 - Sair\n");
	printf("Digite a opcao desejada:\n");
	return;
}

void menu_selection (int menu_option) { // função que direciona o programa para as funções dependendo da opção escolhida.

  switch (menu_option){
    case 1:
      write_file();
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

int write_file(){
  FILE *file;
  char file_name[100], c;
  int t=0, s=0, i=0, j=0, fat_sector, fat_sector_search;
  int num_sector=0, num_cylinder=0, num_track=0;
  fatlist_s *fatlist_files_actual, *fatlist_files_new;

  // O programa pede ao usuário o nome do arquivo com extensão .txt e na
  // mesma pasta onde está sendo executado o programa. Em seguida, abre o
  // arquivo para leitura.
  do{
  	system("clear");
    printf("Informe o nome do arquivo, com '.txt'.\n");
    scanf("%s", file_name);

    file=fopen(file_name, "r");
    //system("clear");
  }while(file==NULL);
  printf("Arquivo aberto com sucesso.\n");

  // Encontra-se o primeiro cluster disponível na primeira trilha do primeiro
  // cilindro. Se não houver, passa para a primeira trilha do próximo cilindro.
  // Caso todas as primeiras trilhas de todos os cilindros estiverem cheias,
  // passa para a segunda trilha do primeiro cilindro e assim por diante.
  while(cylinder[j].track[t].sector[s*4].bytes_s[0]!=0){
    s++; // Verifica cada cluster.
    if(s==15){ // Todos os clusters foram verificados e não foi encontrado espaço vazio.
      j++; // Verifica no próximo cilindro a partir do primeiro cluster.
      s=0;
      if(j==10){ // Todos os cilindros foram verificados e não foi encontrado espaço vazio.
        t++; // Verifica na próxima trilha a partir do primeiro cilindro.
        j=0;
        if(t==5){ // Todas as trilhas foram verificadas e não foi encontrado espaço vazio.
          printf("Disco cheio\n"); // Aviso de disco cheio.
          return 0;
        }
      }
    }
  }

  fat_sector=find_fat_sector(j, t, s);
  fatlist_files_actual=(fatlist_s*)malloc(sizeof(fatlist_files_actual));
  if (fatlist_files_initial==NULL){
    fatlist_files_initial=(fatlist_s*)malloc(sizeof(fatlist_files_initial));
    fatlist_files_actual=fatlist_files_initial;
  }
  else{//se a lista fat já está inicializada
    	fatlist_files_actual=fatlist_files_initial;
    do{//move atual até o fim da lista
      fatlist_files_actual=fatlist_files_actual->next_file;
    }while(fatlist_files_actual->next_file!=NULL);  
  }
  fatlist_files_new=(fatlist_s*)malloc(sizeof(fatlist_files_new));
  strcpy(fatlist_files_new->file_name, file_name);
  fatlist_files_new->first_sector=fat_sector;
  fatlist_files_actual->next_file=fatlist_files_new;
  fatlist_files_new->first_sector=fat_sector;
  fat_sector_search=fat_sector;
  while(!feof(file)){
		c=fgetc(file);
    cylinder[j].track[t].sector[s].bytes_s[i]=c;
    if(fatlist_sectors[fat_sector].used==0){
    	fatlist_sectors[fat_sector].used=1;
    }
    i++;
    if(i==512){
      s++;
      fat_sector_search++;
      while(fatlist_sectors[fat_sector_search].used==1){
        s++;
        fat_sector_search++;
      }

      i=0;
      if(s==60){
        j++;
        s=0;
        if(j==10){
          t++;
          j=0;
          if(t==5){
            printf("Disco cheio, dados truncados.\n");
            return 0;
          }
        }
      }
    }
  }
  fclose(file);
  return 1;
}

// Calcula a posição do array equivalente ao número do cilindro, da trilha e do setor encontrado.
int find_fat_sector(int j, int t, int s){
  int sum;
  sum=(j*300)+(t*60)+s;
  return sum;
}