# pylint: disable=invalid-name
# pylint: disable=missing-docstring

# RUN ON PYPY3


def main():
    N, K = map(int, input().split())
    H = [0] + [int(x) for x in input().split()]
    DP = [[-1 for _ in range(K + 1)] for _ in range(N + 1)]
    DP[0] = [0 for _ in range(K + 1)]
    for i in range(1, N + 1):
        for j in range(K + 1):
            for k in range(max(i - j - 1, 0), i):
                previous = DP[k][j - (i - k - 1)]
                if previous == -1:
                    continue

                if DP[i][j] == -1 or previous + max(H[i] - H[k], 0) < DP[i][j]:
                    DP[i][j] = previous + max(H[i] - H[k], 0)

                if j > 0 and DP[i][j - 1] != -1 and DP[i][j - 1] < DP[i][j]:
                    DP[i][j] = DP[i][j - 1]
    ans = -1
    for i in range(N + 1):
        if K < (N - i):
            continue
        if ans == -1 or DP[i][min(K - (N - i), K)] < ans:
            ans = DP[i][min(K - (N - i), K)]
    print(ans)


if __name__ == '__main__':
    main()
