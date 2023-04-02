// Douglas Diaz R.

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <sstream>


#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ENTER 13

using namespace std;

COORD coord = {0,0};

void llenarBuscaminas(char *matriz[], int tam, int minas);
void buscaminas(char *matriz[], int tam, int minas, char *nombre);
char minasCercanas(char *matriz[], int tam);
void gotoxy(int x, int y);
void pintarTablero8x8(char *matriz[], int tam);
void pintarTablero16x16(char *matriz[], int tam);
void AltEnter();
void quitarCursor();
void leerTecla(char *matriz[], int tam, int maxX, int maxY, char *nombre);
void imprimirPuntaje(int puntaje);
void gameOver(char *nombre, int puntaje);
bool winner(char *matriz[], int tam);
void winLetreroRegistro(char *nombre, int puntaje);
void leerArchivo();


int main(){
	int m1 = 8, m2 = 16, opcion = 0;
	char nombre[30];
	char **matriz = new char*[m1];
	for(int i=0; i < m1; i++) {
		matriz[i] = new char[m1];
	}	
	quitarCursor();
	AltEnter();
	
	cout << "NOMBRE: "; cin.getline(nombre, 30);
	system("cls");
	do{
		cout << "--------BUSCAMINAS----------" << endl << endl;
		cout << "1. Nivel principiante." << endl;
		cout << "2. Nivel avanzado." << endl;
		cout << "3. Informe historico de puntos" << endl;
		cout << "4. Salir." << endl << endl;
		cout << "Ingrese una opcion: ";
		cin >> opcion;
		
		switch(opcion) {
			case 1:
				system("cls");
				buscaminas(matriz, m1, 10, nombre);
				cin.ignore();
				system("pause > nul");
			break;
			case 2:
				for(int i = 0; i < m1; i++){
					delete[] matriz[i];
				}
				delete[] matriz;
				
				matriz = new char*[m2];
				for(int i = 0; i < m2; i++){
					matriz[i] = new char[m2];
				}
				
				buscaminas(matriz, m2, 40, nombre);
			break;
			case 3:
				system("cls");
				cout << "INFORME HISTORICO DE PUNTOS" << endl;
				leerArchivo(); 
			break;
			case 4:
				cout << "Salida . . ."; // esto es provicional, posteriormente será el boton de salida
			break;
			default:
				cout << "Ingrese una opcion valida.";
			break;
		}
	}while(opcion != 4);
	
	getche();
	return 0;
}

 void gotoxy(int x,int y){  
      HANDLE hcon;  
      hcon = GetStdHandle(STD_OUTPUT_HANDLE);  
      COORD dwPos;  
      dwPos.X = x;  
      dwPos.Y= y;  
      SetConsoleCursorPosition(hcon,dwPos);  
 }
 
 void quitarCursor() {
    CONSOLE_CURSOR_INFO cci = {100, FALSE};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
}

void llenarBuscaminas(char *matriz[], int tam, int minas){

    int qMinas = 0;

    srand(time(NULL));
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(qMinas < minas && rand() % 5 == 0){
                *(*(matriz+i)+j) = 'M';
                qMinas++;
            } else if (*(*(matriz+i)+j) != 'M') {
                *(*(matriz+i)+j) = ' ';
            }
        }
    }

    // Colocar minas adicionales en posiciones aleatorias si no se alcanzó el total
    while(qMinas < minas){
        int i = rand() % tam;
        int j = rand() % tam;
        if(*(*(matriz+i)+j) == ' '){
            *(*(matriz+i)+j) = 'M';
            qMinas++;
        }
    }

    // Contar minas adyacentes para cada posición
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(*(*(matriz+i)+j) != 'M'){
                int minasAdyacentes = 0;
                for(int x = max(0, i-1); x <= min(tam-1, i+1); x++){
                    for(int y = max(0, j-1); y <= min(tam-1, j+1); y++){
                        if(*(*(matriz+x)+y) == 'M'){
                            minasAdyacentes++;
                        }
                    }
                }
                if(minasAdyacentes == 0){
                    *(*(matriz+i)+j) = '1';
                } else {
                    *(*(matriz+i)+j) = minasAdyacentes + '0';
                }
            }
        }
    }
}

void buscaminas(char *matriz[], int tam, int minas, char *nombre) {
	
	llenarBuscaminas(matriz, tam, minas);
	if(tam == 8){  
		pintarTablero8x8(matriz, tam);
		leerTecla(matriz, tam , 65, 18, nombre);
	}else if(tam == 16) {
		pintarTablero16x16(matriz, tam);
		leerTecla(matriz, tam, 97, 34, nombre);
	}
	
}

char minasCercanas(char *matriz[], int tam){
	
	char retorno;
	
    for(int i = 0; i < tam; i++){
        for(int j = 0; j < tam; j++){
            if(*(*(matriz+i)+j) != 'M'){
                int contadorMinas = 0;
                for(int x = i-1; x <= i+1; x++){
                    for(int y = j-1; y <= j+1; y++){
                        if(x >= 0 && x < tam && y >= 0 && y < tam && matriz[x][y] == 'M'){
                            contadorMinas++;
                        }
                    }
                }
                if(contadorMinas > 0){
                    *(*(matriz+i)+j) = contadorMinas + '0';
                }else{
                	matriz[i][j] = '1';
				}
            }
        }
    }
    
    return retorno;
}

void pintarTablero8x8(char *matriz[], int tam){
	
	// Esquina superior izquierda
	gotoxy(35, 3);
	cout << char(201)<< endl;
	
	// Lineas horizontales
	for(int i = 36; i < 68; i++) {
		for(int j = 3; j < 20; j += 2){
			gotoxy(i, j);
			cout << char(205);
		}
	}
	//Esquina superior derecha
	gotoxy(67, 3);
	cout << char(187);
	
	//Lineas verticales
	for(int i = 4; i < 19; i++){
		for(int j = 35; j < 68; j += 4){
			gotoxy(j, i);
			cout << char(186);
		}
	}
	
	// Esquina inferior izquierda
	gotoxy(35, 19);
	cout << char(200);
	
	// Esquina inferior derecha
	gotoxy(67, 19);
	cout << char(188);
	
	
	// Cruces en el centro
	for(int i = 39; i < 64; i += 4){
		for(int j = 5; j < 18; j += 2){
			gotoxy(i, j);
			cout << char(206);
			
			gotoxy(i , 3);
			cout << char(203);
			
			gotoxy(i, 19);
			cout << char(202);	
		}
	}
	
	// Cruces en borde izquiedo-derecho
	for(int i = 5; i < 19; i+=2){
		gotoxy(35, i);
		cout << char(204);
			
		gotoxy(67, i);
		cout << char(185);
	}
	
		
	// Mostrar datos dentro de dibujo
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			gotoxy(37+4*j , 4+2*i);
			if(matriz[i][j] == 'M'){
				cout << matriz[i][j];
			}else{
				cout << '*';
			}
		}
	}   

	
	cout << endl;
}

void pintarTablero16x16(char *matriz[], int tam){
	
	// Esquina superior izquierda
	gotoxy(35, 3);
	cout << char(201)<< endl;
	
	// Lineas horizontales
	for(int i = 36; i < 99; i++) {
		for(int j = 3; j < 37; j += 2){
			gotoxy(i, j);
			cout << char(205);
		}
	}
	//Esquina superior derecha
	gotoxy(99, 3);
	cout << char(187);
	
	//Lineas verticales
	for(int i = 4; i < 36; i++){
		for(int j = 35; j < 100; j += 4){
			gotoxy(j, i);
			cout << char(186);
		}
	}
	
	// Esquina inferior izquierda
	gotoxy(35, 35);
	cout << char(200);
	
	// Esquina inferior derecha
	gotoxy(99, 35);
	cout << char(188);
	
	
	// Cruces en el centro
	for(int i = 39; i < 99; i += 4){
		for(int j = 5; j < 35; j += 2){
			gotoxy(i, j);
			cout << char(206);
			
			gotoxy(i , 3);
			cout << char(203);
			
			gotoxy(i, 35);
			cout << char(202);	
		}
	}
	
	// Cruces en borde izquiedo-derecho
	for(int i = 5; i < 35; i+=2){
		gotoxy(35, i);
		cout << char(204);
			
		gotoxy(99, i);
		cout << char(185);
	}
		
	// Mostrar datos dentro de dibujo
	for(int i = 0; i < tam; i++){
		for(int j = 0; j < tam; j++){
			gotoxy(37+4*j , 4+2*i);
			if(matriz[i][j] == 'M'){
				cout << matriz[i][j];
			}else{
				cout << '*';
			}
		}
	}   
	
	       
	cout << endl;
}

void AltEnter() {
    keybd_event(VK_MENU,
                0x38,
                0,
                0);
    keybd_event(VK_RETURN,
                0x1c,
                0,
                0);
    keybd_event(VK_RETURN,
                0x1c,
                KEYEVENTF_KEYUP,
                0);
    keybd_event(VK_MENU,
                0x38,
                KEYEVENTF_KEYUP,
                0);
    return;
} 

void leerTecla(char *matriz[], int tam, int maxX, int maxY, char *nombre) {
	bool gameO = false, win = false;
	int puntaje = 0, x = 37, y = 4, i = 0, j = 0;
	char movimiento = ' ';
	
	gotoxy(105, 10);
	cout << "Nombre: " << nombre;
	gotoxy(105,12);
	cout << "Puntaje: " << puntaje;
		
	gotoxy(x,y);	
	while(movimiento != 27 && !gameO && !win){
		movimiento = getch();
		switch(movimiento){
			case ARRIBA:
				if(y > 4){
					gotoxy(x, y-2);
					y -= 2;
					if(i > 0){
						i -= 1;
					}
				}
			break;
			case IZQUIERDA:
				if(x > 37){
					gotoxy(x-4,y);
					x -= 4;
					if(j > 0){
						j--;
					}
				}
			break;
			case DERECHA:
				if(x < maxX){
					gotoxy(x+4, y);
					x += 4;
					if( j < tam) {
						j++;
					}
				}
			break;
			case ABAJO:
				if(y < maxY){
					gotoxy(x, y+2);
					y += 2;
					if(i < tam) {
						i++;
					}
				}
			break;
			case ENTER:
				if(matriz[i][j] == 'M'){
					gameO = true;
					gameOver(nombre, puntaje);
				}else if(matriz[i][j] != 'M'){
					if(winner(matriz, tam)){
						win = true;	
						winLetreroRegistro(nombre, puntaje);				
					}
					cout << matriz[i][j];
					puntaje += matriz[i][j] - '0';
					gotoxy(105,12);
					cout << "Puntaje: " << puntaje;
					matriz[i][j] = 'D';
				}		
		}		
		
	}
	
}

bool winner(char *matriz[], int tam){
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            if (matriz[i][j] != 'M' && matriz[i][j] != 'D') {
                // Si la casilla no es una mina y no está descubierta, retorna false
                return false;
            }
        }
    }
    // Si todas las casillas son 'D' o 'M', retorna true
    return true;
}

void gameOver(char *nombre, int puntaje){
	system("cls");
	fstream archivo;
	ostringstream ss;
	
	ss << nombre << " " << puntaje << '\n';
	
	string cadena = ss.str();
	
	archivo.open("informe.bin", ios::binary | ios::app);
	archivo.write(cadena.c_str(), cadena.size());
	archivo.close();	
	
	for(int i = 50; i < 83; i++){
		gotoxy(i,12);
		cout << char(205);
		
		gotoxy(i,20);
		cout << char(205);
	}
	
	for(int i = 12; i < 20; i++){
		gotoxy(50, i);
		cout << char(186);
		
		gotoxy(83, i);
		cout << char(186);
	}
	
	//Esquinas letrero
	gotoxy(50, 12);
	cout << char(201);
	gotoxy(83, 12);
	cout << char(187);
	
	gotoxy(50, 20);
	cout << char(200);
	gotoxy(83, 20);
	cout << char(188);
	
	gotoxy(62, 15);
	cout << "GAME OVER";
	gotoxy(53, 17);
	cout <<"Jugador: " <<nombre;
	gotoxy(53, 18);
	cout <<"Puntaje: " <<puntaje;
}

void winLetreroRegistro(char *nombre, int puntaje){
	system("cls");
	fstream archivo;
	ostringstream ss;
	
	ss << nombre << " " << puntaje << '\n';
	
	string cadena = ss.str();
	
	archivo.open("informe.bin", ios::binary | ios::app);
	archivo.write(cadena.c_str(), cadena.size());
	archivo.close();	
	
	for(int i = 50; i < 83; i++){
		gotoxy(i,12);
		cout << char(205);
		
		gotoxy(i,20);
		cout << char(205);
	}
	
	for(int i = 12; i < 20; i++){
		gotoxy(50, i);
		cout << char(186);
		
		gotoxy(83, i);
		cout << char(186);
	}
	
	//Esquinas letrero
	gotoxy(50, 12);
	cout << char(201);
	gotoxy(83, 12);
	cout << char(187);
	
	gotoxy(50, 20);
	cout << char(200);
	gotoxy(83, 20);
	cout << char(188);
	
	gotoxy(62, 15);
	cout << "WINNER!";
	gotoxy(53, 17);
	cout <<"Jugador: " <<nombre;
	gotoxy(53, 18);
	cout <<"Puntaje: " <<puntaje;	
}

void leerArchivo() {
    string linea;
    ifstream archivo("informe.bin", ios::binary);
    if (!archivo) {
        cout << "Error al abrir el archivo." << endl;
    }
    else {
        while (getline(archivo, linea)) {
            cout << linea << endl;
        }
        archivo.close();
    }
}









