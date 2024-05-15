#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ����������� ���� �����
typedef char* KeyType;

// ����������� ���� ����������
typedef char* InfoType;

// ��������� �������� ����������
struct Node {
    int release;         // ����� ������
    InfoType info;       // ��������� �� ����������
    struct Node* next;   // ��������� �� ��������� �������
};

// ��������� �������� ������������ ������
struct KeySpace {
    KeyType key;         // ���� ��������
    Node* node;          // ��������� �� ����������
    struct KeySpace* link; // ��������� �� ��������� �������
};

// ��������� �������
struct Table {
    KeySpace* ks;    // ��������� �� ������������ ������
};


Table* createTable() {
    Table* table = (Table*)malloc(sizeof(Table));
    if (table == NULL) {
        return NULL; // ������ ��������� ������
    }

    table->ks = NULL; // �������������� ������������ ������

    return table;
}

void insertItem(Table* table, KeyType key, InfoType info, int release) {
    if (table == NULL) {
        return; // ������� �� �������
    }

    // ����� ������������� �����
    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            // ���� ��� ����������, ��������� ����� ������ � ����� ������ ����� Node
            Node* currentNode = temp->node;
            while (currentNode->next != NULL) {
                currentNode = currentNode->next;
            }

            Node* newNode = (Node*)malloc(sizeof(Node));
            if (newNode == NULL) {
                return; // ������ ��������� ������
            }
            newNode->release = release;
            newNode->info = info;
            if (newNode->info == NULL) {
                free(newNode);
                return; // ������ ����������� ����������
            }
            newNode->next = NULL;
            currentNode->next = newNode;
            return;
        }
        temp = temp->link;
    }

    // ���� �� ����������, ������� ����� KeySpace
    KeySpace* newKeySpace = (KeySpace*)malloc(sizeof(KeySpace));
    if (newKeySpace == NULL) {
        return; // ������ ��������� ������
    }

    newKeySpace->key = key;
    newKeySpace->node = (Node*)malloc(sizeof(Node));
    if (newKeySpace->node == NULL) {
        free(newKeySpace->key);
        free(newKeySpace);
        return; // ������ ��������� ������
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
        return NULL; // ������� �� �������
    }

    Table* resultTable = createTable();
    if (resultTable == NULL) {
        return NULL; // ������ �������� �������
    }

    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            KeySpace* newKeySpace = (KeySpace*)malloc(sizeof(KeySpace));
            if (newKeySpace == NULL) {
                freeTable(resultTable);
                return NULL; // ������ ��������� ������
            }
            newKeySpace->key = _strdup(temp->key);
            if (newKeySpace->key == NULL) {
                free(newKeySpace);
                freeTable(resultTable);
                return NULL; // ������ ����������� �����
            }

            // �������� ���� Node
            Node* srcNode = temp->node;
            Node* prevNewNode = NULL;
            while (srcNode != NULL) {
                Node* newNode = (Node*)malloc(sizeof(Node));
                if (newNode == NULL) {
                    free(newKeySpace->key);
                    free(newKeySpace);
                    freeTable(resultTable);
                    return NULL; // ������ ��������� ������
                }
                newNode->release = srcNode->release;
                newNode->info = _strdup(srcNode->info);
                if (newNode->info == NULL) {
                    free(newNode);
                    free(newKeySpace->key);
                    free(newKeySpace);
                    freeTable(resultTable);
                    return NULL; // ������ ����������� ����������
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
        return NULL; // ������� �� �������
    }

    KeySpace* temp = table->ks;
    while (temp != NULL) {
        if (strcmp(temp->key, key) == 0) {
            // ����������� �� ����� Node ��� ������� KeySpace
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
    return NULL; // ������� �� ������
}


void deleteItemByKey(Table* table, KeyType key) {
    if (table == NULL) {
        return; // ������� �� �������
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
        return; // ������� �� �������
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

