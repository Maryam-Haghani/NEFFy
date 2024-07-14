# NEFFy: NEFF Calculator and MSA File Converter

## Table of Contents
- [Overview](#overview)
- __Tools__
    - [NEFF Computation](#overview_neff_computation)
    - [MSA File Conversion](#overview_converter)
- [Installation Guide](#installation)
- [How to Use NEFFy](#how-use-neffy)
  - [Executable File](#use_executable)
  - [Python Library](#use_lib)
- [Error Handling](#error_handling)
- [Feedback and Collaboration](#feedback)

\anchor overview
NEFFy is a versatile, efficient, and comprehensive tool for bioinformatics research, offering advanced features for calculating [NEFF](\ref neff) (Normalized Effective Number of Sequences) for Multiple [Sequence](\ref a) Alignments (MSA)s of any biological sequences, including protein, RNA, and DNA across various [MSA formats](\ref msa_formats). It surpasses [existing tools](\ref other_tools) in efficiency and functionality. <br>
Additionally, NEFFy includes built-in support for format conversion, allowing users to seamlessly convert between different [MSA formats](\ref msa_formats).<br>

\anchor overview_neff_computation
### NEFF Computation

NEFF computation tool provides versatile options for analyzing the diversity of sequences in an MSA, including __symmetric__ and __asymmetric NEFF__, __column-wise (per_residue) NEFF__, __multimer MSA NEFF__, and __random masking for denoising__. It caters to various needs in bioinformatics and structural biology, ensuring a comprehensive assessment of sequence alignments.

* __Symmetric NEFF__: [NEFF](\ref neff) uses similatiy of each pair of sequences for determining sequence weights. Symmetry in similarity indicates that the threshold for considering a pair of sequences as similar are consistent for all sequences, leading to symmetry in their similarity assessment. Conkit, Gremlin and rMSA exclusively endorse the symmetric version.<br>

* __Asymmetric NEFF__: In asymmetric version, when determining sequence weights, the threshold of seuence similarity for a pair of sequence, depends on the number of non-gap residues. It makes the cutoff different for each sequence, thus rendering the similarity between a pair of sequences asymmetric. This method provides a more detailed and accurate representation of sequence diversity. While RaptorX exclusively supports the asymmetric version, DeepMSA and NEFFy uniquely stand out as the only tools capable of handling both symmetric and asymmetric versions.<br>

* __Handeling Gaps in Query__: In certain MSA formats, such as the [sto](\ref msa_formats) format, gaps may appear in the query sequence (first sequences in the alignment), signifying ’gaps aligned to insertions’. When performing NEFF computation, some users might prefer to retain these gaps. While other tools adhere to the original MSA file, regardless of whether it includes gaps in the query sequence, NEFFy provides the versatility to handle these gaps. It can either filter them out, along with any corresponding positions in the aligned sequences, or retain them.<br>

* __Handelling Gappy positions__: Adopted from Gremlin, the idea is to identify and remove positions in a sequence alignment that surpass a
certain gap frequency threshold prior to NEFF computation. Essentially, these positions in the sequence alignment display a higher-
than-desired frequency of gaps. By filtering out such positions, NEFF computation can focus on more informative and conserved regions
of the sequences.<br>

* __Handeling Non-standard Residues__: Non-standard residues refer to those residues that lie outside the conventional residue set of biological sequences, typically extending beyond the 20 canonical amino acids for proteins and undefined nucleotides for RNAs and DNAs. For
further clarification, additional information can be found in the [Help](\ref help) page.<br>

* __Handeling of Various Biological Sequences__: Each biological sequence utilizes a distinct collection of symbols to represent its sequence, known as an alphabet. The [Help](\ref help) page provides a reference to these alphabets for proteins, RNSs and DNAs.<br>

* __Column-wise NEFF (Per-Residue NEFF)__: This method computes NEFF for each position in the alignment. It is used by tools like _AlphaFold_ for more precise per-residue sequence diversity assessment. Per-residue NEFF values for each position in the MSA were calculated by summing the weights of the sequences that have a residue (i.e., non-gap characters) at that specific position.<br>

* __Multimer MSA NEFF__: The tool is capable of detecting the MSA format of a multimer, identifying paired MSA sequences, and unpaired sequences across chains in a block-diagonal manner, similar to AlphaFold-Multimer. By specifying the length of the monomers by user, the tool can calculate NEFF for each set.<br>

* __Random Masking for MSA Denoising__: NEFFy can perform MSA denoising by systematically masking sequences across different iterations to eliminate less informative or redundant sequences, which are considered noise. Users can define the percentage of sequences to be masked and the number of iterations for masking. NEFFy calculates the NEFF for each iteration and chooses the masking that yields the highest NEFF value. This represents the most diverse and informative MSA, making it valuable for downstream tasks.<br>

* __Multiple Format Support:__ Various formats are available for representing aligned sequences within an MSA. NEFFy stands as the exclusive tool capable of handling all these formats for NEFF computation. Refer to [MSA formats](\ref msa_formats).

\anchor overview_converter
### MSA File Conversion
The MSA File Conversion tool is designed to handle the conversion of MSA files from one format to another with minimal user intervention. By specifying the input and output files along with their formats, users can effortlessly convert MSA files while preserving sequence integrity and annotations. This tool supports a wide range of [MSA formats](\ref msa_formats), making it an essential utility for researchers working with sequence alignment data.

* __Easy Conversion Process:__ Users only need to specify the input and output files along with their desired formats. The tool handles the rest, including reading the input file, performing the conversion, and writing the output file.<br>

* __Alphabet Specification:__ Each biological sequence utilizes a distinct collection of symbols to represent its sequence, known as an alphabet. The [Help](\ref help) page provides a reference to these alphabets for various biological sequences. The tool allows users to specify the alphabet of the MSA (Protein, RNA, DNA) to ensure proper handling and validation of sequences.<br>

* __Validation Option:__ Users can choose whether to perform validation on sequences, ensuring data integrity and consistency.<br>

* __Multiple Format Support:__ Various formats are available for representing aligned sequences within an MSA. The NEFFy converter can handle conversions between all these formats.Refer to [MSA formats](\ref msa_formats).

<br>

---
\anchor installation
## Installation Guide

NEFFy is designed to be flexible and user-friendly, offering two main versions to cater to different user needs:
1. **Executable File**: Use NEFFy as a command-line interface for seamless integration into automated workflows.
2. **Python Library**: This version acts as a wrapper for NEFFy’s C++ executable files, making it accessible for any user with limited coding experience.

For detailed installation instructions about each version, please visit [Installation Guide](\ref installation_guide) page.

---
\anchor how-use-neffy
## How to Use NEFFy

NEFFy is designed to be flexible and user-friendly, offering two main versions to cater to different user needs:
\anchor use_executable
1. **Executable File**: Use NEFFy as a command-line interface for seamless integration into automated workflows.
\anchor use_lib
2. **Python Library**: This version acts as a wrapper for NEFFy’s C++ executable files, making it accessible for any user with limited coding experience.

Whether you prefer the command-line interface or the simplicity of a Python library, NEFFy ensures smooth and efficient usage for all.<br>
For detailed instructions on using each version, please visit [How to Use](\ref usage_guide) page.

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
