from sys import stdout
from math import sqrt, log

def is_prime(p):
    if p == 2:
        return True  # Lucas-Lehmer test only works on odd primes
    elif p <= 1 or p % 2 == 0:
        return False
    else:
        for i in range(3, int(sqrt(p)) + 1, 2):
            if p % i == 0: return False
        return True

def is_mersenne_prime(p):
    if p == 2:
        return True
    else:
        m_p = (1 << p) - 1
        s = 4
        for i in range(3, p + 1):
            s = (s ** 2 - 2) % m_p
        return s == 0

print("Input number of first Mersenne numbers")
print("Be careful! It's better input 'n' from 2 to 20 due to worktime: ")
n = int(input())
if n < 1:
    print("Number 'n' must be more than 1")
    exit()
print("Mersenne number is: 2^p - 1")
print("Finding Mersenne primes from M1 to M%d:" % n)

count = 0
for p in range(1, n+1):
    mersenne = 2 ** p - 1
    if is_prime(p) and is_mersenne_prime(p):
        print("\033[33m\033[1m\033[3m{}".format("M%d %d" % (p, mersenne)))
        stdout.flush()
        count += 1
    else:
        if mersenne == 1:
            print("\033[33m\033[1m\033[3m{}".format("M%d %d" % (p, mersenne)))
        else:
            print("\033[37m\033[0m{}".format("M%d %d" % (p, mersenne)))
