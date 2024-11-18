# Downloads C++ code from GitHub, compiles it using a Makefile,
# and includes the resulting executable files in the `bin` directory of the package to be used by python methods.

from setuptools import setup, find_packages
from setuptools.command.install import install
import subprocess
import os
import shutil


class CustomInstallCommand(install):
    def run(self):
        # Define the GitHub repository URL for the C++ code
        cpp_repo_url = "https://github.com/Maryam-Haghani/NEFFy.git"
        cpp_clone_path = os.path.join("neffy", "cpp-source")  # Name of the directory to clone into

        # Clone the C++ code from GitHub
        self.clone_cpp_repo(cpp_repo_url, cpp_clone_path)

        # Build the C++ code using the provided Makefile
        self.build_cpp_code(cpp_clone_path)

        install.run(self)


    def clone_cpp_repo(self, repo_url, clone_path):
        print(f"Cloning C++ code from {repo_url}...")
        if os.path.exists(clone_path):
            shutil.rmtree(clone_path)
        subprocess.check_call(["git", "clone", "-b format-issue2", repo_url, clone_path])
        print(f"Cloned C++ code to {clone_path}...")

    def build_cpp_code(self, cpp_clone_path):
        print("Building C++ code using Makefile...")
        # Run `make` in the directory containing the Makefile
        makefile_dir = cpp_clone_path
        subprocess.check_call(["make"], cwd=makefile_dir)

        # Assuming the Makefile generates executable files in the source_path
        # Copy the executables to the bin folder of the Python package
        bin_dir = os.path.join("neffy", "bin")
        os.makedirs(bin_dir, exist_ok=True)

        executables = ['neff', 'converter']
        # Handle .exe suffix for Windows
        if os.name == 'nt':  # Windows
            executables = [exe + '.exe' for exe in executables]

        for executable in executables:
            executable_path = os.path.join(cpp_clone_path, executable)
            if os.path.exists(executable_path):
                shutil.copy(executable_path, os.path.join(bin_dir, executable))

        print("NEFFy executables copied to bin folder")



setup(
    name='neffy',
    version='0.1',
    packages=find_packages(),
    include_package_data=True,
    description='A Python interface of NEFFy C++ tool',
    author='Maryam Haghani',
    author_email='haghani@vt.edu',
    url='https://github.com/Maryam-Haghani/NEFFy',
    install_requires=[
        # List any Python dependencies your package needs
    ],
    package_data={
        'neffy': ['bin/*', 'cpp-source/code/*'],
    },
    entry_points={
        'console_scripts': [
            'compute_neff=neffy.neffy:compute_neff',
            'convert_msa=neffy.neffy:convert_msa',
        ],
    },
    cmdclass={
        'install': CustomInstallCommand,
    },
)
