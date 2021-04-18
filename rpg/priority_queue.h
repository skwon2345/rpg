#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <iostream>

using namespace std;

#include "main.h"
#include "event.h"
#include "node.h"
#include "priority_queue.h"

template <class T>
class Node;


template <class T>
class PriorityQueue {
protected:
    Node<T> *front;
    int numElements;
    
public:
    PriorityQueue();
    
    virtual ~PriorityQueue();
    
    void print();
    
    void enqueue(T *element);
    
    // pre-condition: size() > 0
    T *dequeue();
    
    // pre-condition: size() > 0
    T *peek();
    int size();
};

template <class T>
PriorityQueue<T>::PriorityQueue() {
    front = NULL;
    numElements = 0;
}

template <class T>
PriorityQueue<T>::~PriorityQueue() {
    while (front != NULL) {
        Node<T> *temp = front->next;
        delete front;
        front = temp;
    }
}

template <class T>
void PriorityQueue<T>::print() {
    if (front == NULL) {
        cout << "PriorityQueue::print() : no elements to print." << endl;
        return;
    }
    
    Node<T> *temp = front;
    
    while (temp != NULL) {
        temp->element->print();
        temp = temp->next;
    }
}

template <class T>
void PriorityQueue<T>::enqueue(T *element) {
    Node<T> *toBeEnqueued = new Node<T>(element, NULL);
    numElements++;
    
    //이 temp는 마지막에 delete을 안해줘도 되나?
    
    if (front == NULL) {
        front = toBeEnqueued;
        return;
    }
    
    Node<T> *temp = front;
    
    while (temp != NULL) {
        if (temp->element->time > toBeEnqueued->element->time) {
            if (temp == front) {
                front = toBeEnqueued;
                toBeEnqueued->next = temp;
                return;
            }
            Node<T> *temp1 = front;
            
            while (temp1->next != temp) {
                temp1 = temp1->next;
            }
            
            temp1->next = toBeEnqueued;
            toBeEnqueued->next = temp;
            return;
        }
        temp = temp->next;
    }
    
    if (temp == NULL) {
        temp = front;
        while(temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = toBeEnqueued;
    }
}

// pre-condition: size() > 0
template <class T>
T *PriorityQueue<T>::dequeue() {
    if (front == NULL) {
        cout << "PriorityQueue::dequeue() : no elements to dequeue. " << endl;
        exit(1);;
    }
    
    Node<T> *toBeDequeued = front;
    T *elementToReturn = toBeDequeued->element;
    
    front = toBeDequeued->next;
    
    toBeDequeued->element = NULL;
    delete toBeDequeued;
    numElements--;
    
    return elementToReturn;
}

// pre-condition: size() > 0
template <class T>
T *PriorityQueue<T>::peek() {
    if (front == NULL) {
        cout << "PriorityQueue::dequeue() : no elements to dequeue. " << endl;
        exit(1);;
    }
    
    return front->element;
}

template <class T>
int PriorityQueue<T>::size() {
    return numElements;
}

#endif
