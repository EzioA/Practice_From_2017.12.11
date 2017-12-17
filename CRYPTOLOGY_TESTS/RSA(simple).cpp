#include <stdio.h>

unsigned int gcd_e_1(unsigned int);
unsigned int priv_gen(unsigned int, unsigned int);
unsigned int encrypt(unsigned int, unsigned int, unsigned int);
unsigned int decrypt(unsigned int, unsigned int, unsigned int);
unsigned int mod(unsigned int, unsigned int, unsigned int);

int main()
{
	unsigned int p, q, m;
	unsigned int e, d;
	unsigned int c, result;
	unsigned int fi, n;

	printf("Please input p, q and message:\n");
	scanf("%d%d%d", &p, &q, &m);
	
	fi = (p-1) * (q-1);
	n = p*q;
	e = gcd_e_1(fi);
//	printf("e = %d\n", e);
	d = priv_gen(e, fi);

	c = encrypt(e, m, n);
	printf("encrypted message is %u\n", c);

	result = decrypt(d, c, n);
	printf("decrypted message is %u\n", result);

	return 0;
}

unsigned int gcd_e_1(unsigned int fi)
{
	unsigned int e = 1, _e, temp;

	do 
	{
		_e = ++e;
		while (fi % _e != 0)
		{
			temp = fi;
			fi = _e;
			_e = temp % _e;
		}
	} while (_e != 1);
	
	return e;
}

unsigned int priv_gen(unsigned int e, unsigned int fi)
{
	unsigned int d = 0;

	while ((d * e) % fi != 1)
		d++;

	return d;
}

unsigned int encrypt(unsigned int e, unsigned int m, unsigned int n)
{
/*	unsigned int c = 1, temp;
	temp = pow(m, e);

	while (c % n != temp % n)
		c++;

	return c;
*/	
	return mod(m, e, n);
}

unsigned int decrypt(unsigned int d, unsigned int c, unsigned int n)
{
/*	unsigned int m = 1, temp;
	temp = pow(c, d);

	while (m % n != temp % n)
		m++;

	return m;
*/
	return mod(c, d, n);
}

unsigned int mod(unsigned int g, unsigned int x, unsigned int p)
{
	unsigned int a = g, b = x, y = 1;

	while (b != 0)
	{
		while (b > 0 && b % 2 == 0)
		{
			a = (a*a) % p;
			b = b / 2;
		}
		b = b - 1;
		y = (a*y) % p;
	}
	
	return y;
}