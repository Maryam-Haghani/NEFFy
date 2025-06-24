\page usage_guide How to Use

## Table of Contents
- [C++ Executable File](#executable)
  - [NEFF Computation](#neff_computation)
      - [Usage](#neff_usage)
      - [Parameters](#neff_parameters)
      - [Examples](#neff_example)
  - [MSA File Conversion](#converter)
      - [Usage](#converter_usage)
      - [Parameters](#converter_parameters)
      - [Example](#converter_example)
- [Python Library](#python)
  - [NEFF Computation](#python_neff_main)
    - [compute_neff: NEFF Computation](#python_neff)
        - [Parameters](#python_neff_params)
        - [Example](#python_neff_example)
    - [compute_multimer_neff: NEFF Computation for Multimer MSA](#python_neff_multimer)
      - [Parameters](#python_neff_multimer_params)
      - [Example](#python_neff_multimer_example)
    - [compute_residue_neff: Per-Residue (Column-Wise) NEFF Computation](#python_neff_residue)
      - [Parameters](#python_neff_residue_params)
      - [Example](#python_neff_residue_example)
  - [convert_msa: MSA File Conversion](#python_converter)
      - [Parameters](#python_convert_msa_params)
      - [Example](#python_convert_msa_example)

<br>

\anchor executable
#  C++ Executable File

\anchor neff_computation
## NEFF Computation
\anchor neff_usage
### Usage:
```sh
./neff --file=<input_file> [options]
```

To calculate NEFF, provide an MSA file and indicate the desired parameters. NEFF will be computed and presented as the result.

\anchor neff_parameters
### Parameters:
The code accepts the following command-line flags:
| Flag | Description | Required | Default Value | Example	| 
|------|-------------|----------|---------------|---------|
| `--file=<list of filenames>` | Input files (comma-separated, no spaces) containing multiple sequence alignments | Yes | N/A | `--file=my_alignment.fasta` |
| `--alphabet=<value>` | Alphabet of MSA <br /> __0__: Protein <br /> __1__: RNA <br /> __2__: DNA | No | 0 | `--alphabet=1` |
| `--check_validation=[true/false]` | Validate the input MSA file based on alphabet or not | No | false | `--check_validation=true` |
| `--threshold=<value>`	| Similarity threshold for sequence weighting, must be between 0 and 1. | No | 0.8 | `--threshold=0.7` |
| `--norm=<value>` | Normalization option for NEFF <br /> __0__: Normalize by the square root of sequence length <br /> __1__: Normalize by the sequence length <br /> __2__: No Normalization | No | 0 | `--norm=2` |
| `--omit_query_gaps=[true/false]` | Omit gap positions of query sequence from all sequences for NEFF computation | No | true | `--omit_query_gaps=true`	|
| `--is_symmetric =true/false]` | Consider gaps in number of differences when computing sequence similarity cutoff (asymmetric) or not (symmetric)| No | true | `--is_symmetric=false`	|
| `--non_standard_option=<value>` | Options for handling non-standard letters of the specified alphabet <br /> __0__: Treat them the same as standard letters <br /> __1__: Consider them as gaps when computing similarity cutoff of sequences (only used in asymmetryc version) <br /> __2__: Consider them as gaps in computing similarity cutoff and checking position of match/mismatch | No | 0 | `--non_standard_option=1` |
| `--depth=<value>` | Depth of MSA to be used in NEFF computation (starting from the first sequence) | No | inf (consider the whole sequence) | `--depth=10` <br />(if given value is greater than original depth, it considers the original depth) |
| `--gap_cutoff=<value>`| Threshold for considering a position as gappy and removing that (between 0 and 1) | No | 1 (no gappy position) | `--gap_cutoff=0.7` |
| `--pos_start=<value>`| Start position of each sequence to be considered in NEFF (inclusive) | No | 1 (the first position) | `--pos_start=10` |
| `--pos_end=<value>`| Last position of each sequence to be considered in NEFF (inclusive) | No | inf (consider the whole sequence) | `--pos_end=50` (if given value is greater than the length of the MSA sequences, consider length of sequences in the MSA)|
| `--only_weights=[true/false]` | Return only sequence weights, rather than the final NEFF | No | false | `--only_weights=true`    |
| `--multimer_MSA=[true/false]` | Compute NEFF for  MSA of a multimer | No | false | `--multimer_MSA=true`    |
| `--stoichiom=<value>` | Stochiometry of the multimer | when _multimer_MSA_=true |  | `--stoichiom=A2B1`    |
| `--chain_length=<list of values>` | Length of the chains in a heteromer  | when _multimer_MSA_=true and multimer is a heteromer | 0 | `--chain_length=17 45`    |
| `--residue_neff=[true/false]` | Compute per-residue (column-wise) NEFF | No | false | `--residue_neff=true`    |
| `--skip_lines=<value>` | Number of lines to skip at the beginning of the input file. | No | 0 | `--skip_lines=1` |


\anchor neff_example
### Examples:

- __Compute Symmetric NEFF for protein MSA (No Normalization):__
```sh
    ./neff --file=../MSAs/example.a2m --norm=2 
```
Result:
> MSA sequence length: 56<br>
> MSA depth: 5<br>
> NEFF: 5  
<br>

- __Compute NEFF for RNA MSA with Default Normalization (sqrt(L)) using 50% Gap Cutoff:__
```sh
    ./neff --file=../MSAs/rna.fasta --gap_cutoff=0.5 --alphabet=1
```
Result:
> MSA sequence length: 28<br>
> MSA depth: 1176<br>
> NEFF: 5.08477
<br>

- __Compute Sequence Weights for protein MSA with Default Normalization (sqrt(L)):__
```sh
    ./neff --file=../MSAs/example.a2m --only_weights=true --threshold=0.2
```
Result:
> MSA sequence length: 56<br>
> MSA depth: 5<br>
> Sequence weights:<br>
> 1 0.333333 0.25 0.333333 0.25
<br>

- __Compute per-residue (column-wise) NEFF:__
```sh
  ./neff --file=../MSAs/example.a2m --check_validation=true --residue_neff=true
```
Result:
> MSA sequence length: 56<br>
> MSA depth: 5<br>
> Per-residue (column-wise) NEFF:<br>
> 0.668153 0.534522 0.668153 0.534522 0.668153 0.668153 0.668153 0.668153 0.534522 0.534522 0.534522 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.534522 0.534522 0.534522 0.668153 0.400892 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.668153 0.534522 0.534522 0.534522 0.534522 0.534522 0.400892 0.534522 0.534522 0.534522 0.400892 0.133631 0.133631 0.133631 0.133631 0.133631 0.133631 0.133631 0.133631<br>
> Median of per-residue (column-wise) NEFF: 0.668153
<br>

- __Compute Asymmetric NEFF for an Integration of MSAs with Default Normalization (sqrt(L)):__
```sh
    ./neff --file=../MSAs/uniref90_hits.sto,../MSAs/bfd_uniclust_hits.a3m,../MSAs/mgnify_hits.sto --depth=2048 --is_symmetric=false
    
```
Result:
> MSA length: 338<br>
> MSA depth: 2048<br>
> NEFF: 106.067
The tool will integrate MSAs from the list of files in the specified order and compute the NEFF value for the integrated MSA up to a depth of 2048. If the tool reaches the specified depth before all files are integrated, it will stop processing the remaining files.<br>
This process is akin to how AlphaFold2.3 produces the final MSA for a protein by combining three distinct MSAs.<br>
<br><br>


- __Compute NEFF for MSA of a Homomer (Default Normalization):__
```sh
  ./neff --file=../MSAs/homomer.aln --multimer_MSA=true --stoichiom=A2
```
Result:
> MSA sequence length: 102<br>
> MSA depth: 2048<br>
> NEff of entire MSA: 34.8453<br>
> Neff of individual MSA: 49.2787<br>

The tool will detect a single repetition of the individual MSA of the chain within the given MSA file and report the NEFF value for it. If the provided MSA is not in the format of a multimer MSA of a homomer (i.e., the same MSA file repeated 'n' times, where here n=2), the tool will raise an error.
<br><br>

- __Compute NEFF for MSA of a Heteromer (L Normalization):__
```sh
  ./neff --file=../MSAs/heteromer.aln --multimer_MSA=true --stoichiom=A2B1 --chain_length=51,73 --norm=1
```
Result:
> MSA sequence length: 175<br>
> MSA depth: 3072<br>
> NEFF of entire MSA:3.2836<br>
> NEFF of Paired MSA (depth=1024): 3.02721<br>
> NEFF of Individual MSA for Chain A (depth=1024): 4.37843<br>
> NEFF of Individual MSA for Chain B (depth=1024): 3.0194
The tool will identify paired MSA sequences and the sequences for each individual MSA of chains in the given MSA file, assuming the first monomer has a length of 51 residues and the second has a length of 73. It will report NEFF values for the paired MSA as well as for the MSAs corresponding to unpaired sequences. If the provided MSA is not in the format of a multimer MSA of a heteromer, the tool will raise an error.
<br><br>

\anchor converter
## MSA File Conversion
To convert an MSA file, specify the input file, output file, and the desired input and output formats. The tool will read the input file, perform the conversion, and write the resulting MSA to the output file in the specified format.

\anchor converter_usage
### Usage:
```sh
./converter --in_file=<input_file> --out_file=<input_file> [options]
```
\anchor converter_parameters
### Parameters:
The code accepts the following command-line flags:
| Flag | Description | Required | Default Value | Example	| 
|------|-------------|----------|---------------|---------|
| `--in_file=<filename>` | Specifies the input MSA file to be converted.<br /> Replace `<filename>` with the path and name of the input file | Yes | N/A | `--in_file=input.fasta` |
| `--out_file=<filename>`| Specifies the output file where the converted MSA will be saved.<br /> Replace `<filename>` with the desired path and name of the output file | Yes | N/A | `--out_file=output.a2m` |
| `--alphabet=<value>` | Alphabet of MSA <br /> __0__: Protein <br /> __1__: RNA <br /> __2__: DNA | No | 0 | `--alphabet=1` |
| `--check_validation=[true/false]` | Validate the input MSA file based on alphabet or not | No | true | `--check_validation=true` |

Please note that the conversion is performed based on the specified input and output file extensions.

\anchor converter_example
### Examples:
- __Convert an A3M file to Stockholm format with RNA alphabet:__
```sh
./converter --in_file=../MSAs/example.a2m --out_file=../MSAs/example.sto --alphabet=1
```
- __Convert a STO file to Clustal format without validation:__
```sh
./converter --in_file=../MSAs/example.sto --out_file=../MSAs/example.clustal --check_validation=false
```
- __Convert an FASTA file to ALN format with RNA alphabet:__
```sh
./converter --in_file=../MSAs/rna.fasta --out_file=../MSAs/rna.aln --alphabet=1
```

<br>

---
\anchor python
# Python Library

\anchor python_neff_main
## NEFF Computation

\anchor python_neff
## `compute_neff`
\anchor python_neff_params
### Parameters:
The method accepts the following parameters:

| Parameter             | Type              | Required | Default Value                | Description                                                                         |
|-----------------------|-------------------|----------|------------------------------|-------------------------------------------------------------------------------------|
| `file`                | list [string]            | Yes      | N/A                          | Path to the input file containing the multiple sequence alignment (MSA)             |
| `only_weights`        | bool              | No       | False                        | Return only sequence weights, rather than the final NEFF                          |
| `alphabet`            | Alphabet (Enum)   | No       | Alphabet.Protein             | Enum to specify the type of sequences in the MSA (__Protein__, __RNA__, or __DNA__) |
| `check_validation`    | bool              | No       | False                        | Validate the input MSA file based on alphabet or not                                |
| `threshold`           | float             | No       | 0.8                          | Similarity threshold for sequence weighting, must be between 0 and 1               |
| `norm` | Normalization (Enum) | No | Normalization.Sqrt_Length | Enum to specify normalization method for NEFF (__Sqrt_Length__, __Length__, or __No_Normalization__)|
| `omit_query_gaps`     | bool              | No       | True                         | Omit gap positions of query sequence from all sequences for NEFF computation     |
| `is_symmetric` | bool | No | True | Consider gaps in number of differences when computing sequence similarity cutoff (asymmetric) or not (symmetric) |
| `non_standard_option` | NonStandardOption (Enum) | No | NonStandardOption.AsStandard | Enum to handle non-standard residues of the specified alphabet (__AsStandard__, __ConsiderGapInCutoff__, __ConsiderGap__) |
| `depth`               | int             | No       | inf (consider the whole sequence) | Depth of MSA to be used in NEFF computation (starting from the first sequence) |
| `gap_cutoff`          | float               | No       | 1 (no gappy position)        | Threshold for considering a position as gappy and removing that (between 0 and 1)   |
| `pos_start`           | int               | No       | 1 (the first position)       | Start position of each sequence to be considered in NEFF (inclusive)                |
| `pos_end`             | int             | No       | inf (consider the whole sequence) | Last position of each sequence to be considered in NEFF (inclusive)            |
| `skip_lines`          | int               | No       | 0                            | Number of lines to skip at the beginning of the input file.                               |

\anchor python_neff_example
### Examples:
- __Compute Symmetric NEFF for protein MSA with No Normalization:__
```sh
  import neffy

  def main():
      try:
          msa_length, msa_depth, neff = neffy.compute_neff(
            file='../MSAs/example.a2m',
            norm=neffy.Normalization.No_Normalization
            )

          print(f"MSA length: {msa_length}")
          print(f"MSA depth: {msa_depth}")
          print(f"NEFF: {neff}")

      except RuntimeError as e:
          print(e)

  if __name__ == "__main__":
      main()
```
Result:
> MSA length: 56<br>
> MSA depth: 5<br>
> NEFF: 5.0  
<br>

- __Compute NEFF for RNA MSA with Default Normalization (sqrt(L)) using 50% Gap Cutoff:__
```sh
  import neffy

  def main():
      try:
          msa_length, msa_depth, neff = neffy.compute_neff(
            file='../MSAs/rna.fasta',
            alphabet=neffy.Alphabet.RNA,
            gap_cutoff=0.5)

          print(f"MSA length: {msa_length}")
          print(f"MSA depth: {msa_depth}")
          print(f"NEFF: {neff}")

      except RuntimeError as e:
          print(e)

  if __name__ == "__main__":
      main()
```
Result:
> MSA length: 28<br>
> MSA depth: 1176<br>
> NEFF: 5.08477
<br>

- __Compute Sequence Weights for protein MSA with Default Normalization (sqrt(L)):__
```sh
  import neffy

  def main():
      try:
          msa_length, msa_depth, weights = neffy.compute_neff(
            file='../MSAs/example.a2m',
            only_weights=True,
            threshold=0.2)

          print(f"MSA length: {msa_length}")
          print(f"MSA depth: {msa_depth}")
          print(f"Sequence weights: {weights}")

      except RuntimeError as e:
          print(e)

  if __name__ == "__main__":
      main()
```
Result:
> MSA length: 56<br>
> MSA depth: 5<br>
> Sequence weights: [1.0, 0.333333, 0.25, 0.333333, 0.25]
<br>

- __Compute Asymmetric NEFF for an Integration of MSAs with Default Normalization (sqrt(L)):__
```sh
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
    
```
Result:
> MSA length: 338<br>
> MSA depth: 2048<br>
> NEFF: 106.067

The tool will integrate MSAs from the list of files in the specified order and compute the NEFF value for the integrated MSA up to the gievn depth. If the tool reaches the specified depth before all files are integrated, it will stop processing the remaining files.<br>
This process is akin to how AlphaFold2.3 produces the final MSA for a protein by combining three distinct MSAs.<br>
<br><br>

\anchor python_neff_multimer
## `compute_multimer_neff`
\anchor python_neff_multimer_params
### Parameters:
The method accepts the following parameters:

| Parameter             | Type              | Required | Default Value                | Description                                                                         |
|-----------------------|-------------------|----------|------------------------------|-------------------------------------------------------------------------------------|
| `file`                | string            | Yes      | N/A                          | Path to the input file containing the multiple sequence alignment (MSA)             |
| `stoichiom`    | string | Yes | - | Stochiometry of the multimer
| `chain_length` | list [int] | when multimer is a heteromer | 0   | Length of the chains in a heteromer  |
| `alphabet`            | Alphabet (Enum)   | No       | Alphabet.Protein             | Enum to specify the type of sequences in the MSA (__Protein__, __RNA__, or __DNA__) |
| `check_validation`    | bool              | No       | False                        | Validate the input MSA file based on alphabet or not                                |
| `threshold`           | float             | No       | 0.8                          | Similarity threshold for sequence weighting, must be between 0 and 1               |
| `norm` | Normalization (Enum) | No | Normalization.Sqrt_Length | Enum to specify normalization method for NEFF (__Sqrt_Length__, __Length__, or __No_Normalization__)|
| `omit_query_gaps`     | bool              | No       | True                         | Omit gap positions of query sequence from all sequences for NEFF computation     |
| `is_symmetric` | bool | No | True | Consider gaps in number of differences when computing sequence similarity cutoff (asymmetric) or not (symmetric) |
| `non_standard_option` | NonStandardOption (Enum) | No | NonStandardOption.AsStandard | Enum to handle non-standard residues of the specified alphabet (__AsStandard__, __ConsiderGapInCutoff__, __ConsiderGap__) |
| `depth`               | int             | No       | inf (consider the whole sequence) | Depth of MSA to be used in NEFF computation (starting from the first sequence) |
| `gap_cutoff`          | float           | No       | 1 (no gappy position)        | Threshold for considering a position as gappy and removing that (between 0 and 1)   |
| `pos_start`           | int             | No       | 1 (the first position)       | Start position of each sequence to be considered in NEFF (inclusive)                |
| `pos_end`             | int             | No       | inf (consider the whole sequence) | Last position of each sequence to be considered in NEFF (inclusive)            |
| `skip_lines`          | int               | No       | 0                            | Number of lines to skip at the beginning of the input file.                               |

\anchor python_neff_multimer_example
### Examples:
- __Compute NEFF for MSA of a Homomer (Default Normalization):__
```sh
import neffy

def main():
    try:
        msa_length, msa_depth, entire_neff, neff_values = neffy.compute_multimer_neff(
          file='../MSAs/homomer.aln',
          stoichiom="A2"
          )

        print(f"MSA length: {msa_length}")
        print(f"MSA depth: {msa_depth}")
        print(f"NEff of entire MSA: {entire_neff}")
        print(f"neff of individual MSA: {neff_values}")

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
```
Result:
> MSA length: 102<br>
> MSA depth: 2048<br>
> NEFF of entire MSA: 34.8453<br>
> NEFF of individual MSA: 49.2787
The tool will detect a single repetition of the individual MSA of the chain within the given MSA file and report the NEFF value for it. If the provided MSA is not in the format of a multimer MSA of a homomer (i.e., the same MSA file repeated 'n' times given in 'stoichiom'), the tool will raise an error.
<br><br>

- __Compute NEFF for MSA of a Heteromer (L Normalization):__
```sh
  import neffy


  def main():
      try:
          msa_length, msa_depth, entire_neff, neff_values = neffy.compute_multimer_neff(
                file='../MSAs/heteromer.aln',
                stoichiom="A2B1",
                chain_length=[51, 73],
                norm=neffy.Normalization.Length)

          print(f"MSA length: {msa_length}")
          print(f"MSA depth: {msa_depth}")
          print(f"NEff of entire MSA: {entire_neff}")
          print(f"NEFF values: {neff_values}")

      except RuntimeError as e:
          print(e)

  if __name__ == "__main__":
      main()
```
Result:
> MSA length: 175<br>
> MSA depth: 3072<br>
> NEff of entire MSA: 3.2836<br>
> NEFF values: {'paired': {'neff': 3.02721, 'depth': 1024}, 'A': {'neff': 4.37843, 'depth': 1024}, 'B': {'neff': 3.0194, 'depth': 1024}}

The tool will identify paired MSA sequences and the sequences for each individual MSA of chains in the given MSA file, based on the given lengths in 'chain_length'. It will report NEFF values for the paired MSA as well as for the MSAs corresponding to unpaired sequences. If the provided MSA is not in the format of a multimer MSA of a heteromer, the tool will raise an error.
<br><br>

\anchor python_neff_residue
## `compute_residue_neff`
\anchor python_neff_residue_params
### Parameters:
The method accepts the following parameters:

| Parameter             | Type              | Required | Default Value                | Description                                                                         |
|-----------------------|-------------------|----------|------------------------------|-------------------------------------------------------------------------------------|
| `file`                | list [string]     | Yes      | N/A                          | Path to the input file containing the multiple sequence alignment (MSA)             |
| `alphabet`            | Alphabet (Enum)   | No       | Alphabet.Protein             | Enum to specify the type of sequences in the MSA (__Protein__, __RNA__, or __DNA__) |
| `check_validation`    | bool              | No       | False                        | Validate the input MSA file based on alphabet or not                                |
| `threshold`           | float             | No       | 0.8                          | Similarity threshold for sequence weighting, must be between 0 and 1               |
| `norm` | Normalization (Enum) | No | Normalization.Sqrt_Length | Enum to specify normalization method for NEFF (__Sqrt_Length__, __Length__, or __No_Normalization__)|
| `omit_query_gaps`     | bool              | No       | True                         | Omit gap positions of query sequence from all sequences for NEFF computation     |
| `is_symmetric` | bool | No | True | Consider gaps in number of differences when computing sequence similarity cutoff (asymmetric) or not (symmetric) |
| `non_standard_option` | NonStandardOption (Enum) | No | NonStandardOption.AsStandard | Enum to handle non-standard residues of the specified alphabet (__AsStandard__, __ConsiderGapInCutoff__, __ConsiderGap__) |
| `depth`               | int             | No       | inf (consider the whole sequence) | Depth of MSA to be used in NEFF computation (starting from the first sequence) |
| `gap_cutoff`          | float               | No       | 1 (no gappy position)        | Threshold for considering a position as gappy and removing that (between 0 and 1)   |
| `pos_start`           | int               | No       | 1 (the first position)       | Start position of each sequence to be considered in NEFF (inclusive)                |
| `pos_end`             | int             | No       | inf (consider the whole sequence) | Last position of each sequence to be considered in NEFF (inclusive)            |
| `skip_lines`          | int               | No       | 0                            | Number of lines to skip at the beginning of the input file.                               |

\anchor python_neff_residue_example
### Examples:
- __Compute per-residue (column-wise) NEFF:__
```sh
import neffy

def main():
    try:
        msa_length, msa_depth, residue_neffs, median = neffy.compute_residue_neff(
          file='../MSAs/example.a2m',
          check_validation=True)

        print(f"MSA length: {msa_length}")
        print(f"MSA depth: {msa_depth}")
        print("Per-residue (column-wise) NEFF:\n", residue_neffs)
        print("Median of per-residue (column-wise) NEFF:", median)

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
```
Result:
> MSA length: 56<br>
> MSA depth: 5<br>
> Per-residue (column-wise) NEFF:<br>
> [0.668153, 0.534522, 0.668153, 0.534522, 0.668153, 0.668153, 0.668153, 0.668153, 0.534522, 0.534522, 0.534522, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.534522, 0.534522, 0.534522, 0.668153, 0.400892, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.668153, 0.534522, 0.534522, 0.534522, 0.534522, 0.534522, 0.400892, 0.534522, 0.534522, 0.534522, 0.400892, 0.133631, 0.133631, 0.133631, 0.133631, 0.133631, 0.133631, 0.133631, 0.133631]<br>
> Median of per-residue (column-wise) NEFF: 0.668153
<br>

\anchor python_converter
## MSA File Conversion
## `convert_msa`
\anchor python_convert_msa_params
### Parameters:
The method accepts the following parameters:

| Parameter | Type | Required | Default Value | Description |
|-----------|------|----------|---------------|-------------|
| in_file | string | Yes | N/A | Path to the input MSA file that needs to be converted |
| out_file | string | Yes | N/A | Path where the converted MSA file will be saved |
| alphabet | Alphabet (Enum) | No | Alphabet.Protein | Enum to specify the type of sequences in the MSA (__Protein__, __RNA__, or __DNA__) |
| check_validation | bool | No | True | Whether to check the validation of the MSA file before conversion |

\anchor python_convert_msa_example
### Example:
```bash
import sys
import neffy

def main():
    try:
        output = neffy.convert_msa(
          in_file='../MSAs/example.a2m',
          out_file='../MSAs/example.sto')

        print(output)

    except RuntimeError as e:
        print(e)

if __name__ == "__main__":
    main()
```

<br><br>
<span style="color:purple;">
<!-- As the `NEFFy` Python library is currently in its beta version, the stable version of the library will include changes where the methods **compute_neff** and **convert_msa** will accept arguments as method parameters. -->
**** All example MSAs used here can be found in the ['MSA' folder of Github repository](https://github.com/Maryam-Haghani/NEFFy/tree/main/MSAs).

**** All Python scripts used in the examples can be found in the ['example' folder of Github repository](https://github.com/Maryam-Haghani/NEFFy/tree/python-lib/example).
</span>

<br>

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.

