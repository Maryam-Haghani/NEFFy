from setuptools import setup, find_packages

setup(
    name='neffy',
    version='0.1',
    packages=find_packages(),
    include_package_data=True,
    description='A Python version of neffy C++ tool',
    author='Maryam Haghani',
    author_email='haghani@vt.edu',
    url='https://github.com/Maryam-Haghani/neffy',
    install_requires=[
        # c++ version 17 or higher
    ],
    package_data={
        'neffy': ['bin/*'],
    },
    entry_points={
        'console_scripts': [
            'compute_neff=neffy.neffy:compute_neff',
            'convert_msa=neffy.neffy:convert_msa',
        ],
    }
)
