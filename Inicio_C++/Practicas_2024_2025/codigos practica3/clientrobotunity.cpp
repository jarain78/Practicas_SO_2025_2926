#include "clientrobotunity.h"

using namespace std;


// Callback para escribir los datos recibidos en el archivo de salida
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total_size = size * nmemb;
    
    // Si estamos guardando la imagen, userp es un puntero a un archivo de salida
    ofstream* output = static_cast<ofstream*>(userp);
    output->write(static_cast<char*>(contents), total_size);  // Guardamos los datos en el archivo
    return total_size;
}

// Función para obtener y guardar la imagen
void get_image(const string& server_url) {
    CURL* curl;
    CURLcode res;
    ofstream output("captura.jpg", ios::binary);  // Crear un archivo de imagen en modo binario
    
    if (!output) {
        cerr << "Error al abrir el archivo para guardar la imagen." << endl;
        return;
    }

    string image_url = server_url + "/camera";  // URL de la imagen

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, image_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);  // Configurar el callback
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &output);  // Pasar el archivo de salida al callback
        res = curl_easy_perform(curl);  // Ejecutar la solicitud
        if (res != CURLE_OK) {
            cerr << "Error al obtener la imagen: " << curl_easy_strerror(res) << endl;
        } else {
            cout << "Imagen guardada como 'captura.jpg'." << endl;
        }
        curl_easy_cleanup(curl);
    } else {
        cerr << "Error al inicializar cURL." << endl;
    }

    output.close();  // Cerrar el archivo
}

// Función para obtener y guardar los datos (posición y LiDAR)
void get_data(const string& server_url) {
    CURL* curl;
    CURLcode res;
    string readBuffer;

    // Obtener la posición
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, (server_url + "/position").c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);  // Escribir los datos en el buffer
        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            json pos_data = json::parse(readBuffer);  // Parsear los datos JSON
            ofstream pos_file("posicion.json");  // Guardar la posición en un archivo JSON
            pos_file << pos_data.dump(4);  // Escribir con formato legible (4 espacios)
            pos_file.close();
            cout << "Datos de posición guardados como 'posicion.json'." << endl;
        } else {
            cerr << "Error al obtener la posición: " << curl_easy_strerror(res) << endl;
        }

        // Obtener los datos LiDAR
        readBuffer.clear();  // Limpiar el buffer
        curl_easy_setopt(curl, CURLOPT_URL, (server_url + "/lidar").c_str());
        res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            json lidar_data = json::parse(readBuffer);  // Parsear los datos LiDAR
            ofstream lidar_file("lidar.json");  // Guardar los datos LiDAR en un archivo JSON
            lidar_file << lidar_data.dump(4);  // Escribir con formato legible (4 espacios)
            lidar_file.close();
            cout << "Datos LiDAR guardados como 'lidar.json'." << endl;
        } else {
            cerr << "Error al obtener los datos LiDAR: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
    } else {
        cerr << "Error al inicializar cURL." << endl;
    }
}
