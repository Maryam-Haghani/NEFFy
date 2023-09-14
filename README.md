# Neffinator: NEFF Calculator and MSA File Converter
This tool provides functionality to compute the Number of Effective sequences (NEFF) and convert Multiple Sequence Alignment (MSA) files between different formats.

### Project Components
This project consists of the following components:
* __FlagHandler__: Handles command-line flags. <br>
* __MSAReader__: Reads an MSA file and processes the sequences in a specific format. <br>
* __MSAWriter__: Writes MSA to an output file in a specific format. <br>
* __Common__: Additional utility functions and common definitions. <br>

### Getting Started:
Download the source code and compile it using a C++ compiler that is compatible with C++11 or a more recent version. The provided Makefile in the repository can also be utilized for this purpose. <br>
Once the compilation is complete, you can run the program via the command line.

## 1. NEFF Computation 

NEFF computation determines the effective number of homologous sequences within a Multiple Sequence Alignment (MSA). It takes into account sequence similarities and provides a measure of sequence diversity.  <br>
The code takes command-line flags for input and subsequently computes the score for an MSA file.

### Usage
To calculate NEFF, provide an MSA file and indicate the desired flags. NEFF will be computed and presented as the result.

#### Command-line Flags:
The code accepts the following command-line flags:
| Flag | Description | Required | Default Value | Example	| 
|------|-------------|----------|---------------|---------|
| `--file=<filename>` | Input file containing multiple sequence alignment  | Yes | N/A | `--file=my_alignment.fasta` |
| `--alphabet=<value>` | Alphabet of MSA <br /> __0__: Protein <br /> __1__: RNA <br /> __2__: DNA | No | 0 | `--alphabet=1` |
| `--check_validation=[true/false]` | Whether to validate the input MSA file based on alphabet or not | No | false | `--check_validation=true` |
| `--threshold=<value>`	| Threshold value of considering two sequences similar (between 0 and 1) | No | 0.8 | `--threshold=0.7` |
| `--norm=<value>` | Normalization option for NEFF <br /> __0__: Normalize by the square root of sequence length <br /> __1__: Normalize by the sequence length <br /> __2__: No Normalization | No | 0 | `--norm=2` |
| `--omit_query_gaps=[true/false]` | Whether to omit gap positions of query sequence from entire sequences for NEFF computation | No | true | `--omit_query_gaps=true`	|
| `--is_symmetric =true/false]` | Whether to consider gaps in number of differences when computing sequence similratity cutoff | No | true | `--is_symmetric=false`	|
| `--non_standard_option=<value>` | Options for handling non-standard letters of the specified alphabet <br /> __0__: Treat them the same as standard letters <br /> __1__: Consider them as gaps when computing similarity cutoff of sequences (only used in asymmetryc version) <br /> __2__: Consider them as gaps in computing similarity cutoff and checking position match/mismatche | No | 0 | `--non_standard_option=1` |
| `--depth=<value>` | Depth of MSA to be cosidered in computation (strating from the first sequence) | No | inf (consider all sequences) | `--depth=10` <br />(if given value is greater than original depth, it considers the original depth) |
| `--gap_cutoff=<value>`| Threshold for considering a position as gappy and removing that (between 0 and 1) | No | 1 (no gappy position) | `--gap_cutoff=0.7` |


#### Example
    ./neff --file=alignment.fasta --threshold=0.6 --norm=2 --is_symmetric=false --check_validation=true
  As output, it will print the final MSA lenght, depth and Neff to the console, based on the given options.

## 2. MSA File Conversion
The MSA file conversion allows you to convert MSA files between different supported formats. <br />
All you need is specify the input and output files with their formats, and the tool will perform the conversion accordingly.

### Usage
To convert an MSA file, specify the input file, output file, and the desired input and output formats. The tool will read the input file, perform the conversion, and write the resulting MSA to the output file in the specified format.

#### Command-line Flags:
The code accepts the following command-line flags:
| Flag | Description | Required | Default Value | Example	| 
|------|-------------|----------|---------------|---------|
| `--in_file=<filename>` | Specifies the input MSA file to be converted.<br /> Replace `<filename>` with the path and name of the input file | Yes | N/A | `--in_file=input.fasta` |
| `--out_file=<filename>`| Specifies the output file where the converted MSA will be saved.<br /> Replace `<filename>` with the desired path and name of the output file | Yes | N/A | `--out_file=output.a2m` |
| `--alphabet=<value>` | Alphabet of MSA <br /> __0__: Protein <br /> __1__: RNA <br /> __2__: DNA | No | 0 | `--alphabet=1` |
| `--check_validation=[true/false]` | Whether to validate the input MSA file based on alphabet or not | No | true | `--check_validation=true` |

Please note that the conversion is performed based on the specified input and output file extensions.

#### Example
Suppose you have an MSA file named "input.fasta" that you want to convert to the A2M format and save it as "output.a2m".

    ./converter --in_file=input.fasta --out_file=output.a2m

##  Supported File Formats   
- __A2M__ (aligned FASTA-like format)
- __A3M__ (compressed aligned FASTA-like format with lowercase letters for insertions)
- __FASTA__ (FASTA format)
- __STO__ (Stockholm format)
- __CLUSTAL__ (CLUSTAL format)
- __ALN__ (ALN format)
- __AFA__ (fasta format)
- __PFAM__ (format mostly used for neucleotides)

## Error Handling
If any errors occur during the execution of the MSA Processor, an error message will be displayed, describing the issue encountered. <br>
Please refer to the error message for troubleshooting or make necessary corrections to the input.

----------------
For further assistance or inquiries, please consult the documentation or contact the application developer.

