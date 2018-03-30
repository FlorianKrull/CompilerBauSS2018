/* Calculate the factorial of number */
double fac(int n)
{
     if (n == 0){
    	 return 1;
     }
     return n * fac(n - 1);
}

void main() {
	print("Please enter a number: ");

	int n;
	n = read_int();
	print_nl();

	double result;
	result = fac(n);

	print("fac(");
	print_int(n);
	print(") = ");
	print_float(result);
	print_nl();
}
