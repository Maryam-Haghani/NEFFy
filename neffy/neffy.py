import subprocess
import os

def compute_neff(args):
    # Get the path to the executable within the package
    script_dir = os.path.abspath(os.path.dirname(__file__))
    neff_path = os.path.join(script_dir, 'bin', 'neff')
    
    result = subprocess.run([neff_path] + args, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr)
    return result.stdout

def convert_msa(args):
    # Get the path to the executable within the package
    script_dir = os.path.abspath(os.path.dirname(__file__))
    converter_path = os.path.join(script_dir, 'bin', 'converter')
    
    result = subprocess.run([converter_path] + args, capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(result.stderr)
    return result.stdout
