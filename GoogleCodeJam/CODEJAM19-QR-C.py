import math


def find_begin_index(adj):
    for i in range(len(adj) - 1):
        if adj[i] != adj[i + 1]:
            return (i + 1, math.gcd(adj[i], adj[i + 1]))
    exit(1)


def solve_case():
    N, L = map(int, input().split())
    adj = [int(x) for x in input().split()]
    cipher_text = [-1] * (L + 1)
    begin_index, cipher_text[begin_index] = find_begin_index(adj)
    for i in reversed(range(begin_index)):
        cipher_text[i] = adj[i] // cipher_text[i + 1]
    for i in range(begin_index, L):
        cipher_text[i + 1] = adj[i] // cipher_text[i]
    alphabet_primes = sorted(list(set(cipher_text)))
    alphabet = dict()
    assert(len(alphabet_primes) == 26)
    for i, prime in enumerate(alphabet_primes):
        alphabet[prime] = chr(ord('A') + i)
    message = ''.join([alphabet[x] for x in cipher_text])
    return message


def main():
    n = int(input())
    for i in range(n):
        print("Case #{}: {}".format(i + 1, solve_case()))


if __name__ == "__main__":
    main()
