void main() {
	int a, b;
	int c;
	double f;

	print("Please enter a number: ");
	a = read_int();
	print_nl();
	print("Please enter a number: ");
	b = read_int();
	print_nl();

	c = a + b;
	print("Sum = ");
	print_int(c);
	print_nl();

	f = 70.0 / 3.0;
	print("Value of 70.0/3.0 = ");
	print_float(f);
	print_nl();

	f = f + c;
	print("Sum = ");
	print_float(f);
	print_nl();
}
