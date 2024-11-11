#include <iostream>
#include <tuple>
#include <map>

class Nodo {
private:
    std::tuple<int, int> coordenadas;  // Tupla de la coordenada
    float distancia;  // Distancia desde el nodo origen
    Nodo* predecesor;     // Puntero al nodo anterior en la ruta
    std::map<std::tuple<int,int>, int> adyacentes; // Diccionario sobre sus nodos adyacentes

public:
    // Constructor
    Nodo(std:: tuple<int, int> coordenadas);

    // getters
    float getDistancia() const;
    Nodo* getPredecesor() const;
    std::tuple<int,int> getCoordenadas() const;
    std::map<std::tuple<int,int>, int> getAdyacentes() const;
    // setters
    void setDistancia(float distancia);
    void setPredecesor(Nodo* predecesor);
    void setCoordenadas(std::tuple<int, int> coordenadas);
    void setAdyacentes(std::map<std::tuple<int,int>, int> adyacentes);

    // Methods
    void configurarAdyacente(std::tuple<int, int> coordenadas, float peso);

};
