#ifndef LIST_H
#define LIST_H

#include <iostream>

using namespace std;

#include "main.h"
#include "node.h"

template<class T>
class Node;

template <class T>
class List {
protected:
public:
    Node<T> *head;
    int numNodes;
    
    List();
    virtual ~List();
    
    void reset();
    
    void print();

    bool find(T *element);
    void insert(T *element);
    
    T *removeElementAt(int index);
    
    int size();
};

template <class T>
List<T>::List() {
    head = NULL;
    numNodes = 0;
}

template <class T>
List<T>::~List() {
    while (head != NULL) {
        Node<T> *temp = head->next;
        delete head;
        head = temp;
    }
}

template <class T>
void List<T>::reset() {
    while (head != NULL) {
        Node<T> *temp = head->next;
        head->reset();
        
        head = NULL; // ???????
        head = temp;
    }
        
    numNodes = 0;
}

template <class T>
void List<T>::print() {
    if (head == NULL) {
        cout << "List<T>::print() : error - head is NULL" << endl;
        exit(1);
    }
    Node<T> *temp = head;
    
    cout << "[";
    while (temp != NULL) {
        temp->element->print();
        temp = temp->next;
        if (temp != NULL) {
            cout << "|";
        }
    }
    cout << "]";
}

template <class T>
bool List<T>::find(T *element) {
    Node<T> *current = head;
    
    while (current != NULL) {
        if (current->element == element) {
            return true;
        }
        
        current = current->next;
    }
    
    return false;
}

template <class T>
void List<T>::insert(T *element) {
//    if (element == NULL) {
//        cout << "List<T>::insert(): if (element == NULL) {" << endl;
//        exit(1);
//    }
//    
//    cout << "List<T>::insert(): element = " << element << endl;

    if (head == NULL) {
        head = new Node<T>(element, NULL);
//        cout << "List<T>::insert(): " << head << endl;
        numNodes++;
        return;
    }
    
    Node<T> *temp = new Node<T>(element, head);
    
//    delete head;
    
    head = temp;
//    cout << "List<T>::insert(): " << head << endl;
    
    numNodes++;
}

template <class T>
T *List<T>::removeElementAt(int index) {
    if (head == NULL) {
        cout << "List<T>::removeElementAt() : error - head is NULL." << endl;
        return NULL;
    }
    
    Node<T> *toBeRemoved = head;
    T *toBeReturned = NULL;
    
    if (index == 0) {
        head = head->next;
        toBeReturned = toBeRemoved->element;
        toBeRemoved = NULL;
        numNodes--;
    }
    
    else if (index > 0) {
        Node<T> *prev = head;
        
        for (int i = 0; i < index; i++) {
            toBeRemoved = toBeRemoved->next;
        }
        while (prev->next != toBeRemoved) {
            prev = prev->next;
        }
        
        prev->next = toBeRemoved->next;
        toBeReturned = toBeRemoved->element;
        toBeRemoved = NULL;
        numNodes--;
    }
    
    else {
        cout << "Backpack::removeItemAt() : error - index input < 0" << endl;
        exit(1);
    }
    
    return toBeReturned;
}

template <class T>
int List<T>::size() {
    return numNodes;
}

#endif
