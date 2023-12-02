#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h> //libreria de directorios 
#include "hashmap.h"
#include "list.h"

typedef struct usuario {
  char rut[15];
  char nombre[50];
  char edad[3];
  char sexo[10];
  List * lista_horas; // horas medicas
  List * semana[7]; // ejercicio
  int semana_suenio[7]; // horas sueño
  List * semana_medicamentos[7]; // medicamento
  float imc; //IMC
  const char *rango_imc;
  const char *consejo_imc;
} Data_usuario;

typedef struct dia {
  char fecha[10];
  char hora[6];
  char nombre_doctor[20];
  char informacion_extra[500];
} horas_dia_medica;

typedef struct Ejercicio {
    char nombre[100];
    char reps_tiempo[200];
} Ejercicio;

typedef struct Medicamento {
    char hora[6];
    char nombre[50];
} Medicamento;


// #####################  FUNCIONES EXTRAS  #####################
void obtenerEntrada(char *cadena, int tallaMax) {
  fgets(cadena, tallaMax, stdin);
  size_t largo = strlen(cadena);
  if (largo > 0 && cadena[largo - 1] == '\n') {
    cadena[largo - 1] = '\0';
  }
}

char * validarEntrada(char *cadena){
  while(1) {
    obtenerEntrada(cadena, 100);
    if(strlen(cadena)<100) {
      return cadena;
    } else {
      printf("LIMITE DE CARACTERES EXCEDIDO, MAXIMO DE 100 CARACTERES. \nRepita el dato: ");
    }
  }
}

// -------------------- HORAS MEDICAS--------------------

void registro_horas_medicas(Data_usuario * usuario){
  List* lista_horas = usuario->lista_horas;
  char entrada[6];
  horas_dia_medica * nueva_hora_medica = (horas_dia_medica *) malloc(sizeof(horas_dia_medica));

  printf("Ingrese los datos según se le soliciten para registrar su hora.\n");
  printf("Ingrese la fecha en formato dia/mes/año: \n");
  scanf(" %[^\n]", nueva_hora_medica->fecha);
  printf("Ingrese la hora en formato 24 horas en orden hora:minutos : \n");
  scanf(" %[^\n]", nueva_hora_medica->hora);
  printf("Ingrese el nombre del médico : \n");
  scanf(" %[^\n]", nueva_hora_medica->nombre_doctor);
  printf("Desea añadir algún comentario extra a la hora? (si/no): \n");
  scanf(" %[^\n]", entrada);
  if (strcmp(entrada,"si") == 0){
    printf("Ingrese comentarios extra de la consulta: \n");
    scanf(" %[^\n]", nueva_hora_medica->informacion_extra);
  }
  else {
    strcpy(nueva_hora_medica->informacion_extra,"Información no ingresada.");
  }

  pushBack(lista_horas, nueva_hora_medica);
  printf("\nHora registrada con éxito.\n");
}

// -------------------- MOSTRAR HORAS MEDICAS --------------------

int comparar_horas(const void *a, const void *b) {
  horas_dia_medica * hora1 = *(horas_dia_medica **) a;
  horas_dia_medica * hora2 = *(horas_dia_medica **) b;
  return strcmp(hora1->hora, hora2->hora);
}

void mostrar_registro_horas(Data_usuario * usuario){
  List *lista = usuario->lista_horas;

  int largo = sizeList(lista);
  horas_dia_medica **horas_array = (horas_dia_medica **)malloc(largo * sizeof(horas_dia_medica *));
  if (horas_array == NULL) {
    fprintf(stderr, "error en la asignación de memoria.\n");
    return;
  }

  Node * nodo_aux = lista->head;
  int i = 0;
  while (nodo_aux != NULL) {
    horas_dia_medica * hora = (horas_dia_medica *) nodo_aux->data;
    horas_array[i] = hora;
    nodo_aux = nodo_aux->next;
    i++;
  }
  qsort(horas_array, largo, sizeof(Data_usuario *), comparar_horas);

  for (int j = 0; j < largo; j++) {
    horas_dia_medica *hora_Medica = horas_array[j];
    printf("\nHora: %s\nFecha: %s \nNombre del médico: %s\nInformacion extra: %s\n", hora_Medica->hora, hora_Medica->fecha, hora_Medica->nombre_doctor, hora_Medica->informacion_extra);
  }

  free(horas_array);
}

// -------------------- REGISTRAR EJERCICIOS --------------------

void registrar_ejercicio(Data_usuario * usuario){
  int dia;
  while(1) {
    printf("\n𝙿𝚊𝚛𝚊 𝚛𝚎𝚊𝚕𝚒𝚣𝚊𝚛 𝚎𝚕 𝚛𝚎𝚐𝚒𝚜𝚝𝚛𝚘 𝚍𝚎 𝚜𝚞 𝚎𝚓𝚎𝚛𝚌𝚒𝚌𝚒𝚘, 𝚙𝚛𝚒𝚖𝚎𝚛𝚘 𝚍𝚎𝚋𝚎𝚛á 𝚒𝚗𝚐𝚛𝚎𝚜𝚊𝚛 𝚎𝚕 𝚗ú𝚖𝚎𝚛𝚘 𝚍𝚎𝚕 𝚍í𝚊 𝚌𝚘𝚛𝚛𝚎𝚜𝚙𝚘𝚗𝚍𝚒𝚎𝚗𝚝𝚎.\n");
    printf("\n      𝐇𝐨𝐫𝐚𝐫𝐢𝐨 𝐬𝐞𝐦𝐚𝐧𝐚𝐥\n");
    printf("1-. Lunes\n");
    printf("2-.Martes\n");
    printf("3-.Miércoles\n");
    printf("4-.Jueves\n");
    printf("5-. Viernes\n");
    printf("6-.Sábado\n");
    printf("7-.Domingo\n");
    printf("8-.Volver atrás\n");
    scanf("%d", &dia);
    if(dia==8) break;

    List *lista_ejercicios = usuario->semana[dia-1];

    Ejercicio* ejercicio = (Ejercicio*)malloc(sizeof(Ejercicio));
    printf("\nIngrese el nombre del ejercicio:\n");
    scanf("%s", ejercicio->nombre);

    printf("Ingrese la cantidad de repeticiones: \n");
    scanf("%s", ejercicio->reps_tiempo);

    pushBack(lista_ejercicios, ejercicio);
    
  }
}

// -------------------- ELIMINAR MEDICAMENTO ------------------- o

void eliminar_ejercicios_semanales(Data_usuario * usuario) {

  int dia;
  while(1) {
    printf("\n𝙿𝚊𝚛𝚊 𝚛𝚎𝚊𝚕𝚒𝚣𝚊𝚛 𝚕𝚊 𝚎𝚕𝚒𝚖𝚒𝚗𝚊𝚌𝚒ó𝚗 𝚍𝚎 𝚜𝚞 𝚎𝚓𝚎𝚛𝚌𝚒𝚌𝚒𝚘, 𝚙𝚛𝚒𝚖𝚎𝚛𝚘 𝚍𝚎𝚋𝚎𝚛á 𝚒𝚗𝚐𝚛𝚎𝚜𝚊𝚛 𝚎𝚕 𝚗ú𝚖𝚎𝚛𝚘 𝚍𝚎𝚕 𝚍í𝚊 𝚌𝚘𝚛𝚛𝚎𝚜𝚙𝚘𝚗𝚍𝚒𝚎𝚗𝚝𝚎.\n");
    printf("\n      𝐇𝐨𝐫𝐚𝐫𝐢𝐨 𝐬𝐞𝐦𝐚𝐧𝐚𝐥\n");
    printf("1-. Lunes\n");
    printf("2-.Martes\n");
    printf("3-.Miércoles\n");
    printf("4-.Jueves\n");
    printf("5-. Viernes\n");
    printf("6-.Sábado\n");
    printf("7-.Domingo\n");
    printf("8-.Volver atrás\n");
    scanf("%d", &dia);
    if(dia==8) break;
    char nombre_ejercicio[50];
    char hora[5];
    printf("\nIngrese el nombre del ejercicio que desea eliminar:\n");
    printf("Nombre: ");
    scanf("%s", nombre_ejercicio);
    
    List *lst_ejercicio = usuario->semana[dia-1];

    Node * current = lst_ejercicio->head;
    while (current!=NULL){
      Ejercicio* ejercicio = (Ejercicio*)current->data;
      if(strcmp(ejercicio->nombre,nombre_ejercicio)==0){
        popCurrent(lst_ejercicio);
        printf("\n ✔ Ejercicio eliminado.\n");
        return;
      }
      current=current->next;
    }
    printf("\nNo se encontró el ejercicio solicitado.\n");
  }
}




// -------------------- MOSTRAR EJERCICIOS --------------------

void mostrar_ejercicios_semanales(Data_usuario * usuario){
  for(int i = 0; i < 7; i++){
    if(i==0){
      printf("\n╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑳𝑼𝑵𝑬𝑺 𝄖𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==1){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑴𝑨𝑹𝑻𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==2){
      printf("╭𝄖𝄖𝄖𝄖 𝑴𝑰𝑬𝑹𝑪𝑶𝑳𝑬𝑺 𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==3){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑱𝑼𝑬𝑽𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==4){
      printf("╭𝄖𝄖𝄖𝄖𝄖 𝑽𝑰𝑬𝑹𝑵𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==5){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑺𝑨𝑩𝑨𝑫𝑶 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==6){
      printf("╭𝄖𝄖𝄖𝄖𝄖 𝑫𝑶𝑴𝑰𝑵𝑮𝑶 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    List* lst_dia = usuario->semana[i];
    Node* current = lst_dia->head;
    while(current!=NULL){
      Ejercicio* ejercicio = (Ejercicio*)current->data;
      printf("\nEjercicio: %s\nReps/Tiempo: %s\n", ejercicio->nombre, ejercicio->reps_tiempo);
      current=current->next;
    }

    printf("\n╰𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖╯\n");
  }
}

//--------------------------REGISTRAR IMC---------------------------

void calcular_imc(Data_usuario * usuario){
    
  printf("\n𝙼𝚎𝚍𝚒𝚊𝚗𝚝𝚎 𝚎𝚕 𝚒𝚗𝚐𝚛𝚎𝚜𝚘 𝚍𝚎 𝚜𝚞 𝚙𝚎𝚜𝚘 𝚢 𝚊𝚕𝚝𝚞𝚛𝚊, 𝚗𝚞𝚎𝚜𝚝𝚛𝚊 𝚌𝚊𝚕𝚌𝚞𝚕𝚊𝚍𝚘𝚛𝚊 𝚍𝚎 𝙸𝙼𝙲 𝚗𝚘𝚜 𝚍𝚒𝚛á 𝚎𝚗 𝚚𝚞𝚎 𝚌𝚊𝚝𝚎𝚐𝚘𝚛í𝚊 𝚜𝚎 𝚎𝚗𝚌𝚞𝚎𝚗𝚝𝚛𝚊.\n");
  printf("\nIngrese su peso en kilogramos: ");
  float peso;
  scanf("%f", &peso);

  printf("\nIngrese su altura en metros: ");
  float altura;
  scanf("%f", &altura);

  float imc = peso / (altura * altura);

  const char *rango;
  const char *consejo;

  if (imc < 18.5){
    rango = "Bajo peso ⚠";
    consejo = "𝙲𝚘𝚗𝚜𝚎𝚓𝚘: 𝙰𝚞𝚖𝚎𝚗𝚝𝚎 𝚜𝚞 𝚒𝚗𝚐𝚎𝚜𝚝𝚊 𝚍𝚎 𝚌𝚊𝚕𝚘𝚛í𝚊𝚜 𝚢 𝚗𝚞𝚝𝚛𝚒𝚎𝚗𝚝𝚎𝚜, 𝚌𝚘𝚗𝚜𝚒𝚍𝚎𝚛𝚎 𝚑𝚊𝚋𝚕𝚊𝚛 𝚌𝚘𝚗 𝚞𝚗 𝚗𝚞𝚝𝚛𝚒𝚌𝚒𝚘𝚗𝚒𝚜𝚝𝚊.";
  } 
  else if (imc < 25){
    rango = "Peso normal ✔";
    consejo = "𝙲𝚘𝚗𝚜𝚎𝚓𝚘: 𝙰𝚞𝚖𝚎𝚗𝚝𝚎 𝚜𝚞 𝚒𝚗𝚐𝚎𝚜𝚝𝚊 𝚍𝚎 𝚌𝚊𝚕𝚘𝚛í𝚊𝚜 𝚢 𝚗𝚞𝚝𝚛𝚒𝚎𝚗𝚝𝚎𝚜, 𝚌𝚘𝚗𝚜𝚒𝚍𝚎𝚛𝚎 𝚑𝚊𝚋𝚕𝚊𝚛 𝚌𝚘𝚗 𝚞𝚗 𝚗𝚞𝚝𝚛𝚒𝚌𝚒𝚘𝚗𝚒𝚜𝚝𝚊.";
  } 
  else if (imc < 30){
    rango = "Sobrepeso ⚠";
    consejo = "𝙲𝚘𝚗𝚜𝚎𝚓𝚘: 𝙰𝚍𝚘𝚙𝚝𝚎 𝚑á𝚋𝚒𝚝𝚘𝚜 𝚊𝚕𝚒𝚖𝚎𝚗𝚝𝚒𝚌𝚒𝚘𝚜 𝚖á𝚜 𝚜𝚊𝚕𝚞𝚍𝚊𝚋𝚕𝚎𝚜 𝚢 𝚊𝚞𝚖𝚎𝚗𝚝𝚎 𝚕𝚊 𝚊𝚌𝚝𝚒𝚟𝚒𝚍𝚊𝚍 𝚏í𝚜𝚒𝚌𝚊.";
  } 
  else{
    rango = "Obesidad ⚠";
    consejo = "𝙲𝚘𝚗𝚜𝚎𝚓𝚘: 𝙲𝚘𝚗𝚜𝚞𝚕𝚝𝚎 𝚊 𝚞𝚗 𝚙𝚛𝚘𝚏𝚎𝚜𝚒𝚘𝚗𝚊𝚕 𝚍𝚎 𝚕𝚊 𝚜𝚊𝚕𝚞𝚍 𝚙𝚊𝚛𝚊 𝚛𝚎𝚌𝚒𝚋𝚒𝚛 𝚘𝚛𝚒𝚎𝚗𝚝𝚊𝚌𝚒ó𝚗 𝚜𝚘𝚋𝚛𝚎 𝚕𝚊 𝚙é𝚛𝚍𝚒𝚍𝚊 𝚍𝚎 𝚙𝚎𝚜𝚘.";
  }

  usuario->imc = imc;
  usuario->rango_imc=rango;
  usuario->consejo_imc=consejo;
  
  printf("\nSu imc corresponde a %.2lf\nUsted se encuentra en el rango de %s\n\n%s\n",imc,rango, consejo);
   
}

// -------------------- REGISTRAR SUEÑO --------------------

void registrar_horas_suenio(Data_usuario * usuario) {
  int dia;
  while(1) {
    printf("\n𝙰𝚗𝚝𝚎𝚜 𝚍𝚎 𝚛𝚎𝚐𝚒𝚜𝚝𝚛𝚊𝚛 𝚜𝚞 𝚝𝚒𝚎𝚖𝚙𝚘 𝚍𝚎 𝚜𝚞𝚎ñ𝚘, 𝚒𝚗𝚐𝚛𝚎𝚜𝚎 𝚎𝚕 𝚗ú𝚖𝚎𝚛𝚘 𝚌𝚘𝚛𝚛𝚎𝚜𝚙𝚘𝚗𝚍𝚒𝚎𝚗𝚝𝚎 𝚊𝚕 𝚍í𝚊 𝚎𝚗 𝚎𝚕 𝚚𝚞𝚎 𝚍𝚎𝚜𝚎𝚊 𝚛𝚎𝚐𝚒𝚜𝚝𝚛𝚊𝚛 𝚕𝚊𝚜 𝚑𝚘𝚛𝚊𝚜.\n");
    printf("\n      𝐇𝐨𝐫𝐚𝐫𝐢𝐨 𝐬𝐞𝐦𝐚𝐧𝐚𝐥\n");
    printf("1-. Lunes\n");
    printf("2-.Martes\n");
    printf("3-.Miércoles\n");
    printf("4-.Jueves\n");
    printf("5-. Viernes\n");
    printf("6-.Sábado\n");
    printf("7-.Domingo\n");
    printf("8-.Volver atrás\n");
    scanf("%d", &dia);
    if(dia==8) break;

    printf("\nIngrese la cantidad de horas que durmió:\n");
    scanf("%d", &(usuario->semana_suenio[dia-1]));
  }
}

// ------- CALCULAR PROMEDIO -------
float prom_horas_suenio(Data_usuario * usuario) {
  float suma_horas = 0;
  int total_dias = 0;

  for(int i = 0; i < 7; i++) {
    int horas_dia = usuario->semana_suenio[i];
    if(horas_dia > 0) {
      total_dias++;
      suma_horas += horas_dia;
    }
  }
  if (total_dias == 0) {
      return 0.0;  // evitar división por cero
  }
  return suma_horas/total_dias;
}

// ------- REINICIAR SEMANA -------
void reiniciar_semana(Data_usuario * usuario) {
  for(int i = 0; i < 7; i++) {
    usuario->semana_suenio[i] = 0;
  }
}

// -------------------- MOSTRAR SUEÑO PROM--------------------

void mostrar_horas_semanales(Data_usuario * usuario) {
  for(int i = 0; i < 7; i++){
    if(i==0){
      printf("\n╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑳𝑼𝑵𝑬𝑺 𝄖𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==1){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑴𝑨𝑹𝑻𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==2){
      printf("╭𝄖𝄖𝄖𝄖 𝑴𝑰𝑬𝑹𝑪𝑶𝑳𝑬𝑺 𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==3){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑱𝑼𝑬𝑽𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==4){
      printf("╭𝄖𝄖𝄖𝄖𝄖 𝑽𝑰𝑬𝑹𝑵𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==5){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑺𝑨𝑩𝑨𝑫𝑶 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==6){
      printf("╭𝄖𝄖𝄖𝄖𝄖 𝑫𝑶𝑴𝑰𝑵𝑮𝑶 𝄖𝄖𝄖𝄖𝄖╮\n");
    }

    printf("\n  %d hrs. dormidas\n", usuario->semana_suenio[i]);
    printf("\n╰𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖╯\n");
  }
  float promedio = prom_horas_suenio(usuario);
  printf("\nSu promedio de horas dormidas es: %.2f\n", promedio);

  // ************ CONSEJO ************
  int edad_int = atoi(usuario->edad);
  int horas_min;
  int horas_max;
  float prom_min;
  float prom_max;
  if(edad_int <= 12) {
    horas_min = 9;
    horas_max = 12;
  }
  if(edad_int > 12 && edad_int <= 18) {
    horas_min = 8;
    horas_max = 10;
  }
  if(edad_int > 18 && edad_int < 65) {
    horas_min = 7;
    horas_max = 9;
  }
  else{
    horas_min = 5;
    horas_max = 6;
  }
  printf("\n☛ Al tener %d años de edad, tu cuerpo debe tener al menos entre %d y %d horas de descanso, según expertos.\n", edad_int, horas_min, horas_max);
  if(promedio < horas_min) {
    printf("\n⚠ Tu promedio de horas de sueño es BAJO ⚠\n𝙻𝚊 𝚏𝚊𝚕𝚝𝚊 𝚍𝚎 𝚜𝚞𝚎ñ𝚘 𝚙𝚞𝚎𝚍𝚎 𝚝𝚎𝚗𝚎𝚛 𝚌𝚘𝚗𝚜𝚎𝚌𝚞𝚎𝚗𝚌𝚒𝚊𝚜 𝚗𝚎𝚐𝚊𝚝𝚒𝚟𝚊𝚜 𝚎𝚗 𝚕𝚊 𝚜𝚊𝚕𝚞𝚍 𝚏í𝚜𝚒𝚌𝚊 𝚢 𝚖𝚎𝚗𝚝𝚊𝚕, 𝚊𝚞𝚖𝚎𝚗𝚝𝚊𝚗𝚍𝚘 𝚎𝚕 𝚛𝚒𝚎𝚜𝚐𝚘 𝚍𝚎 𝚊𝚌𝚌𝚒𝚍𝚎𝚗𝚝𝚎𝚜. 𝙿𝚊𝚛𝚊 𝚊𝚞𝚖𝚎𝚗𝚝𝚊𝚛 𝚝𝚞 𝚙𝚛𝚘𝚖𝚎𝚍𝚒𝚘, 𝚙𝚛𝚞𝚎𝚋𝚊 𝚎𝚜𝚝𝚊𝚛 𝚎𝚗 𝚞𝚗 𝚊𝚖𝚋𝚒𝚎𝚗𝚝𝚎 𝚌ó𝚖𝚘𝚍𝚘 𝚢 𝚎𝚟𝚒𝚝𝚊 𝚕𝚊𝚜 𝚙𝚊𝚗𝚝𝚊𝚕𝚕𝚊𝚜 𝚊𝚣𝚞𝚕𝚎𝚜 𝚊𝚕 𝚖𝚎𝚗𝚘𝚜 𝚍𝚘𝚜 𝚑𝚘𝚛𝚊𝚜 𝚊𝚗𝚝𝚎𝚜 𝚍𝚎 𝚍𝚘𝚛𝚖𝚒𝚛.\n");
  }
  else if(promedio > horas_max) {
    printf("\n⚠ Tu promedio de horas de sueño es ALTO ⚠\n𝙴𝚕 𝚎𝚡𝚌𝚎𝚜𝚘 𝚍𝚎 𝚜𝚞𝚎ñ𝚘 𝚙𝚞𝚎𝚍𝚎 𝚝𝚎𝚗𝚎𝚛 𝚌𝚘𝚗𝚜𝚎𝚌𝚞𝚎𝚗𝚌𝚒𝚊𝚜 𝚗𝚎𝚐𝚊𝚝𝚒𝚟𝚊𝚜 𝚎𝚗 𝚕𝚊 𝚜𝚊𝚕𝚞𝚍 𝚏í𝚜𝚒𝚌𝚊 𝚢 𝚖𝚎𝚗𝚝𝚊𝚕, 𝚊𝚞𝚖𝚎𝚗𝚝𝚊𝚗𝚍𝚘 𝚎𝚕 𝚛𝚒𝚎𝚜𝚐𝚘 𝚍𝚎 𝚊𝚌𝚌𝚒𝚍𝚎𝚗𝚝𝚎𝚜. 𝙿𝚊𝚛𝚊 𝚍𝚒𝚜𝚖𝚒𝚗𝚞𝚒𝚛 𝚝𝚞 𝚙𝚛𝚘𝚖𝚎𝚍𝚒𝚘, 𝚙𝚛𝚞𝚎𝚋𝚊 𝚙𝚘𝚗𝚎𝚛 𝚊𝚕𝚊𝚛𝚖𝚊𝚜 𝚢 𝚎𝚜𝚝𝚊𝚋𝚕𝚎𝚌𝚎𝚛 𝚑𝚘𝚛𝚊𝚛𝚒𝚘𝚜 𝚍𝚎 𝚜𝚞𝚎ñ𝚘 𝚌𝚘𝚗𝚜𝚒𝚜𝚝𝚎𝚗𝚝𝚎𝚜, 𝚒𝚗𝚌𝚕𝚞𝚜𝚘 𝚕𝚘𝚜 𝚏𝚒𝚗𝚎𝚜 𝚍𝚎 𝚜𝚎𝚖𝚊𝚗𝚊.\n");
  } else {
    printf("\n✔ Tu promedio de horas de sueño es ADECUADO ✔\n𝙾𝚋𝚝𝚎𝚗𝚎𝚛 𝚕𝚊 𝚌𝚊𝚗𝚝𝚒𝚍𝚊𝚍 𝚊𝚍𝚎𝚌𝚞𝚊𝚍𝚊 𝚍𝚎 𝚜𝚞𝚎ñ𝚘 𝚎𝚜 𝚋𝚎𝚗𝚎𝚏𝚒𝚌𝚒𝚘𝚜𝚘 𝚙𝚊𝚛𝚊 𝚕𝚊 𝚜𝚊𝚕𝚞𝚍, 𝚖𝚎𝚓𝚘𝚛𝚊 𝚎𝚕 𝚎𝚜𝚝𝚊𝚍𝚘 𝚍𝚎 á𝚗𝚒𝚖𝚘, 𝚎𝚕 𝚛𝚎𝚗𝚍𝚒𝚖𝚒𝚎𝚗𝚝𝚘 𝚏í𝚜𝚒𝚌𝚘, 𝚎𝚕 𝚎𝚚𝚞𝚒𝚕𝚒𝚋𝚛𝚒𝚘 𝚑𝚘𝚛𝚖𝚘𝚗𝚊𝚕, 𝚎𝚕 𝚖𝚎𝚝𝚊𝚋𝚘𝚕𝚒𝚜𝚖𝚘, 𝚕𝚊 𝚜𝚊𝚕𝚞𝚍 𝚌𝚊𝚛𝚍𝚒𝚘𝚟𝚊𝚜𝚌𝚞𝚕𝚊𝚛, 𝚌𝚘𝚗𝚝𝚛𝚘𝚕𝚊 𝚎𝚕 𝚙𝚎𝚜𝚘 𝚢 𝚏𝚘𝚛𝚝𝚊𝚕𝚎𝚌𝚎 𝚎𝚕 𝚜𝚒𝚜𝚝𝚎𝚖𝚊 𝚒𝚗𝚖𝚞𝚗𝚘𝚕ó𝚐𝚒𝚌𝚘. ¡𝙴𝚡𝚌𝚎𝚕𝚎𝚗𝚝𝚎, 𝚜𝚒𝚐𝚞𝚎 𝚊𝚜í!\n");
  }
}

// -------------------- REGISTRAR MEDICAMENTO --------------------

void registrar_medicamento(Data_usuario * usuario){
  printf("\n𝙿𝚊𝚛𝚊 𝚛𝚎𝚐𝚒𝚜𝚝𝚛𝚊𝚛 𝚖𝚎𝚍𝚒𝚌𝚊𝚖𝚎𝚗𝚝𝚘𝚜 𝚍𝚎𝚋𝚎 𝚝𝚘𝚖𝚊𝚛 𝚎𝚗 𝚌𝚞𝚎𝚗𝚝𝚊 𝚕𝚊𝚜 𝚑𝚘𝚛𝚊𝚜 𝚎𝚡𝚊𝚌𝚝𝚊𝚜 𝚍𝚎 𝚒𝚗𝚐𝚎𝚜𝚝𝚊, 𝚎𝚜 𝚍𝚎𝚌𝚒𝚛 𝚚𝚞𝚎 𝚜𝚒 𝚜𝚞 𝚛𝚎𝚌𝚎𝚝𝚊 𝚒𝚗𝚍𝚒𝚌𝚊 𝚞𝚗 𝚛𝚎𝚖𝚎𝚍𝚒𝚘 𝚌𝚊𝚍𝚊 𝟾 𝚑𝚘𝚛𝚊𝚜 𝚙𝚘𝚛 𝟹 𝚍í𝚊𝚜, 𝚙𝚘𝚛 𝚎𝚓𝚎𝚖𝚙𝚕𝚘, 𝚞𝚜𝚝𝚎𝚍 𝚍𝚎𝚋𝚎𝚛á 𝚊𝚗𝚘𝚝𝚊𝚛 𝚌𝚊𝚍𝚊 𝚝𝚘𝚖𝚊 𝚜𝚎𝚐ú𝚗 𝚌𝚘𝚛𝚛𝚎𝚜𝚙𝚘𝚗𝚍𝚊 𝚎𝚗 𝚌𝚊𝚍𝚊 𝚍í𝚊 𝚌𝚘𝚗 𝚜𝚞 𝚑𝚘𝚛𝚊. (𝙻𝚞𝚗𝚎𝚜 0𝟾:𝟶𝟶, 𝙻𝚞𝚗𝚎𝚜 𝟷𝟼:𝟶𝟶, 𝙼𝚊𝚛𝚝𝚎𝚜 𝟶𝟶:𝟶𝟶,...𝚎𝚝𝚌)\n");
  int dia;
  while(1) {
    printf("\n\n  RECUERDE ESCRIBIR SIEMPRE LAS HORAS RELLENANDO CON 0 POR EJ: 08:00 , 01:00 ,23:00\n\n");
    printf("\n      𝐇𝐨𝐫𝐚𝐫𝐢𝐨 𝐬𝐞𝐦𝐚𝐧𝐚𝐥\n");
    printf("1-. Lunes\n");
    printf("2-.Martes\n");
    printf("3-.Miércoles\n");
    printf("4-.Jueves\n");
    printf("5-. Viernes\n");
    printf("6-.Sábado\n");
    printf("7-.Domingo\n");
    printf("8-.Volver atrás\n");
    scanf("%d", &dia);
    if(dia==8) break;

    List *lista_medicamento = usuario->semana_medicamentos[dia-1];

    Medicamento* medicamento = (Medicamento*)malloc(sizeof(Medicamento));
    printf("\nIngrese el nombre del medicamento:\n");
    scanf("%s", medicamento->nombre);

    printf("\n\n  RECUERDE ESCRIBIR SIEMPRE LAS HORAS RELLENANDO CON 0 POR EJ: 08:00 , 01:00 ,23:00\n\n");
    printf("Ingrese la hora de ingesta:\n");
    scanf("%s", medicamento->hora);

    pushBack(lista_medicamento, medicamento);
  }
}

// -------------------- ELIMINAR MEDICAMENTO --------------------
void eliminar_medicamento(Data_usuario * usuario){
  
  int dia;
  while(1) {
    printf("\n𝙿𝚊𝚛𝚊 𝚛𝚎𝚊𝚕𝚒𝚣𝚊𝚛 𝚕𝚊 𝚎𝚕𝚒𝚖𝚒𝚗𝚊𝚌𝚒ó𝚗 𝚍𝚎 𝚜𝚞 𝚖𝚎𝚍𝚒𝚌𝚊𝚖𝚎𝚗𝚝𝚘, 𝚙𝚛𝚒𝚖𝚎𝚛𝚘 𝚍𝚎𝚋𝚎𝚛á 𝚒𝚗𝚐𝚛𝚎𝚜𝚊𝚛 𝚎𝚕 𝚗ú𝚖𝚎𝚛𝚘 𝚍𝚎𝚕 𝚍í𝚊 𝚌𝚘𝚛𝚛𝚎𝚜𝚙𝚘𝚗𝚍𝚒𝚎𝚗𝚝𝚎.\n");
    printf("\n      𝐇𝐨𝐫𝐚𝐫𝐢𝐨 𝐬𝐞𝐦𝐚𝐧𝐚𝐥\n");
    printf("1-. Lunes\n");
    printf("2-.Martes\n");
    printf("3-.Miércoles\n");
    printf("4-.Jueves\n");
    printf("5-. Viernes\n");
    printf("6-.Sábado\n");
    printf("7-.Domingo\n");
    printf("8-.Volver atrás\n");
    scanf("%d", &dia);
    if(dia==8) break;
    char nombre_medicamento[50];
    char hora[5];
    printf("\nIngrese el nombre del medicamento y la hora que desea eliminar:\n");
    printf("Nombre: ");
    scanf("%s", nombre_medicamento);
    printf("Hora: ");
    scanf("%s", hora);
    
    List *lista_medicamento = usuario->semana_medicamentos[dia-1];
    
    Node * current=lista_medicamento->head;
    while (current!=NULL){
      Medicamento* medicamento = (Medicamento*)current->data;
      if(strcmp(medicamento->nombre,nombre_medicamento)==0){
        if(strcmp(medicamento->hora,hora)==0){
          popCurrent(lista_medicamento);
          printf("\n ✔ Medicamento eliminado.\n");
          return;
        }
      }
      current=current->next;
      }
      printf("\nNo se encontró el medicamento o la hora ingresada.\n");
  }
}
// ------------------- MOSTRAR MEDICAMENTO -------------------

void Ordenar_Medicamentos_por_hora(List *lista) {
  Node *current = lista->head;
  Node *aux_sig = NULL;
  Medicamento *temp;

  if (lista->head == NULL) {
    return;
  } else {
    while (current != NULL) {
      aux_sig = current->next;
      while (aux_sig != NULL) {
        Medicamento * medicamento1= current->data;
        Medicamento * medicamento2= aux_sig->data;
        if (strcmp(medicamento1->hora, medicamento2->hora) > 0) {
          temp = current->data;
          current->data = aux_sig->data;
          aux_sig->data = temp;
        }
        aux_sig = aux_sig->next;
      }
      current = current->next;
    }
  }
}

void mostrar_medicamento(Data_usuario * usuario){
  for(int i = 0; i < 7; i++){
    if(i==0){
      printf("\n╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑳𝑼𝑵𝑬𝑺 𝄖𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==1){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑴𝑨𝑹𝑻𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==2){
      printf("╭𝄖𝄖𝄖𝄖 𝑴𝑰𝑬𝑹𝑪𝑶𝑳𝑬𝑺 𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==3){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑱𝑼𝑬𝑽𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==4){
      printf("╭𝄖𝄖𝄖𝄖𝄖 𝑽𝑰𝑬𝑹𝑵𝑬𝑺 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==5){
      printf("╭𝄖𝄖𝄖𝄖𝄖𝄖 𝑺𝑨𝑩𝑨𝑫𝑶 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    else if(i==6){
      printf("╭𝄖𝄖𝄖𝄖𝄖 𝑫𝑶𝑴𝑰𝑵𝑮𝑶 𝄖𝄖𝄖𝄖𝄖╮\n");
    }
    List* lst_dia = usuario->semana_medicamentos[i];
    Ordenar_Medicamentos_por_hora(lst_dia);
    Node* current = lst_dia->head;
    while(current!=NULL){
      Medicamento* medicamento = (Medicamento*)current->data;
      printf("\n%s, toma a las: %s hrs.\n", medicamento->nombre, medicamento->hora);
      current=current->next;
    }
    printf("\n╰𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖𝄖╯\n");
  }
}

// ------------------- PREGUNTA DEL DIA -------------------

typedef struct Pregunta
{
    char enunciado[300];
    char opciones[4][200];
    char respuesta_correcta;
    char dato_de_interes[400];
} Pregunta;

Pregunta preguntas[] = {
  {"¿Cuántos litros de agua se recomienda generalmente beber al día para mantenerse hidratado?",
   {"2,5", "3,5", "4,5", "5,5"},
   'b',
   "En promedio, los hombres pueden necesitar alrededor de 3.7 litros diarios, mientras que las mujeres necesitan aproximadamente 2.7 litros. La diferencia se debe a la masa muscular, que retiene más agua en los hombres."},
  {"¿Cuál de las siguientes afirmaciones es cierta sobre el sueño adecuado?",
   {"El sueño profundo es tan importante como la cantidad total de horas.", "Dormir consistentemente 5-7 horas es beneficioso para la salud.", "Dormir más de 10 horas regularmente mejora la salud.", "El insomnio no afecta la salud en general."},
   'a',
   "El insomnio afecta a alrededor del 10% de la población adulta mundial, siendo más común en mujeres y personas mayores."},
  {"¿Cuál de las siguientes afirmaciones describe con mayor precisión uno de los beneficios de la actividad física regular en la salud?",
   {"Reducción de la frecuencia cardíaca en reposo.", "Estimulación del apetito y mejor absorción de nutrientes.", "Mejora de la capacidad cognitiva y reducción del riesgo de deterioro mental.", "Aumento de la densidad ósea y fortalecimiento de los músculos."},
   'c',
   "La actividad física regular también se ha asociado con la liberación de endorfinas, neurotransmisores que actúan como analgésicos naturales y generadores de bienestar, contribuyendo positivamente a la salud mental al reducir el estrés y mejorar el estado de ánimo."},
  {"¿Cuál es el valor de IMC que se considera saludable para alguien de 70 kg de peso y 1,75 de altura?",
   {"20-24.9", "25-29.9", "30-34.9", "35-39.9"},
   'a',
   "Un IMC demasiado bajo puede indicar malnutrición o insuficiente ingesta de nutrientes, lo que puede afectar negativamente la salud ósea, inmunológica y reproductiva. Un IMC elevado está asociado con un mayor riesgo de enfermedades crónicas como diabetes tipo 2, enfermedades cardíacas y problemas articulares. "},
  {"¿Cuál de los siguientes alimentos tiene más calorías?",
   {"Quinoa", "Palta", "Espárragos", "Yogur natural sin azúcar"},
   'b',
   "La palta es el alimento más rico en calorías de la lista, con aproximadamente 160 calorías en 100 gramos, seguida por la quinoa con 120 a 130, el yogur sin azúcar con 50 a 60, y los espárragos con 20 a 25"},
  {"¿Cuál de los siguientes ejercicios ayuda a quemar más calorías por tiempo invertido?",
   {"Natación a ritmo moderado", "Yoga", "Levantamiendo de pesas", "Correr a ritmo moderado"},
   'd',
   "Correr a ritmo moderado tiende a quemar más calorías por tiempo invertido en comparación con natación, yoga y levantamiento de pesas debido a su naturaleza cardiovascular y el uso de múltiples grupos musculares."},
  {"¿Cuál es el componente principal de la dieta que proporciona energía al cuerpo?",
   {"Proteínas", "Vitaminas", "Carbohidratos", "Minerales"},
   'c',
   "Los carbohidratos son la principal fuente de energía para el cuerpo. Se dividen en simples (azúcares) y complejos (almidones y fibras). Es esencial equilibrar la ingesta de carbohidratos para mantener niveles de energía adecuados, pero también es importante incluir proteínas, grasas saludables, vitaminas y minerales para una dieta completa y balanceada."},
};

void hacer_pregunta(Data_usuario * usuario, Pregunta preguntas[], int num_preguntas) {
    int indice_pregunta = rand() % num_preguntas;
    printf("\n%s\n", preguntas[indice_pregunta].enunciado);
    for (int i = 0; i < 4; i++) 
    {
        printf("%c. %s\n", 'a' + i, preguntas[indice_pregunta].opciones[i]);
    }

    char respuesta_usuario;
    printf("Ingrese la letra correspondiente a su respuesta (a, b, c, d): ");
    scanf(" %c", &respuesta_usuario);

    while (getchar() != '\n');

    if (respuesta_usuario == preguntas[indice_pregunta].respuesta_correcta)
    {
        printf("¡Respuesta correcta!\n");
    }
    else
    {
        printf("Respuesta incorrecta. La respuesta correcta es %c.\n", preguntas[indice_pregunta].respuesta_correcta);
    }

    printf("Dato de interés: %s\n", preguntas[indice_pregunta].dato_de_interes);
    printf("Presione Enter para volver al menú.");
    while (getchar() != '\n');
}

// --------------------    MOSTRAR GLOBAL   --------------------

char * obtenerHoraSantiago() {
   
    time_t tiempo_actual;
    struct tm *info_tiempo;
    static char tiempo_str[100];

    time(&tiempo_actual);
    tiempo_actual -= 3 * 60 * 60; 

    info_tiempo = localtime(&tiempo_actual); 
   
    strftime(tiempo_str, sizeof(tiempo_str), "%Y-%m-%d %H:%M:%S", info_tiempo);

    
    return tiempo_str;
}

void mostrar_global(Data_usuario * usuario){
  char *horaSantiago = obtenerHoraSantiago();
  printf("\nLa hora actual STGO CHILE : %s\n", horaSantiago);
  
  printf("\n ... CARGANDO DATOS DE %s", usuario->nombre);
  
  printf("\n\n       FICHA PERSONAL\n");
  printf("\nNombre: %s", usuario->nombre);
  printf("\nRut: %s", usuario->rut);
  printf("\nEdad: %s", usuario->edad);
  printf("\nSexo(M/F): %s", usuario->sexo);

  printf("\n\n   HORAS MÉDICAS REGISTRADAS \n");
  mostrar_registro_horas(usuario);
  
  printf("\n\n   HORAS DE SUEÑO REGISTRADAS \n");
  mostrar_horas_semanales(usuario);

  printf("\n\n   RUTINAS DE EJERCICIO REGISTRADAS \n");
  mostrar_ejercicios_semanales(usuario);
}

// ------------- FUNCIÓN EXTRAER DATOS DE USUARIO YA EXISTENTE -------------
void extraerDatos(Data_usuario *usuario, char clave[10]) {
  char ruta_archivo[150];
  sprintf(ruta_archivo, "usuarios/%s", clave);

  FILE *archivo = fopen(ruta_archivo, "r");
  if (archivo == NULL) {
      perror("No se pudo abrir el archivo");
      return;
  }
  char linea[100];
  while (fgets(linea, sizeof(linea), archivo) != NULL) {
      // buscar cadenas y extraer
      if (strstr(linea, "RUT:") != NULL) {
          sscanf(linea, "RUT: %s[^\n]", usuario->rut);
      }
      else if (strstr(linea, "Nombre:") != NULL) {
          sscanf(linea, "Nombre: %[^\n]", usuario->nombre);
      }
      else if (strstr(linea, "Edad:") != NULL) {
          sscanf(linea, "Edad: %[^\n]", usuario->edad);
      }
      else if (strstr(linea, "Sexo:") != NULL) {
          sscanf(linea, "Sexo: %[^\n]", usuario->sexo);
      }
  }
  fclose(archivo);
  //printf("RUT: %s\nNombre: %s\nEdad: %s\nSexo: %s\n", usuario->rut, usuario->nombre, usuario->edad, usuario->sexo);

  usuario->lista_horas = createList();
  for(int i = 0; i < 7; i++) {
      usuario->semana[i] = createList();          
  }
  for(int i = 0; i < 7; i++) {
    usuario->semana_medicamentos[i] = createList();          
  }
}
// -------------------- IMPRIMIR FICHA MÉDICA -------------------

void guardar_en_archivo(Data_usuario *usuario, char *clave) {
  char ruta_archivo[150];
  sprintf(ruta_archivo, "usuarios/%s", clave);

  // Formatear casero
  FILE *archivoa = fopen(ruta_archivo, "w");
  fclose(archivoa);

  FILE *archivo = fopen(ruta_archivo, "a+");
  if (archivo == NULL) {
    fprintf(stderr, "No se pudo abrir el archivo para escritura.\n");
    return;
  }

  if (archivo) {
    char *horaSantiago = obtenerHoraSantiago();
    
    fprintf(archivo, "------- FICHA PERSONAL %s-------\n", horaSantiago);
    fprintf(archivo, "RUT: %s\nNombre: %s\nEdad: %s\nSexo: %s\n", usuario->rut, usuario->nombre, usuario->edad, usuario->sexo);

    // Impresion de horas medicas
      List *lista = usuario->lista_horas;
      int largo = sizeList(lista);
      horas_dia_medica **horas_array = (horas_dia_medica **)malloc(largo * sizeof(horas_dia_medica *));
      if (horas_array == NULL) {
        fprintf(stderr, "error en la asignación de memoria.\n");
        return;
      }
      Node * nodo_aux = lista->head;
      int i = 0;
      while (nodo_aux != NULL) {
        horas_dia_medica * hora = (horas_dia_medica *) nodo_aux->data;
        horas_array[i] = hora;
        nodo_aux = nodo_aux->next;
        i++;
      }

      fprintf(archivo, "\n------- HORAS MEDICAS REGISTRADAS-------");
      for (int j = 0; j < largo; j++) {
        horas_dia_medica *hora_Medica = horas_array[j];
        fprintf(archivo, "\nHora: %s\nFecha: %s \nNombre del médico: %s\nInformacion extra: %s\n", hora_Medica->hora, hora_Medica->fecha, hora_Medica->nombre_doctor, hora_Medica->informacion_extra);
      }
      free(horas_array);

       // Impresión de DATOS IMC
      fprintf(archivo, "\n------- IMC -------\n");
      fprintf(archivo, "Su imc corresponde a %.2lf \nUsted se encuentra en el rango %s\n%s\n",usuario->imc ,usuario->rango_imc, usuario->consejo_imc);

      // Impresion de horas sueño
      fprintf(archivo, "\n------- HORAS DE SUEÑO -------");
      for(int i = 0; i < 7; i++) {
        if(i==0){
          fprintf(archivo, "\nLunes: ");
        }
        else if(i==1){
          fprintf(archivo, "\nMartes: ");
        }
        else if(i==2){
          fprintf(archivo, "\nMiercoles: ");
        }
        else if(i==3){
          fprintf(archivo, "\nJueves: ");
        }
        else if(i==4){
          fprintf(archivo, "\nViernes: ");
        }
        else if(i==5){
          fprintf(archivo, "\nSabados: ");
        }
        else if(i==6){
          fprintf(archivo, "\nDomingo: ");
        }
        fprintf(archivo, "%d", usuario->semana_suenio[i]);
      }
      // rutinas ejercicio
      fprintf(archivo, "\n\n------- RUTINAS DE EJERCICIOS -------");
      for(int i = 0; i < 7; i++){
        if(i==0){
          fprintf(archivo, "\nLunes: ");
        }
        else if(i==1){
          fprintf(archivo, "\nMartes: ");
        }
        else if(i==2){
          fprintf(archivo, "\nMiercoles: ");
        }
        else if(i==3){
          fprintf(archivo, "\nJueves: ");
        }
        else if(i==4){
          fprintf(archivo, "\nViernes: ");
        }
        else if(i==5){
          fprintf(archivo, "\nSabados: ");
        }
        else if(i==6){
          fprintf(archivo, "\nDomingo: ");
        }
        List* lst_dia = usuario->semana[i];
        Node* current = lst_dia->head;
        while(current != NULL){
          Ejercicio* ejercicio = (Ejercicio*)current->data;
          fprintf(archivo, " Ejercicio: ""%s"", Reps/Tiempo: %s", ejercicio->nombre, ejercicio->reps_tiempo);
          current=current->next;
        }
      }
      // registro medicamentos
      fprintf(archivo, "\n\n------- MEDICAMENTOS REGISTRADOS -------");
      for(int i = 0; i < 7; i++){
        if(i==0){
          fprintf(archivo, "\nLunes: ");
        }
        else if(i==1){
          fprintf(archivo, "\nMartes: ");
        }
        else if(i==2){
          fprintf(archivo, "\nMiercoles: ");
        }
        else if(i==3){
          fprintf(archivo, "\nJueves: ");
        }
        else if(i==4){
          fprintf(archivo, "\nViernes: ");
        }
        else if(i==5){
          fprintf(archivo, "\nSabados: ");
        }
        else if(i==6){
          fprintf(archivo, "\nDomingo: ");
        }
        List* lst_dia = usuario->semana_medicamentos[i];
        Node* current = lst_dia->head;
        while(current != NULL){
          Medicamento* medicamento = (Medicamento*)current->data;
          fprintf(archivo, " ""%s"" a las %s, ", medicamento->nombre, medicamento->hora);
          current=current->next;
        }
      }
    fclose(archivo);
  }
}

// ----------------------------------- M A I N --------------------------------------------
int main() {
  HashMap * usuarios = createMap(1000);
  int num_preguntas = sizeof(preguntas) / sizeof(preguntas[0]);  

  // -------------------- MENU DE INGRESO --------------------
  int opcion;
  while (1) {
      // ------ LEER RUTS (CLAVE) DE CARPETA Y GUARDA EN HASHMAP ------
        DIR *directory;
        struct dirent *entry;

        char *carpeta="usuarios";
        directory = opendir(carpeta);

        if (directory != NULL) {
            // Lee cada entrada del directorio
            printf("\nArchivos cargados (para testeo)\n\n");
            while ((entry = readdir(directory)) != NULL) {
                if (entry->d_name[0] != '.') { // Ignora las entradas '.' y '..'
                    
                    printf("  archivo: %s\n",entry->d_name);
                    Data_usuario * nuevo_usuario = (Data_usuario*)malloc(sizeof(Data_usuario));
                    insertMap(usuarios, entry->d_name, nuevo_usuario);
                }
            }
            closedir(directory);
        } else {
            perror("Error al abrir el directorio");
            return 1;
        }
   
    printf("\n════════════ ✢ 𝙱𝚒𝚎𝚗𝚟𝚎𝚗𝚒𝚍𝚘 𝚊 𝙲𝚑𝚎𝚌𝚔𝚂𝚊𝚕𝚞𝚍 ✢ ════════════\n");
    
    printf("\n𝙶𝚛𝚊𝚌𝚒𝚊𝚜 𝚙𝚘𝚛 𝚎𝚕𝚎𝚐𝚒𝚛𝚗𝚘𝚜, 𝚝𝚞 𝚜𝚊𝚕𝚞𝚍 𝚎𝚜 𝚕𝚘 𝚖á𝚜 𝚒𝚖𝚙𝚘𝚛𝚝𝚊𝚗𝚝𝚎\n 𝚙𝚊𝚛𝚊 𝚗𝚘𝚜𝚘𝚝𝚛𝚘𝚜. 𝙿𝚘𝚛 𝚏𝚊𝚟𝚘𝚛 𝚒𝚗𝚐𝚛𝚎𝚜𝚊 𝚙𝚊𝚛𝚊 𝚌𝚘𝚖𝚎𝚗𝚣𝚊𝚛 𝚊\n           𝚞𝚝𝚒𝚕𝚒𝚣𝚊𝚛 𝚗𝚞𝚎𝚜𝚝𝚛𝚘𝚜 𝚜𝚎𝚛𝚟𝚒𝚌𝚒𝚘𝚜.\n");
    
    printf("\n1. Ingresar a cuenta existente");
    printf("\n2. Crear nuevo usuario");
    printf("\n3. ⮌ Cerrar aplicación\n");
    scanf("%d", &opcion);
    printf("\n═════════════════════════════════════════════════════\n");

      if(opcion == 3) {
        printf("\nGracias por usar CheckSalud, nos vemos!");
        return 0;
      }

      if(opcion == 2) {
        // -------------------- CREAR USUARIO --------------------
        getchar();
        printf("\n𝙿𝚘𝚛 𝚏𝚊𝚟𝚘𝚛, 𝚛𝚎𝚕𝚕𝚎𝚗𝚊 𝚕𝚘𝚜 𝚜𝚒𝚐𝚞𝚒𝚎𝚗𝚝𝚎𝚜 𝚍𝚊𝚝𝚘𝚜. 𝚁𝚎𝚌𝚞𝚎𝚛𝚍𝚊 𝚚𝚞𝚎 𝚗𝚘𝚜𝚘𝚝𝚛𝚘𝚜 𝚗𝚘𝚜 𝚊𝚍𝚊𝚙𝚝𝚊𝚖𝚘𝚜 𝚊 𝚝𝚒, 𝚎𝚜𝚝𝚘 𝚗𝚘𝚜 𝚙𝚎𝚛𝚖𝚒𝚝𝚒𝚛á 𝚖𝚘𝚜𝚝𝚛𝚊𝚛𝚝𝚎 𝚛𝚎𝚌𝚘𝚖𝚎𝚗𝚍𝚊𝚌𝚒𝚘𝚗𝚎𝚜 𝚢 𝚊𝚕𝚎𝚛𝚝𝚊𝚜 𝚖á𝚜 𝚙𝚎𝚛𝚜𝚘𝚗𝚊𝚕𝚒𝚣𝚊𝚍𝚊𝚜.\n");

        char nombre[100], rut[10], edad[3], sexo[100];
        printf("\nNombre: ");
        validarEntrada(nombre);
        printf("Rut: ");
        validarEntrada(rut);
        printf("Edad: ");
        validarEntrada(edad);
        printf("Sexo(M/F): ");
        validarEntrada(sexo);

        Pair * existe = searchMap(usuarios, rut);
        if(existe != NULL){
          printf("\nEsta cuenta ya existe, inténtelo de nuevo\n.");
          continue;
        }

        Data_usuario * nuevo_usuario = (Data_usuario *)malloc(sizeof(Data_usuario));
        strcpy(nuevo_usuario->nombre, nombre);
        strcpy(nuevo_usuario->rut, rut);
        strcpy(nuevo_usuario->edad, edad);
        
        strcpy(nuevo_usuario->sexo, sexo);
        nuevo_usuario->lista_horas = createList();
        for(int i = 0; i < 7; i++) {
          nuevo_usuario->semana[i] = createList();          
        }

        insertMap(usuarios, nuevo_usuario->rut, nuevo_usuario);

        char nombre_archivo[50];
        sprintf(nombre_archivo, "usuarios/%s.txt", nuevo_usuario->rut); 
        FILE *archivo = fopen(nombre_archivo, "w"); 
        if (archivo) {
            fprintf(archivo, "RUT: %s\nNombre: %s\nEdad: %s\nSexo: %s\n", rut, nombre, edad, sexo);
            fclose(archivo); 
            printf("\nUsuario creado exitosamente! Para poder ingresar a tu cuenta, deberás ingresar tu rut como contraseña de la misma forma que lo hayas ingresado aquí.\n");
            printf("\n═════════════════════════════════════════════════════\n");
        } else {
            printf("Error al crear el archivo del usuario.\n");
        }
      }
      else if(opcion == 1) {
        // -------------------- INGRESAR A USUARIO --------------------
        getchar();
        char clave[20];
        printf("\n Contraseña: ");
        validarEntrada(clave);
        printf("\n═════════════════════════════════════════════════════\n");

        char extension[] = ".txt";
        // Concatenar la extensión al string original
        strcat(clave, extension);
        Pair * clave_buscada = searchMap(usuarios,clave);

        if (clave_buscada != NULL) {
          Data_usuario * usuario = (Data_usuario*) clave_buscada->value;

          // ----- EXTRAER DATOS DEL ARCHIVO -----
          extraerDatos(usuario,clave);

          printf("\n           Bienvenido, %s!", usuario->nombre);
          int opcion2;
          // -------------------- MENU APLICACION --------------------
          while(1){
            char *horaSantiago = obtenerHoraSantiago();
            printf( "       %s\n", horaSantiago);
            printf("\n        ¿Qué opcion deseas realizar?\n");
            printf("\n1. Registrar horas médicas");
            printf("\n2. Calcular y registrar IMC");
            printf("\n3. Registrar horas de sueño");
            printf("\n4. Registrar rutinas de ejercicio");
            printf("\n5. Registrar Medicamento");
            printf("\n6. Quiz-Salud()");
            printf("\n7. Mostrar todos los datos recopilados");
            printf("\n8. ⮌ Guardar y cerrar sesión \n");
            scanf("%d", &opcion2);
            printf("\n═════════════════════════════════════════════════════\n");

            // -------------------- HORAS MEDICAS --------------------
            if(opcion2 == 1) {
              getchar();
              char opcion_horas;
              while(opcion_horas != 'c') {
                printf("\na) Registrar hora.\n﹥𝙸𝚗𝚐𝚛𝚎𝚜𝚎 𝚍𝚊𝚝𝚘𝚜 𝚌𝚘𝚖𝚘 𝚕𝚊 𝚏𝚎𝚌𝚑𝚊, 𝚑𝚘𝚛𝚊, 𝚎𝚕 𝚖é𝚍𝚒𝚌𝚘 𝚝𝚛𝚊𝚝𝚊𝚗𝚝𝚎 𝚢 𝚌𝚘𝚖𝚎𝚗𝚝𝚊𝚛𝚒𝚘𝚜 𝚎𝚡𝚝𝚛𝚊𝚜, 𝚌𝚘𝚖𝚘 𝚎𝚕 𝚖𝚘𝚝𝚒𝚟𝚘 𝚍𝚎 𝚜𝚞 𝚌𝚘𝚗𝚜𝚞𝚕𝚝𝚊 𝚖é𝚍𝚒𝚌𝚊.");
                printf("\nb) Ver historial de horas registradas.\n﹥𝚅𝚎𝚊 𝚝𝚘𝚍𝚊𝚜 𝚕𝚊𝚜 𝚑𝚘𝚛𝚊𝚜 𝚛𝚎𝚐𝚒𝚜𝚝𝚛𝚊𝚍𝚊𝚜 𝚊𝚗𝚝𝚎𝚛𝚒𝚘𝚛𝚖𝚎𝚗𝚝𝚎.");
                printf("\nc) ⮌ Volver.\n");
                scanf("%s", &opcion_horas);

                if(opcion_horas == 'a') {
                  getchar();
                  registro_horas_medicas(usuario);
                }
                else if(opcion_horas == 'b') {
                  getchar();
                  mostrar_registro_horas(usuario);
                }
              } 
            }

            // -------------------- IMC --------------------
            if (opcion2==2){
              getchar();
              calcular_imc(usuario);
            }

            // -------------------- HORAS DE SUEÑO --------------------
            if (opcion2==3){
              getchar();
              char opcion_suenio;
              while(opcion_suenio != 'd') {
                printf("\na) Registrar horas dormidas.\n﹥𝚁𝚎𝚐𝚒𝚜𝚝𝚛𝚎 𝚜𝚞𝚜 𝚑𝚘𝚛𝚊𝚜 𝚍𝚎 𝚜𝚞𝚎ñ𝚘 𝚜𝚎𝚕𝚎𝚌𝚌𝚒𝚘𝚗𝚊𝚗𝚍𝚘 𝚞𝚗 𝚍í𝚊.");
                printf("\nb) Reiniciar semana.\n﹥𝚅𝚘𝚕𝚟𝚎𝚛 𝚊 𝚛𝚎𝚐𝚒𝚜𝚝𝚛𝚊𝚛 𝚍𝚊𝚝𝚘𝚜 𝚙𝚊𝚛𝚊 𝚞𝚗𝚊 𝚜𝚎𝚖𝚊𝚗𝚊 𝚗𝚞𝚎𝚟𝚊.");
                printf("\nc) Ver horas y calcular promedio.\n﹥𝚁𝚎𝚟𝚒𝚜𝚎 𝚕𝚊𝚜 𝚑𝚘𝚛𝚊𝚜 𝚍𝚎 𝚝𝚘𝚍𝚘𝚜 𝚕𝚘𝚜 𝚍í𝚊𝚜 𝚢 𝚟𝚎𝚊 𝚜𝚞 𝚙𝚛𝚘𝚖𝚎𝚍𝚒𝚘 𝚍𝚎 𝚜𝚞𝚎ñ𝚘.");
                printf("\nd) ⮌ Volver.\n");
                scanf("%s", &opcion_suenio);

                if (opcion_suenio == 'a') {
                  getchar();
                  registrar_horas_suenio(usuario);
                }
                else if (opcion_suenio == 'b') {
                  getchar();
                  reiniciar_semana(usuario);
                }
                else if (opcion_suenio == 'c') {
                  getchar();
                  mostrar_horas_semanales(usuario);
                }
                printf("\n═════════════════════════════════════════════════════\n");
              }
            }

            // -------------------- RUTINA EJERCICIOS --------------------
            if (opcion2==4){
              getchar();
              char opcion_ejercicios;
              while(opcion_ejercicios != 'd') {
                printf("\na) Registrar rutina de ejercicio.\n﹥𝚁𝚎𝚐𝚒𝚜𝚝𝚛𝚎 𝚎𝚕 𝚗𝚘𝚖𝚋𝚛𝚎 𝚢 𝚛𝚎𝚙𝚎𝚝𝚒𝚌𝚒𝚘𝚗𝚎𝚜 𝚍𝚎 𝚜𝚞 𝚎𝚓𝚎𝚛𝚌𝚒𝚌𝚒𝚘 𝚙𝚊𝚛𝚊 𝚞𝚗 𝚍í𝚊 𝚎𝚗 𝚎𝚜𝚙𝚎𝚌í𝚏𝚒𝚌𝚘.");
                printf("\nb) Eliminar rutina de ejercicio.\n﹥𝙰𝚌𝚝𝚞𝚊𝚕𝚒𝚌𝚎 𝚜𝚞𝚜 𝚛𝚞𝚝𝚒𝚗𝚊𝚜 𝚎𝚕𝚒𝚖𝚒𝚗𝚊𝚗𝚍𝚘 𝚊𝚕𝚐𝚞𝚗𝚊 𝚍𝚎 𝚞𝚗 𝚍í𝚊 𝚊 𝚎𝚕𝚎𝚌𝚌𝚒ó𝚗.");
                printf("\nc) Ver rutinas de ejercicio de la semana.\n﹥𝚁𝚎𝚟𝚒𝚜𝚎 𝚚𝚞𝚎 𝚎𝚓𝚎𝚛𝚌𝚒𝚌𝚒𝚘𝚜 𝚛𝚎𝚊𝚕𝚒𝚣ó 𝚘 𝚍𝚎𝚜𝚎𝚊 𝚛𝚎𝚊𝚕𝚒𝚣𝚊𝚛 𝚎𝚗 𝚕𝚊 𝚜𝚎𝚖𝚊𝚗𝚊.");
                printf("\nd) ⮌ Volver.\n");
                scanf("%s", &opcion_ejercicios);

                if (opcion_ejercicios == 'a') {
                  getchar();
                  registrar_ejercicio(usuario);
                }
                else if (opcion_ejercicios == 'b') {
                  getchar();
                  eliminar_ejercicios_semanales(usuario);
                }
                else if (opcion_ejercicios == 'c') {
                  getchar();
                  mostrar_ejercicios_semanales(usuario);
                }
                printf("\n═════════════════════════════════════════════════════\n");
              }
            }

            // -------------------- REGISTRAR MEDICAMENTO --------------------
            if (opcion2==5){
              getchar();
              char opcion_medicamento;
              while(opcion_medicamento != 'd') {
                printf("\na) Registrar toma de medicamentos.\n﹥𝚁𝚎𝚐𝚒𝚜𝚝𝚛𝚎 𝚗𝚘𝚖𝚋𝚛𝚎 𝚍𝚎 𝚖𝚎𝚍𝚒𝚌𝚊𝚖𝚎𝚗𝚝𝚘 𝚢 𝚜𝚞 𝚑𝚘𝚛𝚊𝚛𝚒𝚘 𝚍𝚎 𝚌𝚘𝚗𝚜𝚞𝚖𝚘.");
                printf("\nb) Eliminar medicamento.\n﹥𝙰𝚌𝚝𝚞𝚊𝚕𝚒𝚌𝚎 𝚜𝚞 𝚌𝚊𝚕𝚎𝚗𝚍𝚊𝚛𝚒𝚘 𝚎𝚕𝚒𝚖𝚒𝚗𝚊𝚗𝚍𝚘 𝚞𝚗 𝚖𝚎𝚍𝚒𝚌𝚊𝚖𝚎𝚗𝚝𝚘 𝚙𝚊𝚛𝚊 𝚞𝚗 𝚍í𝚊 𝚜𝚎𝚕𝚎𝚌𝚌𝚒𝚘𝚗𝚊𝚍𝚘.");
                printf("\nc) Ver medicamentos vigentes.\n﹥𝚁𝚎𝚟𝚒𝚜𝚎 𝚜𝚞 𝚌𝚊𝚕𝚎𝚗𝚍𝚊𝚛𝚒𝚘 𝚍𝚎 𝚖𝚎𝚍𝚒𝚌𝚊𝚌𝚒ó𝚗 𝚙𝚘𝚛 𝚍í𝚊 𝚢 𝚑𝚘𝚛𝚊 .");
                printf("\nd) ⮌ Volver.\n");
                scanf("%s", &opcion_medicamento);

                if (opcion_medicamento == 'a') {
                  getchar();
                  registrar_medicamento(usuario);
                }
                else if (opcion_medicamento == 'b') {
                  getchar();
                  eliminar_medicamento(usuario);
                }
                else if (opcion_medicamento == 'c') {
                  getchar();
                  mostrar_medicamento(usuario);
                }
                printf("\n═════════════════════════════════════════════════════\n");
              }
            }

            // -------------------- QUIZ --------------------
            
            if (opcion2==6){
              hacer_pregunta(usuario,preguntas,num_preguntas);
            }
            // -------------------- MOSTRAR TODO --------------------
            if (opcion2==7){
              getchar();
              mostrar_global(usuario);
            }

            // -------------------- SALIR --------------------
            if (opcion2==8){
              getchar();
              printf("\n⟳  GUARDANDO Y CERRANDO SESION......");
              
              guardar_en_archivo(usuario,clave);          
              
              printf("\n⟳  VOLVIENDO AL MENÚ PRINCIPAL......\n");
              break;
            }
            printf("\n═════════════════════════════════════════════════════\n");
            printf("\n            Hola de nuevo, %s!", usuario->nombre);
          }
        } else {
          printf("Usuario no encontrado, por favor inténtelo de nuevo.\n");
        }
    }
  }
}
