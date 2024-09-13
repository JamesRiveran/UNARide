#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <array>
#include <cmath>

struct Nodo {
    sf::CircleShape circulo;
    sf::Vector2f posicion;
    bool fijo;

    Nodo(const sf::Vector2f& pos, float radius, bool is_fixed)
        : circulo(radius), posicion(pos), fijo(is_fixed) {
        circulo.setPosition(pos - sf::Vector2f(radius, radius));
        circulo.setFillColor(sf::Color::Green);
    }
};

struct Calle {
    std::size_t nodo1;
    std::size_t nodo2;
    std::array<sf::Vertex, 2> line;
    bool fija;
    bool bidireccional;

    Calle(std::size_t n1, std::size_t n2, const sf::Vector2f& pos1, const sf::Vector2f& pos2, bool is_fixed, bool is_bidireccional)
        : nodo1(n1), nodo2(n2), fija(is_fixed), bidireccional(is_bidireccional) {
        line[0] = sf::Vertex(pos1, bidireccional ? sf::Color::Blue : sf::Color::Red);
        line[1] = sf::Vertex(pos2, bidireccional ? sf::Color::Blue : sf::Color::Red);
    }
};

static void guardarNodosCalles(const std::vector<Nodo>& nodos, const std::vector<Calle>& calles) {
    std::ofstream archivo("Nodes.txt");
    if (!archivo) {
        std::cerr << "Error: No se pudo crear o abrir el archivo 'Nodes.txt'." << std::endl;
        return;
    }

    archivo << "Nodos\n";
    for (const auto& nodo : nodos) {
        archivo << nodo.posicion.x << ' ' << nodo.posicion.y << '\n';
    }

    archivo << "Calles\n";
    for (const auto& calle : calles) {
        archivo << calle.nodo1 << ' ' << calle.nodo2 << ' ' << calle.bidireccional << '\n';
    }

    std::cout << "Nodos y calles guardados exitosamente en 'Nodes.txt'." << std::endl;
}

static std::pair<std::vector<Nodo>, std::vector<Calle>> cargarNodosCalles() {
    std::vector<Nodo> nodos;
    std::vector<Calle> calles;
    std::ifstream archivo("Nodes.txt");
    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo para cargar los nodos y calles." << std::endl;
        return { nodos, calles };
    }

    std::string linea;
    bool leyendoNodos = false;
    while (std::getline(archivo, linea)) {
        if (linea == "Nodos") {
            leyendoNodos = true;
        }
        else if (linea == "Calles") {
            leyendoNodos = false;
        }
        else if (leyendoNodos) {
            std::istringstream iss(linea);
            float x, y;
            if (iss >> x >> y) {
                nodos.emplace_back(sf::Vector2f(x, y), 5.f, true);
            }
        }
        else {
            std::istringstream iss(linea);
            std::size_t nodo1, nodo2;
            bool bidireccional;
            if (iss >> nodo1 >> nodo2 >> bidireccional && nodo1 < nodos.size() && nodo2 < nodos.size()) {
                calles.emplace_back(nodo1, nodo2, nodos[nodo1].posicion, nodos[nodo2].posicion, true, bidireccional);
                if (bidireccional) {
                    calles.emplace_back(nodo2, nodo1, nodos[nodo2].posicion, nodos[nodo1].posicion, true, bidireccional);
                }
            }
        }
    }

    std::cout << "Nodos y calles cargados desde 'Nodes.txt'." << std::endl;
    return { nodos, calles };
}

std::size_t encontrarNodoCercano(const std::vector<Nodo>& nodos, const sf::Vector2f& posicion, float rango = 10.f) {
    for (std::size_t i = 0; i < nodos.size(); ++i) {
        if (std::hypot(nodos[i].posicion.x - posicion.x, nodos[i].posicion.y - posicion.y) < rango) {
            return i;
        }
    }
    return nodos.size();
}

void dibujarFlecha(sf::RenderWindow& window, const sf::Vector2f& inicio, const sf::Vector2f& fin) {
    sf::Vector2f direccion = fin - inicio;
    float longitud = std::sqrt(direccion.x * direccion.x + direccion.y * direccion.y);

    sf::Vector2f unitario = direccion / longitud;

    sf::Vector2f puntoMedio = (inicio + fin) / 2.f;

    sf::VertexArray flecha(sf::Triangles, 3);
    flecha[0].position = puntoMedio;
    flecha[1].position = puntoMedio + sf::Vector2f(-unitario.y, unitario.x) * 5.f;
    flecha[2].position = puntoMedio + sf::Vector2f(unitario.y, -unitario.x) * 5.f;

    flecha[0].color = sf::Color::Yellow;
    flecha[1].color = sf::Color::Yellow;
    flecha[2].color = sf::Color::Yellow;

    window.draw(flecha);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Mapa con Grafo Interactivo");

    sf::Texture texture;
    if (!texture.loadFromFile("../map.png")) {
        std::cerr << "No se pudo cargar el mapa." << std::endl;
        return 1;
    }

    sf::Sprite sprite(texture);
    sprite.setScale(
        static_cast<float>(window.getSize().x) / texture.getSize().x,
        static_cast<float>(window.getSize().y) / texture.getSize().y
    );

    auto [nodos, calles] = cargarNodosCalles();

    bool seleccionNodo = false;
    sf::Vector2f nodoInicial;
    std::size_t nodoInicialIndice = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f clickPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                std::size_t nodoCercano = encontrarNodoCercano(nodos, clickPos);

                if (nodoCercano < nodos.size()) {
                    if (!seleccionNodo) {
                        nodoInicial = nodos[nodoCercano].posicion;
                        nodoInicialIndice = nodoCercano;
                        seleccionNodo = true;
                    }
                    else {
                        bool bidireccional = (sf::Keyboard::isKeyPressed(sf::Keyboard::B));
                        calles.emplace_back(nodoInicialIndice, nodoCercano, nodoInicial, nodos[nodoCercano].posicion, false, bidireccional);
                        seleccionNodo = false;
                    }
                }
                else {
                    nodos.emplace_back(clickPos, 5.f, false);
                }
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                guardarNodosCalles(nodos, calles);
            }
        }

        window.clear();
        window.draw(sprite);

        for (const auto& nodo : nodos) {
            window.draw(nodo.circulo);
        }

        for (const auto& calle : calles) {
            window.draw(calle.line.data(), 2, sf::Lines);
            if (!calle.bidireccional) {
                dibujarFlecha(window, calle.line[0].position, calle.line[1].position);
            }
        }

        window.display();
    }

    return 0;
}
