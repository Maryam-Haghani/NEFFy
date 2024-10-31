import neffy


def main():
    try:
        msa_length, msa_depth, residue_neffs, median = neffy.compute_residue_neff(file='../MSAs/example.a2m',
                                                                                       check_validation=True)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth (#unique sequences): {msa_depth}")
        print("Per-residue (column-wise) NEFF:\n", residue_neffs)
        print("Median of per-residue (column-wise) NEFF:", median)

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
