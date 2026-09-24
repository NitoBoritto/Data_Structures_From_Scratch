from queue import PriorityQueue, Queue


def drain_queue(data_queue: Queue | PriorityQueue, name: str) -> None:
    """Remove and display every item in a queue."""
    print(f"\n{name} dequeue:")
    while data_queue.empty() == False:
        print(f'Element "{data_queue.get()}" has been removed')
    print(f"{name} size: {data_queue.qsize()}")


def basic_queue() -> None:
    data_queue = Queue()

    for item in ("Data Structures", 35, "CS50", 911.119):
        data_queue.put(item)

    print("Basic Queue Implementation")
    print(f"Type: {type(data_queue)}")
    print(f"Queue size: {data_queue.qsize()}")
    print(f'Element "{data_queue.get()}" has been removed')
    print(f'Element "{data_queue.get()}" has been removed')
    print(f"Queue empty check: {data_queue.empty()}")
    drain_queue(data_queue, "Queue")


def priority_queue() -> None:
    priority_queue = PriorityQueue()

    for item in (72, 35, 911.119, 0.75):
        priority_queue.put(item)

    print("\nPriority Queue Implementation")
    print(f"Type: {type(priority_queue)}")
    print(f"Queue size: {priority_queue.qsize()}")
    print(f'Element "{priority_queue.get()}" has been removed')
    print(f'Element "{priority_queue.get()}" has been removed')
    print(f"Queue empty check: {priority_queue.empty()}")
    drain_queue(priority_queue, "Priority queue")


def main() -> None:
    basic_queue()
    priority_queue()


if __name__ == "__main__":
    main()