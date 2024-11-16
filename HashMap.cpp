* Universidad de La Frontera
 * Ingeniería Civil Electrónica/Telemática
 *
 * IIE-344 ESTRUCTURA DE DATOS Y ALGORITMOS
 *
 * Tarea 2
 *
 * Autor 1: Carla Anabalon (c.anabalon03@ufromail.cl)
 * Autor 2: Genesis Schaff (g.schaff01@ufromail.cl)
 *
 * Fecha: 14/11/24
 *
 * Descripción general del programa: Implementación de un HashMap que soporta múltiples valores
 * por clave utilizando listas enlazadas, con operaciones de inserción, búsqueda, eliminación,
 * rehashing y serialización.
 */

#include "hashmap.h"

// Función hash simple (módulo)
/*
 * Nombre de la función: hash.
 * Tipo de función: int.
 * Parámetros: “key” de tipo int, “size” de tipo int.
 * Dato de retorno: int, el índice hash calculado.
 * Descripción de la función: Calcula un índice hash para una clave específica usando la operación módulo.
 */
int hash(int key, int size) {
    return key % size;
}

// Crear un nuevo HashMap
/*
 * Nombre de la función: hashmap_create.
 * Tipo de función: HashMap*.
 * Parámetros: “size” de tipo int.
 * Dato de retorno: Un puntero a la estructura HashMap creada.
 * Descripción de la función: Inicializa un HashMap con una tabla de tamaño especificado y
 * marca todas las entradas como vacías.
 */
HashMap* hashmap_create(int size) {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->size = size;
    map->count = 0;
    map->table = (HashMapEntry *)malloc(sizeof(HashMapEntry) * size);

    for (int i = 0; i < size; i++) {
        map->table[i].occupied = false;
        map->table[i].deleted = false;
        map->table[i].values = NULL;
    }

    return map;
}

// Liberar memoria del HashMap
/*
 * Nombre de la función: hashmap_free.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*.
 * Dato de retorno: ninguno.
 * Descripción de la función: Libera toda la memoria asociada al HashMap, incluidas las listas enlazadas.
 */
void hashmap_free(HashMap *map) {
    for (int i = 0; i < map->size; i++) {
        if (map->table[i].occupied && !map->table[i].deleted) {
            ValueNode *current = map->table[i].values;
            while (current != NULL) {
                ValueNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
    }
    free(map->table);
    free(map);
}

// Reajustar el tamaño del HashMap (rehash)
/*
 * Nombre de la función: hashmap_rehash.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*.
 * Dato de retorno: ninguno.
 * Descripción de la función: Redimensiona la tabla al doble de su tamaño actual, reinserta
 * todas las claves y copia las listas enlazadas.
 */
void hashmap_rehash(HashMap *map) {
    int old_size = map->size;
    HashMapEntry *old_table = map->table;

    map->size *= 2;
    map->table = (HashMapEntry *)malloc(sizeof(HashMapEntry) * map->size);

    for (int i = 0; i < map->size; i++) {
        map->table[i].occupied = false;
        map->table[i].deleted = false;
        map->table[i].values = NULL;
    }

    for (int i = 0; i < old_size; i++) {
        if (old_table[i].occupied && !old_table[i].deleted) {
            ValueNode *current = old_table[i].values;
            while (current != NULL) {
                hashmap_insert(map, old_table[i].key, current->value);
                current = current->next;
            }
        }
    }

    free(old_table);
}

// Insertar un par clave-valor en el HashMap
/*
 * Nombre de la función: hashmap_insert.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*, “key” de tipo int, “value” de tipo int.
 * Dato de retorno: ninguno.
 * Descripción de la función: Inserta un par clave-valor en el HashMap. Si la clave ya existe,
 * agrega el valor a la lista enlazada de valores asociados.
 */
void hashmap_insert(HashMap *map, int key, int value) {
    if ((float)map->count / map->size >= LOAD_FACTOR_THRESHOLD) {
        hashmap_rehash(map);
    }

    int index = hash(key, map->size);
    int original_index = index;

    while (map->table[index].occupied && !map->table[index].deleted && map->table[index].key != key) {
        index = (index + 1) % map->size;
        if (index == original_index) {
            fprintf(stderr, "HashMap esta lleno\n");
            return;
        }
    }

    if (!map->table[index].occupied || map->table[index].deleted) {
        map->table[index].key = key;
        map->table[index].values = NULL;
        map->table[index].occupied = true;
        map->table[index].deleted = false;
        map->count++;
    }

    ValueNode *new_node = (ValueNode *)malloc(sizeof(ValueNode));
    new_node->value = value;
    new_node->next = map->table[index].values;
    map->table[index].values = new_node;
}

// Obtener y mostrar los valores de una clave
/*
 * Nombre de la función: hashmap_get.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*, “key” de tipo int.
 * Dato de retorno: ninguno.
 * Descripción de la función: Busca una clave en el HashMap y muestra todos los valores asociados.
 * Si la clave no se encuentra, imprime un mensaje.
 */
void hashmap_get(HashMap *map, int key) {
    int index = hash(key, map->size);
    int original_index = index;

    while (map->table[index].occupied) {
        if (map->table[index].key == key && !map->table[index].deleted) {
            printf("los valores para la llave (key) %d: ", key);
            ValueNode *current = map->table[index].values;
            while (current != NULL) {
                printf(",%d", current->value);
                current = current->next;
            }
            printf("\n");
            return;
        }
        index = (index + 1) % map->size;
        if (index == original_index) {
            break;
        }
    }

    printf("Key %d no encontrada\n", key);
}

// Eliminar una clave y sus valores
/*
 * Nombre de la función: hashmap_remove.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*, “key” de tipo int.
 * Dato de retorno: ninguno.
 * Descripción de la función: Elimina una clave y todos los valores asociados de la tabla.
 */
void hashmap_remove(HashMap *map, int key) {
    int index = hash(key, map->size);
    int original_index = index;

    while (map->table[index].occupied) {
        if (map->table[index].key == key && !map->table[index].deleted) {
            ValueNode *current = map->table[index].values;
            while (current != NULL) {
                ValueNode *temp = current;
                current = current->next;
                free(temp);
            }
            map->table[index].values = NULL;
            map->table[index].deleted = true;
            map->count--;
            return;
        }
        index = (index + 1) % map->size;
        if (index == original_index) {
            break;
        }
    }

    printf("Key no encontrada\n");
}

// Serializar el HashMap en un archivo
/*
 * Nombre de la función: hashmap_serialize.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*, “filename” de tipo const char*.
 * Dato de retorno: ninguno.
 * Descripción de la función: Guarda en un archivo todas las claves y sus listas de valores.
 */
void hashmap_serialize(HashMap *map, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("no se pudo abrir el file");
        return;
    }

    for (int i = 0; i < map->size; i++) {
        if (map->table[i].occupied && !map->table[i].deleted) {
            fprintf(file, "%d|", map->table[i].key);
            ValueNode *current = map->table[i].values;
            while (current != NULL) {
                fprintf(file, "%d", current->value);
                if (current->next != NULL) {
                    fprintf(file, ",");
                }
                current = current->next;
            }
            fprintf(file, "\n");
        }
    }

    fclose(file);
}

// Deserializar el HashMap desde un archivo
/*
 * Nombre de la función: hashmap_deserialize.
 * Tipo de función: void.
 * Parámetros: “map” de tipo HashMap*, “filename” de tipo const char*.
 * Dato de retorno
*/
void hashmap_deserialize(HashMap *map, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("no se pudo abrir el file");
        return;
    }

    int key, value;
    while (fscanf(file, "%d|%d", &key, &value) == 2) {
        hashmap_insert(map, key, value);
    }

    fclose(file);
}
