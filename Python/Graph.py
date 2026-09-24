import matplotlib.pyplot as plt
import networkx as nx

def draw_graph(graph: nx.Graph, title: str, weighted: bool = False) -> None:
        figure = plt.figure(num=title)
        position = nx.spring_layout(graph, seed=42)
        nx.draw(
                graph,
                position,
                with_labels=True,
                font_weight="bold",
                node_size=800,
                node_color="lightblue",
                arrows=graph.is_directed(),
        )

        if weighted:
                labels = nx.get_edge_attributes(graph, "weight")
                nx.draw_networkx_edge_labels(graph, position, edge_labels=labels)

        figure.suptitle(title)
        plt.show()
        plt.close()


def basic_graph() -> nx.Graph:
        graph = nx.Graph()
        graph.add_edges_from(((1, 2), (1, 3), (2, 3), (3, 4), (5, 4)))
        return graph


def directed_graph() -> nx.DiGraph:
        graph = nx.DiGraph()
        graph.add_edges_from(((1, 2), (1, 3), (2, 3), (3, 4), (5, 4)))
        return graph


def weighted_graph() -> nx.Graph:
        graph = nx.Graph()
        graph.add_weighted_edges_from(
                (("A", "B", 4), ("B", "C", 2), ("C", "D", 1),
                 ("D", "E", 3), ("E", "A", 5))
        )
        return graph


def weighted_directed_graph() -> nx.DiGraph:
        graph = nx.DiGraph()
        graph.add_weighted_edges_from(
                (("A", "B", 4), ("B", "C", 2), ("C", "D", 1),
                 ("D", "C", 8), ("D", "E", 3), ("E", "A", 5))
        )
        return graph


def main() -> None:
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