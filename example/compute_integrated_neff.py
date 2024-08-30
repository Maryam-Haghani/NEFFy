import neffy

def main():
    try:
        msa_length, msa_depth, neff = neffy.compute_neff(
            file=['../MSAs/uniref90_hits.sto', '../MSAs/bfd_uniclust_hits.a3m', '../MSAs/mgnify_hits.sto'],
            is_symmetric = False,
            depth=2048)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth: {msa_depth}")
        print(f"NEFF: {neff}")

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
