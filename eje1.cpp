#include <bits/stdc++.h>
using namespace std;

// Implementación básica de Tabla Hash con "chaining"
template <typename K, typename V>
class HashTable {
private:
    // Cada bucket es una lista enlazada de pares clave-valor
    vector<list<pair<K,V>>> table;
    int capacity;
    int size;

    // Función hash simple usando std::hash
    int getIndex(const K &key) {
        return hash<K>{}(key) % capacity;
    }

public:
    HashTable(int cap) {
        capacity = cap;
        size = 0;
        table.resize(capacity);
    }

    // Insertar o actualizar un valor
    void insert(const K &key, const V &value) {
        int index = getIndex(key);
        for (auto &kv : table[index]) {
            if (kv.first == key) {
                kv.second = value; // actualiza si ya existe
                return;
            }
        }
        table[index].push_back({key, value});
        size++;
    }

    // Buscar valor por clave
    bool search(const K &key, V &value) {
        int index = getIndex(key);
        for (auto &kv : table[index]) {
            if (kv.first == key) {
                value = kv.second;
                return true;
            }
        }
        return false; // no encontrado
    }

    // Eliminar un valor
    bool remove(const K &key) {
        int index = getIndex(key);
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (it->first == key) {
                table[index].erase(it);
                size--;
                return true;
            }
        }
        return false;
    }

    // Mostrar contenido de la tabla
    void display() {
        for (int i = 0; i < capacity; i++) {
            cout << i << ": ";
            for (auto &kv : table[i]) {
                cout << "(" << kv.first << "," << kv.second << ") -> ";
            }
            cout << "NULL\n";
        }
    }
};

class Problema1 {
public:
    int problema1(string s) {
        HashTable<char,int> v(16);
        v.insert('I',1); v.insert('V',5); v.insert('X',10);
        v.insert('L',50); v.insert('C',100); v.insert('D',500); v.insert('M',1000);

        int n = s.size(), ans = 0;
        for (int i = 0; i < n; ++i) {
            int cur, next = 0;
            v.search(s[i], cur);
            if (i+1 < n){
                v.search(s[i+1], next);
            }
            ans += (cur < next) ? -cur : cur;
        }
        return ans;
    }
};

int main() {
    Problema1 sol;

    // 1) Un solo símbolo
    assert(sol.problema1("I") == 1);

    // 2) Repetición simple
    assert(sol.problema1("III") == 3);

    // 3) Par sustractivo IV
    assert(sol.problema1("IV") == 4);

    // 4) Par sustractivo IX
    assert(sol.problema1("IX") == 9);

    // 5) Mixto con varios símbolos
    assert(sol.problema1("LVIII") == 58);      // 50 + 5 + 3

    // 6) Caso clásico
    assert(sol.problema1("MCMXCIV") == 1994);  // 1000 + 900 + 90 + 4

    // 7) XLIX = 49
    assert(sol.problema1("XLIX") == 49);

    // 8) XCIV = 94
    assert(sol.problema1("XCIV") == 94);

    // 9) CDXLIV = 444
    assert(sol.problema1("CDXLIV") == 444);

    // 10) MMMDCCCLXXXVIII = 3888
    assert(sol.problema1("MMMDCCCLXXXVIII") == 3888);

    cout << "Todos los tests pasaron.";
    return 0;
}