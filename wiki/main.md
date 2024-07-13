# NEFFy: NEFF Calculator and MSA File Converter

## Table of Contents
- [Overview](#overview)
- Tools
    - [NEFF Computation](#overview_neff_computation)
    - [MSA File Conversion](#overview_converter)
- [How to Use NEFFy](#how-use-neffy)
  - [Executable File](#use_executable)
  - [Python Library](#use_lib)
- [Error Handling](#error_handling)
- [Feedback and Collaboration](#feedback)

\anchor overview
NEFFy is a versatile, efficient, and comprehensive tool for bioinformatics research, offering advanced features for calculating [NEFF](\ref neff) (Normalized Effective Number of Sequences) for Multiple [Sequence](\ref a) Alignments (MSA)s of any biological sequences, including protein, RNA, and DNA across various [MSA formats](\ref msa_formats). It surpasses existing tools in efficiency and functionality. <br>
Additionally, NEFFy includes built-in support for format conversion, allowing users to seamlessly convert between different [MSA formats](\ref msa_formats).<br>

\anchor overview_neff_computation
### NEFF Computation

NEFF computation tool provides versatile options for analyzing the diversity of sequences in an MSA, including symmetric and asymmetric NEFF, column-wise NEFF, paired and unpaired MSA NEFF, and random masking for denoising. It caters to various needs in bioinformatics and structural biology, ensuring a comprehensive assessment of sequence alignments.

* __Symmetric NEFF__: In this mode, the threshold for considering a pair of sequences as homologous depends on the length of the alignment. This threshold is equal for all sequences, leading to symmetry in similarities.

* __Asymmetric NEFF__: Here, the threshold depends on the number of non-gap residues, making the cutoff different for each sequence. This method provides a more detailed and accurate representation of sequence diversity.

* __Column-wise NEFF (Per-Residue NEFF)__: This method computes NEFF for each position in the alignment. It is used by tools like AlphaFold for more precise per-residue sequence diversity assessment. 
Per-residue NEFF values for each position in the MSA were calculated by summing the weights of the sequences that have a residue (i.e., non-gap characters) at that specific position.

* __Multimer MSA NEFF__: The tool is capable of detecting the MSA format of a multimer, identifying paired MSA sequences, and unpaired sequences across chains in a block-diagonal manner, similar to AlphaFold-Multimer. By specifying the length of the monomers by user, the tool can calculate NEFF for each set.

* __Random Masking for MSA Denoising__: NEFFy can perform MSA denoising by systematically masking sequences across different iterations to eliminate less informative or redundant sequences, which are considered noise. Users can define the percentage of sequences to be masked and the number of iterations for masking. NEFFy calculates the NEFF for each iteration and chooses the masking that yields the highest NEFF value. This represents the most diverse and informative MSA, making it valuable for downstream tasks.

* __Multiple Format Support:__ The tool supports various [MSA formats](\ref msa_formats).
    <!-- * __a2m:__ FASTA-like format with additional annotations.
    * __a3m:__ Extended A2M format.
    * __fasta:__ Standard format with subformats such as afa, fas, fst, fsa.
    * __sto:__ Stockholm format.
    * __clustal:__ Clustal format.
    * __aln:__ ALN format.
    * __pfam:__ PFAM format. -->

Please fine to the document of how to use executable and python library.

\anchor overview_converter
### MSA File Conversion
The MSA File Conversion tool is designed to handle the conversion of MSA files from one format to another with minimal user intervention. By specifying the input and output files along with their formats, users can effortlessly convert MSA files while preserving sequence integrity and annotations. This tool supports a wide range of [MSA formats](\ref msa_formats), making it an essential utility for researchers working with sequence alignment data.

* __Easy Conversion Process:__ Users only need to specify the input and output files along with their desired formats. The tool handles the rest, including reading the input file, performing the conversion, and writing the output file.

* __Alphabet Specification:__ The tool allows users to specify the alphabet of the MSA (Protein, RNA, DNA) to ensure proper handling and validation of sequences.

* __Validation Option:__ Users can choose whether to perform validation on sequences, ensuring data integrity and consistency.

* __Multiple Format Support:__ The tool supports various [MSA formats](\ref msa_formats).

<br>

---
\anchor how-use-neffy
## How to Use NEFFy

NEFFy is designed to be flexible and user-friendly, offering two main versions to cater to different user needs:
\anchor use_executable
1. **Executable File**: Use NEFFy as a command-line interface for seamless integration into automated workflows.
\anchor use_lib
2. **Python Library**: This version acts as a wrapper for NEFFy’s C++ executable files, making it accessible for any user with limited coding experience.

Whether you prefer the command-line interface or the simplicity of a Python library, NEFFy ensures smooth and efficient usage for all.<br>
For detailed installation and usage guides, please visit the [Installation Guide](\ref installation_guide) and [How to Use](\ref usage_guide) pages.

---
\anchor error_handling
## Error Handling

If any errors occur during the execution of the MSA Processor, an error message will be displayed, describing the issue encountered. <br>
Please refer to the error message for troubleshooting or make necessary corrections to the input.

---
<!-- ### <span style="color:green"> Community Feedback and Collaboration </span> -->
\anchor feedback
## Feedback and Collaboration
We highly value the input and collaboration from our community. You can share your feedback, report issues, and collaborate with us via:
- __Email__: You can reach us directly [contact the developer](mailto:haghani@vt.edu) for any inquiries, suggestions, or issues you encounter. We strive to respond to all emails promptly.
- __GitHub Issues__: For more technical feedback, bug reports, and feature requests, please use our [GitHub Issues page](https://github.com/Maryam-Haghani/Neffy/issues).
