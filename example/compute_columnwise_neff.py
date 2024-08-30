import neffy


def main():
    try:
        msa_length, msa_depth, column_wise_neffs, avg = neffy.compute_column_wise_neff(file='../MSAs/example.a2m',
                                                                                       check_validation=True)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth: {msa_depth}")
        print("Column-wise NEFF:", column_wise_neffs)
        print("Average of Column-wise NEFF:", avg)

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
