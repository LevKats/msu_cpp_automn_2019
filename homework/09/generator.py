from random import randint


SIZE = 10 ** 6


def main():
    with open("output_file", "wb") as f:
        f.write(bytearray([randint(0, 255) for _ in range(SIZE * 8)]))


if __name__ == "__main__":
    main()

