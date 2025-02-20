\page installation_guide Installation Guide

## Table of Contents
- [C++ Executable File](#executable_install)
- [Python Library](#python_install)
  - [Installation from Source](#python_source)
  - [Installation from PyPI](#python_pypi)
  - [Installation from BioConda](#python_bioconda)

<br>

\anchor executable_install
# C++ Executable File
To install the executable version of NEFFy:
1. Clone the repository:
    ```bash
    git clone https://github.com/Maryam-Haghani/Neffy.git
    ```
2. Navigate to the project directory:
    ```bash
    cd Neffy
    ```
3. Compile the code using a C++ compiler that supports C++17 or a newer version, through the provided [Makefile](https://github.com/Maryam-Haghani/Neffy/blob/main/Makefile) in the repository.
    ```
    make
    ```
  - **If the `make` command is not available on your operating system, refer to the [Help](\ref help) page.**

Once the compilation is complete, you can run the program via the command line. <br/>
This package is cross-platform and works on Linux, Windows, and macOS without requiring additional compilation.

<br>

---
\anchor python_install
# Python Library

\anchor python_source
## Installation from Source

To install the library from the source:

1. Clone the repository:
    ```bash
    git clone https://github.com/Maryam-Haghani/Neffy.git
    ```
2. Navigate to the project directory:
    ```bash
    cd Neffy
    ```
3. Ensure you have `setuptools` and `wheel` installed:
    ```bash
    pip install setuptools wheel
    ```
4. Build the source distribution and wheel:
    ```bash
    python setup.py sdist bdist_wheel
    ```
5. Install the package from the root directory of the project:
    ```bash
    pip install .
    ```
   
- Alternatively, you can install the package directly from the built wheel file (in the `dist` directory):
    ```bash
    pip install dist/neffy-0.1-py3-none-any.whl
    ```

\anchor python_pypi
## Installation from PyPI
Our Python library is available on [`PyPI`](https://pypi.org/project/neffy).
This release provides pre-built, platform-specific wheels for Windows, Linux, and macOS, as well as a source distribution for building on other operating systems. This streamlines the installation and management process using pip.<br>
- To install the package from PyPI, run:
    ```bash
    pip install neffy
    ```

\anchor python_bioconda
## Installation from BioConda:
You can also install neffy through [`BioConda`](https://bioconda.github.io/contributor/guidelines.html) by running the following commands:

```bash
conda config --add channels conda-forge
conda install -c bioconda neffy
```
The first command adds the conda-forge channel to your Conda configuration, which is necessary to access a broader range of packages and dependencies that might not be available on the default channels.

<br>

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.
