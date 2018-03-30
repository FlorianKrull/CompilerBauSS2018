void main() {
	print("Please enter the number (must be less than 100): ");
	int n, i, j, temp;
	n = read_int();
	print_nl(n);

	print("Please enter value of the array that need to be sorted: ");
	int[100] arr;
	for (i = 0; i < n; i++) {
		arr[i] = read_int();
		print_nl();
	}

	/* Start bubble sort algorithm */
	for (i = 0; i < n - 1; i++) {
		for (j = n - 1; j > i; j--)
			if (arr[j - 1] > arr[j]) {
				/* swap arr[j - 1] and arr[i] */
				temp = arr[j - 1];
				arr[j - 1] = arr[i];
				arr[i] = temp;
			}
	}

	print("New ordered array after bubble sorting: ");
	print_nl();
	for (i = 0; i < n; i++) {
		print_int(arr[i]);
		print_nl();
	}
}
