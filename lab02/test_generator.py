import random, string

f = open("tests/e2e/02.txt", mode="w")

size = 1000
s = "a"

def get_random_string():
    length = random.randint(1, 256)
    random_list = [ random.choice(string.ascii_letters) for _ in range(length) ]
    return "".join(random_list)

for i in range(size):
    s = get_random_string()
    f.write(f"+ {s} {i}\n")
    f.write(f"{s}\n")
