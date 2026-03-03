
#ifndef CLIENTROBOTUNITY_H //evita errores, solo se procesa 1 vez
#define CLIENTROBOTUNITY_H

#include <iostream>
#include <fstream>
#include <curl/curl.h>  // Libreria cURL para solicitudes HTTP
#include <nlohmann/json.hpp>  // Para trabajar con JSON
#include <string>

using namespace std;
using json = nlohmann::json;

class clientrobotunity {  // declara una clase
    public:  //crea el objeto y se conecta con el servidor UNITY
        clientrobotunity(const string& server_url);

        void send_points();   // enviamos los puntos al servidor Unity
        void get_image();     // obtenemos la imagen 
        void get_lidar();     // obtenemos los datos del sensor LIDAR

    private: // guarda la base del servidor de UNITY
        string server_url;
};

#endif
