# Neffy Python Wrapper

This is a Python wrapper for the Neffy tool.

## Installation

### From Source

To install the library from the source:

1. Clone the repository:
    ```bash
    git clone https://github.com/Maryam-Haghani/Neffy.git
    ```
2. Checkout the specific branch:
    ```bash
    git checkout python-lib
    ```
3. Navigate to the project directory:
    ```bash
    cd Neffy
    ```
4. Ensure you have `setuptools` and `wheel` installed:
    ```bash
    pip install setuptools wheel
    ```
5. Build the source distribution and wheel:
    ```bash
    python setup.py sdist bdist_wheel
    ```
6. Install the package from the root directory of the project:
    ```bash
    pip install .
    ```

    Alternatively, you can install the package directly from the built wheel file (in the `dist` directory):
    ```bash
    pip install dist/neffy-0.1-py3-none-any.whl
    ```

### From PyPI (will be distributed at a later date):

To install the package from PyPI:

```bash
pip install neffy
```

## Usage
- An example of neff computation:
```bash
python ./example/compute_neff.py --file=example.a2m
```

- An example of MSA conversion:
```bash
python ./example/convert_msa.py --in_file=example.a2m --out_file=example.fasta
```