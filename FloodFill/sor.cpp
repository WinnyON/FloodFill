#pragma once
#include "sor.h"
#include <iostream>
using namespace std;

template<typename T>
Node<T>::Node() {
	T data = NULL;
	next = nullptr;
}

template<typename T>
Node<T>::Node(T _data) {
	data = _data;
	next = nullptr;
}

template<typename T>
void Node<T>::setInfo(T _data) {
	data = _data;
}

template<typename T>
void Node<T>::setNext(Node<T>* elem) {
	next = elem;
}

template<typename T>
T Node<T>::getData() {
	return data;
}

template<typename T>
Node<T>* Node<T>::getNext() {
	return next;
}

template<typename T>
Sor<T>::Sor() {
	elso = utolso = nullptr;
	size = 0;
}

template<typename T>
Sor<T>::Sor(T _data) {
	Node<T>* ujElem;
	ujElem = new Node<T>(_data);
	elso = utolso = ujElem;
	size = 1;
}

template<typename T>
Sor<T>::~Sor() {
	Node<T>* mutato = elso;
	deleteAll(mutato);
}
template<typename T>
void Sor<T>::clear() {
	Node<T>* mutato = elso;
	deleteAll(mutato);
	elso = utolso = nullptr;
}
template<typename T>
bool Sor<T>::isEmpty() {
	if (utolso == nullptr) {
		return true;
	}
	return false;
}

template<typename T>
void Sor<T>::push(T _data) {
	Node<T>* ujElem = new Node<T>(_data);
	if (elso == nullptr) {
		elso = ujElem;
		size = 1;
	}
	else {
		utolso->setNext(ujElem);
		size++;
	}
	utolso = ujElem;
}

template<typename T>
T Sor<T>::peek() {
	return elso->getData();
}

template<typename T>
void Sor<T>::pop() {
	if (!isEmpty()) {
		Node<T>* temp = elso;
		elso = elso->getNext();
		if (elso == nullptr) {
			utolso = nullptr;
		}
		delete temp;
		size--;
	}
}

template<typename T>
int Sor<T>::getSize() {
	return size;
}

template<typename T>
void Sor<T>::deleteAll(Node<T>* mutato) {
	if (mutato != nullptr) {
		deleteAll(mutato->getNext());
		delete mutato;
	}
}
