import subprocess
import os
from enum import Enum
import re
import sys
from typing import Union, List


# Enum for different biological alphabets
class Alphabet(Enum):
    Protein = 0
    RNA = 1
    DNA = 2


# Enum for non-standard options
class NonStandardOption(Enum):
    AsStandard = 0
    ConsiderGapInCutoff = 1
    ConsiderGap = 2


# Enum for normalization methods
class Normalization(Enum):
    Sqrt_Length = 0
    Length = 1
    No_Normalization = 2


# To execute binary file with arguments
def run_exe(args, exe_file):
    try:
        # Get the absolute path to the executable file
        script_dir = os.path.abspath(os.path.dirname(__file__))
        exe_path = os.path.join(script_dir, 'bin', exe_file)

        # Use different subprocess methods depending on Python version
        if sys.version_info >= (3, 7):
            result = subprocess.run([exe_path] + args, capture_output=True, text=True)
            stdout, stderr = result.stdout, result.stderr
        else:
            result = subprocess.Popen([exe_path] + args,
                                      stdout=subprocess.PIPE, stderr=subprocess.PIPE, universal_newlines=True)
            stdout, stderr = result.communicate()

        # Raise an error if the executable returns a non-zero exit code
        if result.returncode != 0:
            raise RuntimeError(f"{exe_file} execution failed with error:\n{stderr}")
        return stdout
    except FileNotFoundError:
        raise FileNotFoundError(f"Executable file '{exe_file}' not found.")
    except subprocess.SubprocessError as e:
        raise RuntimeError(f"Subprocess execution failed: {str(e)}")
    except Exception as e:
        raise RuntimeError(f"An unexpected error occurred during {exe_file} execution: {str(e)}")


# To validate input parameters
def _check_flags(params):
    # Check file exist
    files = params['file'] if isinstance(params['file'], list) else [params['file']]

    for file_path in files:
        if not os.path.exists(file_path):
            raise FileNotFoundError(f"File {file_path} does not exist.")

    # Check if the 'threshold' parameter is between 0 and 1
    if 'threshold' in params and not 0 < params['threshold'] < 1:
        raise ValueError("Threshold must be between 0 and 1")

    # Check if the 'gap_cutoff' parameter is between 0 and 1
    if 'gap_cutoff' in params and not 0 <= params['gap_cutoff'] <= 1:
        raise ValueError("Gap cutoff must be between 0 and 1")


# To validate masking-related parameters
def _check_masking(mask_count, mask_frac):
    # Ensure 'mask_count' is positive and 'mask_frac' is between 0 and 1
    if mask_count == 0:
        raise ValueError("When masking, 'mask_count' must be a positive number.")
    if not 0 < mask_frac < 1:
        raise ValueError("When masking, 'mask_frac' should be a number between 0 and 1")


# To validate stoichiometry and determine if it's a homomer
def _check_stoichiometry(stoichiom, chainLength):
    # Regex pattern to match stoichiometry format
    pattern = r'^([A-Z][1-9][0-9]*)+$'

    # Check stoichiometry format
    if not re.fullmatch(pattern, stoichiom):
        raise ValueError(f"Invalid stoichiometry format: '{stoichiom}'")

    # Extract and validate elements in stoichiometry
    elements = re.findall(r'([A-Z])([1-9][0-9]*)', stoichiom)
    if not all(elements[i - 1][0] < elements[i][0] for i in range(1, len(elements))):
        raise ValueError(f"Invalid stoichiometry format: '{stoichiom}'")

    is_homomer = len(elements) == 1 and elements[0][0] == 'A'

    # Ensure chainLength is provided for heteromers
    if not is_homomer and len(chainLength) == 1:
        raise ValueError("The chainLength must be provided when the stoichiometry represents a heteromer.")

    return is_homomer


# Parse general result output
# Extract sequence length and depth from output
def parse_result(output):
    return (
        int(re.search(r'MSA sequence length:\s*(\d+)', output).group(1)),
        int(re.search(r'MSA depth:\s*(\d+)', output).group(1))
    )

# Parse NEFF value and optionally sequence weights
def parse_neff(output, only_weights):
    msa_length, msa_depth = parse_result(output)

    if only_weights:
        value = list(map(float, re.findall(r'Sequence weights:\n([\d\s\.]+)', output)[0].split()))

    else:
        value = float(re.search(r'NEFF:\s*([\d.]+)', output).group(1))

    return msa_length, msa_depth, value

# Parse NEFF masking results
def parse_neff_masking(output):
    msa_length, msa_depth = parse_result(output)

    return (
        msa_length, msa_depth,
        float(re.search(r'Initial NEFF:\s*([\d.eE+-]+)', output).group(1)),
        float(re.search(r'highest value:\s*([\d.eE+-]+)', output).group(1)),
        re.search(r"NEFF values for each mask iteration have been saved in '(.*?)'", output).group(1),
        re.search(r"Masked MSA file corresponding to the highest NEFF value has been saved in '(.*?)'", output).group(
            1),
        float(re.search(r'NEFF computation:\s*([\d.eE+-]+)', output).group(1))
    )


# Parse per-residue (column-wise) NEFF results
def parse_residue_neff(output):
    msa_length, msa_depth = parse_result(output)

    lines = output.strip().splitlines()
    return (
        msa_length, msa_depth,
        list(map(float, lines[3].strip().split())),
        float(lines[4].split(":")[1].strip())
    )


# Parse multimer NEFF results
def parse_multimer_neff_results(output, is_homomer):
    msa_length, msa_depth = parse_result(output)
    entire_neff = None
    neff_values = {}

    # Parse the NEFF values depending on whether it's a homomer or heteromer
    for line in output.strip().splitlines():
        if line.startswith("NEFF of entire MSA:"):
            entire_neff = float(line.split(":")[1].strip())
            continue

        if not is_homomer:
            # Parse NEFF values for paired and individual chains
            if "NEFF of Paired MSA" in line:
                paired_neff = float(re.search(r"NEFF of Paired MSA.*: ([\d.]+)", line).group(1))
                paired_depth = int(re.search(r"depth=(\d+)", line).group(1))
                neff_values["paired"] = {'neff': paired_neff, 'depth': paired_depth}
            elif "NEFF of Individual MSA for Chain" in line:
                chain_name = re.search(r"Chain (\w+)", line).group(1)
                neff = float(re.search(r": ([\d.]+)", line).group(1))
                depth = int(re.search(r"depth=(\d+)", line).group(1))
                neff_values[chain_name] = {'neff': neff, 'depth': depth}
            elif "does not have an individual MSA, skipping NEFF calculation" in line:
                chain_name = re.search(r"Chain (\w+)", line).group(1)
                neff_values[chain_name] = {'neff': None, 'depth': 0}

        else:
            if "NEFF of Individual MSA" in line:
                neff_values = float(re.search(r": ([\d.]+)", line).group(1))

    return msa_length, msa_depth, entire_neff, neff_values


# To build command-line arguments for NEFF computation
def build_args(params):
    args = []

    for key, value in params.items():
        if isinstance(value, Enum):
            args.append(f"--{key}={value.value}")  # use the enum's value directly
        elif key in ['stoichiom', 'file']:
            args.append(f"--{key}={value}")  # do not make value lowercase
        else:
            args.append(f"--{key}={','.join(map(str, value)) if isinstance(value, list) else str(value).lower()}")

    return args


# Main function to compute NEFF for a given MSA
def compute_neff(
        file: Union[str, List[str]],
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
        only_weights: bool = False
):
    try:

        params = locals()

        _check_flags(params)

        params['file'] = file if isinstance(file, str) else ",".join(file)
        args = build_args(params)

        # Run neff executable
        output = run_exe(args, 'neff')
        return parse_neff(output, only_weights)
    except Exception as e:
        raise RuntimeError(f"Error in 'compute_neff': {str(e)}")


# Function to compute NEFF with masking options enabled
def compute_neff_masking(
        file: Union[str, List[str]],
        mask_frac: float = 0,
        mask_count: int = 0,
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
        pos_end: int = 'inf'
):
    try:
        params = locals()
        params['file'] = file if isinstance(file, str) else ",".join(file)
        params['mask_enabled'] = True
        params['mask_count'] = mask_count
        params['mask_frac'] = mask_frac

        _check_flags(params)
        args = build_args(params)
        _check_masking(mask_count, mask_frac)

        # Run neff executable
        output = run_exe(args, 'neff')
        return parse_neff_masking(output)
    except Exception as e:
        raise RuntimeError(f"Error in 'compute_neff_masking': {str(e)}")


# Function to compute per-residue (column-wise) NEFF
def compute_residue_neff(
        file: Union[str, List[str]],
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
        pos_end: int = 'inf'
):
    try:
        params = locals()
        params['file'] = file if isinstance(file, str) else ",".join(file)
        params['residue_neff'] = True

        _check_flags(params)
        args = build_args(params)

        # Run neff executable
        output = run_exe(args, 'neff')
        return parse_residue_neff(output)
    except Exception as e:
        raise RuntimeError(f"Error in 'compute_residue_neff': {str(e)}")


# Function to compute NEFF for multimeric structures
def compute_multimer_neff(
        file: str,
        stoichiom: str,
        chain_length: List[int] = [0],
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
        pos_end: int = 'inf'
):
    try:
        params = locals()
        params['multimer_MSA'] = True

        _check_flags(params)
        is_homomer = _check_stoichiometry(stoichiom, chain_length)
        args = build_args(params)

        # Run neff executable
        output = run_exe(args, 'neff')
        return parse_multimer_neff_results(output, is_homomer)
    except Exception as e:
        raise RuntimeError(f"Error in 'compute_multimer_neff': {str(e)}")


# Function to convert MSA file formats
def convert_msa(
        in_file: str,
        out_file: str,
        alphabet: Alphabet = Alphabet.Protein,
        check_validation: bool = False
):
    try:
        # Check file exist
        if not os.path.exists(in_file):
            raise FileNotFoundError(f"File {in_file} does not exists.")

        args = [
            f"--in_file={in_file}",
            f"--out_file={out_file}",
            f"--alphabet={alphabet.value}",
            f"--check_validation={str(check_validation).lower()}"
        ]

        # Run the converter executable
        return run_exe(args, 'converter')
    except Exception as e:
        raise RuntimeError(f"Error in 'convert_msa': {str(e)}")
