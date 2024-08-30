# NEFFy: NEFF Calculator and MSA File Converter

## Table of Contents
- [Overview](#overview)
    - [NEFF Computation](#overview_neff_computation)
    - [MSA File Conversion](#overview_converter)
- [Installation Guide](#installation)
- [How to Use NEFFy](#how-use-neffy)
  - [Executable File](#use_executable)
  - [Python Library](#use_lib)
- [Error Handling](#error_handling)
- [Feedback and Collaboration](#feedback)

<br>

\anchor overview
## Overview
NEFFy is a versatile, efficient, and comprehensive tool for bioinformatics research, offering advanced features for calculating [NEFF](\ref neff) (Normalized Effective Number of Sequences) for Multiple [Sequence](\ref a) Alignments (MSA)s of any biological sequences, including `protein`, `RNA`, and `DNA` across various [MSA formats](\ref msa_formats). It surpasses [existing tools](\ref other_tools) in efficiency and functionality. <br>
Additionally, NEFFy includes built-in support for format conversion, allowing users to seamlessly convert between different [MSA formats](\ref msa_formats).<br>


\anchor overview_neff_computation
### NEFF Computation
The NEFF computation tool provides versatile options for analyzing the diversity of sequences in an MSA. It caters to various needs in bioinformatics and structural biology, ensuring a comprehensive assessment of sequence alignments. Here are its features:

* __Symmetric NEFF__: [NEFF](\ref neff) uses similatiy of each pair of sequences for determining sequence weights. Symmetry in similarity indicates that the threshold for considering a pair of sequences as similar are consistent for all sequences, leading to symmetry in their similarity assessment. \htmlonly<a href="other_tools.html#conkit">Conkit</a>\endhtmlonly, \htmlonly<a href="other_tools.html#gremlin">Gremlin</a>\endhtmlonly and \htmlonly<a href="other_tools.html#rMSA">rMSA</a>\endhtmlonly exclusively endorse the symmetric version.<br>
<br>
* __Asymmetric NEFF__: In asymmetric version, when determining sequence weights, the threshold of seuence similarity for a pair of sequence, depends on the number of non-gap residues. It makes the cutoff different for each sequence, thus rendering the similarity between a pair of sequences asymmetric. This method provides a more detailed and accurate representation of sequence diversity. While \htmlonly<a href="other_tools.html#raptorX">RaptorX</a>\endhtmlonly exclusively supports the asymmetric version, \htmlonly<a href="other_tools.html#deep_msa">DeepMSA</a>\endhtmlonly and NEFFy uniquely stand out as the only tools capable of handling both symmetric and asymmetric versions.<br>
<br>
* __Handeling Gaps in Query__: In certain MSA formats, such as the \htmlonly<a href="msa_formats.html#sto">stockholm (sto)</a>\endhtmlonly format, gaps may appear in the query sequence (first sequences in the alignment), signifying ’gaps aligned to insertions’. When performing NEFF computation, some users might prefer to retain these gaps. While other tools adhere to the original MSA file, regardless of whether it includes gaps in the query sequence, NEFFy provides the versatility to handle these gaps. It can either filter them out, along with any corresponding positions in the aligned sequences, or retain them.<br>
<br>
* __Handeling Gappy positions__: Adopted from \htmlonly<a href="other_tools.html#gremlin">Gremlin</a>\endhtmlonly, the idea is to identify and remove positions in a sequence alignment that surpass a certain gap frequency threshold prior to NEFF  computation. Essentially, these positions in the sequence alignment display a higher-than-desired frequency of gaps. By filtering out such positions, NEFF computation can focus on more informative and conserved regionsof the sequences.<br>
<br>
* __Handeling Non-standard Residues__: Non-standard residues refer to those residues that lie outside the conventional residue set of biological sequences, typically extending beyond the 20 canonical amino acids for proteins and undefined nucleotides for `RNA`s and `DNA`s. For further clarification, additional information can be found in the \htmlonly<a href="help.html#non_standard">Help</a>\endhtmlonly page.<br>
<br>
* __Handeling of Various Biological Sequences__: Each biological sequence utilizes a distinct collection of symbols to represent its sequence, known as an alphabet. The \htmlonly<a href="help.html#alphabet">Help</a>\endhtmlonly page provides a reference to these alphabets for `protein`s, `RNA`s and `DNA`s.<br>
<br>
* __Column-wise NEFF (Per-Residue NEFF)__: This method computes NEFF for each position in the alignment. It is used by tools like `AlphaFold` for more precise per-residue sequence diversity assessment. Per-residue NEFF values for each position in the MSA were calculated by summing the weights of the sequences that have a residue (i.e., non-gap characters) at that specific position.<br>
<br>
* __Multimer MSA NEFF__: 
The tool can calculate NEFF for MSAs of a multimeric protein. Tools like AlphaFold-Multimer generate these multimeric MSAs, where, for homomers (assemblies comprising multiple copies of the same chain), the MSA consists of multiple copies of the same sequence alignment. For heteromers (assemblies comprising two or more different chains), the MSA includes paired sequence alignments followed by individual MSAs for each chain in a block-diagonal arrangement. It's possible that some chains may not have an individual MSA, but a paired MSA is always present.
<br>
The tool can identify the multiple sequence alignment (MSA) format for both heteromers and homomers based on the provided stoichiometry of the complex. As described by the [Protein Data Bank](https://www.rcsb.org/docs/general-help/symmetry-resources-in-the-pdb), "Stoichiometry indicates the number of chains participating in the assembly and whether the assembly is a homomer or a heteromer."
<br>
NEFFy computes NEFF values for such MSAs by using the stoichiometry and the lengths of the chains within the complex for heteromers. It identifies sequences that are either paired or unpaired across the chains, with the unpaired MSA sequences arranged in a block-diagonal pattern after the paired sequences. The tool then calculates NEFF for each of these components. For homomers, NEFF is calculated based on the individual MSA.<br>
<br>
* __Random Masking for MSA Denoising__: <br>
NEFFy enables MSA masking by systematically applying a user-defined percentage of sequence masking across multiple iterations. For each iteration, NEFFy calculates the NEFF value and selects the iteration that yields the highest NEFF. The goal of this masking process is to denoise the MSA using the NEFF metric. If the highest NEFF value exceeds the initial NEFF, it indicates that the masking process has successfully removed less informative or redundant sequences, resulting in a more diverse and refined MSA, even with a reduced number of sequences. This refined MSA can then be utilized for downstream applications.<br>
<br>
* __Multiple Format Support:__ Various [formats](\ref msa_formats) are available for representing aligned sequences within an MSA. NEFFy stands as the exclusive tool capable of handling all these formats for NEFF computation.<br>
<br>
* __NEFF Computation for an Integration of MSAs:__ In all versions of NEFF calculation, except for the NEFF of multimer MSA, NEFFy can accept multiple input files as long as all sequences are of the same length. It combines the sequences, removing any duplicates, and then computes the NEFF for the integrated MSA.

<br><br>

\anchor overview_converter
### MSA File Conversion
The MSA File Conversion tool is designed to handle the conversion of MSA files from one format to another with minimal user intervention. By specifying the input and output files along with their formats, users can effortlessly convert MSA files while preserving sequence integrity and annotations. This tool supports a wide range of [MSA formats](\ref msa_formats), making it an essential utility for researchers working with sequence alignment data.<br>
<br>
* __Easy Conversion Process:__ Users only need to specify the input and output files along with their desired formats. The tool handles the rest, including reading the input file, performing the conversion, and writing the output file.<br>
<br>
* __Alphabet Specification:__ Each biological sequence utilizes a distinct collection of symbols to represent its sequence, known as an alphabet. The \htmlonly<a href="help.html#alphabet">Help</a>\endhtmlonly page provides a reference to these alphabets for various biological sequences. The tool allows users to specify the alphabet of the MSA (`protein`, `RNA`, `DNA`) to ensure proper handling and validation of sequences.<br>
<br>
* __Validation Option:__ Users can choose whether to perform validation on sequences, ensuring data integrity and consistency.<br>
<br>
* __Multiple Format Support:__ Various formats are available for representing aligned sequences within an MSA. The NEFFy converter can handle conversions between all these [formats](\ref msa_formats).

<br>

---
\anchor installation
## Installation Guide

NEFFy is designed to be flexible and user-friendly, offering two main versions to cater to different user needs:
1. **Executable File**: Use NEFFy as a command-line interface for seamless integration into automated workflows.
2. **Python Library**: This version acts as a wrapper for NEFFy’s C++ executable files, making it accessible for any user with limited coding experience.

For detailed installation instructions about each version, please visit [Installation Guide](\ref installation_guide) page.

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
For detailed instructions on using each version, please visit [How to Use](\ref usage_guide) page.

<br>

---
\anchor error_handling
## Error Handling

If any errors occur during the execution of the MSA Processor, an error message will be displayed, describing the issue encountered.
Please refer to the error message for troubleshooting or make necessary corrections to the input.

<br>

---
<!-- ### <span style="color:green"> Community Feedback and Collaboration </span> -->
\anchor feedback
## Feedback and Collaboration
We highly value the input and collaboration from our community. You can share your feedback, report issues, and collaborate with us via:
- __Email__: You can reach us directly [contact the developer](mailto:haghani@vt.edu) for any inquiries, suggestions, or issues you encounter. We strive to respond to all emails promptly.
- __GitHub Issues__: For more technical feedback, bug reports, and feature requests, please use our [GitHub Issues page](https://github.com/Maryam-Haghani/Neffy/issues).
