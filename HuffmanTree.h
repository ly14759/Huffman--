#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <assert.h>

using namespace std;

template<class W>
struct HufmanTreeNode
{
	W _weight;   //权
	HufmanTreeNode<W>* _left;
	HufmanTreeNode<W>* _right;
	HufmanTreeNode<W>* _parent;

	HufmanTreeNode<W>(const W& w)
		:_weight(w)
		, _left(NULL)
		, _right(NULL)
		, _parent(NULL)
	{
		;
	}
};

template<class W>
class HufmanTree
{
	typedef HufmanTreeNode<W> Node;
public:
	HufmanTree()
		: _root(NULL)
	{}
	HufmanTree(W* array, size_t size, const W& invalid)
	{	
		// 空间开辟，内存管理 方面的基本语法概念太差。。
		Node **arr = new Node*[size];
		//Node* arr[10];
		for (size_t i = 0; i < size; ++i)
		{
			arr[i] = new Node(array[i]);
		}
		//冒泡排序
		for (size_t i = 0; i < size - 1; ++i)
		{
			for (size_t j = 0; j < size - i - 1; ++j)
			{
				if (arr[j]->_weight > arr[j + 1]->_weight)
				{
					Node* tmp = arr[j];
					arr[j] = arr[j + 1];
					arr[j + 1] = tmp;
				}
			}
		}
		//去非法操作
		size_t i = 0;
		for (i; i < size; ++i)
		{
			if (arr[i]->_weight != invalid)
				break;
		}
		for (size_t j = 0; j < size - i; ++j)
		{
			arr[j] = arr[j + i];
		}
		size = size - i;
		//// 空间开辟，内存管理 方面的基本语法概念太差。。
		//Node **arr = new Node*[size];
		////Node* arr[10];
		//for (size_t i = 0; i < size; ++i)
		//{
		//	arr[i] = new Node(array[i]);
		//}
		while (size != 1)
		{
			for (size_t i = 0; i < size - 1; ++i)
			{
				if (arr[i]->_weight > arr[i + 1]->_weight)
				{
					Node* tmp = arr[i];
					arr[i] = arr[i + 1];
					arr[i + 1] = tmp;
				}
			}
			Node* parent = new Node(arr[0]->_weight + arr[1]->_weight);
			parent->_left = arr[0];
			parent->_right = arr[1];
			arr[0]->_parent = parent;
			arr[1]->_parent = parent;
			arr[0] = parent;
			for (size_t i = 1; i < size - 1; ++i)
				arr[i] = arr[i + 1];
			size--;
		}
		_root = arr[0];
	}
	//void _CreateHufmanTree(W* array, size_t size, const W& invalid)  //给一个序列
	//{
	//	struct Compare
	//	{
	//		bool operator()(Node* left, Node* right)
	//		{
	//			return left->_weight < right->_weight;
	//		}
	//	};
	//	Heap<Node*, Compare> hp;
	//	//放堆，将权放入
	//	for (size_t i = 0; i < size; ++i)
	//	{
	//		if (array[size] != invalid)
	//		{
	//			hp.Push(new HufmanTreeNode<W>(array[i]));
	//		}
	//		while (hp.Size() > 1)
	//		{
	//			Node* left = hp.Top();
	//			hp.Pop();
	//			Node* right = hp.Top();
	//			hp.Pop();
	//			Node* parent = new Node(left->_weight + right->_weight);
	//			parent->_left = left;
	//			parent->_right = right;
	//			hp.Push(parent);//..................
	//		}
	//		_root = hp.Top();
	//	}
	//}


	//void _CreateHufmanTree(W* array, size_t size, const W& invalid) //使用结构体指针数组构造
	//{
	//	// 空间开辟，内存管理 方面的基本语法概念太差。。
	//	Node **arr = new Node*[size];
	//	//Node* arr[10];
	//	for (size_t i = 0; i < size; ++i)
	//	{
	//		arr[i] = new Node(array[i]);
	//	}
	//	while (size != 1)
	//	{
	//		for (int i = 0; i < size - 1; ++i)
	//		{
	//			if (arr[i]->_weight > arr[i + 1]->_weight)
	//			{
	//				Node* tmp = arr[i];
	//				arr[i] = arr[i + 1];
	//				arr[i + 1] = tmp;
	//			}
	//		}
	//		Node* parent = new Node(arr[0]->_weight + arr[1]->_weight);
	//		parent->_left = arr[0];
	//		parent->_right = arr[1];
	//		arr[0]->_parent = parent;
	//		arr[1]->_parent = parent;
	//		arr[0] = parent;
	//		for (int i = 1; i < size - 1; ++i)
	//			arr[i] = arr[i + 1];
	//		size--;
	//	}
	//	_root = arr[0];
	//}
	~HufmanTree()
	{
		_Destroy(_root);
	}
	void _Destroy(Node*& root)
	{
		if (root)
		{
			_Destroy(root->_left);
			_Destroy(root->_right);
			delete root;
			root = NULL;
		}
	}

	void HufmanCoding()
	{
		//1.找到叶子节点
		//   递归遍历 左为空，右为空
		//2.根据父指针，从下往上写编码，最后反一下。
	}
	Node*& GetRoot()
	{
		return _root;
	}
protected:
	//HuffmanTree(const HuffmanTree<W>&);
	//HuffmanTree<W>& operator=(const HuffmanTree<W>&);
	Node* _root;
};

void testhumT()
{
	int array[] = { 9,12,6,3,5,15 };
	HufmanTree<int> ht(array, sizeof(array) / sizeof(array[0]), 0);
}
