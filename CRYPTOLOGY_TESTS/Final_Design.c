#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

unsigned int gcd_e_1(unsigned int);
unsigned int priv_gen(unsigned int, unsigned int);
unsigned int encrypt(unsigned int, unsigned int, unsigned int);
unsigned int decrypt(unsigned int, unsigned int, unsigned int);
unsigned int mod(unsigned int, unsigned int, unsigned int);
unsigned int euclid(unsigned int, unsigned int);
int rsa();
int elgamal(unsigned int *, unsigned int *);


int main()
{
	unsigned int r, s;	//sign
	int message = rsa();
	elgamal(&r, &s);

	return 0;
}

int rsa()
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
	printf("Encrypted message is %u\n", c);

	result = decrypt(d, c, n);
	printf("Decrypted message is %u\n", result);

	return c;
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
	int flag = 0;
	if (a > INT_MAX)
	{
		flag = 1;
		a = UINT_MAX + 1 - a;
	}

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
	
	return (flag == 1) ? p - y : y;
}

int elgamal(unsigned int *r, unsigned int *s)
{
	unsigned int p, g, xu, yu, m1, m2, k;
	unsigned int c1_r;
	printf("Input p, g and your private key(between 1 and p-1):\n");
	scanf("%u%u%u", &p, &g, &xu);

//	srand(time(NULL));
//	xu = (rand() % (p-1)) + 1;
	yu = mod(g, xu, p);	//public key

	printf("Input h(m) and k(2 <= k <= p-2):\n");
	scanf("%u%u", &m1, &k);
//	printf("p = %u, g = %u, xu = %u, m1 = %u, r = %u, yu = %u\n", p, g, xu, m1, r, yu);
	*r = mod(g, k, p);
	*s = ((mod((m1-xu*(*r)), 1, (p-1))) * euclid(k, (p-1))) % (p-1);
	printf("Digital sign:(%u, %u)\n", r, s);	
//	printf("The encrypted result:(%u, %u)\n", *r, *s);
/*	if ((c1_r = euclid(*r, p) ) == -1)
	{
		printf("error!\n");
		exit(0);
	}
*/
//	printf("c1_r = %u\n", c1_r);
//	m2 = ((*s % p) * mod(c1_r, xu, p)) % p;
//	printf("The decrypted message is :%u\n", m2);
	if (((mod(yu, *r, p) * mod(*r, *s, p)) % p) == mod(g, m1, p))
		printf("Digital sign is valid!\n");
	else
		printf("Digital sign is invalid!\n");

	return 0;
}

unsigned int euclid(unsigned int a, unsigned int b)
{
	unsigned int x1, x2, x3, y1, y2, y3;
	unsigned int q, t1, t2, t3;
	x1 = 1, x2 = 0, x3 = b;
	y1 = 0, y2 = 1, y3 = a;

	while (1)
	{
		if (y3 == 0)
			exit(0);
		if (y3 == 1)
			return y2 > INT_MAX ? (b - (UINT_MAX + 1 - y2)) : y2;

		q = x3 / y3;
		t1 = x1 - q*y1;
		t2 = x2 - q*y2;
		t3 = x3 - q*y3;
		x1 = y1, x2 = y2, x3 = y3;
		y1 = t1, y2 = t2, y3 = t3;	
	}
}
