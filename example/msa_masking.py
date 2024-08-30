import sys
import neffy


def main():
    try:
        msa_length, msa_depth, initial_neff, highest_neff, result_file, masked_msa_with_highest_neff, time\
            = neffy.compute_neff_masking(file='../MSAs/T1127.sto', threshold=0.6, mask_count=10,
                                         mask_frac=0.2, norm=neffy.Normalization.No_Normalization)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth: {msa_depth}")
        print(f"Initial NEFF: {initial_neff}")
        print(f"Highest NEFF among maskings: {highest_neff}")
        print(f"Result file: {result_file}")
        print(f"Masked MSA with highest NEFF: {masked_msa_with_highest_neff}")
        print(f"Time taken: {time} seconds")

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
