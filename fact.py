def factorial(n: int) -> None:
    if (n == 1):
        return 1
    return n * factorial(n-1)
