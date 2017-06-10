#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void initialize_disk();

void show_menu ();

void menu_selection (int menu_option);

void write_file();

void read_file ();

void delete_file ();

void find_drive_sector(int fat_number, int j, int t, int s);

int find_fat_sector(int j, int t, int s);

void show_fat();

typedef struct block {
  char bytes_s[512];
} block;

typedef struct  sectror_array {
  block sector[60];
} sectror_array;

typedef struct track_array {
  sectror_array track[5];
} track_array;

typedef struct  fatlist_s {
  unsigned int used;
  unsigned int eof;
  unsigned int next;
} fatlist;

typedef struct fatent_s {
  char file_name[100];
  unsigned int first_sector;
  struct fatent_s *next_file;
} fatent;
track_array cylinder[10];
fatlist_s fatlist_sectors[3000];
fatent_s *fatlist_files_initial=NULL;

void show_menu () { // simples função que limpa a tela e mostra o menu.
  //system("clear");
  printf("1 - Escrever Arquivo\n2 - Ler Arquivo\n3 - Apagar Arquivo\n4 - Mostrar Tabela FAT\n5 - Sair\n");
  return;
}

void menu_selection (int menu_option) { // função que direciona o programa para as funções dependendo da opção escolhida.

  switch (menu_option){
    case 1:
      system("clear");
      write_file();
      break;
    case 2:
      system("clear");
      read_file();
      break;
    case 3:
      system("clear");
      delete_file();	
      break;
    case 4:
      system("clear");
      show_fat();
      break;
    case 5:
      break;
    default:
      break;
  }
  return;
}

int find_fat_sector(int j, int t, int s){
  int sum;
  sum=(j*300)+(t*60)+s;
  return sum;
}
void find_drive_sector(int fat_number, int j, int t, int s){ // NECESSÁRIO CHECAR!!!!
  j=fat_number/300;
  fat_number=fat_number%300;
  t=fat_number/60;
  fat_number=fat_number%60;
  s=fat_number;
  return;
}
void initialize_disk(){
  int c=0, t=0, s=0, b=0;
  while(c<11){
    if(b==512){
      b=0;
      s++;
      if(s==60){
        s=0;
        t++;
        if(t==5){
          t=0;
          c++;
          if(c==10){
            return;
          }
        }
      }
    }
    cylinder[c].track[t].sector[s].bytes_s[b]=0;
    b++;
    //printf("%d %d %d %d, %c\n",c, t, s ,b, cylinder[c].track[t].sector[s].bytes_s[b]);
  }
}

void write_file (){
  FILE *file;
  char file_name[100], c;
  int t=0, s=0, i=0, j=0, fat_sector, fat_sector_search;
  fatent_s *fatlist_files_actual, *fatlist_files_new;

  //do{
    printf("Informe o nome do arquivo, com '.txt'.\n");
    scanf("%s", file_name);
    file=fopen(file_name, "r");
  //}while(file==NULL);
  printf("Arquivo aberto com sucesso.\n");
  //getchar();
  while(cylinder[j].track[t].sector[s*4].bytes_s[0]!=0){
    printf("%d, %d, %d\n", s, j, t);
    //getchar();
    s++;
    if(s==15){
      j++;
      s=0;
      if(j==10){
        t++;
        j=0;
        if(t==5){
          printf("Disco cheio\n");
          return;
        }
      }
    }
  }
  fat_sector=find_fat_sector(j, t, s);
  fatlist_files_actual=(fatent_s*)malloc(sizeof(fatlist_files_actual));
  if (fatlist_files_initial==NULL){
    fatlist_files_initial=(fatent_s*)malloc(sizeof(fatlist_files_initial));
    fatlist_files_actual=fatlist_files_initial;
  }
  else{/*se a lista fat já está inicializada*/
    fatlist_files_actual=fatlist_files_initial;
    do{/*move atual até o fim da lista*/
      fatlist_files_actual=fatlist_files_actual->next_file;
    }while(fatlist_files_actual->next_file!=NULL);
  }
  fatlist_files_new=(fatent_s*)malloc(sizeof(fatlist_files_new));
  strcpy(fatlist_files_new->file_name, file_name);
  fatlist_files_new->first_sector=fat_sector;
  fatlist_files_actual->next_file=fatlist_files_new;
  fatlist_files_new->first_sector=fat_sector;
  fat_sector_search=fat_sector;
  while((c=fgetc(file)!=EOF)){
    cylinder[j].track[t].sector[s].bytes_s[i]=c;
    if(fatlist_sectors[fat_sector].used==0){fatlist_sectors[fat_sector].used=1;}
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
            return;
          }
        }
      }
    }
  }
  fatlist_sectors[fat_sector_search].eof=1;
}

void read_file () {
  int sector_number=0, j=0, t=0, s=0, i=0;
  char file_name[100], c=0;
  fatent_s *fatlist_files_actual=NULL;
  FILE *out;

  out=fopen("saida.txt", "w");
  printf("Informe o nome do arquivo, com '.txt'.\n");
  scanf("%s", file_name);
  fatlist_files_actual=fatlist_files_initial;
  while (((strcmp((fatlist_files_actual->file_name), file_name))!=0)&&(fatlist_files_actual->next_file!=NULL)){
    fatlist_files_actual=fatlist_files_actual->next_file;
  }
  sector_number=fatlist_files_actual->first_sector;
  while((fatlist_sectors[sector_number].eof)!=1){
    find_drive_sector(sector_number, j, t, s);
    for(i=0; i<512; i++){
      c=cylinder[j].track[t].sector[s].bytes_s[i];
      fprintf(out, "%c", c);
    }
  }
  fclose(out);
  return;
}

void delete_file() {
  char file_name[100];
  fatent_s *fatlist_files_actual, *fatlist_files_auxiliar;
  unsigned int first_sector;

  printf("Digite o nome do arquivo: "); //Pegando nome do arquivo a ser excluido
  scanf("%s", file_name);

  if(fatlist_files_initial != NULL) { //Se a tabela FAT nao estiver vazia
    fatlist_files_actual = fatlist_files_initial; 
    while(fatlist_files_actual != NULL) { //Enquanto nao chegarmos no ultimo arquivo da tabela FAT
      if(!strcmp(fatlist_files_actual->file_name, file_name)) { //Se o arquivo pertencer a tabela FAT
          first_sector = fatlist_files_actual->first_sector; //Pegando primeiro setor do arquivo
          fatlist_files_auxiliar = fatlist_files_actual->next_file; //Excluindo arquivo da tabela FAT
          strcpy(fatlist_files_actual->file_name, fatlist_files_auxiliar->file_name);
          fatlist_files_actual->first_sector = fatlist_files_auxiliar->first_sector;
          fatlist_files_actual->next_file = fatlist_files_auxiliar->next_file; //Fim da exclusao
          break;
      }
      fatlist_files_actual = fatlist_files_actual->next_file;
    } 
    if(fatlist_files_actual == NULL) {
      printf("Arquivo nao esta na tabela FAT.\n");
      return;
    }

  } else { //Se a tabela FAT estiver vazia
    printf("Tabela FAT vazia.\n");
    return;
  }
  do { //Enquanto nao chegarmos no ultimo setor do arquivo
    fatlist_sectors[first_sector].used = 0; //Marcando campo used com 0
    first_sector = fatlist_sectors[first_sector].next; //Indo para o proximo setor
  } while(fatlist_sectors[first_sector].eof != 1);
}

void show_fat (){
	int sector_number=0, j=0, t=0, s=0, i=0, cont=1;
	int total_sectors=0, sectors[200];
	fatent *fatlist_files_auxiliar = NULL;
	fatlist_files_auxiliar = fatlist_files_initial;


    printf ("NOME \t\t TAMANHO EM DISCO \t LOCALIZACAO \n");
	while(fatlist_files_auxiliar->next_file!= NULL)
	{
		fatlist_files_auxiliar = fatlist_files_auxiliar->next_file;
		sector_number=fatlist_files_auxiliar->first_sector;
		total_sectors=1;
		sectors[total_sectors-1]=sector_number;
  		while((fatlist_sectors[sector_number].eof)!=1){
  			total_sectors++;
    		find_drive_sector(sector_number, j, t, s);
    		sectors[total_sectors-1]=sector_number;
    	}
    	printf ("%s \t %d \t\t\t %u", fatlist_files_auxiliar->file_name, total_sectors*512, sectors[0]);
    	while(cont<total_sectors){
    		printf (", %u", sectors[cont]);
    	}
    	printf("\n");
    }
    printf("\n\n");
}



int main (){
  int menu_option=0;

  initialize_disk();
  while (menu_option!=5){
    show_menu();
    scanf("%d", &menu_option);
    while ((menu_option!=1)
      &&(menu_option!=2)
      &&(menu_option!=3)
      &&(menu_option!=4)
      &&(menu_option!=5)) { // garantia de entrada correta.
        show_menu();
        printf("Escolha invalida!\nEntre novamente.\n");
        scanf("%d", &menu_option);
    }
    menu_selection(menu_option);
  }
  return 0;
}