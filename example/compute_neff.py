import neffy

def main():
    try:
        msa_length, msa_depth, neff = neffy.compute_neff(file='../MSAs/example.a2m',
                                                         norm=neffy.Normalization.No_Normalization)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth (#unique sequences): {msa_depth}")
        print(f"NEFF: {neff}")

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
