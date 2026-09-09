# Command Reference Guide

| Command | Arguments | Description | Example Usage |
| :--- | :--- | :--- | :--- |
| `i` | `<line_num> <text>` | Inserts text at the specified line number | `i 1 Hello World` |
| `d` | `<line_num>` | Deletes the line at the specified line number | `d 1` |
| `p` | *None* | Prints all current document lines | `p` |
| `s` | `<filename>` | Saves in-memory document to disk | `s doc.txt` |
| `l` | `<filename>` | Loads document file into memory | `l doc.txt` |
| `f` | `<word>` | Searches lines for a keyword | `f Hello` |
| `c` | *None* | Shows total line count and word count | `c` |
| `q` | *None* | Quits the editor | `q` |