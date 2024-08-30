import neffy


def main():
    try:
        msa_length, msa_depth, weights = neffy.compute_neff(file='../MSAs/example.a2m', only_weights=True, threshold=0.2)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth: {msa_depth}")
        print(f"Sequence weights: {weights}")

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
