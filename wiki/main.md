# NEFF Calculator and MSA File Converter
This tool provides functionality to compute the Number of Effective sequences ([NEFF](\ref neff)) and convert Multiple [Sequence](\ref a) Alignment (MSA) files between different [MSA formats](\ref msa_formats).

## Table of Contents

- [Getting Started](#getting-started)
- [NEFF Computation](#neff-computation)
- [MSA File Conversion](#msa-file-conversion)
- [Error Handling](#error-handling)

\anchor getting-started
## Getting Started
Download the source code and compile it using a C++ compiler that is compatible with C++17 or a more recent version. The provided Makefile in the repository can also be utilized for this purpose. You can navigate to the Makefile directory and easily enter the following command in the terminal:<br>

```
make
```
Once the compilation is complete, you can run the program via the command line.

\anchor neff-computation
## NEFF Computation
NEFF computation determines the effective number of homologous sequences within an MSA. It takes into account sequence similarities and provides a measure of sequence diversity.  <br>
The code takes command-line flags for input and subsequently computes the score for an MSA file.
<br />
The detailed usage documentation can be found on the [NEFF Computation](\ref neff_computation) page.

\anchor msa-file-conversion
## MSA File Conversion
The MSA File Conversion tool provides a convenient way to convert Multiple Sequence Alignment (MSA) files between various supported formats. <br />
This versatile utility simplifies the process of format conversion, ensuring compatibility across different bioinformatics software and analysis pipelines. Simply specify the input and output files along with their formats, and the tool will handle the conversion for you.
<br />
The detailed usage documentation can be found on the [MSA File Conversion](\ref converter) page.

\anchor error-handling
## Error Handling
If any errors occur during the execution of the MSA Processor, an error message will be displayed, describing the issue encountered. <br>
Please refer to the error message for troubleshooting or make necessary corrections to the input.

<br><br><br><br>

### <span style="color:green"> Community Feedback and Collaboration </span>
We highly value the input and collaboration from our community. You can share your feedback, report issues, and collaborate with us via:
- __Email__: You can reach us directly [contact the developer](mailto:haghani@vt.edu) for any inquiries, suggestions, or issues you encounter. We strive to respond to all emails promptly.
- __GitHub Issues__: For more technical feedback, bug reports, and feature requests, please use our [GitHub Issues page](https://github.com/Maryam-Haghani/Neffy/issues).


<!-- ----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository. -->
