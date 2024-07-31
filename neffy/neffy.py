import subprocess
import os
from enum import Enum

class Alphabet(Enum):
    Protein = 0
    RNA = 1
    DNA = 2

class NonStandardOption(Enum):
    AsStandard = 0          # treat the the same as standard residues, default in deepMSA symmetric
    ConsiderGapInCutoff = 1 # consider them as gaps when computing cutoff of sequence (only used in asymmetryc version, default in deepMSA assymetric)
    ConsiderGap = 2         # consider them as gaps in computing similarity cutoff and checking position match/mismatch, default in Gremlin

class Normalization(Enum):
    Sqrt_Length = 0  # Square root of the length
    Length = 1       # Length
    No_Normalization = 2


def _check_flags(threshold, gap_cutoff, mask_frac):
    if not (0 <= threshold <= 1):
        raise ValueError("Threshold must be between 0 and 1")
    if not (0 <= gap_cutoff <= 1):
        raise ValueError("Gap cutoff must be between 0 and 1")
    if not (0 <= mask_frac <= 100):
        raise ValueError("Mask Fraction must be between 0 and 100")


def compute_neff(
    file: str,
    alphabet: Alphabet = Alphabet.Protein,
    check_validation: bool = False,
    threshold: float = 0.8,
    norm: Normalization = Normalization.Sqrt_Length,
    omit_query_gaps: bool = True,
    is_symmetric: bool = True,
    non_standard_option: NonStandardOption = NonStandardOption.AsStandard,
    depth: int = 'inf',
    gap_cutoff: float = 1,
    pos_start: int = 1,
    pos_end: int = 'inf',
    only_weights: bool = False,
    mask_enabled: bool = False,
    mask_frac: float = 0,
    mask_count: int = 0,
    multimer_MSA: bool = False,
    first_monomer_length: int = 0,
    column_neff: bool = False
):

    _check_flags(threshold, gap_cutoff, mask_frac)

    # Get the path to the executable within the package
    script_dir = os.path.abspath(os.path.dirname(__file__))
    neff_path = os.path.join(script_dir, 'bin', 'neff')

    args = [
        f"--file={file}",
        f"--alphabet={alphabet.value}",
        f"--check_validation={str(check_validation).lower()}",
        f"--threshold={threshold}",
        f"--norm={norm.value}",
        f"--omit_query_gaps={str(omit_query_gaps).lower()}",
        f"--is_symmetric={str(is_symmetric).lower()}",
        f"--non_standard_option={non_standard_option.value}",
        f"--depth={depth}",
        f"--gap_cutoff={gap_cutoff}",
        f"--pos_start={pos_start}",
        f"--pos_end={pos_end}",
        f"--only_weights={str(only_weights).lower()}",
        f"--mask_enabled={str(mask_enabled).lower()}",
        f"--mask_frac={mask_frac}",
        f"--mask_count={mask_count}",
        f"--multimer_MSA={str(multimer_MSA).lower()}",
        f"--first_monomer_length={first_monomer_length}",
        f"--column_neff={str(column_neff).lower()}"
    ]
    
    result = subprocess.run([neff_path] + args, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr)
    return result.stdout


def convert_msa(
    in_file: str,
    out_file: str,
    alphabet: Alphabet = Alphabet.Protein,
    check_validation: bool = False
):
    # Get the path to the executable within the package
    script_dir = os.path.abspath(os.path.dirname(__file__))
    converter_path = os.path.join(script_dir, 'bin', 'converter')

    args = [
        f"--in_file={in_file}",
        f"--out_file={out_file}",
        f"--alphabet={alphabet.value}",
        f"--check_validation={str(check_validation).lower()}"
    ]
    
    result = subprocess.run([converter_path] + args, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr)
    return result.stdout
