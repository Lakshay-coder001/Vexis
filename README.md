# VEXIS — A Cryptographic Encryption Tool

**Vexis** is a lightweight, command-line file encryption and decryption tool developed in **C**. It provides a simple interface for encrypting and decrypting files directly from the terminal.

The project focuses on cryptographic operations, secure password handling, file management, and low-level system programming.

Vexis is designed with simplicity, modularity, and efficiency in mind, making it a practical exploration of cryptography and systems programming in C.

---

## Features

- **File Encryption** — Encrypt files using a simple command-line interface.
- **File Decryption** — Restore encrypted files to their original contents.
- **Password Protection** — Password-based encryption and decryption workflow.
- **Custom File Extension** — Encrypted files use the `.vxs` extension.
- **Custom Output File** — Specify the destination of decrypted files.
- **Command-Line Interface** — Perform operations directly from the terminal.
- **Error Handling** — Handle invalid arguments, unsupported files, and operational errors.
- **Modular Architecture** — Separate modules for encryption, CLI processing, password management, and file operations.
- **Cross-Platform Development** — Developed using standard C and platform-specific functionality where required.

---

## Technologies Used

| Technology | Purpose |
|------------|---------|
| C | Core programming language |
| GCC | C compiler |
| GNU Make | Build automation |
| SHA-256 | Cryptographic hashing |
| Standard C Library | File handling and memory management |
| Windows / Linux | Development environments |

---

## Installation

### Prerequisites

Ensure the following tools are installed:

- GCC compiler
- GNU Make
- Git

### Clone the Repository

```bash
git clone <repository-url>
cd vexis
```

### Compile the Project

Use the provided Makefile:

```bash
make
```

This compiles the source files and generates the Vexis executable.

---

## Usage

Vexis supports two primary operations:

1. Encryption
2. Decryption

### 1. Encrypt a File

To encrypt a file, use:

```bash
./vexis encrypt <filename>
```

**Example:**

```bash
./vexis encrypt secret.txt
```

The encrypted file uses the `.vxs` extension.

**Example output filename:**

```text
secret.txt.vxs
```

### 2. Decrypt a File

To decrypt an encrypted file:

```bash
./vexis decrypt <filename.vxs> -o <output_file>
```

**Example:**

```bash
./vexis decrypt secret.txt.vxs -o secret.txt
```

The `-o` option specifies the output filename and is required for decryption.

---

## Command-Line Options

| Option | Description |
|--------|-------------|
| `encrypt` | Encrypt a file |
| `decrypt` | Decrypt an encrypted file |
| `--help` | Display help information |
| `-h` | Short form of help |
| `--version` | Display application version |
| `-v` | Short form of version |
| `-o <output_file>` | Specify the decrypted output filename |

### Help Command

```bash
./vexis --help
```

**Output:**

```text
VEXIS - A Cryptographic Encryption Tool

USAGE:
  ./vexis encrypt <filename>
  ./vexis decrypt <filename.vxs> -o <output_file>

ACTIONS:
 1. encrypt
 2. decrypt

OPTIONS:
 1. --help or -h
 2. --version or -v
 3. -o <output_file> (required for decrypt)
```

### Version Command

```bash
./vexis --version
```

Displays the current version of Vexis.

---

## Project Structure

```text
vexis/
│
├── include/
│   ├── cli.h
│   ├── crypto.h
│   ├── config.h
│   ├── output.h
│   ├── password.h
│   ├── sha256.h
│   └── vex_error.h
│
├── src/
│   ├── main.c
│   ├── cli.c
│   ├── crypto.c
│   ├── output.c
│   ├── password.c
│   ├── sha256.c
│   └── vex_error.c
│
├── Makefile
└── README.md
```

### Module Description

| Module | Responsibility |
|--------|----------------|
| `main.c` | Application entry point |
| `cli.c` | Command-line argument parsing |
| `crypto.c` | Encryption and decryption operations |
| `password.c` | Password input and memory handling |
| `sha256.c` | SHA-256 hashing implementation |
| `output.c` | Output file processing |
| `vex_error.c` | Error handling and reporting |
| `config.h` | Project configuration |

---

## Encryption and Decryption Workflow

### Encryption

```text
User Input File
       |
       v
Command-Line Parsing
       |
       v
Password Input
       |
       v
Cryptographic Processing
       |
       v
Encrypted File (.vxs)
```

### Decryption

```text
Encrypted File (.vxs)
       |
       v
Command-Line Parsing
       |
       v
Password Input
       |
       v
Cryptographic Processing
       |
       v
Decrypted Output File
```

---

## Error Handling

Vexis includes error-handling functionality to detect and report problems such as:

- Invalid command-line arguments
- Unsupported file types or parameters
- Missing input files
- File access failures
- Invalid output parameters
- Cryptographic operation failures

The error-handling module provides a centralized approach to reporting application errors.

---

## Learning Objectives

This project demonstrates practical experience with:

- Systems programming in C
- File input/output operations
- Command-line interface development
- Cryptographic programming
- SHA-256 hashing
- Password handling and memory management
- Pointers and dynamic memory allocation
- Modular programming with header and source files
- Error handling
- Makefiles and compilation

---

## Security Considerations

Vexis is an educational cryptographic project.

The security of encrypted files depends on the implementation of the encryption algorithm, password-based key derivation, random number generation, and integrity verification.

SHA-256 is a hashing algorithm and does not independently provide file encryption.

Vexis should not be relied upon for protecting highly sensitive information without thorough testing and independent security review.

---

## Future Enhancements

- Improved cryptographic security and authentication
- Support for additional file types
- Enhanced command-line options
- Improved error reporting
- Automated unit testing
- Better cross-platform compatibility
- Performance optimization for large files

---

## Author

**Lakshay Kumar**

Developed as a systems programming and cryptography project using C.

---

**VEXIS — Simple Command-Line File Encryption in C.**
