\page neff Number of EFFective sequences (NEFF)

NEFF measures sequence diversity within MSAs, which are crucial for extracting correlated mutation information and essential for biological investigations like contact map and structure prediction. NEFF has shown a strong correlation with prediction accuracy in models such as AlphaFold.
<br>

For an MSA, NEFF can be formulated as:

\f[
\left( \frac{1}{\sqrt{L}} \right) \sum_{n=1}^{N} \frac{1}{1 + \sum_{m=1, m \neq n}^{N} I[S_{m,n} \geq thr]}
\f]

<br>

where \f$L\f$ is the number of residues in the sequence, \f$N\f$ is the number of sequences in an MSA, \f$S_{m,n}\f$ is the sequence identity between \f$m\f$-th and \f$n\f$-th sequences, \f$thr\f$ is the threshold cutoff to determine whether two sequences are similar or not, and \f$I\f$ is the inversion bracket, meaning that \f$I[S_{m,n} \geq thr]\f$ equals 1 if \f$S_{m,n} \geq thr\f$ and 0 otherwise.

Note that \f$\frac{1}{\sqrt{L}}\f$ is used as a normalization factor here.

Generally, one can see NEFF simply as a normalized summation of sequence weights for all sequences in an MSA. If the number of sequences (including itself) similar to sequence \f$i\f$ is \f$n_i\f$, then its sequence weight is \f$\frac{1}{n_i}\f$. This approach for calculating NEFF has been widely used in various contact and structure prediction tools, as demonstrated in references [1-6].

<br><br><br><br><br>

#### References
<small>
1. Morcos, F., et al. "Direct-coupling analysis of residue coevolution captures native contacts across many protein families." Proceedings of the National Academy of Sciences 108.49 (2011): E1293-E1301.
2. Simkovic, F., et al. "ConKit: a Python interface to contact predictions." Bioinformatics 33.14 (2017): 2209-2211.
3. Wu, Q., et al. "Analysis of several key factors affecting DCA-based contact prediction in metagenome coevolution." Bioinformatics 35.14 (2019): 2497-2503.
4. Zhang, J., et al. "DeepMSA: constructing deep multiple sequence alignment to improve contact prediction and fold-recognition for distant-homology proteins." Bioinformatics 36.5 (2020).
5. Liu, Y., et al. "Protein contact prediction using metagenome sequence data improves fold recognition." Bioinformatics 37.12 (2021): 1770-1776.
6. Li, Y., et al. "TripletRes: fragment-free protein structure prediction using triplet transformers." Bioinformatics 37.22-23 (2021): 4101-4107.
</small>
<br>

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.