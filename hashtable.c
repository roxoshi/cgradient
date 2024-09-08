#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 10
#define DELETED_NODE (person*)(0xFFFFFFFFFFFFFFFUL)

typedef struct {
    char name[MAX_NAME];
    int age;
} person;

person * hash_table[TABLE_SIZE];

unsigned int hash(char *name) {
    int length = strlen(name);
    unsigned int hash_value = 0;
    for (int i=0; i < length; i++) {
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % TABLE_SIZE;
    }
    return hash_value;
}

void init_hash_table() {
    for (int i=0; i < TABLE_SIZE; i++) {
        hash_table[i] = NULL;
    }
}

void print_table() {
    printf("Start===\n");
    for (int i=0; i < TABLE_SIZE; i++) {
        if (hash_table[i] == NULL) {
            printf("\t%i\t---\n", i);
        } else if (hash_table[i] == DELETED_NODE) {
            printf("\t%i\t--DELETED--\n", i);
        } else {
            printf("\t%i\t%s\n", i, hash_table[i]->name);
        }
    }
    printf("End===\n");
}

bool hash_table_insert(person *p) {
    if (p == NULL) return false;
    int index = hash(p->name);
    for (int i=0; i < TABLE_SIZE; i++) {
        int try = (i + index) % TABLE_SIZE;
        if (hash_table[try] == NULL || 
            hash_table[try] == DELETED_NODE) {
            hash_table[try] = p;
            return true;
        } 
    }
    return false;
}

person * hash_table_lookup(char *name) {
    int index = hash(name);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int try = (i + index) % TABLE_SIZE;
        if (hash_table[try] == NULL) {
            return false;
        } else if (hash_table[try] == DELETED_NODE) {
            continue;
        } else if (hash_table[try] != NULL &&
            strncmp(hash_table[try]->name, name, TABLE_SIZE) == 0) {
            return hash_table[try];
        }
    }
    return NULL;
}

person * hash_table_delete(char *name) {
    int index = hash(name);
    for (int i = 0; i < TABLE_SIZE; i++) {
        int try = (i + index) % TABLE_SIZE;
        if (hash_table[try] == NULL) {
            return false;
        } else if (hash_table[try] == DELETED_NODE) {
            continue;
        } else if (strncmp(hash_table[index]->name, name, TABLE_SIZE) == 0) {
            person *tmp = hash_table[index];
            hash_table[index] = NULL;
            return tmp;
        }
    }
    return NULL;    
}

int main() {
    init_hash_table();
    print_table();

    person rajat = {.name="Rajat", .age=30};
    person pragya = {.name="Pragya", .age=30};
    person popo = {.name="Popo", .age=28};
    person tammy = {.name="Tammy", .age=19};
    person candice = {.name="Candice", .age=40};
    person jenna = {.name="Jenna", .age=21};
    person kaamini = {.name="Kaamini", .age=35};

    hash_table_insert(&rajat);
    hash_table_insert(&pragya);
    hash_table_insert(&popo);
    hash_table_insert(&tammy);
    hash_table_insert(&candice);
    hash_table_insert(&jenna);
    hash_table_insert(&kaamini);
    print_table();

    person *tmp = hash_table_lookup("Rajat");
    if (tmp == NULL) {
        printf("Not found\n");
    } else {
        printf("Found %s.\n", tmp->name);
    }
    hash_table_delete("Rajat");
    print_table();
    // printf("Rajat => %u\n", hash("Rajat"));
    // printf("Pragya => %u\n", hash("Pragya"));
    // printf("Popo => %u\n", hash("Popo"));
    // printf("Joma => %u\n", hash("Joma"));
    // printf("Gogo => %u\n", hash("Gogo"));
    return 0;
}