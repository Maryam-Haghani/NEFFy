import sys
import neffy


def main():
    try:
        output = neffy.convert_msa(in_file='./example/example.a2m', out_file='./example/example.fasta')
        print(output)
    except RuntimeError as e:
        print(f"{e}")

if __name__ == "__main__":
    main()
