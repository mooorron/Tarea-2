#include "hashmap.h"

int main() {
    HashMap *map = hashmap_create(INITIAL_SIZE);

    hashmap_insert(map, 3, 300);
    hashmap_insert(map, 3, 400);
    hashmap_insert(map, 3, 500);
    hashmap_insert(map, 4, 100);
    hashmap_insert(map, 5, 5);
    hashmap_insert(map, 9, 100);
    hashmap_insert(map, 5, 10);
    hashmap_insert(map, 6, 500);
    hashmap_insert(map, 7, 100);
    hashmap_insert(map, 20, 5);
    hashmap_insert(map, 14, 100);
    hashmap_insert(map, 29, 10);
    hashmap_insert(map, 28, 10);
    hashmap_insert(map, 27, 10);
    hashmap_insert(map, 24, 10);
    hashmap_get(map, 3);
    hashmap_get(map, 4);
    hashmap_get(map, 9);
    hashmap_get(map, 5);



    hashmap_serialize(map, "hashmap.txt");
    return 0;
}
