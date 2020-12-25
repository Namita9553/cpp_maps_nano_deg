#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    // Store the nodes you find in the RoutePlanner's start_node and end_node attributes.
    start_node = &(model.FindClosestNode(start_x, start_y));
    end_node = &(model.FindClosestNode(end_x,end_y));
}


// TODO 3: Implement the CalculateHValue method.
// Tips:
// - You can use the distance to the end_node for the h value.
// - Node objects have a distance method to determine the distance to another node.

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    float h = node->distance(*end_node);
    return h;

}


// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
// Tips:
// - Use the FindNeighbors() method of the current_node to populate current_node.neighbors vector with all the neighbors.
// - For each node in current_node.neighbors, set the parent, the h_value, the g_value. 
// - Use CalculateHValue below to implement the h-Value calculation.
// - For each node in current_node.neighbors, add the neighbor to open_list and set the node's visited attribute to true.

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();
    for(RouteModel::Node *x : current_node->neighbors) {
        if( !(x->visited)) {
        x->h_value = CalculateHValue(x);
        x->parent=current_node;
        x->visited=true;
        x->g_value=current_node->g_value+1;
        open_list.push_back(x);
        }
    }
}


// TODO 5: Complete the NextNode method to sort the open list and return the next node.
// Tips:
// - Sort the open_list according to the sum of the h value and g value.
// - Create a pointer to the node in the list with the lowest sum.
// - Remove that node from the open_list.
// - Return the pointer.
bool cpmr(RouteModel::Node*lhs,RouteModel::Node*rhs) {
    return lhs->g_value+lhs->h_value<rhs->g_value+rhs->h_value;
}
RouteModel::Node *RoutePlanner::NextNode() {
    sort(open_list.begin(),open_list.end(), cpmr);

    RouteModel::Node* tmp= open_list[0];
    open_list.erase(open_list.begin());
    return tmp;
}


// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
// Tips:
// - This method should take the current (final) node as an argument and iteratively follow the 
//   chain of parents of nodes until the starting node is found.
// - For each node in the chain, add the distance from the node to its parent to the distance variable.
// - The returned vector should be in the correct order: the start node should be the first element
//   of the vector, the end node should be the last element.

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    while (!(current_node->x==start_node->x && current_node->y==start_node->y))
    {
       distance+= current_node->distance(*(current_node->parent)) ;
       path_found.push_back(*current_node);
    }
    path_found.push_back(*current_node);

    reverse(path_found.begin(),path_found.end());
    // TODO: Implement your solution here.

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}


// TODO 7: Write the A* Search algorithm here.
// Tips:
// - Use the AddNeighbors method to add all of the neighbors of the current node to the open_list.
// - Use the NextNode() method to sort the open_list and return the next node.
// - When the search has reached the end_node, use the ConstructFinalPath method to return the final path that was found.
// - Store the final path in the m_Model.path attribute before the method exits. This path will then be displayed on the map tile.

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    current_node =start_node;
    current_node->g_value=0;
    current_node->visited =true;
    current_node->h_value= CalculateHValue(current_node);
    open_list.clear();
    open_list.push_back(current_node);
    while (!open_list.empty())
    {
        current_node = NextNode();
        AddNeighbors(current_node);
       if(current_node->x==end_node->x && current_node->y==end_node->y) {
           break;
       }
    }
    m_Model.path=ConstructFinalPath(current_node);
    // TODO: Implement your solution here.

}