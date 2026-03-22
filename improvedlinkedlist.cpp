#include <iostream>

struct Node {
    int data;
    Node* next;
};

// using a tail pointer helps eliminate the while loop and makes my code more efficient, since it no longer has to "find" the end, the tail pointer tells it where the end is
void insertEnd(Node*& head, Node*& tail, int ndata) {
    Node* newNode = new Node{ndata, nullptr};

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

void display(Node* head) {
    while (head != nullptr) {
        std::cout << head->data << " ";
        head = head->next;
    }
    std::cout << std::endl;
}

int main() {
    Node* head = nullptr;
    Node* tail = nullptr;

    insertEnd(head, tail, 5);
    insertEnd(head, tail, 6);
    insertEnd(head, tail, 7);

    display(head);
// added cleanup
    while (head != nullptr) {
        Node* temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}