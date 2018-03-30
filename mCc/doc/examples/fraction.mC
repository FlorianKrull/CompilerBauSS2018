struct fraction {
	int num;		//numerator
	int den;		//denominator
};

void print(fraction f) {
	print_int(f->num);
	print("/");
	print_int(f->den);
}

fraction reverse(fraction f) {
	fraction temp;
	if (temp.den != 0) {
		temp.num = f.den;
		temp.den = f.num;
	}
	return temp;
}

void main()
{
	print("Please enter the numerator and denominator of your fraction: ");
	fraction my_frac;
	my_frac.num = read_int();
	print_nl();
	my_frac.dem = read_int();
	print_nl();
	
	fraction reversed_frac = reverse(my_frac);
	print("Revesed fraction: ");
	print(reversed_frac);
}
