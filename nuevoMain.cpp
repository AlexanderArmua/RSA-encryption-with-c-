#include <iostream> 
#include <windows.h> //Agregado para cambiar el color
#include <conio.h> //Para el getch...
#include <cmath> //Funciones matematicas: sqrt

/*
 * Programar un encriptador de palabras mediante el metodo RSA,
 * o dos metodos de encriptacion simples (en caso de no querer usar RSA);
 * donde dado un programa se muestre el mismo encriptado, con la opcion
 * de recuperar el mensaje original
 * */

using namespace std;

//Cambiar el color de las letras
void color(int X){ 
	SetConsoleTextAttribute(GetStdHandle (STD_OUTPUT_HANDLE),X); 
	/* 
		0 = Negro 
		1 = Azul 
		2 = Verde 
		3 = Aguamarina 
		4 = Rojo 
		5 = Purpura 
		6 = Amarillo 
		7 = Blanco 
		8 = Gris 
		9 = Azul claro 
	*/
}

//Se hace uso del algoritmo de Euclides para averiguar el MCD de dos numeros. Este metodo fue creado por el matematico griego Euclides.
//https://es.wikipedia.org/wiki/Algoritmo_de_Euclides
long mcdEuclides(long n1, long n2){
	long maximo, minimo, resto;
	
	//Identificamos el maximo y el minimo de los parametros.
	if(n1>=n2){
		maximo=n1;
		minimo=n2;
	}else{
		maximo=n2;
		minimo=n1;
	}
	
	while(minimo!=0){
		resto=maximo%minimo;
		maximo=minimo;
		minimo=resto;
	}
	
	return maximo;
}

//Retorna true si el numero que le pasamos es par.
bool esPar(long numero){
	if(numero%2==0){
		return true;
	}else{
		return false;
	}
}

//Por tentativa http://es.wikihow.com/saber-si-un-n%C3%BAmero-es-primo
bool esPrimo(long n1){
	//TODOS los numeros pares a excepcion del dos son compuestos.
	if(esPar(n1) && n1!=2){
		return false;
	}else{
		/*
		 * ceil "redondea" hacia arriba.
		 * sqrt retorna la raiz cuadrada.
		 * Se divide hasta la raiz cuadrada del numero ya que dividir por los numeros siguientes seria redundante. Ejemplo:
		 * Analizar 100:
		 * 1x100=100  -  2x50=100  -  4x25=100  -  5x20=100  
		 * 10x10=100 (10 es raiz de 100)
		 * 20x5=100  -  25x4=100  -  50x2=100  -  100x1=100
		 * Como se puede ver a partir de la raiz en adelante los factores se reinvierten y por eso se divide hasta la raiz cuadrada del numero en cuestion.
		 * */
		for( long i = 3; i < ceil( sqrt( n1 ) ); i++ ){
			//Si no es par
			if(!esPar(i)){
				//Si el resto de dividir n1 por el iterador es 0 significa que n1 es un numero compuesto.
				if(n1%i==0){
					return false;
				}
			}
		}
		return true;
	}
}

//Retorna un numero primo mayor que el parametro pasado
long damePrimo(long mayorQue){
	long numeroPrimo=0, numero=mayorQue;
	do{
		//Con cada repeticion al numero le sumo 1 para ir probando hasta encontrar el primo
		numero++;
		if(esPrimo(numero)){
			numeroPrimo=numero;
		}
	}while(numeroPrimo==0);
	 
	return numeroPrimo;
}
 
//Obtener numero aleatorio entre dos rangos definidos
long numeroAleatorioEntre(long minimo, long maximo){
	return (minimo+rand()%((maximo+1)-minimo));
}
 
//Obtener numero aleatorio mayor que otor numero
long numeroAleatorioMayorQue(long minimo){
	return (minimo+rand());
}

//Algoritmo de euclides extendido
long* algEucExtendido( long n1, long n2 ){   
	long array[3], x = 0, y = 0, d = 0, x2 = 1, x1 = 0, y2 = 0, y1 = 1, q = 0, r = 0;
	if( n2 == 0 ){
		array[0] = n1;
		array[1] = 1; 
		array[2] = 0;  
	}else{
		while(n2>0){
			q = ( n1 / n2 ); 
			r = n1 - q * n2; 
			x = x2 - q * x1; 
			y = y2 - q * y1; 
			n1 = n2; 
			n2 = r; 
			x2 = x1; 
			x1 = x; 
			y2 = y1;             
			y1 = y; 
		}
		array[0] = n1;   // mcd (n1,n2)
		array[1] = x2;   // x
		array[2] = y2;   // y
	}
	return array;
}

//Inverso de un numero en un modulo
long Inverso_Zn( long a, long n ){
	long* ptr,	array[3];
	ptr = algEucExtendido( n, a );
 
	array[0] = *ptr;
	array[1] = *( ptr + 1 );
	array[2] = *( ptr + 2 );
   
	if( array[0] != 1 ){
		return -1;
	}else{
		if( array[2] < 0 ){
			array[2] += n;
		}
		return array[2];
	}  
}

//Exponenciacion de un numero en un modulo
unsigned long long  Exponenciacion_Zn(unsigned long long  a,unsigned long long  k,unsigned long long  n){
	// convertimos "k" a binario
	unsigned long long numero = k;
 
	unsigned long long bin[300];
	unsigned long long  ind = 0;
	while( numero >= 2 ){
	bin[ind++] = numero % 2;
	numero/=2;      
	}     
	bin[ind] = numero;
	unsigned long long  tam = ind + 1;
	// for(long i=0;i<tam;i++)
	// cout<<bin[i]<<endl;     
	/////////////////////////////   
      
	unsigned long long  b = 1;
	if( k == 0 ){
		return b;
	}
   
	unsigned long long  A = a;   
	for( long i = ( tam - 1 ); i >= 0; i-- ){
		b = ( b * b ) % n;
		if( bin[i] == 1 ){
			b=(A*b)%n; 
		}
	// cout<<"b :"<<b<<endl;   
	}
return b;
}

/*
	Encripta el mensaje y retorna en un array de 3 posiciones:
	[0] = Clave Publica.
	[1] = Clave Privada.
	[2] = Mensaje encriptado.
*/
long*encriptar(long mensaje){
	long array[3], cifrado=0;

	//Generacion del par de claves
	//Paso 1: Generar dos numeros primos aleatorios GRANDES con el mismo tama�o.
	long primoUno = damePrimo( numeroAleatorioEntre( 50000, 99999 )	);
	long primoDos = damePrimo( numeroAleatorioEntre( 10000, 49999 )	);
	
	//Paso 2: Se calcula el producto entre los dos numeros primos.
	long productoPrimos = ( primoUno * primoDos );  // n
	array[0] = productoPrimos;
	
	//Paso 3: Creacion de la clave publica.
	long clavePublica = ( ( primoUno - 1 ) * ( primoDos - 1 ) ); // d   p(n) fi
	
	//Paso 4: Seleccionamos un entero aleatorio 'e' tal que mcd('e',clavePublica) = 1   y   1 < 'e' < clavePublica
	long e;
	do{
		e = numeroAleatorioEntre( 2, clavePublica );
	}while( mcdEuclides( e, clavePublica ) != 1 );
	
	long exponenteClavePrivada = Inverso_Zn(e,clavePublica); // d
	array[1]=exponenteClavePrivada;
	
	cifrado=Exponenciacion_Zn(mensaje,e,productoPrimos);
	
	array[2]=cifrado;
	
	return array;
}

/*
	Esta funcion se ejecutaria desde el lado del servidor ya que como poseo la clave privada, 
	con solo recibir el mensaje encriptado y la clave publica puedo desencriptar el mensaje.
*/
long desencriptar(long clavePublica, long clavePrivada, long mensajeCifrado){
	long decifrado = Exponenciacion_Zn(mensajeCifrado,clavePrivada,clavePublica);
	
	return decifrado;
}

int main(){
	color(8);
	cout<<"\t Encriptacion y Desencriptacion de un mensaje con RSA\n";
	cout<<"\t ----------------------------------------------------\n\n";
	cout<<"Consigna:\n";
	cout<<"---------\n";
	cout<<"\t*************************************************************************\n";
	cout<<"\t* Programar un encriptador de palabras mediante el metodo RSA,          *\n";
 	cout<<"\t* o dos metodos de encriptacion simples (en caso de no querer usar RSA).*\n";
 	cout<<"\t* donde dado un programa se muestre el mismo encriptado, con la opcion  *\n";
 	cout<<"\t* de recuperar el mensaje original                                      *\n";
 	cout<<"\t*************************************************************************\n\n";
 		
	long* clavesEncriptacion=0, clavePublica=0, clavePrivada=0, cifrado=0;
	
	long mensaje=0, respuesta=0;
	
	do{
			color(7);
			cout<<"\t\t Que desea realizar?\n";
			cout<<"\t\t -------------------\n\n";
			cout<<"\t\t [1] -> Encriptar un mensaje."<<endl;
			cout<<"\t\t [2] -> Desencriptar un mensaje."<<endl;
			cout<<"\t\t [0] -> Salir del programa."<<endl<<endl;
			cout<<"Quiero: ";
			cin>>respuesta;
			cout<<endl;
			
			if(respuesta==1){
				color(3);
				cout<<"\tUsted eligio encriptar un mensaje\n";
				cout<<"\t---------------------------------\n\n";
				
				cout<<"Ingrese el mensaje que desea encriptar: "; color(9);
				cin>>mensaje;
				color(3);
				
				clavesEncriptacion = encriptar(mensaje);
				
				clavePublica = *clavesEncriptacion;
				clavePrivada = * ( clavesEncriptacion + 1 );
				cifrado = * ( clavesEncriptacion + 2 );
				
				cout<<"\n - El mensaje "; color(9); cout<<mensaje; color(3); cout<<" cifrado es: "; color(9); cout<<cifrado<<"\n\n"; color(3);
				
				cout<<" - Clave Publica: "; color(9); cout<<clavePublica<<"\n"; color(3);
				
				cout<<" - Clave Privada: "; color(9); cout<<clavePrivada<<"\n\n";
				
			}else if(respuesta==2){
				color(5);
				cout<<"\tUsted eligio desencriptar un mensaje\n\n";
				
				cout<<"Ingrese el mensaje a decifrar: "; color(4);
				cin>>cifrado; color(5);
				cout<<"Ingrese la clave publica: "; color(4);
				cin>>clavePublica; color(5);
				cout<<"Ingrese la clave privada: "; color(4);
				cin>>clavePrivada; color(5);
				
				cout<<"\n - El mensaje "; color(4); cout<<cifrado; color(5); cout<<" decifrado es "; color(4); cout<< desencriptar(clavePublica, clavePrivada, cifrado)<<"\n\n";
				
			}
	}while(respuesta!=0);
	
	cout<<"Adios"<<endl;
	
	//Final de los programas
    getch();
    return 0;
	system("pause");

}