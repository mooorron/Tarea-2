#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define INITIAL_SIZE 11
#define LOAD_FACTOR_THRESHOLD 0.7

// Nodo para almacenar un valor en la lista enlazada
typedef struct ValueNode {
    int value;
    struct ValueNode *next;
} ValueNode;

// Estructura para una entrada en el HashMap
typedef struct {
    int key;
    ValueNode *values; // Lista enlazada de valores linked list
    bool occupied;
    bool deleted;
} HashMapEntry;

// Estructura del HashMap
typedef struct {
    HashMapEntry *table;
    int size;
    int count;
} HashMap;

// Funciones para gestionar el HashMap
HashMap* hashmap_create(int size);
void hashmap_free(HashMap *map);
void hashmap_insert(HashMap *map, int key, int value);
void hashmap_get(HashMap *map, int key);
void hashmap_remove(HashMap *map, int key);
void hashmap_serialize(HashMap *map, const char *filename);
void hashmap_deserialize(HashMap *map, const char *filename);
#endif // HASHMAP_H
