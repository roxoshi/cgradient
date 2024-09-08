#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_NAME 256
#define TABLE_SIZE 10


typedef struct {
    char name[MAX_NAME];
    int age;
    struct person *next;
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
        } else {
            printf("\t%i\t ", i);
            person *tmp = hash_table[i];
            while (tmp != NULL) {
                printf("%s - ", hash_table[i]->name);
                tmp = tmp -> next;
            }
            printf("\n");
        }
    printf("End===\n");
    }
}

bool hash_table_insert(person *p) {
    if (p == NULL) return false;
    int index = hash(p->name);
    p -> next = hash_table[index];
    hash_table[index] = p;
    return true;
}

person * hash_table_lookup(char *name) {
    int index = hash(name);
    person *tmp = hash_table[index];
    while (tmp != NULL && strncmp(tmp->name, name, MAX_NAME) != 0) {
        tmp = tmp -> next;
    }
    return tmp;
}

person * hash_table_delete(char *name) {
    int index = hash(name);
    person *tmp = hash_table[index];
    person *prev = NULL;
    while (tmp != NULL && strncmp(tmp->name, name, MAX_NAME) != 0) {
        prev = tmp;
        tmp = tmp -> next;
    }
    if (tmp == NULL) return NULL;
    if (prev == NULL) {
        //deleting the head
        hash_table[index] = tmp -> next;
    } else {
        prev -> next = tmp -> next;
    }
    return tmp;
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
    person janice = {.name="Janice", .age=35};
    person samantha = {.name="Samantha", .age=35};
    person nala = {.name="Nala", .age=35};
    person billu = {.name="Billu", .age=35};

    hash_table_insert(&rajat);
    hash_table_insert(&pragya);
    hash_table_insert(&popo);
    hash_table_insert(&tammy);
    hash_table_insert(&candice);
    hash_table_insert(&jenna);
    hash_table_insert(&kaamini);
    hash_table_insert(&janice);
    hash_table_insert(&samantha);
    hash_table_insert(&nala);
    hash_table_insert(&billu);
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