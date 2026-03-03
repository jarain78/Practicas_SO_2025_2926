#include <iostream>
#include <vector>

using namespace std;

int cont = 0; // Creamos un contador

/*vector<vector<double> > matriz = {
    {41, 45, 49, 43}, 
    {37, 31, 45, 43}, 
    {43, 48, 50, 51}, 
    {47, 42, 41, 45}
}; // Creamos matriz directa (4x4)
*/

vector<vector<int>> matriz(4, vector<int>(4, 0));


// Rellenamos matriz utilizando push_back
//cint >> matriz[0].push_back = matriz[0][5];
//cin >> matriz[1].push_back = matriz[1][5];
//cin >> matriz[2].push_back = matriz[2][5];
//cin >> matriz[3].push_back = matriz[3][5];

int main(){
    int elemento_1=0;
    cin >> elemento_1;

    // Guardamos los datos
    //ofstream matriz("data.txt"); // Crea o sobrescribe un archivo con los datos

    // Calculamos eficiencia
    for (int i = 0; i < 60; i++) {
        for (int j = 0; j < 100; j++) {
            cout << matriz[i][j] << endl;
            cout << "conta: " << (float)i / ((i + 1));
        }
        cout << endl;

        if ((i + j) <= 75) { // Comprobamos que cada suma de índices sea menor de 75
            cont++;
            cout << "Hay " << cont << " motores defectuosos.";
        }
    }

    return 0;
}