\page converter MSA File Conversion


## Overview
The MSA File Conversion tool is designed to handle the conversion of MSA files from one format to another with minimal user intervention. By specifying the input and output files along with their formats, users can effortlessly convert MSA files while preserving sequence integrity and annotations. This tool supports a wide range of formats, making it an essential utility for researchers working with sequence alignment data.

## Features
* __Easy Conversion Process:__ Users only need to specify the input and output files along with their desired formats. The tool handles the rest, including reading the input file, performing the conversion, and writing the output file.

* __Alphabet Specification:__ The tool allows users to specify the alphabet of the MSA (Protein, RNA, DNA) to ensure proper handling and validation of sequences.

* __Validation Option:__ Users can choose whether to perform validation on sequences, ensuring data integrity and consistency.

* __Multiple Format Support:__ The tool supports various MSA formats including:

    * __a2m:__ FASTA-like format with additional annotations.
    * __a3m:__ Extended A2M format.
    * __fasta:__ Standard format with subformats such as afa, fas, fst, fsa.
    * __sto:__ Stockholm format.
    * __clustal:__ Clustal format.
    * __aln:__ ALN format.
    * __pfam:__ PFAM format.

In the [MSA format page](\ref msaFormats), you will find a brief explanation of each format, along with an illustrative alignment example for each one.

For further details, please refer to the [Usage](#usage) section and [Example](#example).

\anchor usage
### Usage
To convert an MSA file, specify the input file, output file, and the desired input and output formats. The tool will read the input file, perform the conversion, and write the resulting MSA to the output file in the specified format.

#### Command-line Flags:
The code accepts the following command-line flags:
| Flag | Description | Required | Default Value | Example	| 
|------|-------------|----------|---------------|---------|
| `--in_file=<filename>` | Specifies the input MSA file to be converted.<br /> Replace `<filename>` with the path and name of the input file | Yes | N/A | `--in_file=input.fasta` |
| `--out_file=<filename>`| Specifies the output file where the converted MSA will be saved.<br /> Replace `<filename>` with the desired path and name of the output file | Yes | N/A | `--out_file=output.a2m` |
| `--alphabet=<value>` | Alphabet of MSA <br /> __0__: Protein <br /> __1__: RNA <br /> __2__: DNA | No | 0 | `--alphabet=1` |
| `--check_validation=[true/false]` | Validate the input MSA file based on alphabet or not | No | true | `--check_validation=true` |

Please note that the conversion is performed based on the specified input and output file extensions.

\anchor example
### Example
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
----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.
