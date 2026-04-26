#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

int main() {
        //abrir un archivo 
        ifstream archivo("datos_pinza.txt");
        //crear un archivo de salida
        ofstream archivo_salida("resultado_pinza.txt");
        //comprobacion de que el archivo se abre correctamente
        if(!archivo){
            cout<<"error,no se pudo abrir el archivo"<<endl;
            return 1;
        }
        
        float datos[100][3];
        int ids[100];
        float galga[100];
        float fuerza_izq[100];
        float fuerza_der[100];
        int n=0; //contador de datos
        //lectura del archivo:
        
        while(n<100 && archivo>>ids[n]>>galga[n]>>fuerza_izq[n]>>fuerza_der[n]){ //leer los datos del archivo pinza.txt
            //guardamos losd datos en una matriz:
            datos[n][0]=galga[n];
            datos[n][1]=fuerza_izq[n];
            datos[n][2]=fuerza_der[n];
            n++;
        }
        archivo.close(); //cerramos el archcivo 
        
        
        //mostrar datos :
        cout<<"Datos leidos:"<<endl;
        for(int i=0;i<n;i++){
            cout<<"ID: "<<ids[i]
                <<" Galga: "<<galga[i]
                <<" Fuerza izquierda: "<<fuerza_izq[i]
                <<" Fuerza derecha: "<<fuerza_der[i]<<endl;
        }

        //calcular media de cada sensor:
        float suma_galga=0,suma_izq=0,suma_der=0;
        for(int i=0;i<n;i++){
            suma_galga+=galga[i];
            suma_izq+=fuerza_izq[i];
            suma_der+=fuerza_der[i];
        }
        float media_galga=suma_galga/n;
        float media_izq=suma_izq/n;
        float media_der=suma_der/n;

        //mostrar las medias:
        cout<<"Las medias son:"<<endl;
        cout<<"Galga:"<<media_galga<<endl;
        cout<<"Fuerza Izquierda:"<<media_izq<<endl;
        cout<<"Fuerza Derecha:"<<media_der<<endl;


        //escribir los datos en el archivo:
        archivo_salida<<"Resultados de pinza:"<<endl;
        archivo_salida<<"datos:"<<endl;
        for(int i=0;i<n;i++){
            archivo_salida<<"ID:"<<ids[i]
                          <<" Galga:"<<galga[i]
                          <<" Fuerza izquierda:"<<fuerza_izq[i]
                          <<" Fuerza derecha:"<<fuerza_der[i]<<endl;
        }
        archivo_salida<<"\nMedias:\n";
        archivo_salida<<"Galga = "<<media_galga<<endl;
        archivo_salida<<"Fuerza izquierda = "<<media_izq<<endl;
        archivo_salida<<"Fuerza derecha = "<<media_der<<endl;
        


        //evaluar la estabilidad del agarre de la pinza:
        archivo_salida<<"Clasificacion:"<<endl;//corregir
        for(int i=0;i<n;i++){
            float diferencia=abs(fuerza_izq[i]-fuerza_der[i]);
            if(diferencia>0.15){
                archivo_salida<<ids[i]<<"  inestable"<<endl;
                cout<<ids[i]<<" inestable"<<endl;
            }            else{
                archivo_salida<<ids[i]<<"  estable"<<endl;
                cout<<ids[i]<<" estable"<<endl;
            }
        }
        archivo_salida.close(); //cerramos el archivo de salida
        cout<<"Resultados escritos en resultado_pinza.txt"<<endl;
    return 0;
}
