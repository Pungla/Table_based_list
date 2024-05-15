#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Определение типа ключа
typedef char* KeyType;

// Определение типа информации
typedef char* InfoType;

// Структура элемента информации
struct Node {
    int release;         // Номер версии
    InfoType info;       // Указатель на информацию
    struct Node* next;   // Указатель на следующий элемент
};

// Структура элемента пространства ключей
struct KeySpace {
    KeyType key;         // Ключ элемента
    Node* node;          // Указатель на информацию
    struct KeySpace* link; // Указатель на следующий элемент
};

// Структура таблицы
struct Table {
    KeySpace* ks;    // Указатель на пространство ключей
};


Table* createTable() {
    Table* table = (Table*)malloc(sizeof(Table));
    if (table == NULL) {
        return NULL; // Ошибка выделения памяти
    }

    table->ks = NULL; // Инициализируем пространство ключей

    return table;
}

void insertItem(Table* table, KeyType key, InfoType info, int release) {
    if (table == NULL) {
        return; // Таблица не создана
    }

    // Поиск существующего ключа
    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            // Ключ уже существует, добавляем новую версию в конец списка узлов Node
            Node* currentNode = temp->node;
            while (currentNode->next != NULL) {
                currentNode = currentNode->next;
            }

            Node* newNode = (Node*)malloc(sizeof(Node));
            if (newNode == NULL) {
                return; // Ошибка выделения памяти
            }
            newNode->release = release;
            newNode->info = info;
            if (newNode->info == NULL) {
                free(newNode);
                return; // Ошибка копирования информации
            }
            newNode->next = NULL;
            currentNode->next = newNode;
            return;
        }
        temp = temp->link;
    }

    // Ключ не существует, создаем новый KeySpace
    KeySpace* newKeySpace = (KeySpace*)malloc(sizeof(KeySpace));
    if (newKeySpace == NULL) {
        return; // Ошибка выделения памяти
    }

    newKeySpace->key = key;
    newKeySpace->node = (Node*)malloc(sizeof(Node));
    if (newKeySpace->node == NULL) {
        free(newKeySpace->key);
        free(newKeySpace);
        return; // Ошибка выделения памяти
    }
    newKeySpace->node->release = release;
    newKeySpace->node->info = info;
    newKeySpace->node->next = NULL;

    if (table->ks == NULL) {
        table->ks = newKeySpace;
        newKeySpace->link = NULL;
    }
    else {
        KeySpace* temp = table->ks;
        while (temp->link != NULL) {
            temp = temp->link;
        }
        temp->link = newKeySpace;
        newKeySpace->link = NULL;
    }
}

void freeTable(Table* table) {
    if (table == NULL) {
        return;
    }

    KeySpace* temp = table->ks;
    while (temp != NULL) {
        KeySpace* next = temp->link;
        free(temp->key);
        free(temp->node->info);
        free(temp->node);
        free(temp);
        temp = next;
    }

    free(table);
}

Table* searchItemByKey(Table* table, KeyType key) {
    if (table == NULL) {
        return NULL; // Таблица не создана
    }

    Table* resultTable = createTable();
    if (resultTable == NULL) {
        return NULL; // Ошибка создания таблицы
    }

    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            KeySpace* newKeySpace = (KeySpace*)malloc(sizeof(KeySpace));
            if (newKeySpace == NULL) {
                freeTable(resultTable);
                return NULL; // Ошибка выделения памяти
            }
            newKeySpace->key = _strdup(temp->key);
            if (newKeySpace->key == NULL) {
                free(newKeySpace);
                freeTable(resultTable);
                return NULL; // Ошибка копирования ключа
            }

            // Копируем узлы Node
            Node* srcNode = temp->node;
            Node* prevNewNode = NULL;
            while (srcNode != NULL) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                if (newNode == NULL) {
                    free(newKeySpace->key);
                    free(newKeySpace);
                    freeTable(resultTable);
                    return NULL; // Ошибка выделения памяти
                }
                newNode->release = srcNode->release;
                newNode->info = _strdup(srcNode->info);
                if (newNode->info == NULL) {
                    free(newNode);
                    free(newKeySpace->key);
                    free(newKeySpace);
                    freeTable(resultTable);
                    return NULL; // Ошибка копирования информации
                }
                newNode->next = NULL;

                if (prevNewNode == NULL) {
                    newKeySpace->node = newNode;
                }
                else {
                    prevNewNode->next = newNode;
                }
                prevNewNode = newNode;
                srcNode = srcNode->next;
            }

            newKeySpace->link = NULL;

            if (resultTable->ks == NULL) {
                resultTable->ks = newKeySpace;
            }
            else {
                KeySpace* tempResult = resultTable->ks;
                while (tempResult->link != NULL) {
                    tempResult = tempResult->link;
                }
                tempResult->link = newKeySpace;
            }
        }
        temp = temp->link;
    }
    return resultTable;
}

InfoType searchItemByVersion(Table* table, KeyType key, int release) {
    if (table == NULL) {
        return NULL; // Таблица не создана
    }

    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            // Итерируемся по узлам Node для данного KeySpace
            Node* currentNode = temp->node;
            while (currentNode != NULL) {
                if (currentNode->release == release) {
                    return currentNode->info;
                }
                currentNode = currentNode->next;
            }
        }
        temp = temp->link;
    }
    return NULL; // Элемент не найден
}


void deleteItemByKey(Table* table, KeyType key) {
    if (table == NULL) {
        return; // Таблица не создана
    }

    KeySpace* prev = NULL;
    KeySpace* curr = table->ks;

    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            if (prev == NULL) {
                table->ks = curr->link;
                prev = table->ks;
            }
            else {
                prev->link = curr->link;
            }
            free(curr->key);
            free(curr->node->info);
            //free(curr->node->release);
            free(curr->node);
            free(curr);
            curr = prev;
        }
        else {
            prev = curr;
            curr = curr->link;
        }
    }
}

void deleteItemByVersion(Table* table, KeyType key, int release) {
    if (table == NULL) {
        return; // Таблица не создана
    }

    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0 && temp->node->release == release) {
            free(temp->key);
            free(temp->node->info);
            free(temp->node);
            temp = temp->link;
        }
        temp = temp->link;
    }
}

