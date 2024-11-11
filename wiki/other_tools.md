\page other_tools Other Tools

## Table of Contents
- [RaptorX](#raptorX)
- [Conkit](#conkit)
- [DeepMSA](#deep_msa)
- [Gremlin](#gremlin)
- [rMSA](#rMSA)

<br>

Here, we provide an exploration of the tools which have integrated NEFF calculations into their core functions. We will delve into the primary purposes each of these tools serve and provide an exploration of the various features they offer for executing NEFF computations.

<br>

\anchor raptorX
## `RaptorX`
`RaptorX`, a well-established protein structure prediction method developed since 2012, specializes in predicting protein tertiary and contact structures, particularly for sequences lacking close homologs in the Protein Data Bank [1]. `RaptorX` includes an integrated Python helper function for NEFF calculation, which is specifically designed for computing asymmetric NEFF. This tool is primarily designed for handling aligned `a2m` and `a3m` formats, without the specification of biological sequence alphabets. The code is available on [GitHub](https://github.com/j3xugit/RaptorX-3DModeling/tree/master/BuildFeatures/Helpers).

<br>

---
\anchor conkit
## `Conkit`
[`Conkit`](https://www.conkit.org) is a Python library designed to facilitate the management and manipulation of residue-residue contact prediction data [2]. Among its key functionalities, it provides parsers for different MSA formats, allowing conversion between different MSAs, as well as the the capability to compute NEFF and sequence weights, with a performance boost from `Cython` for time efficiency. The source code is available on [GitHub](https://github.com/rigdenlab/conkit/blob/master/conkit/core/sequencefile.py). <br>
While supporting multiple MSA formats, it does not specify the biological sequence type for NEFF computation. Additionally, when calculating symmetric NEFF, it gives an integer-valued NEFF value without normalization. Furthermore, `Conkit` can handle gaps in query sequences, but this option is available exclusively in the `a3m` format, distinguishing it from the `a3m-inserts` format, which includes gap positions of the query sequence in MSAs.

<br>

---
\anchor deep_msa
## `DeepMSA`
[`DeepMSA`](https://zhanggroup.org/DeepMSA/) is an open-source tool designed for constriction of deep and sensitive MSAs [3]. It achieves this by leveraging homologous sequences and alignments derived from a diverse range of whole-genome and metagenome databases, utilizing complementary hidden Markov model algorithms. `DeepMSA` includes a built-in feature for NEFF computation in `C++`, which is exclusively available for the `aln` format. It offers support for both symmetric and asymmetric NEFF calculation for `protein` alphabets. The source code can be accessed through [this link](https://github.com/kad-ecoli/MSAParser/blob/master/calNf.cpp) on GtiHub.

<br>

---
\anchor gremlin
## `Gremlin`
`Gremlin` is a method for prediction of residue-residue contacts that uses the power of sequence coevolution and structural context data using a pseudo-likelihood approach. This allows for more precise prediction in protein structures, even when working with a limited set of homologous sequences [4]. `Gremlin` has been implemented in both `Python` and `C++`, and within its code, it includes the capability to compute symmetric NEFF.<br>
When computing NEFF, `Gremlin` treats all non-standard residues as equivalent to gaps. It can support various biological sequences, including `protein`s, `RNA`s, and `DNA`s. `Gremlin` offers the feature to exclude gappy positions from the MSA and supports `fasta` and `aln` MSA formats. The source code is accessible on GitHub at [this link](https://github.com/sokrypton/GREMLIN_CPP/blob/master/gremlin_cpp.cpp).

<br>

---
\anchor rMSA
## `rMSA`
`rMSA` is a hierarchical pipeline designed for conducting sensitive searches and precise alignments of RNA homologs for a RNA sequence \cite[5]. `rMSA` includes a native feature for computing NEFF values specifically customized for `RNA` sequences. Research in RNA 3D structure prediction and protein-nucleotide structure prediction has employed `rMSA` to generate MSAs for RNA sequences [6-8]. <br>

<br><br>

#### References
<small>

1. Xu, J., et al. "RaptorX: exploiting structure information for protein alignment by statistical inference." Proteins: Structure, Function, and Bioinformatics 80.1 (2012): 62-71.
2. Simkovic, F., et al. "ConKit: a Python interface to contact predictions." Bioinformatics 33.15 (2017): 2209-2211.
3. Zhang, C., et al. "DeepMSA: constructing deep multiple sequence alignment to improve contact prediction and fold-recognition for distant-homology proteins." Bioinformatics 36.7 (2020).
4. Baker, D., et al. "GREMLIN: An Online Server for Protein Structure Prediction." Nucleic Acids Research 41 (2013): W309-W314.
5. Zhang, J., et al. "rMSA: a new method for generating multiple sequence alignments for RNA homologs." Nucleic Acids Research 51.7 (2023): e42.
6. Baek, M., et al. "Accurate prediction of protein-nucleotide complex structures using a deep learning method." Nature 603.7903 (2022): 484-488.
7. Feng, P., et al. "trRosettaRNA: Protein structure prediction with deep learning and co-evolutionary data." Nature Methods 19.5 (2022): 560-565.
8. Zhang, Y., et al. "DeepFoldRNA: accurate RNA secondary structure prediction with deep learning." Nature Communications 13.1 (2022): 1680.
</small>
<br>

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.
