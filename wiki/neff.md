\page neff NEFF Computation Tool

## Overview
This program computes the Number of Effective Sequences (NEFF) for a multiple sequence alignment (MSA) file. NEFF is a measure of the effective sequence number that accounts for the redundancy and similarity of sequences in the MSA and provides a measure of sequence diversity. It is widely used in bioinformatics to assess the diversity of a set of sequences.

## Features
This NEFF computation tool provides versatile options for analyzing the diversity of sequences in an MSA, including symmetric and asymmetric NEFF, column-wise NEFF, paired and unpaired MSA NEFF, and random masking for denoising. It caters to various needs in bioinformatics and structural biology, ensuring a comprehensive assessment of sequence alignments.

* __Symmetric NEFF__: In this mode, the threshold for considering a pair of sequences as homologous depends on the length of the alignment. This threshold is equal for all sequences, leading to symmetry in similarities.

* __Asymmetric NEFF__: Here, the threshold depends on the number of non-gap residues, making the cutoff different for each sequence. This method provides a more detailed and accurate representation of sequence diversity.

* __Column-wise NEFF (Per-Residue NEFF)__: This method computes NEFF for each position in the alignment. It is used by tools like AlphaFold for more precise per-residue sequence diversity assessment. 
Per-residue NEFF values for each position in the MSA were calculated by summing the weights of the sequences that have a residue (i.e., non-gap characters) at that specific position.

* __Paired MSA and Unpaired MSA NEFF__: The tool can recognize paired MSA sequences and unpaired sequences across chains in a block-diagonal fashion, as used by AlphaFold-Multimer. By providing the length of the first monomer, the tool can compute NEFF for each set.

* __Random Masking for MSA Denoising based on NEFF__: Users can specify the percentage of sequences to be masked and the number of iterations for masking. The tool computes NEFF for each iteration and reports the highest NEFF value along with the corresponding masked MSA. This feature helps in denoising the MSA for downstream tasks.

For further details, please refer to the [Usage](#usage) section and [Example](#example).

\anchor usage
## Usage
```sh
./neff --file=<input_file> [options]
```
The code takes command-line flags for input and subsequently computes the score for an MSA file.

To calculate NEFF, provide an MSA file and indicate the desired flags. NEFF will be computed and presented as the result.

### Options:
The code accepts the following command-line flags:
| Flag | Description | Required | Default Value | Example	| 
|------|-------------|----------|---------------|---------|
| `--file=<filename>` | Input file containing multiple sequence alignment  | Yes | N/A | `--file=my_alignment.fasta` |
| `--alphabet=<value>` | Alphabet of MSA <br /> __0__: Protein <br /> __1__: RNA <br /> __2__: DNA | No | 0 | `--alphabet=1` |
| `--check_validation=[true/false]` | Validate the input MSA file based on alphabet or not | No | false | `--check_validation=true` |
| `--threshold=<value>`	| Threshold value of considering two sequences similar (between 0 and 1) | No | 0.8 | `--threshold=0.7` |
| `--norm=<value>` | Normalization option for NEFF <br /> __0__: Normalize by the square root of sequence length <br /> __1__: Normalize by the sequence length <br /> __2__: No Normalization | No | 0 | `--norm=2` |
| `--omit_query_gaps=[true/false]` | Omit gap positions of query sequence from entire sequences for NEFF computation | No | true | `--omit_query_gaps=true`	|
| `--is_symmetric =true/false]` | Consider gaps in number of differences when computing sequence similarity cutoff (asymmetric) or not (symmetric)| No | true | `--is_symmetric=false`	|
| `--non_standard_option=<value>` | Options for handling non-standard letters of the specified alphabet <br /> __0__: Treat them the same as standard letters <br /> __1__: Consider them as gaps when computing similarity cutoff of sequences (only used in asymmetryc version) <br /> __2__: Consider them as gaps in computing similarity cutoff and checking position of match/mismatch | No | 0 | `--non_standard_option=1` |
| `--depth=<value>` | Depth of MSA to be used in NEFF computation (starting from the first sequence) | No | inf (consider all sequences) | `--depth=10` <br />(if given value is greater than original depth, it considers the original depth) |
| `--gap_cutoff=<value>`| Threshold for considering a position as gappy and removing that (between 0 and 1) | No | 1 (no gappy position) | `--gap_cutoff=0.7` |
| `--pos_start=<value>`| Start position of each sequence to be considered in NEFF (inclusive) | No | 1 (the first position) | `--pos_start=10` |
| `--pos_end=<value>`| Last position of each sequence to be considered in NEFF (inclusive) | No | inf (consider all sequence) | `--pos_end=50` (if given value is greater than the length of the MSA sequences, consider length of sequences in the MSA)|
| `--only_weights=[true/false]` | Return only sequence weights, rather than the final NEFF | No | false | `--only_weights=true`    |
| `--mask_enabled=[true/false]` | Enable random sequence masking for NEFF calculation and return the masking with the highest NEFF | No | false | `--mask_enabled=true`    |
| `--mask_percent=<value>` | Percentage of sequences to be masked in each masking iteration | when _mask_enabled_=true | 0 | `--mask_percent=0.4`    |
| `--mask_count=<value>` | Number of masking iterations | when _mask_enabled_=true | 0 | `--mask_count=0`    |
| `--multimer_MSA=[true/false]` | Compute NEFF for both paired MSA and individual monomer MSAs when MSA is in the form of multimer MSA and composed of 3 parts | No | false | `--multimer_MSA=true`    |
| `--first_monomer_length=<value>` | Length of the first monomer, which is used to obtain NEFF for both paired MSA and individual monomer MSA | when _multimer_MSA_=true | 0 | `--first_monomer_length=17`    |
| `--column_neff=[true/false]` | Compute Column-wise NEFF | No | false | `--column_neff=true`    |

### Supported File Formats
- __A2M__ (aligned FASTA-like format)
- __A3M__ (compressed aligned FASTA-like format with lowercase letters for insertions)
- __FASTA__, __AFA__, __FAS__, __FST__, __FSA__ (FASTA format)
- __STO__ (Stockholm format)
- __CLUSTAL__ (CLUSTAL format)
- __ALN__ (ALN format)
- __PFAM__ (format mostly used for nucleotides)

In the [MSA format page](\ref msaFormats), you will find a brief explanation of each format, along with an illustrative alignment example for each one.

\anchor example
## Example

- __Compute Asymmetric NEFF for RNA MSA:__
```sh
    ./neff --file=example.a3m --threshold=0.7 --norm=2 --is_symmetric=false --alphabet=1
```
The resulting NEFF value will be displayed on the console based on the specified options.
.
- __Compute NEFF for Multimer MSA:__
```sh
  ./neff --file=example.sto --multimer_MSA=true --first_monomer_length=20
```
The tool will identify paired MSA sequences and sequences for the individual MSAs of monomers in the _example.sto_ file, assuming the first monomer has a length of 20 residues. It will report NEFF values for the paired MSA as well as for the MSAs corresponding to unpaired sequences. If the provided MSA is not in the form of a multimer MSA, the tool will raise an error stating "MSA is not in the form of multimer MSA".
.
- __Compute Column-wise NEFF:__
```sh
  ./neff --file=example.aln --column_neff=true
```
The result will display the NEFF value for each position in the alignment on a per-residue (column-wise) basis.
.
- __Random Masking and NEFF Calculation:__
```sh
  ./neff --file=example.fasta --mask_enabled=true --mask_count=10 --mask_percent=0.2
```
The tool will mask 20% of sequences in the example.fasta MSA file and repeat this process 10 times. The NEFF values from each iteration will be saved in the _neff_values.txt_ file. The masked MSA corresponding to the highest NEFF value will be saved in the _MSA_with_highest_neff.fasta_ file, representing the denoised MSA of the given example.fasta MSA, which can be used for downstream tasks requiring a high-quality MSA.

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.
