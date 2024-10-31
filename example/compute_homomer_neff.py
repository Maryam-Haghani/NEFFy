import neffy


def main():
    try:
        msa_length, msa_depth, entire_neff, neff_values = (
            neffy.compute_multimer_neff(file='../MSAs/homomer.aln', stoichiom="A2"
                                        ))

        print(f"MSA length: {msa_length}")
        print(f"MSA depth (#unique sequences): {msa_depth}")
        print(f"NEff of entire MSA: {entire_neff}")
        print(f"neff of individual MSA: {neff_values}")

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
