from queue import LifoQueue as Stack


print('''
===================
Stack Implemenation
===================
''')

# Initialization
stack = Stack()

print(type(stack)) # queue.LifoQueue

# Insertion (Push)
stack.put(False)
stack.put('Stacking')
stack.put('Nice')
stack.put(4444)
stack.put(0.99)

# Check Size
print(f'\nStack size: {stack.qsize()}') # 5

# Pop Item From Queue (Pop)
print(f'\nElement "{stack.get()}" has been removed') # 0.99
print(f'Element "{stack.get()}" has been removed') # 4444

# Empty Check
print(f'\nStack empty check: {stack.empty()}') # False

# Pop Sequentially
print('\nDequeue in progress:')
while stack.empty() == False:
    print(f'Element "{stack.get()}" has been removed')
print(f'Stack Size {stack.qsize()}')
