from subprocess import Popen, PIPE
from sys import argv


COMMAND = argv[1]


def check(inpt, exp):
    p = Popen([COMMAND, inpt], stdin=PIPE, stdout=PIPE, stderr=PIPE,
          bufsize=-1)
    output, error = p.communicate()
    res = output.decode("utf-8")
    if res != exp:
        print("expected {} got {}".format(exp, res))


def main():
    tests = [
                ("10", "10\n"),
                ("-123", "-123\n"),
                ("-1    + 8", "7\n"),
                ("kjlsdf", "error: invalid character: k\n"),
                ("2*3", "6\n"),
                ("1 + 4 / 8 + 2 -- - 8", "error: can't parse ---\n"),
                ("1 + 4 / 8 + 2 -+ 8", "error: can't parse -+\n"),
                ("1 + 4 / 8 + 2 -- 8", "11\n"),
                ("-1    + 8", "7\n"),
                ("1000000000000000", "error: too long number 1000000000000000\n"),
                ("+8 + 7", "error: there must be - or digit on pos 0: str[0]=+\n"),
                ("-1    + 8 +", "error: there can't be operators in the end: +\n"),
                ("-1    /0", "error: zero division\n"),
                ("-100000 * 100000", "error: to big numbers to multiply\n"),
                ("1 + 2147483647", "error: to big numbers to sum\n"),
                ("1 - 1 - 1 - 1", "-2\n"),
                ("11 * -11/-2 + " * 5000 + "1", "300001\n"),
            ]
    for inpt, exp in tests[::]:
        check(inpt, exp)


if __name__ == "__main__":
    main()

