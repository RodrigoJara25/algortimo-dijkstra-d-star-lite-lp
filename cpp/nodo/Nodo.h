class Nodo {
private:
    int x;                   // Coordenada X
    int y;                   // Coordenada Y
    float distancia_origen;  // Distancia desde el nodo origen
    Nodo* nodo_anterior;     // Puntero al nodo anterior en la ruta

public:
    // Constructor
    Nodo(int x, int y);

    // getters
    int getX() const;
    int getY() const;
    float getDistancia() const;
    Nodo* getNodoAnterior() const;

    // setters
    void setDistancia(float distancia);
    void setNodoAnterior(Nodo* anterior);

    // Metodos
    void calcularVecinos(); 
};
