# Assembler Project

> Final Project for the System Programming Lab  
> 🧠 Done by: **Miri Bookman** & **Yocheved Weiser**  
> 📅 Date: 21.03.2023  
> 🏫 Open University of Israel

---

## 📝 Task Summary

Build an **assembler** for a **fictional assembly language** as defined by the Open University course.  
The program takes `.as` source files and processes them through several compilation stages to produce output files (`.ob`, `.ext`, `.ent`).

---

## 🧩 Project Goals

1. **Memory efficiency**  
   Dynamically allocate memory as needed for each output, avoiding waste.

2. **Separation of concerns**  
   Code is modular, each file handles a specific task. Shared state is accessed only through defined interfaces.

3. **Data abstraction**  
   Use of `typedef`, `struct`, `enum`, and constants to build readable and maintainable data structures.

---

## 📁 Directory Structure

| File/Folder         | Description |
|---------------------|-------------|
| `assembler.*`       | Main file – handles CLI input and file processing |
| `pre_assembler.*`   | Handles macro expansion and macro list management |
| `first_pass.*`      | Parses lines in the first pass, builds symbol table |
| `second_pass.*`     | Resolves symbols, writes memory image |
| `text_correctness.*`| Utility functions for syntax checks |
| `errors.*`          | Error checking and reporting module |
| `globals.h`         | Shared data types, constants |
| `*.as`, `*.am`      | Sample source files |

---

## 🔄 Compilation Stages

### 1️⃣ Macros Expansion

- Replaces macros in the `.as` file and creates a `.am` file.
- Uses a `MacroList` to store macro definitions.
- If macro syntax errors are found, skips to next file.

### 2️⃣ First Pass

- Scans the file to:
  - Count memory space needed
  - Build the symbol table
  - Validate syntax
- All errors are collected and displayed.
- If errors exist, halts before second pass.

### 3️⃣ Second Pass

- Allocates memory for output files
- Resolves symbols into final addresses
- Translates code into binary words
- Only one type of error can happen here: unknown symbols

### 4️⃣ Export

- If no errors, generates:
  - `.ob` – Object file
  - `.ext` – External symbols
  - `.ent` – Entry symbols

---

## ⚙️ Make & Run

```bash
make
./assembler file1.as file2.as ...
