#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>

#define Nodo struct nodo
#define Arbol struct arbol


Nodo{
  Arbol*raiz;
  Nodo*siguiente;
  Nodo*anterior;
};


Arbol{
  int letra;
  int frecuencia ;
  Arbol*izquierda;
  Arbol*derecha;
};



void Hoffman();
void insertarOrden(Arbol*);
void lineas(FILE*,int );
void comprimir(char*);
void insertarCola(int);
void ordenar();
void asignarValor(FILE*,Arbol*,int);
int listaVacia();
int buscarASCII(int);
int binAdecimal(int); 

Arbol*desencolar();
Arbol*nuevoArbol(int,int);
Nodo*nuevoNodo(Arbol*);
Nodo*cabeza=NULL;
Nodo*cola=NULL;

int main(int argc, char *argv[]){
	
  char *Archivo= argv[1];
   	
  int len,i,j,numero_de_caracteres=0;
  
  char codigo[100],*binario=NULL,aux[100];
 
printf("%s",argv[1]);

  FILE*frase = fopen(Archivo,"rb"); // ABERTURA DEL ARCHIVO A LEER EN MODO BINARIO 
  
  if(frase==NULL){
    printf("Error en el archivo\n");
    return 0;
  }else
      printf("abierto exitosamente\n");
  

  while(!feof(frase)){
    char caracter=fgetc(frase);
    
    if(caracter==-1){
    //  printf("Error en el archivo\n");
    }
    else{
      numero_de_caracteres++;
      insertarCola(caracter);
    }
    numero_de_caracteres++;
  }

  // EN ESTE WHILE SE CREA LA LISTA DE ARBOLES 
  while(!feof(frase)){
    char caracter=fgetc(frase);
    printf("%d\n",caracter);
    if(caracter==-1)
    break;
    numero_de_caracteres++;
    insertarCola(caracter);
    printf("entra");
  }
  fclose(frase);
  
  
  ordenar();  //ORDENA LA LISTA DE MENOR A MAYOR FRECUENCIA
  
  Hoffman(); // CREA EL ARBOL MADRE
  
  
  FILE*tablaBinaria=fopen("Decode.txt","wb"); //CREA EL TXT PARA METER LA CODIFICACION DEL ARCHIVO
  
  
  asignarValor(tablaBinaria,cabeza->raiz,1 );  // LE ASIGNA LOS CEROS Y 1 A CADA NODO DEL ARBOL, SE LE PASA EL ARCHIVO DONDE VA A GUARDAR LA TABLA, EL INICIO DEL ARBOL, Y UN NUMERO CULAQUIERA 
  
  
  fclose(tablaBinaria);

  /////////////////////////////////////////////////
  
  FILE*tabla = fopen("Decode.txt","rb"); 
  

  FILE*Palabra = fopen(Archivo,"rb"); 
  
  
  if(tabla==NULL || Palabra==NULL){   
    printf("Error en el archivo Decode o Archivo \n");
    return 0;
  }
  
  

  while(!feof(Palabra)){
    char caracter=fgetc(Palabra);
    if(caracter==-1)
    break;
    for(i=0;i<numero_de_caracteres;i++){
      lineas(tabla,i+1); //LE PASA LA TABLA Y  EL NUMERO DE LETRA
      char c=fgetc(tabla);
      if(caracter==c){   //COMPRUEBA SI EL CARACTER DEL ARCHVIO ES EL MISMO DEL DE LA TABLA
        fseek(tabla,2, SEEK_CUR); //SI ES ASI SE POSICIONA EN 2 DE ESA LINEA - QUE ES DONDE SE ENCUENTRA LOS NUMEROS 
        fscanf(tabla," %s",codigo); //LEE EL STRING DE NUMEROS DE ESA LETRA Y LO GUARDA EN CODIGO
        
    len=strlen(codigo);  
    
        if(binario==NULL){
           binario=(char*)calloc((len+1),sizeof(char));     //CREA UN PUNTERO DEL TAMÑO DE LOS NUMEROS DE CADA CHAR y LO INICIALIZA EN 0

        }else{  //SI POR ALGUNA RAZON NUESTRO APUNTADOR NO ESTA INICIALIZADO A NULL, LE ASIGNAMOS NUEVA MEMORIA A LA CUAL APUNTAR
           binario=(char*)realloc(binario,(strlen(binario)+len+1)*sizeof(char));
        }
        
        strcat(binario,codigo); //CONCATENA LA CADENA DE NUMEROS AL APUNTADOR BINARIO
        goto cont;
      }
    }
    cont:
    rewind(tabla);  //REGRESA AL INICIO DEL ARCHIVO
  }
  fclose(Palabra);  //CIERRA EL ARCHIVO
  fclose(tabla); //CIERRA EL ARCHIVO
  comprimir(binario);
}






  
Arbol*nuevoArbol(int dato,int n){
  Arbol*nuevo=(Arbol*)malloc(sizeof(Arbol));
  nuevo->letra=dato;
  nuevo->frecuencia=n;
  nuevo->izquierda=NULL;
  nuevo->derecha=NULL;
  return nuevo;
}

Nodo*nuevoNodo(Arbol*dato){
  Nodo*nuevo=(Nodo*)malloc(sizeof(Nodo));
  nuevo->raiz=dato;
  nuevo->siguiente=NULL;
  nuevo->anterior=NULL;
  return nuevo;
}

/*

************************************************

*/
void insertarCola(int dato){  // CUENTA LA FRECUENCIA DE APARICION DE LOS CARACTERES Y CREA UN ARBOL PARA CADA CARACTER SI ESTE NO HA SIDO CREADO
  if(buscarASCII(dato)==1)  
    return; 
  Arbol*arbolMadre=nuevoArbol(dato,1);   //EN CASO DE NO ESXISTIR EL ARBOL DEL CARACTER CREA EL NODO QUE CONTENGA EL ARBOL Y SI ES EL PRIMER CARACTER EL INICO Y FIN DE LA LISTA ES EL MISMO
  Nodo*nuevo=nuevoNodo(arbolMadre);
  if(cabeza==NULL && cola==NULL){
    cabeza=nuevo;
    cola=nuevo;
  }else{
    cola->siguiente=nuevo;
    nuevo->anterior=cola;
    cola=nuevo;
  }
}


/*

**************************************************

*/
int buscarASCII(int dato){  //BUSCA QUE EL ARBOL NO HA SIDO CREADO 
  Nodo*aux=cabeza;
  
  while(aux!=NULL){
    if(aux->raiz->letra==dato){     //SI YA EXISTE EL ACARACTER SOLO AUMENTA LA FRECUENCIA
      aux->raiz->frecuencia++;
      return 1;
    }
    aux=aux->siguiente;       //SE APUNTA AL SIGUIENTE NODO EN CASO DE NO SER EL CARACTER 
  }
  return 0;
}

void ordenar(){ // ORDENA LA LISTA DE ARBOLES DE MENOR A MAYOR
  if(cabeza!=NULL){
    Nodo*piv=cabeza;
    Nodo*aux=cabeza->siguiente;
    while(piv!=NULL){
      while(aux!=NULL){
        if(aux->raiz->frecuencia<piv->raiz->frecuencia){
          //aux->anterior->siguiente=aux->siguiente;
          if(aux->siguiente==NULL){
              cola=aux->anterior;
              cola->siguiente=NULL;
            }else{
              aux->anterior->siguiente=aux->siguiente;
              aux->siguiente->anterior=aux->anterior;
          }
          aux->anterior=aux->siguiente=NULL;
          aux->siguiente=piv;
          if(piv==cabeza){
            cabeza=aux;
            piv->anterior=aux;
          }else{
            aux->anterior=piv->anterior;
            piv->anterior->siguiente=aux;
            piv->anterior=aux;
          }
          piv=aux;
          aux=piv->siguiente->siguiente;
        }else{
          aux=aux->siguiente;
        }
      }
      piv=piv->siguiente;
      if(piv==NULL){
        aux=NULL;
      }else{
        aux=piv->siguiente;
      }
    }
  }
}

void insertarOrden(Arbol*dato){   //METE EL ARBOL NUEVO QUE CONTIENE DOS ARBOLES A LA LISTA DE NODOS (COMBINACION DE ARBOLES)
  Nodo*nuevo=nuevoNodo(dato);
  Nodo*aux=cabeza,*ant=NULL;
  while(aux!=NULL){
    if(aux->raiz->frecuencia>=nuevo->raiz->frecuencia){
      if(aux==cabeza){
        nuevo->siguiente=cabeza;
        cabeza->anterior=nuevo;
        cabeza=nuevo;
      }else{
            ant->siguiente=nuevo;
          nuevo->anterior=ant;
          nuevo->siguiente=aux;
          aux->anterior=nuevo;
      }
      return;
    }
    ant=aux;
    aux=aux->siguiente;
  }
  if(cabeza==NULL && cola==NULL){
    cabeza=nuevo;
    cola=nuevo;
  }else{
    cola->siguiente=nuevo;
      nuevo->anterior=cola;
      cola=nuevo;
  }
}

Arbol*desencolar(){   //QUITA DE LA LISTA DE ARBOLES EL PRRIMER Y SEGUNDO NODO EL CUAL SERA UTILIZADO PARA EL ALGORITMO 
  Nodo*aux;
  if(listaVacia()){
    return NULL;
  }else{
    aux=cabeza;
    cabeza=cabeza->siguiente;
    aux->siguiente=NULL;
    if(cabeza!=NULL){
      cabeza->anterior=NULL;
    }else{
      cola=NULL;
    }
  }
  Arbol*resultado=aux->raiz;
  free(aux);
  return resultado;
}

int listaVacia(){  //ESTA FUNCION SOLO DICE SI LA LISTA ESTA VACIA
  if(cabeza==NULL && cola==NULL){
    return 1;
  }else{
    return 0;
  }
}


/*                    */




void lineas(FILE*tabla,int num){
  int conta=num-1;
  fseek(tabla, 0, SEEK_SET);  //INICIA PUNTERO DESDE EL COMIENZO DE CADA LINEA 
  prueba:     
  if (conta>0){ 
  while (fgetc(tabla) != '\n'); 
  conta--;
  goto prueba;
    }
}

void Hoffman(){
  int n;
  if(cabeza==cola)
  return;
  Arbol*arbol1=desencolar(); //TENEMOS EL PRIMER ARBOL DE LA LISTA
  Arbol*arbol2=desencolar(); //TENEMOS EL SEGUNDO ARBOL DE LA LISTA
  n=arbol1->frecuencia+arbol2->frecuencia;
  Arbol*arbolMadre=nuevoArbol(239,n); //CREA UN ARBOL QUE CONTENDRA LOS OTROS DOS ARBOLES Y SE LE ASIGNA UN CARACTER EL CUAL NO SE ENCUENTRA EN EL MENSAJE 
  arbolMadre->izquierda=arbol1;
  arbolMadre->derecha=arbol2;
  insertarOrden(arbolMadre);
  
  Hoffman();

}

void asignarValor(FILE*tablaBinaria,Arbol*raiz,int n){ //ASIGNA VALOR A CADA LETRA
    
  if(raiz!=NULL){
    asignarValor(tablaBinaria,raiz->izquierda,n*10); //SE MULTIPLICA POR 10 PARA QUE SIEMPRE LOS ARBOLES DE LA IZQUIERDA CONTENGAN UN 0
    if(raiz->letra!=239){
      if(tablaBinaria==NULL){
        perror("Error en el cargado del archivo - ARCHIVO VACIO");
        return;
      }
      fprintf(tablaBinaria,"%c %i\n",raiz->letra,n);
    }
    asignarValor(tablaBinaria,raiz->derecha,n*10+1);//SE MULTIPLICA POR 10+1 PARA QUE LOS ARBOLES DE LA DERECHA TENGAN UN 1
  }
}

void comprimir(char*binario){ 

  FILE*ASCII=fopen("Decode.txt","ab");  //ABRE LA TABLA
  FILE*encriptado = fopen("encriptado.txt","w+b"); //ABRE EL ARCHIVO ENCRIPTADO
  FILE*ArchivoBinario = fopen("bianrio.txt", "w");
  if(ASCII == NULL || encriptado == NULL || ArchivoBinario== NULL){

  	perror("Error en la abertura de algun archivo\n");
	exit(-1);
	
  }
  
  
  
  
  int len=strlen(binario); //GUARDA EL TAMAÑO DEL APUNTADOR HACIA LA CADENA DE UNOS Y CEROS
  
  printf("\n%i Bits\n",len); //IMPRIME EL TAMAÑO DEL ARREGLO QUE SERA LA CANTIDAD DE BITS
  /*
  printf("Codigo Binario: ");
  puts(binario);*/
  fputs(binario,ArchivoBinario);
  fclose(ArchivoBinario);
  int bytes=len/8,sb=len%8,num,aux,j,i;   //LA VARIABLE BYTE DIVIDE PARA SABER EL NUMERO DE BYTES, SB CONTIENE SI HAY NUMERO EXACTO DE BYTES O NO
  char byte[8],sll[7];
  
  printf("Frase Encriptado: ");
  
  /*
  PASAR EL ENCRIPTADO A UN TXT LLAMADO ENCRIPTADO
  */
  
  for(i=1;i<=bytes+1;i++){
    aux=0;
    if(i<bytes+1){
      
      for(j=8*(i-1);j<8*i;j++){  //GUARDA 8 BITS EN LA CADENA BYTE 
      byte[aux]=binario[j];
      aux++;
        }
        
        num=binAdecimal(atoi(byte));  
        fprintf(encriptado,"%c",num);
        printf("%c",num);     //IMPRIME LA CADENA EN ASCII 
        
    }else if(sb!=0){   //ES LO MISMO PERO PARA CADENAS DE BYTES NO COMPLETAS
      for(j=8*(i-1);j<(8*(i-1))+sb;j++){
        sll[aux]=binario[j];
        aux++;
      }
      num=binAdecimal(atoi(sll));
      fprintf(encriptado,"%c",num);
        fprintf(ASCII,"%c",num);
        printf("%c",num);
    }
  }
  printf("\n");
  fclose(ASCII);
}

int binAdecimal(int bin){
  int c=1,d=0,p;    //La ' d ' será el valor que nos devolvera, osea el numero en decimal para imprimirlo en codigo ascii
  while(bin>0)
  {
    p=0;
    p=c*(bin%10);
    d+=p;
    c*=2;
    bin/=10;
  }
  return d;
}
