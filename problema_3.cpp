/* Alumnos
- Paulo Isael Miranda Barrientos
- Jose Daniel Huaman Rosales
- Elmer Jose Manuel Villegas Suarez
*/

#include <bits/stdc++.h>

// Hash Chain -----------------------------------------------------------------------
#include <functional>
#include <stdexcept>

using namespace std;

const int maxColision = 3;
const float maxFillFactor = 0.8;

template<typename TK, typename TV>
struct ChainHashNode {
    TK key;
    TV value;
    size_t hashcode; // evita calcular el hash varias veces
    ChainHashNode* next;
    ChainHashNode(const TK& k, const TV& v, size_t h)
        : key(k), value(v), hashcode(h), next(nullptr) {}
};

template<typename TK, typename TV>
class ChainHashListIterator {
private:
    using Node = ChainHashNode<TK, TV>;
    Node* ptr;

public:

    ChainHashListIterator(Node* p = nullptr) : ptr(p) {}

    // para compatibilidad con STL
    using value_type = Node;
    using reference = Node&;
    using pointer = Node*;

    reference operator*() const { return *ptr; }
    pointer   operator->() const { return ptr; }

    ChainHashListIterator& operator++() {
        if (ptr) ptr = ptr->next;
        return *this;
    }

    bool operator!=(const ChainHashListIterator& other) const {
        return ptr != other.ptr;
    }
    bool operator==(const ChainHashListIterator& other) const {
        return ptr == other.ptr;
    }

};

template<typename TK, typename TV>
class ChainHash
{
private:
    typedef ChainHashNode<TK, TV> Node;
    typedef ChainHashListIterator<TK, TV> Iterator;

    Node** array;  // array de punteros a Node
    int nsize; // total de elementos <key:value> insertados
    int capacity; // tamanio del array
    int *bucket_sizes; // guarda la cantidad de elementos en cada bucket
    int usedBuckets; // cantidad de buckets ocupados (con al menos un elemento)

public:
    ChainHash(int initialCapacity = 10){
        this->capacity = initialCapacity;
        this->array = new Node*[capacity]();
        this->bucket_sizes = new int[capacity]();
        this->nsize = 0;
        this->usedBuckets = 0;
    }

    TV get(TK key){
        size_t hashcode = getHashCode(key);
        size_t index = hashcode % capacity;

        Node* current = this->array[index];
        while(current != nullptr){
            if(current->key == key) return current->value;
            current = current->next;
        }
        throw std::out_of_range("Key no encontrado");
    }

    int size(){ return this->nsize; }

    int bucket_count(){ return this->capacity; }

    int bucket_size(int index) {
        if(index < 0 || index >= this->capacity) throw std::out_of_range("Indice de bucket invalido");
        return this->bucket_sizes[index];
    }

    // TODO: implementar los siguientes métodos
    void set(TK key, TV value){
        size_t h = getHashCode(key);
        size_t index = h%capacity;

        // actualizar si existe
        Node* cur = array[index];
        while (cur) {
            if (cur->key == key) {
                cur->value = value;
                return;
            }
            cur = cur->next;
        }

        // Insertar al frente
        Node* node = new Node(key, value, h);
        if (array[index] == nullptr) usedBuckets++;
        node->next = array[index];
        array[index] = node;

        nsize++;
        bucket_sizes[index]++;

        // condiciones de rehash
        if (bucket_sizes[index] > maxColision || fillFactor() > maxFillFactor) {
            rehashing();
        }
    }

    bool remove(TK key) {
        size_t h = getHashCode(key);
        size_t index = h % capacity;

        Node* cur = array[index];
        Node* prev = nullptr;

        while (cur) {
            if (cur->key == key) {
                if (prev) prev->next = cur->next;
                else array[index] = cur->next;

                delete cur;
                nsize--;
                bucket_sizes[index]--;
                if (array[index] == nullptr) usedBuckets--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // la llave existe en el arreglo
    bool contains(TK key) {
        size_t h = getHashCode(key);
        size_t index = h % capacity;
        Node* cur = array[index];
        while (cur) {
            if (cur->key == key) return true;
            cur = cur->next;
        }
        return false;
    }

    // Iteradores por bucket
    Iterator begin(int index) {
        if (index < 0 || index >= capacity) throw std::out_of_range("Indice de bucket invalido");
        return Iterator(array[index]);
    }

    Iterator end(int index) {
        if (index < 0 || index >= capacity) throw std::out_of_range("Indice de bucket invalido");
        return Iterator(nullptr);
    }

private:
    double fillFactor(){
        return (double)this->usedBuckets / (double)this->capacity;
    }

    size_t getHashCode(TK key){
        std::hash<TK> ptr_hash;
        return ptr_hash(key);
    }

    // mover nodo a un nuevo arreglo
    void moveNodeTo(Node* node, Node** newArray, int* newBucketSizes, int newCap, int& newUsedBuckets) {
        node->next = nullptr;
        size_t idx = node->hashcode % newCap;
        if (newArray[idx] == nullptr) newUsedBuckets++;
        node->next = newArray[idx];
        newArray[idx] = node;
        newBucketSizes[idx]++;
    }

    void rehashing() {
        int newCap = capacity * 2;
        Node** newArray = new Node*[newCap]();
        int* newBucketSizes = new int[newCap]();
        int newUsed = 0;

        // transferir todos los nodos
        for (int i = 0; i < capacity; ++i) {
            Node* cur = array[i];
            while (cur) {
                Node* next = cur->next;
                // hashcode esta en los nodos
                moveNodeTo(cur, newArray, newBucketSizes, newCap, newUsed);
                cur = next;
            }
            array[i] = nullptr;
            bucket_sizes[i] = 0;
        }

        // liberar estructuras antiguas y reemplazar
        delete[] array;
        delete[] bucket_sizes;

        array = newArray;
        bucket_sizes = newBucketSizes;
        capacity = newCap;
        usedBuckets = newUsed;
    }

public:
    ~ChainHash() {
        // liberar todos los nodos
        for (int i = 0; i < capacity; ++i) {
            Node* cur = array[i];
            while (cur) {
                Node* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            array[i] = nullptr;
        }
        delete[] array;
        delete[] bucket_sizes;
        array = nullptr;
        bucket_sizes = nullptr;
        nsize = 0;
        capacity = 0;
        usedBuckets = 0;
    }
};
// -------------------------------------------------------------------------------------------


















class Solution {
private:
    
public:
    bool problema3(vector<vector<char>>& board) {
        
        // Hash tables necesarias
        ChainHash<char, bool> columnas[10];
        ChainHash<char, bool> filas[10];
        ChainHash<char, bool> cuadrantes[10];
        
        for(int i = 0; i < 9; ++i) {
            for(int j = 0; j < 9; ++j) {
                // comprobar que no es un puntito
                if(board[i][j] == '.') continue;
                
                // comprobar en la fila
                if(filas[i].contains(board[i][j])) {
                    // cout << i << " - " << j << "  : " << board[i][j] << "\n";
                    return false;
                }
                filas[i].set(board[i][j], true);
                
                // comprobar en la columnas
                if(columnas[j].contains(board[i][j])) {
                    // cout << i << " - " << j << "  : " << board[i][j] << "\n";
                    return false;
                }
                columnas[j].set(board[i][j], true);
                
                // comprobar en el cuadrante
                int nro_cuadrante = (i / 3) * 3 + (j / 3);
                if(cuadrantes[nro_cuadrante].contains(board[i][j])) {
                    // cout << i << " - " << j << "  : " << board[i][j] << "\n";
                    return false;
                }
                cuadrantes[nro_cuadrante].set(board[i][j], true);
                
            }
        }
        
        return true;
    }
};

































// Helpers
vector<vector<char>> makeBoard(const vector<string>& rs) {
    vector<vector<char>> b(9, vector<char>(9,'.'));
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            b[i][j] = rs[i][j];
    return b;
}
vector<vector<char>> cloneBoard(const vector<vector<char>>& b) { return b; }

int main() {
    Solution sol;

    // Base: tablero válido clásico (ejemplo conocido)
    vector<vector<char>> base = makeBoard({
        "53..7....",
        "6..195...",
        ".98....6.",
        "8...6...3",
        "4..8.3..1",
        "7...2...6",
        ".6....28.",
        "...419..5",
        "....8..79"
    });

    // 1) Válido (base)
    {
        auto b = cloneBoard(base);
        assert(sol.problema3(b) == true);
    }

    // 2) Duplicado en FILA (poner otro '5' en fila 0)
    {
        auto b = cloneBoard(base);
        b[0][2] = '5'; // fila 0 ya tiene '5' en [0][0]
        assert(sol.problema3(b) == false);
    }

    // 3) Duplicado en COLUMNA (col 0: poner otro '5' en fila 1)
    {
        auto b = cloneBoard(base);
        b[1][0] = '5'; // col 0 ya tiene '5' en [0][0]
        assert(sol.problema3(b) == false);
    }

    // 4) Duplicado en CAJA 3x3 (caja superior izquierda)
    {
        auto b = cloneBoard(base);
        b[1][1] = '5'; // misma caja que [0][0]
        assert(sol.problema3(b) == false);
    }

    // 5) Tablero vacío (todo '.'): válido
    {
        auto b = makeBoard({
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            "........."
        });
        assert(sol.problema3(b) == true);
    }

    // 6) Un solo número colocado: válido
    {
        auto b = makeBoard({
            ".........",
            ".........",
            ".........",
            ".........",
            "....7....",
            ".........",
            ".........",
            ".........",
            "........."
        });
        assert(sol.problema3(b) == true);
    }

    // 7) Tablero completo válido (solución correcta)
    {
        auto b = makeBoard({
            "534678912",
            "672195348",
            "198342567",
            "859761423",
            "426853791",
            "713924856",
            "961537284",
            "287419635",
            "345286179"
        });
        assert(sol.problema3(b) == true);
    }

    // 8) Duplicado en caja 3x3 (caso construido desde vacío)
    {
        auto b = makeBoard({
            "9........",
            ".9.......",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            "........."
        }); // dos '9' en la caja (0,0)
        assert(sol.problema3(b) == false);
    }

    // 9) Violación tardía (duplicado aparece en la última celda)
    {
        auto b = makeBoard({
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            ".........",
            "........1"
        });
        b[0][8] = '1'; // misma columna 8 -> duplicado en col al final
        assert(sol.problema3(b) == false);
    }

    // 10) Parcial denso pero válido (varios números sin conflictos)
    {
        auto b = makeBoard({
            "53. .7. ..",
            "6.. 195 ..",
            ".98 ... .6",
            "8.. .6. .3",
            "4.. 8.3 ..",
            "7.. .2. .6",
            ".6. ... 28",
            ".. .419 .5",
            "... .8. .79"
        });
        // El anterior contiene espacios; limpiemos a un formato correcto:
        auto clean = makeBoard({
            "53..7....",
            "6..195...",
            ".98....6.",
            "8.. .6. .3", // para evitar dudas, reusamos base con ligeras variaciones válidas
            "4..8.3..1",
            "7.. .2. .6",
            ".6....28.",
            "...419..5",
            "... .8. .79"
        }); // mejor: usemos una versión válida sin espacios
        clean = makeBoard({
            "53..7....",
            "6..1.5...",
            ".98....6.",
            "8...6...3",
            "4..8.3..1",
            "7...2...6",
            ".6....28.",
            "...419..5",
            "....8..79"
        });
        assert(sol.problema3(clean) == true);
    }

    cout << "Todos los tests unitarios pasaron.\n";
    return 0;
}
