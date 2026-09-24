import queue 
from queue import PriorityQueue

"""
Basic Queue

"""
print('''
===================
Queue Implemenation
===================
''')
# Initialization
queue = queue.Queue()

print(type(queue)) # queue.Queue

# Insertion (Enqueue)
queue.put('Data Structures')
queue.put(35)
queue.put('CS50')
queue.put(911.119)

# Check Size
print(f'\nQueue size: {queue.qsize()}') # 4

# Pop Item From Queue (Dequeue)
print(f'\nElement "{queue.get()}" has been removed') # Data Structures
print(f'Element "{queue.get()}" has been removed') # 35

# Empty Check
print(f'\nQueue empty check: {queue.empty()}') # False

# Dequeue Sequentially
print('Dequeue in progress:\n')
while queue.empty() == False:
    print(f'Element "{queue.get()}" has been removed')
print(f'Queue Size {queue.qsize()}')



"""
Priority Queue
PQ Sorts Values Ascendingly

"""

print('''\n\n
============================
Priority Queue Implemenation
============================
''')

# Initialization
pq = PriorityQueue()

print(type(pq)) # queue.PriorityQueue

# Insertion (Enqueue)
pq.put(72)
pq.put(35)
pq.put(True)
pq.put(911.119)
pq.put(False)
pq.put(0.75)

# Check Size
print(f'\nQueue size: {pq.qsize()}') # 6

# Pop Item From Queue (Dequeue)
print(f'\nElement "{pq.get()}" has been removed') # False
print(f'Element "{pq.get()}" has been removed') # 0.75

# Empty Check
print(f'Queue empty check: {pq.empty()}') # False

# Dequeue Sequentially
print('\nDequeue in progress:\n')
while pq.empty() == False:
    print(f'Element "{pq.get()}" has been removed')
print(f'Queue Size {pq.qsize()}')