def main():
    '''Dynamic Array, No Fixed Size'''
    # Initialization
    arr = [10, 20, 30, 40, 50]

    print('\nArray Implementation')
    print(f'Type: {type(arr)}')
    print(f'Array size: {len(arr)}')
    print(f'Array: {arr}')
    
    # Indexing
    print(f'\nElement at index 2: "{arr[2]}"')
    # Updating Element
    arr[2] = 35
    print(f'Updated element at index 2: {arr[2]}')
    # Adding Element at the end of the array
    arr.append(60)
    print(f'Array after update and append: {arr}\n')

    # Removing Element by Index
    arr.pop(0)
    print(f'Array after removing index 0: {arr}')
    
    
if __name__ == '__main__':
    main()