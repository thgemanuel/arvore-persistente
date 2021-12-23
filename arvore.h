

#ifndef REGTREE_H_
#define REGTREE_H_

#include <sstream>
#include <string>
#include <iostream>
#include <utility>

class arvore;

class Node {
	private:
		Node *esquerda, *direita;
		std::pair<int, int> item;

	public:
		Node(std::pair<int, int> i);
		Node(std::pair<int, int> i, Node *e, Node *d);
		friend class arvore;
};

class arvore {
	private:
		Node *raiz; // raiz
		// métodos internos
		Node* pegarnodo(std::pair<int, int> r, Node *n);
		Node* inserir(std::pair<int, int> r, Node *n);
		bool buscar(std::pair<int, int> r, Node *n);
		Node* remover(std::pair<int, int> r, Node *n);
		Node* removernodo(Node *n);
		Node* removermax(Node *n);
		void apagar(Node *n);
		void printar(Node *n);
	public:
		arvore(); // construtor
		arvore(arvore& st); // construtor de cópia
		 void copiararvore(Node *& n, Node *& sn); // auxilia o construtor de cópia
		void operator =(arvore &st); // sobrecarga do operador =
		void atualizar(std::pair<int, int> r); // atualiza informações dos nós
		void metodopers(std::pair<int, int> r, arvore& t); // método persistente
		void inserir(std::pair<int, int> r); // método de inserção
		bool buscar(std::pair<int, int> r); // método de busca
		void remover(std::pair<int, int> r); // método de remoção
		void printar(); // método de exibição
};


using namespace std;

/// métodos dos nós
Node::Node(pair<int, int> i) {
	item = i;
	esquerda = direita = NULL;
}

Node::Node(pair<int, int> i, Node *e, Node *d) {
    item = i;
	esquerda = e;
	direita = e;
}

/// métodos da árvore binária de registros
arvore::arvore() {
	raiz = NULL;
}

arvore::arvore(arvore& st) {
	if (st.raiz == NULL)
		raiz = NULL;
	else
		//copiararvore(this->raiz, st.raiz); ///--> método de cópia, DESCOMENTE PARA REALIZAR A CÓPIA E COMENTE A LINHA ABAIXO
		this->raiz=st.raiz; ///---> MÉTODO PERSISTENTE
}
///APENAS UTILIZADO NO MÉTODO DE CÓPIA
void arvore::copiararvore(Node *& n, Node *& sn) {
	if (sn == NULL)
		n = NULL;
	else {
		n = new Node(sn->item);
		copiararvore(n->esquerda, sn->esquerda);
		copiararvore(n->direita, sn->direita);

	}
}

void arvore::operator=(arvore& st) {
	if (st.raiz == NULL)
		raiz = NULL;
	else
		//copiararvore(this->raiz, st.raiz); ///--> método de cópia, DESCOMENTE PARA REALIZAR A CÓPIA E COMENTE A LINHA ABAIXO
		this->raiz = st.raiz; ///--> MÉTODO PERSISTENTE
}

void arvore::atualizar(pair<int, int> r) {
	if (buscar(r)) {
		Node *aux = pegarnodo(r, raiz);
		if (aux) {
			aux->item.second += r.second;
			if (aux->item.second <= 0)
				remover(r);
		} else
			cout << "Erro ao atualizar estoque!" << endl;
	} else {
		raiz = inserir(r, raiz);
	}
}

void arvore::metodopers(pair<int, int> r, arvore& t) {
	raiz = new Node(t.raiz->item);
	Node *anterior = t.raiz, *atual = raiz;
	while (anterior != NULL && anterior->item.first != r.first) {
		if (r.first > anterior->item.first) {
			atual->esquerda = anterior->esquerda;
			anterior = anterior->direita;
			if (anterior != NULL)
				atual->direita = new Node(anterior->item);
			else
				atual->direita = new Node(r);
			atual = atual->direita;
		} else {
			atual->direita = anterior->direita;
			anterior = anterior->esquerda;
			if (anterior != NULL)
				atual->esquerda = new Node(anterior->item);
			else
				atual->esquerda = new Node(r);
			atual = atual->esquerda;
		}
	}
	if (anterior != NULL) {
		atual->esquerda = anterior->esquerda;
		atual->direita = anterior->direita;
		atual->item.second += r.second;
		if (atual->item.second <= 0)
			remover(atual->item);
	} else {
		atual->item.second = r.second;
	}
}

Node* arvore::pegarnodo(pair<int, int> r, Node* n) {
	if (n == NULL)
		return NULL;
	else if (n->item.first == r.first)
		return n;
	else if (r.first > n->item.first)
		return pegarnodo(r, n->direita);
	else
		return pegarnodo(r, n->esquerda);
}

void arvore::inserir(pair<int, int> r) {
	raiz = inserir(r, raiz);
}

Node* arvore::inserir(pair<int, int> r, Node* n) {
	if (n == NULL)
		return new Node(r);
	else {
		if (r.first > n->item.first)
			n->direita = inserir(r, n->direita);
		else
			n->esquerda = inserir(r, n->esquerda);
		return n;
	}
}

bool arvore::buscar(pair<int, int> r) {
	return buscar(r, raiz);
}

bool arvore::buscar(pair<int, int> r, Node* n) {
	if (n == NULL)
		return false;
	else if (n->item.first == r.first)
		return true;
	else if (r.first > n->item.first)
		return buscar(r, n->direita);
	else
		return buscar(r, n->esquerda);
}

void arvore::remover(pair<int, int> r) {
	raiz = remover(r, raiz);
}

Node* arvore::remover(pair<int, int> r, Node* n) {
	Node *temporario = NULL;
	if (n != NULL) {
		if (r.first == n->item.first) {
			temporario = n;
			n = removernodo(n);
		}
		else if (r.first > n->item.first)
			n->direita = remover(r, n->direita);
		else
			n->esquerda = remover(r, n->esquerda);
	}
	if (temporario)
		delete temporario;
	return n;
}

Node* arvore::removernodo(Node* n) {
	if (n->esquerda == NULL)
		return n->direita;
	else if (n->direita == NULL)
		return n->esquerda;
	else
		return removermax(n);
}

Node* arvore::removermax(Node* n) {
	if (n->esquerda->direita == NULL) {
		n->esquerda->direita = n->direita;
		return n->esquerda;
	} else {
		Node *nodoaux1 = n->esquerda;
		Node *nodoaux2 = n->esquerda->direita;
		while (nodoaux2->direita != NULL) {
			nodoaux1 = nodoaux2;
			nodoaux2 = nodoaux2->direita;
		}
		nodoaux1->direita = nodoaux2->esquerda;
		nodoaux2->esquerda = n->esquerda;
		nodoaux2->direita = n->direita;
		return nodoaux2;
	}
}

void arvore::printar() {
	printar(raiz);
}

void arvore::printar(Node *n) {
	if (n != NULL) {
		printar(n->esquerda);
		cout << "Codigo: " << n->item.first << "\t Quantidade: " << n->item.second << endl;
		printar(n->direita);
	}
}


#endif
