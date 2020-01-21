def main():
    with open("out.txt", "rb") as f:
        ar = bytearray(f.read())
        ar2 = []
        for i in range(0, len(ar), 8):
            ar2.append(int.from_bytes(ar[i:i + 8], byteorder='little'))
        print(ar2 == sorted(ar2))


if __name__ == "__main__":
    main()
