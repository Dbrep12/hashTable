#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

#define TABLESIZE 10


struct Node{
    char key[100];
    struct Node *next;
};

struct HashTable
{
    char key[100]; //untuk key/value
    struct Node *chain;
}ht[TABLESIZE];

//fungsi inisialisasi
void initHT(){
    for(int i = 0; i < TABLESIZE; i++){
        strcpy(ht[i].key, "");
        ht[i].chain = NULL; 
    }
}

int strToInt(char key[]){
    int sum = 0;
    for(int i = 0; key[i] != '\0'; i++){
        sum += key[i]; //sum of ASCII values of character
    }
    return sum;
}

//Hash Function
int DivHash(char key[]){
    int intKey = strToInt(key);
    return intKey % TABLESIZE;
}

int firstCharHash(char key[]){
    //cek apakah ada data
    if(key[0] == '\0'){
        return -1;
    }
    char firstChar = key[0];

    //cek apakah karakter pertama adalah huruf
    if(!isalpha(firstChar)){
        return -1;
    }
    return tolower(firstChar) - 'a';
}

int midSquare(char key[]) {
    int intKey = strToInt(key);
    int square = intKey * intKey;

    // Ambil 2 digit tengah dari hasil quadrat
    int mid = (square / 100) % 100;
    return mid % TABLESIZE;
}
int folding(char key[]) {
    int intKey = strToInt(key);

    int part1 = intKey / 100;
    int part2 = intKey % 100;

    int hasil = part1 + part2;

    return hasil % TABLESIZE;
}
int digitExtraction(char key[]) {
    int intKey = strToInt(key);
    
    // Amnil 2 digit dari kanan
    int firstDigit = (intKey / 10) % 10;
    int lastDigit = intKey % 10;

    return (firstDigit * 10 + lastDigit) % TABLESIZE; 
}
int rotatingHash(char key[]) {
    int intKey = strToInt(key);

    int rotated = (intKey << 1) | (intKey >> (sizeof(int) * 8 - 1));

    return rotated % TABLESIZE;

}

void linearProbing(char key[], int (*hashFunction)(char[])) {
    int idx = hashFunction(key);
    int originalIdx = idx;

    while (strcmp(ht[idx].key, "") != 0) {
        idx = (idx + 1) % TABLESIZE;
        if (idx == originalIdx) {
            printf("Table penuh!\n");
            return;
        }
    }
    strcpy(ht[idx].key, key);
    printf("Key %s dimasukkan ke index %d dengan Linear Probing.\n", key, idx);
}

void rehashing(char key[], int (*hashFunction)(char[])) {
    int idx = hashFunction(key);
    int attempt = 1;

    while (strcmp(ht[idx].key, "") != 0) {
        idx = (hashFunction(key) + attempt * attempt) % TABLESIZE;
        attempt++;
        if (attempt == TABLESIZE) {
            printf("Table penuh!\n");
            return;
        }
    }
    strcpy(ht[idx].key, key);
    printf("Key %s dimasukkan ke index %d dengan Rehashing.\n", key, idx);
}

void chaining(char key[], int (*hashFunction)(char[])) {
    int idx = hashFunction(key);

    // Simpan data di key jika kosong
    if (strcmp(ht[idx].key, "") == 0) {
        strcpy(ht[idx].key, key);
        printf("Key %s dimasukkan ke index %d dengan Chaining.\n", key, idx);
        return;
    }

    struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("Gagal mengalokasi memori!\n");
        return;
    }
    strcpy(newNode->key, key);
    newNode->next = NULL;

    if (ht[idx].chain == NULL) {
        ht[idx].chain = newNode;
    } else{
        struct Node *temp = ht[idx].chain;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    printf("Key %s dimasukkan ke index %d dengan Chaining.\n", key, idx);
}

//Insert Data ke Hash Table
void insert(char key[], int (*hashFunction)(char[])) {
    int idx = hashFunction(key);

    // cek jika slot masih kosong, masukkan key
    if(strcmp(ht[idx].key, "") == 0){
        strcpy(ht[idx].key, key);
        printf("Key %s dimasukkan ke index %d.\n", key, idx);
    }
    // jika terjadi collision, kasih notif
    else{
        printf("Collision terjadi!\n");
        printf("\nkey %s tidak dapat dimasukan di index %d karena sudah terisi oleh key %s.\n", key, idx, ht[idx].key);
    }
}

//menampilkan Hash Table
void displayHT() {
    printf("\nHash Table\n");
    for (int i = 0; i < TABLESIZE; i++) {
        if (strcmp(ht[i].key, "") != 0) {
            printf("Index %d: %s", i, ht[i].key);

            // Tampilkan chaining jika ada
            struct Node *temp = ht[i].chain;
            while (temp != NULL) {
                printf(" -> %s", temp->key);
                temp = temp->next;
            }
            puts("");
        } else {
            printf("Index %d: Kosong\n", i);
        }
    }
}

void freeHashTable() {
    for (int i = 0; i < TABLESIZE; i++) {
        struct Node *temp = ht[i].chain;
        while (temp != NULL) {
            struct Node *toFree = temp;
            temp = temp->next;
            free(toFree);
        }
        ht[i].chain = NULL;
    }
}

int main(){
    initHT();
    int choice;
    char key[100];

    int (*hashFunction)(char[]);
    void (*collisionHandler)(char[], int (*)(char[]));

    printf("1. Division Hash\n2. First Char Hash\n3. Mid Square\n4. Folding\n5. Digit Extraction\n6. Rotating Hash\n");
    printf("Pilih Hash Function: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: hashFunction = DivHash; break;
        case 2: hashFunction = firstCharHash; break;
        case 3: hashFunction = midSquare; break;
        case 4: hashFunction = folding; break;
        case 5: hashFunction = digitExtraction; break;
        case 6: hashFunction = rotatingHash; break;
        default: return 0;
    }

    printf("\n1. Linear Probing\n2. Rehashing\n3. Chaining\nPilih Metode: ");
    scanf("%d", &choice);
    
    switch (choice) {
        case 1: collisionHandler = linearProbing; break;
        case 2: collisionHandler = rehashing; break;
        case 3: collisionHandler = chaining; break;
        default: return 0;
    }

    while (1) {
        printf("\nMasukkan key (atau 'exit' untuk berhenti): ");
        scanf("%s", key);
        if (strcmp(key, "exit") == 0) break;
        collisionHandler(key, hashFunction);
    }

    displayHT();
    freeHashTable();
    
    return 0;
    // initHT();
    // int choice;
    // char key[100];

    // int (*hashFunction)(char[]);
    // void (*collisionHandler)(char[], int (*)(char[]));

    // // Pilih hash function
    // printf("Pilih Hash Function:\n");
    // printf("1. Division Hash\n2. First Char Hash\n3. Mid Square\n4. Folding\n5. Digit Extraction\n6. Rotating Hash\n");
    // printf("Pilihan: ");
    // scanf("%d", &choice);

    // switch (choice) {
    //     case 1: hashFunction = DivHash; break;
    //     case 2: hashFunction = firstCharHash; break;
    //     case 3: hashFunction = midSquare; break;
    //     case 4: hashFunction = folding; break;
    //     case 5: hashFunction = digitExtraction; break;
    //     case 6: hashFunction = rotatingHash; break;
    //     default: printf("Pilihan tidak valid!\n"); return 0;
    // }

    // // Pilih metode collision handling
    // printf("\nPilih Metode Collision Handling:\n");
    // printf("1. Linear Probing\n2. Rehashing\n3. Chaining\n");
    // printf("Pilihan: ");
    // scanf("%d", &choice);

    // switch (choice) {
    //     case 1: collisionHandler = linearProbing; break;
    //     case 2: collisionHandler = rehashing; break;
    //     case 3: collisionHandler = chaining; break;
    //     default: printf("Pilihan tidak valid!\n"); return 0;
    // }

    // // Input Data
    // while (1) {
    //     printf("\nMasukkan key (atau 'exit' untuk berhenti): ");
    //     scanf("%s", key);
    //     if (strcmp(key, "exit") == 0) break;
    //     collisionHandler(key, hashFunction);
    // }

    // // Tampilkan hash table
    // displayHT();

    // for (int i = 0; i < TABLESIZE; i++) {
    //     Node *temp = ht[i].chain;
    //     while (temp != NULL) {
    //         Node *toFree = temp;
    //         temp = temp->next;
    //         free(toFree);
    //     }
    // }
    // freeHashTable();
    // return 0;
}