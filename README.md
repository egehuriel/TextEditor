# Text Editor with Garbage Collection

## Overview

A terminal-based text editor written in C. The editor manages a text buffer in memory using a doubly linked list structure and performs garbage collection to reclaim deleted entries.

---

## Requirements

- GCC compiler
- Linux operating system
- ncurses library

Install ncurses on Ubuntu/Debian:
```bash
sudo apt-get install libncurses5-dev libncursesw5-dev
```

---

## Build

```bash
make
```

Clean build artifacts:
```bash
make clean
```

Build and run:
```bash
make run
```

---

## Usage

```bash
./editor
```

Once the editor is running, use the following commands:

| Key | Action |
|-----|--------|
| `E` | Open a file — prompts for filename |
| `P` | Print — display the text on screen |
| `I` | Insert — add a new line after the selected line |
| `D` | Delete — remove the selected line |
| `R` | Replace — overwrite a character at cursor position |
| `S` | Save — write the buffer back to the file |
| `G` | Garbage collection — compact the buffer manually |
| `Q` | Quit |
| `↑` `↓` | Move cursor between lines |
| `←` `→` | Move cursor between characters (used with R) |

### Typical workflow

```
E mytext.txt     → opens and loads the file
P                → displays the text
↑ / ↓           → move to the desired line
I                → insert a new line after the cursor
D                → delete the selected line
R                → replace a character on the selected line
S                → save changes
Q                → quit
```

---

## Data Structure

The editor stores text in a fixed-size array of nodes:

```c
struct node {
    char statement[40];   // line content, max 40 characters
    int  next;            // index of the next valid line
    int  prev;            // index of the previous valid line
};

struct node textbuffer[100];  // max 100 lines
int head;                     // index of the first valid line
int tail;                     // index of the last valid line
int freeIndex;                // index of the first free slot
```

Deletion does not physically remove entries — it only unlinks them from the chain. Garbage collection compacts the buffer by shifting all reachable entries to the front.

---

## Garbage Collection

Garbage collection can be triggered in two ways:

- **Manual:** press `G`
- **Automatic:** triggered after every 10 insert/delete operations, or when the buffer is full

When GC runs, all valid lines (reachable via the linked list) are copied to a temporary buffer, the main buffer is cleared, and the valid lines are written back sequentially starting from index 0.

---

## File Structure

```
texteditor/
├── Makefile
├── README.md
├── include/
│   ├── editor.h             # struct, globals, prototypes
│   └── garbageCollection.h  # GC prototypes
└── src/
    ├── main.c               # command loop, ncurses init
    ├── editor.c             # edit(), save(), clearBuffer()
    ├── operations.c         # insert(), delete(), replace()
    ├── ui.c                 # print(), cursorLine(), cursorChar()
    └── garbageCollection.c  # garbageCollection(), maybeAutoGC()
```

---

## Limitations

- Maximum 100 lines per file
- Maximum 40 characters per line
- File must have `.txt` extension

---

CSE 232 Systems Programming - Term Project
Ege Huriel, Cihat Özlük, Musab Tunç, Ferhat Canşi, Atakan Naktürk
Yeditepe University
