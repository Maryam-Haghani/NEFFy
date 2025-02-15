# Downloads C++ code from GitHub, compiles it using a Makefile,
# and includes the resulting executable files in the `bin` directory of the package to be used by python methods.

from setuptools import setup, find_packages
from setuptools.command.build_py import build_py
import subprocess
import os
import shutil


def get_long_description(filename):
    with open(filename, "r", encoding="utf-8") as fh:
        lines = fh.readlines()

    long_description_lines = []
    exclude_section = False

    for line in lines:
        # skip these sections
        if line.startswith("## Table of Contents") or line.startswith("# C++ Executable")\
                or line.startswith("## Library Installation"):
            exclude_section = True
        # include Library usage part or stop excluding when a new top-level section starts
        elif line.startswith("## Library Usage") or (line.startswith("# ") and exclude_section):
            exclude_section = False

        if not exclude_section:
            long_description_lines.append(line)

    return "".join(long_description_lines)


# Read the desired parts of README.md
long_description = get_long_description("README.md")

try:
    from wheel.bdist_wheel import bdist_wheel as _bdist_wheel
except ImportError:
    bdist_wheel = None
else:
    class bdist_wheel(_bdist_wheel):
        def finalize_options(self):
            super().finalize_options()
            self.root_is_pure = False  # Mark the wheel as non-pure (platform-specific)


class CustomBuildPyCommand(build_py):
    """
        A custom build step that compiles the C++ code *only* when building from source.
        - This will run when 'pip install' is forced to build from sdist.
        - It also runs when you do 'python -m build --wheel' (i.e. your local or CI build).
        - It will NOT run at end-user install if they are downloading a pre-built wheel.
        """

    def run(self):
        root_dir = os.path.dirname(__file__)
        print("Compiling C++ executables from source in:", root_dir)

        # Check for the Makefile in the root directory
        makefile_path = os.path.join(root_dir, 'Makefile')
        if not os.path.exists(makefile_path):
            raise RuntimeError(f"Makefile not found in {root_dir}.")

        subprocess.check_call(["make"], cwd=root_dir)

        # Copy  executables into neffy/bin/
        bin_dir = os.path.join(root_dir, 'neffy', 'bin')
        os.makedirs(bin_dir, exist_ok=True)

        executables = ['neff', 'converter']
        # Handle .exe suffix for Windows
        if os.name == 'nt':  # Windows
            executables = [exe + '.exe' for exe in executables]

        for exe in executables:
            executable_path = os.path.join(root_dir, exe)
            if os.path.exists(executable_path):
                shutil.copy(executable_path, bin_dir)

        print("C++ executables compiled and copied into neffy/bin/")

        # Continue with the normal build process
        super().run()



setup(
    name='neffy',
    version='0.1.1',
    author='Maryam Haghani',
    author_email='haghani@vt.edu',
    description='A Python interface of NEFFy C++ tool: NEFF Calculator and MSA File Converter',
    long_description=long_description,
    long_description_content_type="text/markdown",
    url='https://github.com/Maryam-Haghani/NEFFy',
    packages=find_packages(),
    include_package_data=True,
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: GNU General Public License v3 (GPLv3)"
    ],
    install_requires=[
    ],
    package_data={
        'neffy': ['bin/*'],
    },
    entry_points={
        'console_scripts': [
            'compute_neff=neffy.neffy:compute_neff',
            'convert_msa=neffy.neffy:convert_msa',
        ],
    },
    cmdclass={
        # Override the default 'build_py' to include custom compile step
        'build_py': CustomBuildPyCommand,
        'bdist_wheel': bdist_wheel,
    },
)