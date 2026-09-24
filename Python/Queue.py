from queue import PriorityQueue, Queue


def drain_queue(data_queue: Queue | PriorityQueue, name: str) -> None:
    """Remove items in FIFO or priority order until the queue is empty."""
    print(f"\n{name} dequeue:")
    # Queue.get() removes the next available item.
    while data_queue.empty() == False:
        print(f'Element "{data_queue.get()}" has been removed')
    print(f"{name} size: {data_queue.qsize()}")


def basic_queue() -> None:
    """first-in, first-out queue."""
    # Queue is thread-safe and removes items in insertion order.
    data_queue = Queue()

    # Enqueue items at the back of the queue.
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
    """Sorts queue ascendingly."""
    priority_queue = PriorityQueue()

    # PriorityQueue sorts comparable values as they are removed.
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
    """Run both queue demonstrations."""
    basic_queue()
    priority_queue()


if __name__ == "__main__":
    main()