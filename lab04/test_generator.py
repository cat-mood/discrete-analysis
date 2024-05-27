import random
import string

letters = ["a", "b"]

def get_random_letter():
    return random.choice(letters)

for _ in range(10):
    print(get_random_letter() + " ", end="")

print("\n", end="")

for _ in range(100000):
    print(get_random_letter() + " ", end="")