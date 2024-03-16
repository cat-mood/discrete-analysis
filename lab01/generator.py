import sys
import random
import string

TEST_COUNT = 7

def get_random_string():
    length = random.randint(1, 200)
    random_list = [ random.choice(string.ascii_letters) for _ in range(length) ]
    return "".join(random_list)

def get_random_key():
    random_list = []
    random_list.append(random.choice(string.ascii_uppercase))
    random_list.append(' ')
    for _ in range(3):
        random_list.append(str(random.randint(0, 9)))
    random_list.append(' ')
    for _ in range(2):
        random_list.append(random.choice(string.ascii_uppercase))
    return "".join(random_list)

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <test directory>")
        sys.exit(1)

    test_dir = sys.argv[1]

    lines = [0]
    lines.extend([ 10 ** i for i in range(TEST_COUNT) ])

    for enum, test_count in enumerate(range(1, TEST_COUNT+1)):
        test = []
        answer = []

        line_count = lines[enum]
        for _ in range(line_count):
            key = get_random_key()
            value = get_random_string()
            test.append((key, value))

        test_name = "{}/{:02d}".format(test_dir, test_count)
        with open(f'{test_name}.t', 'w') as ftest:
            for key, value in test:
                ftest.write(f'{key}\t{value}\n')

        answer = sorted(test, key=lambda x: x[0])
        with open(f'{test_name}.a', 'w') as ftest:
            for key, value in answer:
                ftest.write(f'{key}\t{value}\n')
main()
