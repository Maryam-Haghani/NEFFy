\page usage_guide How to Use

## Table of Contents
- [C++ Executable File](#executable)
  - [NEFF Computation](#neff_computation)
      - [Usage](#neff_usage)
      - [Parameters](#neff_parameters)
      - [Example](#neff_example)
  - [MSA File Conversion](#converter)
      - [Usage](#converter_usage)
      - [Parameters](#converter_parameters)
      - [Example](#converter_example)
- [Python Library](#python)
  - [NEFF Computation](#python_neff)
      - [Parameters](#python_neff_parameters)
      - [Example](#python_neff_example)
  - [MSA File Conversion](#python_converter)
      - [Parameters](#python_converter_parameters)
      - [Example](#python_converter_example)

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
| `--file=<filename>` | Path to the input file containing the multiple sequence alignment (MSA) data  | Yes | N/A | `--file=my_alignment.fasta` |
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
| `--mask_enabled=[true/false]` | Enable random sequence masking for NEFF calculation and return the masking with the highest NEFF | No | false | `--mask_enabled=true`    |
| `--mask_frac=<value>` | Fraction of sequences to be masked in each masking iteration | when _mask_enabled_=true | 0 | `--mask_frac=0.4` |
| `--mask_count=<value>` | Number of masking iterations | when _mask_enabled_=true | 0 | `--mask_count=0`    |
| `--multimer_MSA=[true/false]` | Compute NEFF for both paired MSA and individual monomer MSAs when MSA is in the form of multimer MSA and composed of multiple monomers | No | false | `--multimer_MSA=true`    |
| `--monomer_length=<value>` | Length of the monomers, which is used to obtain NEFF for both paired MSA and individual monomer MSA | when _multimer_MSA_=true | 0 | `--monomer_length=17`    |
| `--column_neff=[true/false]` | Compute Column-wise NEFF | No | false | `--column_neff=true`    |


\anchor neff_example
### Example:

- __Compute Asymmetric NEFF for RNA MSA:__
```sh
    ./neff --file=example.a3m --threshold=0.7 --norm=2 --is_symmetric=false --alphabet=1
```
The resulting NEFF value will be displayed on the console based on the specified options.
.
- __Compute NEFF for Multimer MSA:__
```sh
  ./neff --file=example.sto --multimer_MSA=true --monomer_length=20
```
The tool will identify paired MSA sequences and sequences for the individual MSAs of monomers in the _example.sto_ file which corresponds to a protein dimer, assuming the first monomer has a length of 20 residues. It will report NEFF values for the paired MSA as well as for the MSAs corresponding to unpaired sequences. If the provided MSA is not in the form of a multimer MSA, the tool will raise an error stating "MSA is not in the form of multimer MSA".
.
- __Compute Column-wise NEFF:__
```sh
  ./neff --file=example.aln --column_neff=true
```
The result will display the NEFF value for each position in the alignment on a per-residue (column-wise) basis.
.
- __Random Masking and NEFF Calculation:__
```sh
  ./neff --file=example.fasta --mask_enabled=true --mask_count=10 --mask_frac=0.2
```
The tool will mask 20% of sequences in the example.fasta MSA file and repeat this process 10 times. The NEFF values from each iteration will be saved in the _neff_values.txt_ file. The masked MSA corresponding to the highest NEFF value will be saved in the _MSA_with_highest_neff.fasta_ file, representing the denoised MSA of the given example.fasta MSA, which can be used for downstream tasks requiring a high-quality MSA.

<br>

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
### Example:
- __Convert an A3M file to Stockholm format with RNA alphabet:__
```sh
./converter --in_file=example.a3m --out_file=output.sto --alphabet=1
```
- __Convert a FASTA file to Clustal format without validation:__
```sh
./converter --in_file=example.fasta --out_file=output.clustal --check_validation=false
```
- __Convert an ALN file to PFAM format with DNA alphabet:__
```sh
./converter --in_file=example.aln --out_file=output.pfam --alphabet=2
```

<br>

---
\anchor python
# Python Library

\anchor python_neff
## NEFF Computation

## `compute_neff` Function

\anchor python_neff_parameters
### Parameters:
The method accepts the following parameters:

| Parameter             | Type              | Required | Default Value                | Description                                                                         |
|-----------------------|-------------------|----------|------------------------------|-------------------------------------------------------------------------------------|
| `file`                | string            | Yes      | N/A                          | Path to the input file containing the multiple sequence alignment (MSA)             |
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
| `only_weights`        | bool              | No       | False                        | Return only sequence weights, rather than the final NEFF                            |
| `mask_enabled`        | bool              | No       | False                        | Enable random sequence masking for NEFF calculation and return the masking with the highest NEFF |
| `mask_frac`        | float    | when mask_enabled=true | 0              | Fraction of sequences to be masked in each masking iteration, must be between 0 and 1 |
| `mask_count`          | int      | when mask_enabled=true | 0              | Number of masking iterations                                                        |
| `multimer_MSA` | bool | No | False | Compute NEFF for both paired MSA and individual monomer MSAs when MSA is in the form of multimer MSA and composed of multiple monomers |
| `monomer_length` | int | when multimer_MSA=true | 0   | Length of the monomers, which is used to obtain NEFF for both paired MSA and individual monomer MSA |
| `column_neff`         | bool              | No       | False       | Compute Column-wise NEFF |

\anchor python_neff_example
### Example:
To compute the neff using python library:

```bash
python compute_neff.py
```

Where `compute_neff.py`:

```python
import sys
import neffy


def main():
    try:
        output = neffy.compute_neff(file='./example/example.a2m', alphabet=neffy.Alphabet.RNA)
        print(output)
    except RuntimeError as e:
        print(f"{e}")

if __name__ == "__main__":
    main()
```
<br>

\anchor python_converter
## MSA File Conversion

\anchor python_converter_parameters
### Parameters:
The method accepts the following parameters:

| Parameter | Type | Required | Default Value | Description |
|-----------|------|----------|---------------|-------------|
| in_file | string | Yes | N/A | Path to the input MSA file that needs to be converted |
| out_file | string | Yes | N/A | Path where the converted MSA file will be saved |
| alphabet | Alphabet (Enum) | No | Alphabet.Protein | Enum to specify the type of sequences in the MSA (__Protein__, __RNA__, or __DNA__) |
| check_validation | bool | No | True | Whether to check the validation of the MSA file before conversion |

\anchor python_converter_example
### Example:
To convert MSA:

```bash
python convert_msa.py
```

Where `convert_msa.py`:

```python
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
```

<span style="color:purple; font-size: larger;">
As the `NEFFy` Python library is currently in its beta version, the stable version of the library will include changes where the methods **compute_neff** and **convert_msa** will accept arguments as method parameters.
</span>

<br>

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.

