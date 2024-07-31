import sys
import neffy


def main():
    try:
        output = neffy.compute_neff(file='./example/example.a2m', only_weights=True)
        print(output)
    except RuntimeError as e:
        print(f"{e}")

if __name__ == "__main__":
    main()
