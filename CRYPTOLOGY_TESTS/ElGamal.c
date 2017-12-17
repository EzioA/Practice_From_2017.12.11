#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>

unsigned int mod(unsigned int, unsigned int, unsigned int);
unsigned int euclid(unsigned int, unsigned int);

int main()
{
	unsigned int p, g, xu, yu, m1, m2, r;
	unsigned int c1, c1_r, c2;
	printf("Input p, g and your private key(between 1 and p-1):\n");
	scanf("%u%u%u", &p, &g, &xu);

//	srand(time(NULL));
//	xu = (rand() % (p-1)) + 1;
	yu = mod(g, xu, p);	//public key

	printf("Input m and r(2 <= r <= p-2):\n");
	scanf("%u%u", &m1, &r);
//	printf("p = %u, g = %u, xu = %u, m1 = %u, r = %u, yu = %u\n", p, g, xu, m1, r, yu);
	c1 = mod(g, r, p);
	c2 = ((m1 % p) * mod(yu, r, p)) % p;
	
	printf("The encrypted result:(%u, %u)\n", c1, c2);
	if ((c1_r = euclid(c1, p) ) == -1)
	{
		printf("error!\n");
		exit(0);
	}
//	printf("c1_r = %u\n", c1_r);
	m2 = ((c2 % p) * mod(c1_r, xu, p)) % p;
	printf("The decrypted message is :%u\n", m2);

	return 0;
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
			return y2 > INT_MAX ? y2 - INT_MAX : y2;

		q = x3 / y3;
		t1 = x1 - q*y1;
		t2 = x2 - q*y2;
		t3 = x3 - q*y3;
		x1 = y1, x2 = y2, x3 = y3;
		y1 = t1, y2 = t2, y3 = t3;	
	}
}
