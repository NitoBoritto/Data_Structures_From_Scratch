def main():
	'''Key values, items in keys for lookup'''
	# Initialization: create an empty dictionary for key-value pairs.
	hash_map = {}

	# Insertion: add values by assigning them to unique keys.
	hash_map['name'] = 'Alice'
	hash_map['age'] = 25
	hash_map['occupation'] = 'Engineer'

	print('\nHashmap Implementation')
	print(f'Type: {type(hash_map)}')
	print(f'Hashmap size: {len(hash_map)}')
	print(f'Hashmap: {hash_map}')

	# Lookup: retrieve a value directly using its key.
	print(f"\nValue for key 'name': {hash_map['name']}")
	# The get() method returns a default instead of raising KeyError.
	print(f"Value for missing key: {hash_map.get('email', 'Not found')}")

	# Updating: assigning to an existing key replaces its value.
	hash_map['age'] = 26
	print(f"Updated value for key 'age': {hash_map['age']}")

	# Membership: check whether a key exists in the hashmap.
	print(f"Key 'occupation' exists: {'occupation' in hash_map}")
	print(f"Key 'email' exists: {'email' in hash_map}")

	# Iteration: access keys, values, or both key-value pairs.
	print('\nHashmap keys:')
	for key in hash_map:
		print(key)

	print('\nHashmap values:')
	for value in hash_map.values():
		print(value)

	print('\nHashmap key-value pairs:')
	for key, value in hash_map.items():
		print(f'{key}: {value}')

	# Removal: delete a key and its associated value.
	removed_value = hash_map.pop('occupation')
	print(f"\nRemoved value: {removed_value}")
	print(f'Hashmap after removal: {hash_map}')


if __name__ == '__main__':
	main()


