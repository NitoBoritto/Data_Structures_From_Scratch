import matplotlib.pyplot as plt
import networkx as nx


def draw_graph(graph: nx.Graph, title: str, weighted: bool = False):
        """Draw a graph and optionally display its edge weights."""
        # Use the graph title for both the window name and the plot heading.
        figure = plt.figure(num=title)
        position = nx.spring_layout(graph, seed=42)

        # Directed graphs display arrows; undirected graphs do not.
        nx.draw(
                graph,
                position,
                with_labels=True,
                font_weight="bold",
                node_size=800,
                node_color="lightblue",
                arrows=graph.is_directed(),
        )

        # Weighted graphs need a separate label for each edge.
        if weighted:
                labels = nx.get_edge_attributes(graph, "weight")
                nx.draw_networkx_edge_labels(graph, position, edge_labels=labels)

        figure.suptitle(title)
        plt.show()
        plt.close()


def basic_graph() -> nx.Graph:
        """Create an undirected graph with unweighted edges."""
        # An undirected edge allows travel in either direction.
        graph = nx.Graph()
        graph.add_edges_from(((1, 2), (1, 3), (2, 3), (3, 4), (5, 4)))
        return graph


def directed_graph() -> nx.DiGraph:
        """Create a directed graph whose edges have a defined direction."""
        # In a directed graph, (1, 2) is different from (2, 1).
        graph = nx.DiGraph()
        graph.add_edges_from(((1, 2), (1, 3), (2, 3), (3, 4), (5, 4)))
        return graph


def weighted_graph() -> nx.Graph:
        """Create an undirected graph with a weight on every edge."""
        graph = nx.Graph()
        # Each tuple represents: source node, destination node, edge weight.
        graph.add_weighted_edges_from(
                (("A", "B", 4), ("B", "C", 2), ("C", "D", 1),
                 ("D", "E", 3), ("E", "A", 5))
        )
        return graph


def weighted_directed_graph() -> nx.DiGraph:
        """Create a directed graph with weighted edges."""
        graph = nx.DiGraph()
        # The reverse edge D -> C has a different cost from C -> D.
        graph.add_weighted_edges_from(
                (("A", "B", 4), ("B", "C", 2), ("C", "D", 1),
                 ("D", "C", 8), ("D", "E", 3), ("E", "A", 5))
        )
        return graph


def main() -> None:
        """Build and display each graph example."""
        draw_graph(basic_graph(), "Basic Graph")
        draw_graph(directed_graph(), "Directed Graph")
        draw_graph(weighted_graph(), "Undirected/Bi-directed Weighted Graph", weighted=True)
        draw_graph(
                weighted_directed_graph(),
                "Weighted Directed Graph",
                weighted=True,
        )


if __name__ == "__main__":
        main()