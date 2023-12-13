#pragma once

#include<iostream>

using namespace std;

namespace MarkAllenWeiss_BST {

	template <typename T>
	class BST
	{
	public:
		BST();
		BST(const BST& rhs);
		BST(BST&& rhs);
		~BST();

		const T& findMin() const;
		const T& findMax() const;
		bool contains(const T& x) const;
		bool isEmpty() const;
		void printTree(ostream& out = std::cout) const;

		void makeEmpty();
		void insert(const T& x);
		void insert(T&& x);
		void remove(const T& x);

		BST& operator=(const BST& rhs);
		BST& operator=(BST&& rhs);

	private:
		struct Node
		{
			T element;
			Node* left;
			Node* right;

			Node(const T& theElement, Node* lt, Node* rt) :
				element(theElement),
				left(lt),
				right(rt)
			{

			}

			Node(T&& theElement, Node* lt, Node* rt) :
				element(std::move(theElement)),
				left(lt),
				right(rt)
			{

			}
		};


		Node* root;

		void insert(const T& x, Node*& t);
		void insert(T&& x, Node*& t);
		void remove(const T& x, Node*& t);
		Node* findMin(Node* t) const;
		Node* findMax(Node* t) const;
		bool contains(const T& x, Node* t) const;
		void makeEmpty(Node*& t);
		void printTree(Node* t, ostream& out) const;
		Node* clone(Node* t) const;
	};


	// ------------------------------------
	//              public
	// ------------------------------------

	template<typename T>
	bool BST<T>::contains(const T& x) const
	{
		return contains(x, root);
	}

	template<typename T>
	void BST<T>::insert(const T& x)
	{
		insert(x, root);
	}

	template<typename T>
	void BST<T>::insert(T&& x)
	{
		insert(std::move(x), root);
	}

	template<typename T>
	void BST<T>::remove(const T& x)
	{
		remove(x, root);
	}

	template<typename T>
	void BST<T>::printTree(ostream& out) const
	{
		printTree(root, out);
	}

	template<typename T>
	const T& BST<T>::findMin() const
	{
		return findMin(root)->element;
	}

	template<typename T>
	const T& BST<T>::findMax() const
	{
		return findMax(root)->element;
	}

	template<typename T>
	void BST<T>::makeEmpty()
	{
		makeEmpty(root);
	}

	template<typename T>
	bool BST<T>::isEmpty() const
	{
		return root == nullptr;
	}

	// ------------------------------------
	//              private
	// ------------------------------------


	template<typename T>
	bool BST<T>::contains(const T& x, Node* t)const
	{
		if (t == nullptr)
		{
			return false;
		}
		else if (x < t->element)
		{
			return contains(x, t->left);
		}
		else if (x > t->element)
		{
			return contains(x, t->right);
		}
		else
		{
			return true;
		}
	}

	template<typename T>
	typename BST<T>::Node* BST<T>::findMin(typename BST<T>::Node* t) const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		if (t->left == nullptr)
		{
			return t;
		}
		return findMin(t->left);
	}

	template<typename T>
	typename BST<T>::Node* BST<T>::findMax(typename BST<T>::Node* t) const
	{

		if (t != nullptr)
		{
			while (t->right != nullptr)
			{
				t = t->right;
			}
		}
		return t;
	}

	template<typename T>
	void BST<T>::insert(const T& x, typename BST<T>::Node*& t)
	{
		if (t == nullptr)
		{
			t = new Node { x, nullptr, nullptr };
		}
		else if (x < t->element)
		{
			insert(x, t->left);
		}
		else if (x > t->element)
		{
			insert(x, t->right);
		}
		else
		{
			;
		}
	}

	template<typename T>
	void BST<T>::insert(T&& x, typename BST<T>::Node*& t)
	{
		if (t == nullptr)
		{
			t = new Node { std::move(x), nullptr, nullptr };
		}
		else if (x < t->element)
		{
			insert(std::move(x), t->left);
		}
		else if (x > t->element)
		{
			insert(std::move(x), t->right);
		}
		else
		{
			;
		}
	}

	template<typename T>
	void BST<T>::remove(const T& x, typename BST<T>::Node*& t)
	{
		if (t == nullptr)
		{
			return;
		}
		if (x < t->element)
		{
			remove(x, t->left);
		}
		else if (x > t->element)
		{
			remove(x, t->right);
		}
		else if (t->left != nullptr && t->right != nullptr)
		{
			t->element = findMin(t->right)->element;
			remove(t->element, t->right);
		}
		else
		{
			Node* oldNode = t;
			t = (t->left != nullptr) ? t->left : t->right;
			delete oldNode;
		}
	}

	template<typename T>
	BST<T>::~BST()
	{
		makeEmpty();
	}

	template<typename T>
	void BST<T>::makeEmpty(typename BST<T>::Node*& t)
	{
		if (t != nullptr)
		{
			makeEmpty(t->left);
			makeEmpty(t->right);
			delete t;
		}
		t = nullptr;
	}

	template<typename T>
	BST<T>::BST() :
		root(nullptr)
	{

	}

	template<typename T>
	BST<T>::BST(const BST<T>& rhs) :
		root(nullptr)
	{
		root = clone(rhs.root);
	}

	template<typename T>
	BST<T>::BST(BST<T>&& rhs) :
		root(rhs.root)
	{
	}

	template<typename T>
	BST<T>& BST<T>::operator=(const BST<T>& rhs)
	{
		root = clone(rhs.root);
		return *this;
	}

	template<typename T>
	BST<T>& BST<T>::operator=(BST<T>&& rhs)
	{
		root = rhs.root;
		return *this;
	}

	template<typename T>
	typename BST<T>::Node* BST<T>::clone(typename BST<T>::Node* t)const
	{
		if (t == nullptr)
		{
			return nullptr;
		}
		else
		{
			return new Node { t->element, clone(t->left), clone(t->right) };
		}
	}

	template<typename T>
	void BST<T>::printTree(typename BST<T>::Node* t, ostream& out)const
	{
		if (t != nullptr)
		{
			out << t->element;
			printTree(t->left, out);
			printTree(t->right, out);
		}
	}
}




