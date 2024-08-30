import sys
import neffy


def main():
    try:
        output = neffy.convert_msa(in_file='../MSAs/example.a2m', out_file='../MSAs/example.sto')
        print(output)
    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
