
//
//  Graph.cpp
//  Example Non-directed, non-weighted graph
//
//  Created by Jim Bailey on 11/25/17.
//  Licensed under a Creative Commons Attribution 4.0 International License.
//

#include <iostream>

#include "WGraph.h"
#include "PQueue.h"

// for exception handling
#include <stdexcept>

// to create formatted strings to return
#include <sstream>
#include <iomanip>

// for breadth first and depth first traversals
#include <queue>
#include <stack>

/*
 * definitions here for reference
 *
struct Edge
{
    int endIndex;
    Edge * next;
};

struct Node
{
    char name;
    bool visited;
    Edge * connects;
};
 *
 */

    // constructor
WGraph::WGraph()
{
        // initialize number of nodes in list
    numNodes = 0;

        // initialize nodeList to nullptrs
    for(int i = 0; i < SIZE; i++)
        nodeList[i] = nullptr;

        // set up edge Matrix to start with no edges
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
            edgeMatrix[i][j] = 0;
}

    // destructor
WGraph::~WGraph()
{
    // delete all connections from each node in nodeList
    for(int i = 0; i < numNodes; i++)
    {
            // similar to destructor on linked list
        Edge * ptr = nodeList[i]->connects;
        while(ptr != nullptr)
        {
            Edge * temp = ptr;
            ptr = ptr->next;
            delete temp;
        }
    }
}

    // add a new node to the graph
    // only failure is if graph arrays are full
void WGraph::addNode(char name)
{
        // alternately, double the size of everything and start over
    if( numNodes >= SIZE )
        throw std::overflow_error("Graph size exceeded!!");

        // create a node with this name
        // initialize it with no edges and not yet visited
    Node * temp = new Node;
    temp->name = name;
    temp->visited = false;
    temp->connects = nullptr;

        // add to the list of nodes in graph
    nodeList[numNodes++] = temp;
}

bool WGraph::addWEdge(char startName, char endName, int weight) {

    if(startName == endName)
        return false;

    int startIndex = findNode(startName);
    int endIndex = findNode(endName);

    if(startIndex == -1 || endIndex == -1)
        return false;

    edgeMatrix[startIndex][endIndex] = weight;
    edgeMatrix[endIndex][startIndex] = weight;

    Edge *startEnd = new Edge;
    startEnd->endIndex = endIndex;
    startEnd->weight = weight;
    startEnd->next = nullptr;
    startEnd->next = nodeList[startIndex]->connects;
    nodeList[startIndex]->connects = startEnd;

    Edge *endStart = new Edge;
    endStart->endIndex = startIndex;
    endStart->weight = weight;
    endStart->next = nullptr;
    endStart->next = nodeList[endIndex]->connects;
    nodeList[endIndex]->connects = endStart;

    return true;
}

// linear search for a node with this name
// return -1 if not found
// otherwise return its index in the nodeList
int WGraph::findNode(char name)
{
    for(int i = 0; i < numNodes; i++)
        if ( nodeList[i]->name == name )
            return i;
    return -1;
}


    // listing of nodes in the order
    // they were added to the graph
std::string WGraph::listNodes()
{
    std::string theList = "";
    for(int i = 0; i < numNodes; i++)
    {
        theList += nodeList[i]->name;
        theList += " ";
    }
    return theList;
}


    // for each node in graph,
    // display its edges, using the edgelist
std::string WGraph::displayWEdges()
{
    std::stringstream ss;
    for(int i = 0; i < numNodes; i++)
    {
            // add the node name to the display
        ss << nodeList[i]->name;
        ss << "-";

            // walk down its list of edges and add them also
        Edge * ptr = nodeList[i]->connects;
        while ( ptr != nullptr )
        {
            ss << nodeList[ptr->endIndex]->name;
            ss << " ";
            ptr = ptr->next;
        }
            // end this line of output
        ss << std::endl;
    }
    return ss.str();
}


    // display the adjacency matrix
    // as 0 for no connection and 1 for connection
std::string WGraph::displayMatrix()
{
    std::stringstream ss;

        // output header line of destinations
    ss << std::setw(2) << " ";
    for(int i = 0; i < numNodes; i++)
        ss << std::setw(4) << nodeList[i]->name;
    ss << std::endl;

        // now output the array
    for(int i = 0; i < numNodes; i++)
    {
            // add the starting node
        ss << std::setw(2) << nodeList[i]->name;

            // now add its connections
        for(int j = 0; j < numNodes; j++)
            ss << std::setw(4) << edgeMatrix[i][j];

            // end the row
        ss << std::endl;
    }
    return ss.str();
}

// depth first traversal
// starts at a given node
// outputs a list of nodes visited
// and a list of any unreached nodes
std::string WGraph::depthFirst(char name)
{
    std::string buffer = "Depth first traversal starting at ";
    buffer += name;
    buffer += "\n";

    Node *startingNode = nullptr;

    int index = findNode(name);

    // Returns an empty string if the node
    // wasn't found.
    if(index == -1)
        return "";

    startingNode = nodeList[index];

    std::stack<Node *> LIFO;

    LIFO.push(startingNode);

    startingNode->visited = true;

    buffer += startingNode->name;
    buffer += " : ";

    while(!LIFO.empty()) {

        Node *currentNode = LIFO.top();

        LIFO.pop();

        for(Edge *ptr = currentNode->connects; ptr != nullptr; ptr = ptr->next) {
            Node *otherEnd = nodeList[ptr->endIndex];

            if(!otherEnd->visited) {
                LIFO.push(currentNode);
                LIFO.push(otherEnd);

                buffer += otherEnd->name;
                buffer += " ";

                otherEnd->visited = true;
                break;
            }
        }
    }

    return buffer;
}

// breadth first traversal
// starts at a given node
// outputs a list of nodes visited
// and a list of any unreached nodes
std::string WGraph::breadthFirst(char name)
{
    std::string buffer = "Breadth first traversal starting at ";
    buffer += name;
    buffer += "\n";

    Node *startingNode = nullptr;

    int index = findNode(name);

    // Returns an empty string if the node
    // wasn't found.
    if(index == -1)
        return "";

    startingNode = nodeList[index];

    std::queue<Node *> FIFO;

    FIFO.push(startingNode);

    startingNode->visited = true;

    // Pretty formatting for the start of
    // the list.
    buffer += startingNode->name;
    buffer += " : ";

    while(!FIFO.empty()) {

        Node *currentNode = FIFO.front();

        FIFO.pop();

        for(Edge *ptr = currentNode->connects; ptr != nullptr; ptr = ptr->next) {

            Node *otherEnd = nodeList[ptr->endIndex];

            if(!otherEnd->visited) {

                FIFO.push(otherEnd);

                buffer += otherEnd->name;
                buffer += " ";

                otherEnd->visited = true;
            }
        }
    }

    resetVisited();

    buffer += "\n";

    return buffer;
}

// helper function to reset all nodes to not visited
void WGraph::resetVisited()
{
    for(int i = 0; i < numNodes; i++)
        nodeList[i]->visited = false;
}

std::string WGraph::minCostTree(char start) {

    std::string minCost = "";

    PQueue pq;

    Node *startNode = nodeList[findNode(start)];
    startNode->visited = true;

    for(Edge *temp = startNode->connects; temp != nullptr; temp = temp->next)
        pq.addEdge(temp);

    minCost += startNode->name;
    minCost += " : ";

    Node *current = startNode;
    Node *lastNode = current;
    Edge *shortest;

    while(!pq.isEmpty()) {

        lastNode = current;

        shortest = pq.removeEdge();
        current = nodeList[shortest->endIndex];
        current->visited = true;

        minCost += lastNode->name;
        minCost += "-";
        minCost += current->name;
        minCost += " ";

        for(Edge *currentEdge = shortest->next; currentEdge != nullptr; currentEdge = currentEdge->next) {
            if(!(nodeList[currentEdge->endIndex]->visited))
                pq.addQueue(currentEdge);
        }
    }

    resetVisited();

    return minCost;
}
