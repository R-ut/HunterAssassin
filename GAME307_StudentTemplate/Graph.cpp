#include "Graph.h"
#include <queue>
#include <map>
#include <cmath> // for heuristics
#include <VMath.h>


Graph::Graph()
{
}

Graph::~Graph()
{
}

bool Graph::OnCreate(std::vector<Node*> nodes_)
{
    // given a list of nodes, initialize a matrix of costs with 0.0
    int numNodes = nodes_.size();
    nodes = nodes_;
    cost.resize(numNodes);
    for (int i = 0; i < numNodes; i++)
    {
        // populate the internal map
        if (i != nodes_[i]->getLabel())
        {
            std::cerr << "node with label 'i' not in the 'i' position for " << i << "\n";
            return false;
        }
        nodes[i] = nodes_[i];

        // set up connections
        cost[i].resize(numNodes);

        for (int j = 0; j < numNodes; j++)
        {
            cost[i][j] = 0.0f;
        }
    }

    return true;
}

int Graph::numNodes()
{
    return nodes.size();
}

void Graph::addWeightedConnection(Node* fromNode, Node* toNode, float weight)
{
    cost[fromNode->getLabel()][toNode->getLabel()] = weight;
}

std::vector<Node*> Graph::neighbours(Node* fromNode)
{
    std::vector<Node*> result = {};
    int from = fromNode->getLabel();
    for (int j = 0; j < numNodes(); j++)
    {
        if (cost[from][j] > 0.0f)
        {
            result.push_back(getNode(j));
        }
    }
    return result;
}

struct NodeAndPriority
{
    Node* node;
    float priority;

    bool operator()(NodeAndPriority const& lhs, NodeAndPriority const& rhs)
    {
        return lhs.priority > rhs.priority;
    }
};


float heuristic(Node* a, Node* b) {
    // Since there is no positional data, using the label difference as a simple heuristic
    return std::abs(a->getLabel() - b->getLabel());
    //hint: label/cols;
    //hint: label / rows;
}

std::vector<Node*> Graph::findPath(Node* startNode, Node* goalNode, std::vector<Node*> &exploredNodes)
{
    std::vector<Node*> result;

    // declarations
    float new_cost;
    float priority;
    Node* currentNode = startNode;

    // frontier
    std::priority_queue<NodeAndPriority, std::deque<NodeAndPriority>, NodeAndPriority > frontier;
    frontier.push(NodeAndPriority{ currentNode, 0.0f });

    // track solution path (came_from)
    std::vector<int> came_from;
    came_from.resize(numNodes(), -1);

    // cost so far storage
    std::map<int, float> cost_so_far;
    int start = startNode->getLabel();
    int goal = goalNode->getLabel();
    cost_so_far[start] = 0.0f;

    // Loop through the frontier while it is not empty
    while (!frontier.empty())
    {
        // Get the top node and save it in currentNode, then pop the node from the frontier
        currentNode = frontier.top().node;
        frontier.pop();
        exploredNodes.push_back(currentNode);
        // If the goal is reached, break out
        if (currentNode == goalNode)
        {
            break;
        }

        // For the neighbours of the current node
        for (Node* nextNode : neighbours(currentNode))
        {
            int nextLabel = nextNode->getLabel();
            new_cost = cost_so_far[currentNode->getLabel()] + cost[currentNode->getLabel()][nextLabel];

            // If the neighbour is not in cost_so_far or the new cost is lower
            if (cost_so_far.find(nextLabel) == cost_so_far.end() || new_cost < cost_so_far[nextLabel])
            {
                // Update data structures
                cost_so_far[nextLabel] = new_cost;
                priority = new_cost + heuristic(nextNode, goalNode);
                frontier.push(NodeAndPriority{ nextNode, priority });
                came_from[nextLabel] = currentNode->getLabel();
            }
        }
    }

    // Follow the breadcrumbs in came_from to rebuild the path, store it in result
    int current = goal;
    if (came_from[current] != -1 || current == start)
    {
        while (current != -1)
        {
            result.push_back(getNode(current));
            current = came_from[current];
        }
        std::reverse(result.begin(), result.end());
    }

    return result;
}
