from queue import LifoQueue


def main():
    '''last-in, first-out stack.'''
    # Initialization
    stack = LifoQueue()

    # put() pushes each item onto the top of the stack.
    for item in (False, 'Stacking', 'Nice', 4444, 0.99):
        stack.put(item)

    print('\nStack Implementation')
    print(f'Type: {type(stack)}')
    print(f'Stack size: {stack.qsize()}\n')
    print(f'Element "{stack.get()}" has been removed')
    print(f'Element "{stack.get()}" has been removed')
    print(f'Stack empty check: {stack.empty()}')

    print('\nStack pop:')
    # get() pops the newest item first.
    while not stack.empty():
        print(f'Element "{stack.get()}" has been removed')
    print(f'Stack size: {stack.qsize()}')


if __name__ == '__main__':
    main()
