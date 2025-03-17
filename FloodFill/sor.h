#pragma once

template<typename T>
class Node {
	T data;
	Node* next;
public:
	Node();
	Node(T);
	void setInfo(T);
	void setNext(Node*);
	T getData();
	Node* getNext();
};

template<typename T>
class Sor {
	Node<T>* elso;
	Node<T>* utolso;
	int size;
	void deleteAll(Node<T>*);

public:
	Sor();
	Sor(T _data);
	~Sor();
	void clear();
	bool isEmpty();
	void push(T _data);
	T peek();
	void pop();
	int getSize();
};

#include "sor.cpp";
