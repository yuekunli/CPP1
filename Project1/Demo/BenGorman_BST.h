#pragma once
#include "LibHeaders.h"

namespace BenGorman_BST {

template <typename T>
class BSTree
{
public:
	BSTree() :
		root(nullptr)
	{

	}
	void Print() const;
	void Insert(T val);
	bool Contains(T val) const;
	void Remove(T val);

private:
	struct TreeNode
	{
		T data;
		std::unique_ptr<TreeNode> left;
		std::unique_ptr<TreeNode> right;

		TreeNode(T data) :
			data(data),
			left(nullptr),
			right(nullptr)
		{

		}
	};

	std::unique_ptr<TreeNode> root;
	std::string SubTreeAsString(const std::unique_ptr<TreeNode>& node) const;
	void Insert(T val, std::unique_ptr<TreeNode>& node);
	bool Contains(T val, std::unique_ptr<TreeNode>& node) const;
	void Remove(T val, std::unique_ptr<TreeNode>& node);
	std::unique_ptr<TreeNode>& FindMin(std::unique_ptr<TreeNode>& node);
};



template<typename T>
void BSTree<T>::Print() const
{
	if (root == nullptr)
	{
		std::cout << "{}" << std::endl;
	}
	else
	{
		std::cout << SubTreeAsString(root) << std::endl;
	}
}


template<typename T>
std::string BSTree<T>::SubTreeAsString(const std::unique_ptr<TreeNode>& node) const
{
	std::string leftStr = (node->left == nullptr) ? "{}" : SubTreeAsString(node->left);
	std::string rightStr = (node->right == nullptr) ? "{}" : SubTreeAsString(node->right);
	std::string result = "{" + std::to_string(node->data) + "," + leftStr + "," + rightStr + "}";
	return result;
}


template<typename T>
void BSTree<T>::Insert(T val)
{
	Insert(val, root);
}


template<typename T>
bool BSTree<T>::Contains(T val) const
{
	return Contains(val, root);
}

template<typename T>
void BSTree<T>::Remove(T val)
{
	Remove(val, root);
}


template<typename T>
void BSTree<T>::Insert(T val, std::unique_ptr<TreeNode>& node)
{
	if (node == nullptr)
	{
		node = std::make_unique<TreeNode>(val);
	}
	else
	{
		if (val < node->data)
		{
			Insert(val, node->left);
		}
		else if (val > node->data)
		{
			Insert(val, node->right)
		}
		else
		{
			std::cout << "value already exists" << std::endl;
		}
	}
}


template<typename T>
bool BSTree<T>::Contains(T val, std::unique_ptr<TreeNode>& node) const
{
	if (node == nullptr)
	{
		return false;
	}
	else if (val == node->data)
	{
		return true;
	}
	else if (val < node->data)
	{
		return Contains(val, node->left);
	}
	else
	{
		return Contains(val, node->right);
	}
}


template<typename T>
void BSTree<T>::Remove(T val, std::unique_ptr<TreeNode>& node)
{
	if (node == nullptr)
	{
		std::cout << "val not found" << std::endl;
	}
	else if (val == node->data)
	{
		if (node->left == nullptr && node->right == nullptr)
		{
			node = nullptr;
		}
		else if (node->left != nullptr && node->right == nullptr)
		{
			node = std::move(node->left);
		}
		else if (node->right != nullptr && node->left == nullptr)
		{
			node = std::move(node->right);
		}
		else
		{
			std::unique_ptr<TreeNode>& minNode = FindMin(node->right);
			node->data = minNode->data;
			Remove(minNode->data, minNode);
		}
	}
	else if (val < node->data)
	{
		Remove(val, node->left);
	}
	else
	{
		Remove(val, node->right);
	}
}


template<typename T>
std::unique_ptr<typename BSTree<T>::TreeNode>& BSTree<T>::FindMin(std::unique_ptr<TreeNode>& node)
{
	if (node == nullptr)
	{
		throw "Min value not found";
	}
	else if (node->left == nullptr)
	{
		return node;
	}
	else
	{
		return FindMin(node->left);
	}
}



}