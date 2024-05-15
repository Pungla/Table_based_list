#include <stdio.h>
#include "table.h" // Подключаем нашу библиотеку

int main() {
    // Создание таблицы
    Table* table = createTable();

    // Вставка элементов в таблицу
    insertItem(table, _strdup("key1"), _strdup("info1"), 1);
    insertItem(table, _strdup("key2"), _strdup("info2"), 1);
    insertItem(table, _strdup("key1"), _strdup("info3"), 2); // Элемент с тем же ключом, но другой версией
    insertItem(table, _strdup("key3"), _strdup("info4"), 1);

    // Вывод содержимого таблицы
    printf("Contents of the table:\n");
    KeySpace* temp = table->ks;
    while (temp != NULL) {
        printf("Key: %s\n", temp->key);
        Node* currentNode = temp->node;
        while (currentNode != NULL) {
            printf("Version: %d, Info: %s\n", currentNode->release, currentNode->info);
            currentNode = currentNode->next;
        }
        temp = temp->link;
    }

    // Поиск всех версий элемента по заданному ключу
    printf("\nSearch results for key 'key1':\n");
    Table* searchResults = searchItemByKey(table, _strdup("key1"));
    temp = searchResults->ks;
    while (temp != NULL) {
        printf("Key: %s\n", temp->key);
        Node* currentNode = temp->node;
        while (currentNode != NULL) {
            printf("Version: %d, Info: %s\n", currentNode->release, currentNode->info);
            currentNode = currentNode->next;
        }
        temp = temp->link;
    }
    freeTable(searchResults); // Освобождаем память, выделенную для результатов поиска

    // Поиск конкретной версии элемента по заданному ключу и номеру версии
    printf("\nSearch result for key 'key1' and version 2:\n");
    InfoType searchResult = searchItemByVersion(table, _strdup("key1"), 2);
    if (searchResult != NULL) {
        printf("Info: %s\n", searchResult);
    }
    else {
        printf("Version not found.\n");
    }

    // Удаление всех элементов с заданным ключом
    deleteItemByKey(table, _strdup("key1"));

    // Вывод содержимого таблицы после удаления
    printf("\nContents of the table after deletion:\n");
    temp = table->ks;
    while (temp != NULL) {
        printf("Key: %s\n", temp->key);
        Node* currentNode = temp->node;
        while (currentNode != NULL) {
            printf("Version: %d, Info: %s\n", currentNode->release, currentNode->info);
            currentNode = currentNode->next;
        }
        temp = temp->link;
    }

    // Освобождение памяти, занятой таблицей
    freeTable(table);

    return 0;
}
