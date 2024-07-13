\page help Help Page

## Table of Contents
- [make Installation](#make_installation)

\anchor make_installation
# `make` Installation

To use the `make` command, follow the steps below according to your operating system:


## macOS
The `make` command is not available by default on macOS. However, it can be easily installed by installing the Xcode Command Line Tools. You can do this by running the following command in the Terminal:
```sh
xcode-select --install
```

## Linux
The `make` command is commonly available by default on many Linux distributions. If it's not installed, it can be easily added using the package manager of your distribution. For example:
- On Debian-based systems (e.g., Ubuntu):
  ```sh
  sudo apt-get install make
  ```
- On Red Hat-based systems (e.g., Fedora):
  ```sh
  sudo dnf install make
  ```
- On Arch-based systems:
  ```sh
  sudo pacman -S make
  ```

## Windows
The `make` command is not available by default on Windows. However, it can be installed through several methods:

1. **Using Cygwin**:
   - Install Cygwin from the [Cygwin website](https://www.cygwin.com/).
   - During the installation, select `make` from the package list.

2. **Using GnuWin**:
   - Go to the [GnuWin website](http://gnuwin32.sourceforge.net/packages/make.htm).
   - Download the `make` package setup executable (e.g., `make-3.81.exe`).
   - Run the installer and follow the instructions to install `make`.
   
3. **Using WSL (Windows Subsystem for Linux)**:
   - Install WSL and a Linux distribution from the Microsoft Store (e.g., Ubuntu).
   - Once the distribution is set up, you can use the package manager to install `make` as you would on a native Linux system.

**Set Up Environment Variables**:
- After installing, add the `make` binary to your system’s PATH.
- Open the Start Menu, search for "Environment Variables," and select "Edit the system environment variables."
- In the System Properties window, click on the "Environment Variables" button.
- In the Environment Variables window, find the "Path" variable in the "System variables" section, select it, and click "Edit."
- Click "New" and add the path to the directory where `make.exe` was installed (e.g., `C:\Program Files (x86)\GnuWin32\bin`).
- Click "OK" to close all the windows.



**Verify the Installation**:
- Open a new Command Prompt window.
- Type `make --version` and press Enter.
- You should see the version information for `make`, confirming that it has been installed correctly.

By following these steps, you can have the `make` command available on macOS, Linux, and Windows, ensuring a consistent development environment across different operating systems.

----------------
For further assistance or inquiries, please [contact the developer](mailto:haghani@vt.edu) or create an [issue](https://github.com/Maryam-Haghani/Neffy/issues) in the GitHub repository.