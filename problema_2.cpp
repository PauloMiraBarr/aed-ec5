/******************************************************************************

                              Online C++ Compiler.
               Code, Compile, Run and Debug C++ program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
// compilador c++17
/*Alumnos
- Jose Daniel Huaman Rosales
- VIllegas suarez ELmer Manuel JOse
- Paulo MIranda Barriendos
  */
#include <bits/stdc++.h>
using namespace std;



using namespace std;

template <typename K, typename V>
class HashTable {
private:
    // Nodo de las entradas dentro de un bucket (chaining)
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    // Nodo de la lista de buckets
    struct Bucket {
        Entry* head;   // lista enlazada de entradas
        Bucket* next;  // siguiente bucket
        Bucket() : head(nullptr), next(nullptr) {}
    };

    Bucket* bucketsHead; // cabeza de la lista de buckets
    int capacity;        // número de buckets
    int sz;              // número de elementos (pares clave-valor)

    size_t getHash(const K& key) const {
        return hash<K>{}(key);
    }

    // Devuelve el puntero al bucket i-ésimo recorriendo la lista
    Bucket* getBucketNode(int index) const {
        Bucket* b = bucketsHead;
        for (int i = 0; i < index && b != nullptr; ++i) b = b->next;
        return b;
    }

public:
    explicit HashTable(int cap = 10) : bucketsHead(nullptr), capacity(max(1, cap)), sz(0) {
        // Construimos la lista enlazada de buckets con 'capacity' nodos
        bucketsHead = new Bucket();
        Bucket* curr = bucketsHead;
        for (int i = 1; i < capacity; ++i) {
            curr->next = new Bucket();
            curr = curr->next;
        }
    }

    ~HashTable() {
        // Liberar todas las entradas de cada bucket
        Bucket* b = bucketsHead;
        while (b) {
            Entry* e = b->head;
            while (e) {
                Entry* tmp = e;
                e = e->next;
                delete tmp;
            }
            Bucket* bt = b;
            b = b->next;
            delete bt;
        }
    }

    void insert(const K& key, const V& value) {
        int index = static_cast<int>(getHash(key) % capacity);
        Bucket* bucket = getBucketNode(index);

        // Buscar si existe para actualizar
        for (Entry* e = bucket->head; e != nullptr; e = e->next) {
            if (e->key == key) {
                e->value = value; // actualizar
                return;
            }
        }
        // Insertar al inicio del bucket
        Entry* newEntry = new Entry(key, value);
        newEntry->next = bucket->head;
        bucket->head = newEntry;
        sz++;
    }

    bool search(const K& key, V& value) const {
        int index = static_cast<int>(getHash(key) % capacity);
        Bucket* bucket = getBucketNode(index);

        for (Entry* e = bucket->head; e != nullptr; e = e->next) {
            if (e->key == key) {
                value = e->value;
                return true;
            }
        }
        return false;
    }

    bool remove(const K& key) {
        int index = static_cast<int>(getHash(key) % capacity);
        Bucket* bucket = getBucketNode(index);

        Entry* curr = bucket->head;
        Entry* prev = nullptr;
        while (curr) {
            if (curr->key == key) {
                if (prev) prev->next = curr->next;
                else bucket->head = curr->next;
                delete curr;
                sz++;
                sz--; // neto: -1 (dejo estas dos líneas para enfatizar el ajuste; puedes dejar solo --sz;)
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    void display() const {
        Bucket* b = bucketsHead;
        int i = 0;
        while (b) {
            cout << i << ": ";
            for (Entry* e = b->head; e != nullptr; e = e->next) {
                cout << "(" << e->key << "," << e->value << ") -> ";
            }
            cout << "NULL\n";
            b = b->next;
            ++i;
        }
    }

    vector<K> keys() const {
        vector<K> all;
        Bucket* b = bucketsHead;
        while (b) {
            for (Entry* e = b->head; e != nullptr; e = e->next) {
                all.push_back(e->key);
            }
            b = b->next;
        }
        return all;
    }

    int size() const { return sz; }
    int bucketCount() const { return capacity; }
};







struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    bool problema2(ListNode *head) {
    
        if(head == nullptr){
            return false;
        }
        if(head->next == nullptr){
            return false;
        }
        if(head->next == head){
            return true;
        }
        
       ListNode* temp = head;
       HashTable<ListNode*,int> mihas(256);
       
       while(temp->next != nullptr){
            int val =0;
           if(mihas.search(temp,val) == true){
               return true;
           }
           mihas.insert(temp,temp->val);
        
           
            temp = temp->next;
       }
       return false;
    }
};


// Función auxiliar para crear lista con ciclo en pos (si pos == -1 -> sin ciclo)
ListNode* createList(vector<int> vals, int pos) {
    if (vals.empty()) return nullptr;
    ListNode* head = new ListNode(vals[0]);
    ListNode* curr = head;
    ListNode* cycleNode = nullptr;

    if (pos == 0) cycleNode = head;

    for (int i = 1; i < vals.size(); i++) {
        curr->next = new ListNode(vals[i]);
        curr = curr->next;
        if (i == pos) cycleNode = curr;
    }
    if (pos != -1) {
        curr->next = cycleNode; // crear ciclo
    }
    return head;
}


int main() {
    Solution sol;

    // 1) Lista vacía
    assert(sol.problema2(nullptr) == false);

    // 2) Lista de un nodo sin ciclo
    ListNode* t1 = createList({1}, -1);
    assert(sol.problema2(t1) == false);

    // 3) Lista de un nodo con ciclo (apunta a sí mismo)
    ListNode* t2 = createList({1}, 0);
    assert(sol.problema2(t2) == true);

    // 4) Lista de dos nodos sin ciclo
    ListNode* t3 = createList({1,2}, -1);
    assert(sol.problema2(t3) == false);

    // 5) Lista de dos nodos con ciclo (tail->head)
    ListNode* t4 = createList({1,2}, 0);
    assert(sol.problema2(t4) == true);

    // 6) Lista de tres nodos con ciclo en el último -> al segundo
    ListNode* t5 = createList({1,2,3}, 1);
    assert(sol.problema2(t5) == true);

    // 7) Lista de tres nodos sin ciclo
    ListNode* t6 = createList({1,2,3}, -1);
    assert(sol.problema2(t6) == false);

    // 8) Lista larga sin ciclo
    ListNode* t7 = createList({1,2,3,4,5,6,7,8,9,10}, -1);
    assert(sol.problema2(t7) == false);

    // 9) Lista larga con ciclo al inicio
    ListNode* t8 = createList({1,2,3,4,5,6,7,8,9,10}, 0);
    assert(sol.problema2(t8) == true);

    // 10) Lista larga con ciclo en el medio
    ListNode* t9 = createList({1,2,3,4,5,6,7,8,9,10}, 4);
    assert(sol.problema2(t9) == true);

    cout << "testsodos los tests unitarios pasaron.\n";
    return 0;
}
