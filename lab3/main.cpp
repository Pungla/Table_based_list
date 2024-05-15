#include <stdio.h>
#include "table.h" // ���������� ���� ����������

int main() {
    // �������� �������
    Table* table = createTable();

    // ������� ��������� � �������
    insertItem(table, _strdup("key1"), _strdup("info1"), 1);
    insertItem(table, _strdup("key2"), _strdup("info2"), 1);
    insertItem(table, _strdup("key1"), _strdup("info3"), 2); // ������� � ��� �� ������, �� ������ �������
    insertItem(table, _strdup("key3"), _strdup("info4"), 1);

    // ����� ����������� �������
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

    // ����� ���� ������ �������� �� ��������� �����
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
    freeTable(searchResults); // ����������� ������, ���������� ��� ����������� ������

    // ����� ���������� ������ �������� �� ��������� ����� � ������ ������
    printf("\nSearch result for key 'key1' and version 2:\n");
    InfoType searchResult = searchItemByVersion(table, _strdup("key1"), 2);
    if (searchResult != NULL) {
        printf("Info: %s\n", searchResult);
    }
    else {
        printf("Version not found.\n");
    }

    // �������� ���� ��������� � �������� ������
    deleteItemByKey(table, _strdup("key1"));

    // ����� ����������� ������� ����� ��������
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

    // ������������ ������, ������� ��������
    freeTable(table);

    return 0;
}
