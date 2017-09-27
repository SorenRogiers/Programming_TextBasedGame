#pragma once

#include <vector>

template <class T>
class Node
{
public:
	Node(T data)
	{
		this->m_Data = data;
	}

	//Members
	T m_Data;
};

template <class T>
class Edge
{
public:
	Edge(Node<T>* startNode, Node<T>* endNode, double weight = 0.0)
	{
		this->m_Nodes[0] = startNode;
		this->m_Nodes[1] = endNode;
		this->m_Weight = weight;
	}

	//Members
	Node<T>* m_Nodes[2];
	double m_Weight = 0.0;
};

template <class T>
class Graph
{
public:
	Graph() {}

	//Add a node to the graph without forming edges
	void AddNode(T data)
	{
		Node<T> node(data);
		this->m_Nodes.push_back(node);
	}

	//Add an edge between 2 nodes
	void AddEdge(Node<T>* startNode, Node<T>* endNode, bool isDirected = true, double weight = 0.0)
	{
		Edge<T> edgeA(startNode, endNode, weight);
		this->m_Edges.push_back(edgeA);

		if (!isDirected)
		{
			Edge<T> edgeB(endNode, startNode, weight);
			this->m_Edges.push_back(edgeB);
		}
	}

	void AddEdge(unsigned int start, unsigned int end, bool directed = true, double weight = 0.0)
	{
		// Convert the array indices into pointers
		Node<T>* startPtr = &this->m_Nodes[start];
		Node<T>* endPtr = &this->m_Nodes[end];
		// Call the usual function to add the edge
		AddEdge(startPtr, endPtr, directed, weight);
	}

	//Add a node to the graph with an edge from parent to child
	void AddNode(T data, Node<T>* parentNode, bool isDirected = true, double weight = 0.0)
	{
		Node<T> node(data);
		this->m_Nodes.push_back(node);
		this->AddEdge(parentNode, &(this->m_Nodes.back()), isDirected, weight);
	}

	//Members
	std::vector<Node<T>> m_Nodes;
	std::vector<Edge<T>> m_Edges;
};
