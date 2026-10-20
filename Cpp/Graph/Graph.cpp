#include <iostream>
#include <cstdlib>
#include <climits>
using namespace std;

struct Edge {
    int destination;
    int weight;
    Edge* next;

    Edge(int Destination, int Weight)
    {
        destination = Destination;
        weight = Weight;
        next = nullptr;
    }
};


class Graph_Number {
private:

    Edge** adjacencyList;
    int vertices;
    int vertexCapacity;
    int edges;
    bool directed;
    bool weighted;

private:

    bool is_valid_vertex(int vertex)
    {
        return vertex >= 0 && vertex < vertices;
    }


    Edge* find_edge(int source, int destination)
    {
        Edge* current = adjacencyList[source];

        while (current != nullptr)
        {
            if (current->destination == destination)
            {
                return current;
            }
            current = current->next;
        }

        return nullptr;
    }


    bool remove_one_edge(int source, int destination)
    {
        Edge* current = adjacencyList[source];
        Edge* previous = nullptr;

        while (current != nullptr)
        {
            if (current->destination == destination)
            {
                if (previous == nullptr)
                {
                    adjacencyList[source] = current->next;
                }
                else
                {
                    previous->next = current->next;
                }

                delete current;
                return true;
            }

            previous = current;
            current = current->next;
        }

        return false;
    }


    void clear_edges()
    {
        for (int i = 0; i < vertices; i++)
        {
            Edge* current = adjacencyList[i];

            while (current != nullptr)
            {
                Edge* nextEdge = current->next;
                delete current;
                current = nextEdge;
            }

            adjacencyList[i] = nullptr;
        }

        edges = 0;
    }


    void ensure_vertex_capacity(int requiredCapacity)
    {
        if (requiredCapacity <= vertexCapacity)
        {
            return;
        }

        int newCapacity = (vertexCapacity > 0) ? vertexCapacity : 1;
        while (newCapacity < requiredCapacity)
        {
            newCapacity *= 2;
        }

        Edge** newAdjacencyList = new Edge*[newCapacity];
        for (int i = 0; i < vertices; i++)
        {
            newAdjacencyList[i] = adjacencyList[i];
        }
        for (int i = vertices; i < newCapacity; i++)
        {
            newAdjacencyList[i] = nullptr;
        }

        delete[] adjacencyList;
        adjacencyList = newAdjacencyList;
        vertexCapacity = newCapacity;
    }


    int breadth_first_search(int start, int* order, bool* visited)
    {
        int* queue = new int[vertices];
        int front = 0;
        int rear = 0;
        int count = 0;

        visited[start] = true;
        queue[rear++] = start;

        while (front < rear)
        {
            int currentVertex = queue[front++];
            order[count++] = currentVertex;

            Edge* currentEdge = adjacencyList[currentVertex];
            while (currentEdge != nullptr)
            {
                int neighbor = currentEdge->destination;

                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    queue[rear++] = neighbor;
                }

                currentEdge = currentEdge->next;
            }
        }

        delete[] queue;
        return count;
    }


    void depth_first_search(int vertex, bool* visited, int* order, int& count)
    {
        visited[vertex] = true;
        order[count++] = vertex;

        Edge* current = adjacencyList[vertex];
        while (current != nullptr)
        {
            if (!visited[current->destination])
            {
                depth_first_search(current->destination, visited, order, count);
            }

            current = current->next;
        }
    }

public:

    // Constructor
    // Creates a graph with numbered vertices from 0 to Vertices - 1
    Graph_Number(int Vertices, bool Is_Directed = false, bool Is_Weighted = false)
    {
        vertices = (Vertices > 0) ? Vertices : 0;
        vertexCapacity = vertices;
        edges = 0;
        directed = Is_Directed;
        weighted = Is_Weighted;

        adjacencyList = (vertices > 0) ? new Edge*[vertices] : nullptr;

        for (int i = 0; i < vertices; i++)
        {
            adjacencyList[i] = nullptr;
        }
    }


    // Destructor
    // Every adjacency node was created with new, so every node must be deleted
    ~Graph_Number()
    {
        clear_edges();
        delete[] adjacencyList;
    }


    // Copy Constructor
    // Creates a deep copy of another graph
    Graph_Number(const Graph_Number& other)
    {
        vertices = other.vertices;
        vertexCapacity = other.vertexCapacity;
        edges = other.edges;
        directed = other.directed;
        weighted = other.weighted;
        adjacencyList = (vertexCapacity > 0) ? new Edge*[vertexCapacity] : nullptr;

        for (int i = 0; i < vertexCapacity; i++)
        {
            adjacencyList[i] = nullptr;
        }

        for (int i = 0; i < vertices; i++)
        {
            Edge* current = other.adjacencyList[i];
            Edge* last = nullptr;

            while (current != nullptr)
            {
                Edge* newEdge = new Edge(current->destination, current->weight);

                if (last == nullptr)
                {
                    adjacencyList[i] = newEdge;
                }
                else
                {
                    last->next = newEdge;
                }

                last = newEdge;
                current = current->next;
            }
        }
    }


    // Assignment Operator
    // Performs deep copy between graphs
    Graph_Number& operator=(const Graph_Number& other)
    {
        if (this == &other)
        {
            return *this;
        }

        Edge** newAdjacencyList = (other.vertexCapacity > 0)
            ? new Edge*[other.vertexCapacity]
            : nullptr;

        for (int i = 0; i < other.vertexCapacity; i++)
        {
            newAdjacencyList[i] = nullptr;
        }

        for (int i = 0; i < other.vertices; i++)
        {
            Edge* current = other.adjacencyList[i];
            Edge* last = nullptr;

            while (current != nullptr)
            {
                Edge* newEdge = new Edge(current->destination, current->weight);

                if (last == nullptr)
                {
                    newAdjacencyList[i] = newEdge;
                }
                else
                {
                    last->next = newEdge;
                }

                last = newEdge;
                current = current->next;
            }
        }

        clear_edges();
        delete[] adjacencyList;

        adjacencyList = newAdjacencyList;
        vertices = other.vertices;
        vertexCapacity = other.vertexCapacity;
        edges = other.edges;
        directed = other.directed;
        weighted = other.weighted;

        return *this;
    }


    // Add a new vertex and return its number
    // Existing vertex numbers never change
    int add_vertex()
    {
        ensure_vertex_capacity(vertices + 1);
        adjacencyList[vertices] = nullptr;
        return vertices++;
    }


    // Add an edge between two existing vertices
    // For an unweighted graph, the edge weight is always 1
    // Negative weights are rejected so Dijkstra's shortest path stays valid
    bool add_edge(int source, int destination, int weight = 1)
    {
        if (!is_valid_vertex(source) || !is_valid_vertex(destination) || weight < 0)
        {
            return false;
        }

        if (find_edge(source, destination) != nullptr)
        {
            return false;
        }

        if (!directed && source != destination
            && find_edge(destination, source) != nullptr)
        {
            return false;
        }

        int edgeWeight = weighted ? weight : 1;
        Edge* forwardEdge = new Edge(destination, edgeWeight);
        Edge* reverseEdge = nullptr;

        if (!directed)
        {
            reverseEdge = new Edge(source, edgeWeight);
        }

        forwardEdge->next = adjacencyList[source];
        adjacencyList[source] = forwardEdge;

        if (reverseEdge != nullptr)
        {
            reverseEdge->next = adjacencyList[destination];
            adjacencyList[destination] = reverseEdge;
        }

        edges++;
        return true;
    }


    // Remove an edge between two existing vertices
    bool remove_edge(int source, int destination)
    {
        if (!is_valid_vertex(source) || !is_valid_vertex(destination)
            || find_edge(source, destination) == nullptr)
        {
            return false;
        }

        remove_one_edge(source, destination);

        if (!directed)
        {
            remove_one_edge(destination, source);
        }

        edges--;
        return true;
    }


    // Check whether an edge exists
    bool has_edge(int source, int destination)
    {
        if (!is_valid_vertex(source) || !is_valid_vertex(destination))
        {
            return false;
        }

        return find_edge(source, destination) != nullptr;
    }


    // Return the edge weight through weight
    // For an unweighted graph, the returned weight is 1
    bool get_edge_weight(int source, int destination, int& weight)
    {
        if (!is_valid_vertex(source) || !is_valid_vertex(destination))
        {
            return false;
        }

        Edge* edge = find_edge(source, destination);
        if (edge == nullptr)
        {
            return false;
        }

        weight = edge->weight;
        return true;
    }


    // Return the number of outgoing edges from a vertex
    int get_degree(int vertex)
    {
        if (!is_valid_vertex(vertex))
        {
            return -1;
        }

        int degree = 0;
        Edge* current = adjacencyList[vertex];

        while (current != nullptr)
        {
            degree++;
            current = current->next;
        }

        return degree;
    }


    // Print all vertices reachable from start using breadth-first search
    int display_breadth_first(int start)
    {
        if (!is_valid_vertex(start))
        {
            cout << "Invalid starting vertex." << endl;
            return 0;
        }

        int* order = new int[vertices];
        bool* visited = new bool[vertices];

        for (int i = 0; i < vertices; i++)
        {
            visited[i] = false;
        }

        int count = breadth_first_search(start, order, visited);

        for (int i = 0; i < count; i++)
        {
            cout << order[i] << " ";
        }
        cout << endl;

        delete[] order;
        delete[] visited;
        return count;
    }


    // Print all vertices reachable from start using depth-first search
    int display_depth_first(int start)
    {
        if (!is_valid_vertex(start))
        {
            cout << "Invalid starting vertex." << endl;
            return 0;
        }

        int* order = new int[vertices];
        bool* visited = new bool[vertices];

        for (int i = 0; i < vertices; i++)
        {
            visited[i] = false;
        }

        int count = 0;
        depth_first_search(start, visited, order, count);

        for (int i = 0; i < count; i++)
        {
            cout << order[i] << " ";
        }
        cout << endl;

        delete[] order;
        delete[] visited;
        return count;
    }


    // Check whether a path exists from source to destination
    bool has_path(int source, int destination)
    {
        if (!is_valid_vertex(source) || !is_valid_vertex(destination))
        {
            return false;
        }

        bool* visited = new bool[vertices];
        for (int i = 0; i < vertices; i++)
        {
            visited[i] = false;
        }

        int* order = new int[vertices];
        int count = breadth_first_search(source, order, visited);
        bool found = visited[destination];

        delete[] order;
        delete[] visited;
        return found && count > 0;
    }


    // Find the shortest path using Dijkstra's algorithm
    // Returns false for invalid vertices, missing paths, or negative edges
    bool shortest_path(int source, int destination, long long& distance,
        int* path, int pathCapacity, int& pathLength)
    {
        pathLength = 0;

        if (!is_valid_vertex(source) || !is_valid_vertex(destination)
            || pathCapacity < 0 || (pathCapacity > 0 && path == nullptr))
        {
            return false;
        }

        for (int i = 0; i < vertices; i++)
        {
            Edge* edge = adjacencyList[i];
            while (edge != nullptr)
            {
                if (edge->weight < 0)
                {
                    return false;
                }
                edge = edge->next;
            }
        }

        long long* distances = new long long[vertices];
        int* previous = new int[vertices];
        bool* visited = new bool[vertices];

        for (int i = 0; i < vertices; i++)
        {
            distances[i] = LLONG_MAX;
            previous[i] = -1;
            visited[i] = false;
        }

        distances[source] = 0;

        for (int step = 0; step < vertices; step++)
        {
            int currentVertex = -1;

            for (int i = 0; i < vertices; i++)
            {
                if (!visited[i] && distances[i] != LLONG_MAX
                    && (currentVertex == -1 || distances[i] < distances[currentVertex]))
                {
                    currentVertex = i;
                }
            }

            if (currentVertex == -1)
            {
                break;
            }

            if (currentVertex == destination)
            {
                break;
            }

            visited[currentVertex] = true;
            Edge* edge = adjacencyList[currentVertex];

            while (edge != nullptr)
            {
                int neighbor = edge->destination;

                if (!visited[neighbor]
                    && distances[currentVertex] <= LLONG_MAX - edge->weight)
                {
                    long long newDistance = distances[currentVertex] + edge->weight;

                    if (newDistance < distances[neighbor])
                    {
                        distances[neighbor] = newDistance;
                        previous[neighbor] = currentVertex;
                    }
                }

                edge = edge->next;
            }
        }

        if (distances[destination] == LLONG_MAX)
        {
            delete[] distances;
            delete[] previous;
            delete[] visited;
            return false;
        }

        int requiredPathLength = 1;
        int currentVertex = destination;

        while (currentVertex != source)
        {
            currentVertex = previous[currentVertex];
            if (currentVertex == -1)
            {
                delete[] distances;
                delete[] previous;
                delete[] visited;
                return false;
            }
            requiredPathLength++;
        }

        if (pathCapacity < requiredPathLength)
        {
            pathLength = requiredPathLength;
            delete[] distances;
            delete[] previous;
            delete[] visited;
            return false;
        }

        currentVertex = destination;
        for (int i = requiredPathLength - 1; i >= 0; i--)
        {
            path[i] = currentVertex;
            if (currentVertex == source)
            {
                break;
            }
            currentVertex = previous[currentVertex];
        }

        distance = distances[destination];
        pathLength = requiredPathLength;

        delete[] distances;
        delete[] previous;
        delete[] visited;
        return true;
    }


    // Count connected components in an undirected graph
    int get_component_count()
    {
        if (directed)
        {
            return -1;
        }

        bool* visited = new bool[vertices];
        int* order = new int[vertices];

        for (int i = 0; i < vertices; i++)
        {
            visited[i] = false;
        }

        int components = 0;
        for (int i = 0; i < vertices; i++)
        {
            if (!visited[i])
            {
                breadth_first_search(i, order, visited);
                components++;
            }
        }

        delete[] order;
        delete[] visited;
        return components;
    }


    // Print each vertex and its adjacent vertices
    int display()
    {
        if (vertices == 0)
        {
            cout << "Graph has no vertices." << endl;
            return 0;
        }

        for (int i = 0; i < vertices; i++)
        {
            cout << i << ": ";
            Edge* current = adjacencyList[i];

            if (current == nullptr)
            {
                cout << "No connections";
            }

            while (current != nullptr)
            {
                cout << current->destination;
                if (weighted)
                {
                    cout << " (weight " << current->weight << ")";
                }
                if (current->next != nullptr)
                {
                    cout << " -> ";
                }
                current = current->next;
            }
            cout << endl;
        }

        return 1;
    }


    // Remove every edge; vertices and graph settings stay the same
    int clear()
    {
        clear_edges();
        return 1;
    }


    // Check whether the graph has no vertices
    bool is_empty()
    {
        return vertices == 0;
    }


    // Return number of vertices
    int get_vertex_count()
    {
        return vertices;
    }


    // Return number of edges (undirected edges are counted once)
    int get_edge_count()
    {
        return edges;
    }


    // Check whether this is a directed graph
    bool is_directed()
    {
        return directed;
    }


    // Check whether this is a weighted graph
    bool is_weighted()
    {
        return weighted;
    }
};


void read_input(int& value)
{
    if (!(cin >> value))
    {
        cerr << "Input error: expected an integer." << endl;
        std::exit(EXIT_FAILURE);
    }
}


int main()
{
    cout << "============================================" << endl;
    cout << "          Welcome To Graph Application" << endl;
    cout << "============================================" << endl;

    int vertexCount;
    do {
        cout << "\nEnter number of vertices: ";
        read_input(vertexCount);
        if (vertexCount < 0) {
            cout << "Number of vertices cannot be negative!" << endl;
        }
    } while (vertexCount < 0);

    int choice;
    do {
        cout << "\n1. Undirected Graph" << endl;
        cout << "2. Directed Graph" << endl;
        cout << "Enter graph type: ";
        read_input(choice);
        if (choice != 1 && choice != 2) {
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 1 && choice != 2);

    bool directed = (choice == 2);
    do {
        cout << "\n1. Unweighted Graph" << endl;
        cout << "2. Weighted Graph" << endl;
        cout << "Enter weight type: ";
        read_input(choice);
        if (choice != 1 && choice != 2) {
            cout << "Invalid choice!" << endl;
        }
    } while (choice != 1 && choice != 2);

    Graph_Number graph(vertexCount, directed, choice == 2);
    int mainChoice;

    do {
        cout << "\n\n============================================" << endl;
        cout << "                 MAIN MENU" << endl;
        cout << "============================================" << endl;
        cout << "1. Basic Operations" << endl;
        cout << "2. Search Operations" << endl;
        cout << "3. Traversal Operations" << endl;
        cout << "4. Graph Operations" << endl;
        cout << "5. Graph Information" << endl;
        cout << "6. Exit" << endl;
        cout << "============================================" << endl;
        cout << "Enter your choice: ";
        read_input(mainChoice);

        if (mainChoice == 1) {
            int operation;

            do {
                cout << "\n\n============================================" << endl;
                cout << "              BASIC OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Add Vertex" << endl;
                cout << "2. Add Edge" << endl;
                cout << "3. Remove Edge" << endl;
                cout << "4. Clear Graph" << endl;
                cout << "5. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(operation);

                if (operation == 1) {
                    int vertex = graph.add_vertex();
                    cout << "Vertex added successfully. Vertex number: " << vertex << endl;
                }
                else if (operation == 2 || operation == 3) {
                    int source;
                    int destination;
                    int weight = 1;
                    cout << "\nEnter source vertex (0 to " << (graph.get_vertex_count() - 1) << "): ";
                    read_input(source);
                    cout << "Enter destination vertex: ";
                    read_input(destination);

                    if (operation == 2 && graph.is_weighted()) {
                        cout << "Enter non-negative weight: ";
                        read_input(weight);
                    }

                    bool success = (operation == 2)
                        ? graph.add_edge(source, destination, weight)
                        : graph.remove_edge(source, destination);

                    if (success) {
                        cout << (operation == 2 ? "Edge added successfully." : "Edge removed successfully.") << endl;
                    }
                    else if (operation == 2 && weight < 0) {
                        cout << "Negative edge weights are not supported." << endl;
                    }
                    else {
                        cout << "Invalid vertices, duplicate edge, or edge not found." << endl;
                    }
                }
                else if (operation == 4) {
                    graph.clear();
                    cout << "All edges removed successfully." << endl;
                }
                else if (operation != 5) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (operation != 5);
        }
        else if (mainChoice == 2) {
            int operation;

            do {
                cout << "\n\n============================================" << endl;
                cout << "              SEARCH OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Check Edge" << endl;
                cout << "2. Check Path" << endl;
                cout << "3. Get Edge Weight" << endl;
                cout << "4. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(operation);

                if (operation >= 1 && operation <= 3) {
                    int source;
                    int destination;
                    cout << "\nEnter source vertex: ";
                    read_input(source);
                    cout << "Enter destination vertex: ";
                    read_input(destination);

                    if (operation == 1) {
                        cout << (graph.has_edge(source, destination) ? "Edge exists." : "Edge not found.") << endl;
                    }
                    else if (operation == 2) {
                        cout << (graph.has_path(source, destination) ? "A path exists." : "No path exists.") << endl;
                    }
                    else {
                        int weight;
                        if (graph.get_edge_weight(source, destination, weight)) {
                            cout << "Edge weight: " << weight << endl;
                        }
                        else {
                            cout << "Edge not found or invalid vertices." << endl;
                        }
                    }
                }
                else if (operation != 4) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (operation != 4);
        }
        else if (mainChoice == 3) {
            int operation;

            do {
                cout << "\n\n============================================" << endl;
                cout << "             TRAVERSAL OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Breadth-First Search (BFS)" << endl;
                cout << "2. Depth-First Search (DFS)" << endl;
                cout << "3. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(operation);

                if (operation == 1 || operation == 2) {
                    int start;
                    cout << "\nEnter starting vertex: ";
                    read_input(start);
                    cout << (operation == 1 ? "BFS: " : "DFS: ");
                    if (operation == 1) {
                        graph.display_breadth_first(start);
                    }
                    else {
                        graph.display_depth_first(start);
                    }
                }
                else if (operation != 3) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (operation != 3);
        }
        else if (mainChoice == 4) {
            int operation;

            do {
                cout << "\n\n============================================" << endl;
                cout << "               GRAPH OPERATIONS" << endl;
                cout << "============================================" << endl;
                cout << "1. Shortest Path (Dijkstra)" << endl;
                cout << "2. Back To Main Menu" << endl;
                cout << "============================================" << endl;
                cout << "Enter your choice: ";
                read_input(operation);

                if (operation == 1) {
                    int source;
                    int destination;
                    long long distance;
                    int pathLength = 0;
                    int* path = (vertexCount > 0) ? new int[vertexCount] : nullptr;

                    cout << "\nEnter source vertex: ";
                    read_input(source);
                    cout << "Enter destination vertex: ";
                    read_input(destination);

                    if (graph.shortest_path(source, destination, distance, path, vertexCount, pathLength)) {
                        cout << "Shortest distance: " << distance << endl;
                        cout << "Path: ";
                        for (int i = 0; i < pathLength; i++) {
                            cout << path[i];
                            if (i < pathLength - 1) {
                                cout << " -> ";
                            }
                        }
                        cout << endl;
                    }
                    else {
                        cout << "No path found, invalid vertices, or graph contains a negative edge." << endl;
                    }

                    delete[] path;
                }
                else if (operation != 2) {
                    cout << "\nInvalid choice!" << endl;
                }
            } while (operation != 2);
        }
        else if (mainChoice == 5) {
            cout << "\nGraph: " << endl;
            graph.display();
            cout << "Vertices: " << graph.get_vertex_count() << endl;
            cout << "Edges: " << graph.get_edge_count() << endl;
            cout << "Type: " << (graph.is_directed() ? "Directed" : "Undirected") << endl;
            cout << "Weights: " << (graph.is_weighted() ? "Enabled" : "Disabled") << endl;

            if (!graph.is_directed()) {
                cout << "Connected components: " << graph.get_component_count() << endl;
            }

            int vertex;
            cout << "\nEnter vertex to get its degree (-1 to skip): ";
            read_input(vertex);
            if (vertex != -1) {
                int degree = graph.get_degree(vertex);
                if (degree == -1) {
                    cout << "Invalid vertex." << endl;
                }
                else {
                    cout << "Degree: " << degree << endl;
                }
            }
        }
        else if (mainChoice != 6) {
            cout << "\nInvalid choice!" << endl;
        }
    } while (mainChoice != 6);

    cout << "\nThank you for using Graph Application." << endl;
    return 0;
}
