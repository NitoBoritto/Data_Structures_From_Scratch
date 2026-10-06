class Node:
	'''Node used by a singly or circular linked list.'''

	def __init__(self, data):
		# Store the value held by this node.
		self.data = data
		# Point to the next node; None means there is no next node yet.
		self.next = None


class DoublyNode(Node):
	'''Node with links to both the next and previous nodes.'''

	def __init__(self, data):
		# Initialize the data and next-node link from Node.
		super().__init__(data)
		# Point back to the previous node in the list.
		self.previous = None


class SinglyLinkedList:
	'''Linked list where every node points only to the next node.'''

	def __init__(self):
		# The first node is the entry point for the list.
		self.head = None

	def append(self, data):
		'''Add a new node to the end of the list.'''
		# Create a node containing the new value.
		new_node = Node(data)
		# The new node becomes the head when the list is empty.
		if self.head is None:
			self.head = new_node
			return

		# Follow next links until the current node is the last node.
		current = self.head
		while current.next is not None:
			current = current.next
		# Link the last node to the new node.
		current.next = new_node

	def insert(self, index, data):
		'''Insert a new node at a zero-based index.'''
		# Create the node that will be inserted.
		new_node = Node(data)
		# At index zero, the new node becomes the head.
		if index == 0:
			new_node.next = self.head
			self.head = new_node
			return
		# Negative indexes are not supported by this linked-list implementation.
		if index < 0:
			raise IndexError("Linked-list index cannot be negative")

		# Find the node immediately before the insertion point.
		previous = self.head
		for _ in range(index - 1):
			if previous is None:
				raise IndexError("Linked-list index out of range")
			previous = previous.next
		# The requested index is past the end of the list.
		if previous is None:
			raise IndexError("Linked-list index out of range")

		# Connect the new node between previous and the following node.
		new_node.next = previous.next
		previous.next = new_node

	def remove(self, data):
		'''Remove the first node containing data.'''
		# An empty list has no node that can be removed.
		if self.head is None:
			raise ValueError("Cannot remove from an empty list")
		# Removing the head moves the head link to the next node.
		if self.head.data == data:
			self.head = self.head.next
			return

		# Keep both nodes so the previous node can skip the removed node.
		previous = self.head
		current = self.head.next
		while current is not None:
			# Connect the previous node directly to the node after current.
			if current.data == data:
				previous.next = current.next
				return
			previous, current = current, current.next
		# No node contained the requested value.
		raise ValueError(f"{data!r} was not found")

	def traverse(self):
		'''Return every node value from head to tail.'''
		# Store values as the list is visited from left to right.
		values = []
		current = self.head
		while current is not None:
			values.append(current.data)
			current = current.next
		return values

	def sort(self):
		'''Sort the list in ascending order by swapping node values.'''
		# Visit each position that needs the next smallest value.
		current = self.head
		while current is not None:
			# Treat the current node as the smallest value found so far.
			smallest = current
			# Search the remaining nodes for a smaller value.
			candidate = current.next
			while candidate is not None:
				if candidate.data < smallest.data:
					smallest = candidate
				candidate = candidate.next
			# Place the smallest value at the current position.
			current.data, smallest.data = smallest.data, current.data
			current = current.next


class DoublyLinkedList:
	'''Linked list where each node points in both directions.'''

	def __init__(self):
		# Head points to the first node and tail points to the last node.
		self.head = None
		self.tail = None

	def append(self, data):
		'''Add a new node to the end of the list.'''
		# Create a node with both links initially empty.
		new_node = DoublyNode(data)
		# The new node is both head and tail when the list is empty.
		if self.tail is None:
			self.head = self.tail = new_node
			return
		# Link the new node back to the old tail.
		new_node.previous = self.tail
		# Link the old tail forward to the new node.
		self.tail.next = new_node
		# Move the tail to the newly appended node.
		self.tail = new_node

	def remove(self, data):
		'''Remove the first node containing data.'''
		# Search forward from the head for the requested value.
		current = self.head
		while current is not None and current.data != data:
			current = current.next
		# The value was not present in the list.
		if current is None:
			raise ValueError(f"{data!r} was not found")

		# Update the head when the first node is removed.
		if current.previous is None:
			self.head = current.next
		else:
			# Skip the removed node from the previous node.
			current.previous.next = current.next
		# Update the tail when the last node is removed.
		if current.next is None:
			self.tail = current.previous
		else:
			# Skip the removed node from the next node as well.
			current.next.previous = current.previous

	def traverse(self, reverse=False):
		'''Return values from head to tail or tail to head.'''
		# Choose the starting node based on the requested direction.
		values = []
		current = self.tail if reverse else self.head
		while current is not None:
			values.append(current.data)
			# Follow previous links in reverse, otherwise follow next links.
			current = current.previous if reverse else current.next
		return values


class CircularLinkedList:
	'''Linked list whose final node points back to the first node.'''

	def __init__(self):
		# Tail is enough to find both the final node and the head node.
		self.tail = None

	def append(self, data):
		'''Add a new node to the end of the circular list.'''
		# Create the node that will be appended.
		new_node = Node(data)
		# A one-node circular list points back to itself.
		if self.tail is None:
			self.tail = new_node
			new_node.next = new_node
			return
		# The head is the node immediately after the tail.
		new_node.next = self.tail.next
		# Insert the new node after the old tail.
		self.tail.next = new_node
		# Make the new node the tail.
		self.tail = new_node

	def remove(self, data):
		'''Remove the first node containing data.'''
		# An empty circular list has no node to remove.
		if self.tail is None:
			raise ValueError("Cannot remove from an empty list")

		# Start at the head and keep the node before current.
		previous = self.tail
		current = self.tail.next
		while True:
			# Remove the current node when its value matches.
			if current.data == data:
				# One node pointing to itself becomes an empty list.
				if current is previous:
					self.tail = None
				else:
					# Bypass the removed node.
					previous.next = current.next
					# Move the tail back when the old tail is removed.
					if current is self.tail:
						self.tail = previous
				return
			# Move to the next pair of nodes.
			previous, current = current, current.next
			# Stop after returning to the head.
			if current is self.tail.next:
				break
		# No node contained the requested value.
		raise ValueError(f"{data!r} was not found")

	def traverse(self):
		'''Return every node value once, starting at the head.'''
		# An empty circular list has no values to visit.
		if self.tail is None:
			return []

		# The node after tail is the head of the circular list.
		values = []
		current = self.tail.next
		while True:
			values.append(current.data)
			current = current.next
			# Stop when traversal reaches the head again.
			if current is self.tail.next:
				break
		return values


def main():
	"""Demonstrate singly, doubly, and circular linked lists."""
	# Initialization: create a singly linked list and add unsorted values.
	singly = SinglyLinkedList()
	for item in (40, 10, 30, 20):
		singly.append(item)
	print("Singly Linked List")
	# Traversal: visit every node from head to tail.
	print(f"Traversal: {singly.traverse()}")
	# Insertion: place a new value at index 2.
	singly.insert(2, 25)
	print(f"After inserting 25 at index 2: {singly.traverse()}")
	# Removal: delete the first node containing 10.
	singly.remove(10)
	print(f"After removing 10: {singly.traverse()}")
	# Sorting: arrange the node values in ascending order.
	singly.sort()
	print(f"After sorting: {singly.traverse()}")

	# Initialization: create a doubly linked list.
	doubly = DoublyLinkedList()
	for item in (10, 20, 30):
		doubly.append(item)
	# Removal: delete the middle node and update both links.
	doubly.remove(20)
	print("\nDoubly Linked List")
	# Traversal: move forward using next links.
	print(f"Forward traversal: {doubly.traverse()}")
	# Traversal: move backward using previous links.
	print(f"Reverse traversal: {doubly.traverse(reverse=True)}")

	# Initialization: create a circular linked list.
	circular = CircularLinkedList()
	for item in (100, 200, 300):
		circular.append(item)
	# Removal: delete a node while preserving the circular connection.
	circular.remove(200)
	print("\nCircular Linked List")
	# Traversal: stop after visiting the head for the second time.
	print(f"Traversal: {circular.traverse()}")


if __name__ == "__main__":
	main()


