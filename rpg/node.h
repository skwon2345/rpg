#ifndef PRIORITYQUEUENODE_H
#define PRIORITYQUEUENODE_H
#include <iostream>

using namespace std;

#include "main.h"
#include "node.h"
#include "event.h"

template <class T>
class Node {
protected:
    
public:
    T *element;
    Node<T> *next;
    
    Node();
    Node(T *element, Node<T> *next);
    virtual ~Node();
    
    //int time();
    
    void reset();
    
    void print();
};

template <class T>
Node<T>::Node() {
    element = new T();
    next = NULL;
}

template <class T>
Node<T>::Node(T *element, Node<T> *next) {
    this->element = element;
    this->next = next;
}

template <class T>
Node<T>::~Node() {
//    if (element != NULL) {
//        delete element;
//        element = NULL;
//    }
    //next는?
    //double deletion??
}

template <class T>
void Node<T>::reset() {
    element->resetAStar();
}

template <class T>
void Node<T>::print() {
    element->print();
}

//template <class T>
//int PriorityQueueNode<T>::time() {
//    return element->time;
//}

#endif

