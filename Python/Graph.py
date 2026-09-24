import networkx as nx
import matplotlib.pyplot as plt

"""
Basic Graph

"""
print('''
===================
Graph Implemenation
===================
''')

# Initialization
graph = nx.Graph()

print(type(graph)) # networkx.classes.graph.Graph

# Insertion
graph.add_edge(1, 2)
graph.add_edge(1, 3)
graph.add_edge(2, 3)
graph.add_edge(3, 4)
graph.add_edge(5, 4)

# Draw Graph
nx.draw(graph,
        with_labels = True,
        font_weight = 'bold',
        node_size = 800)
plt.show()



"""
Directed Graph
Arrows Show Data Flow Between Points

"""
print('''\n
============================
Directed Graph Implemenation
============================
''')

# Initialization
dgraph = nx.DiGraph()

print(type(dgraph)) # networkx.classes.digraph.DiGraph

# Insertion
dgraph.add_edge(1, 2)
dgraph.add_edge(1, 3)
dgraph.add_edge(2, 3)
dgraph.add_edge(3, 4)
dgraph.add_edge(5, 4)

# Draw Graph
nx.draw(dgraph,
        with_labels = True,
        font_weight = 'bold',
        node_size = 800,
        arrows = True)
plt.show()



"""
Bi-Directed Graph
Data flows back and forth between nodes
"""
print('''
===============================
Bi-Directed Graph Implemenation
===============================
''')

# Initialization
bdgraph = nx.Graph()

print(type(bdgraph)) # networkx.classes.graph.Graph

# Insertion
bdgraph.add_edge('A', 'B', weight = 4)
bdgraph.add_edge('B', 'C', weight = 2)
bdgraph.add_edge('C', 'D', weight = 1)
bdgraph.add_edge('D', 'E', weight = 3)
bdgraph.add_edge('E', 'A', weight = 5)

# Draw Graph with Weights
pos = nx.spring_layout(bdgraph)
nx.draw(bdgraph,
        pos,
        with_labels = True,
        font_weight = 'bold',
        node_size = 800,
        node_color = 'lightblue')
labels = nx.get_edge_attributes(bdgraph, 'weight')
nx.draw_networkx_edge_labels(bdgraph, pos, edge_labels = labels)
plt.show()



"""
Weighted Directed Graph
Arrows Show Data Flow Between Points
Weights Show Lowest Cost Per Node

"""
print('''\n
=====================================
Weighted Directed Graph Implemenation
=====================================
''')

# Initialization
wdgraph = nx.DiGraph()

print(type(wdgraph)) # networkx.classes.digraph.DiGraph

# Insertion
wdgraph.add_edge('A', 'B', weight = 4)
wdgraph.add_edge('B', 'C', weight = 2)
wdgraph.add_edge('C', 'D', weight = 1)
wdgraph.add_edge('D', 'C', weight = 8)
wdgraph.add_edge('D', 'E', weight = 3)
wdgraph.add_edge('E', 'A', weight = 5)

# Draw Graph with Weights
pos = nx.spring_layout(wdgraph)
nx.draw(wdgraph,
        pos,
        with_labels = True,
        font_weight = 'bold',
        node_size = 800,
        arrows = True,
        node_color = 'lightblue')
labels = nx.get_edge_attributes(wdgraph, 'weight')
nx.draw_networkx_edge_labels(wdgraph, pos, edge_labels = labels)
plt.show()