# BPE (Byte Pair Encoding) in C

A simple implementation of Byte Pair Encoding compression algorithm in C. BPE is a data compression technique that iteratively replaces the most frequent pair of bytes/characters with a new token.

## Tools

- **txt2bpe** - Compresses text using BPE algorithm
- **bpe2dot** - Visualizes BPE pairs as a DOT graph
- **bpe_inspect** - Inspects BPE files (if available)

## Usage

### 1. Compress Text (txt2bpe)

```bash
# Compile
gcc -o txt2bpe txt2bpe.c

# Compress text
txt2bpe "your text here" output.bpe
```

**Example:**
```bash
txt2bpe "aaabbbcccaaabbbccc" compressed.bpe
```

### 2. Generate Graph Visualization (bpe2dot)

```bash
# Compile
gcc -o bpe2dot bpe2dot.c bpe.c

# Generate DOT graph
bpe2dot input.bpe output.dot

# View graph (requires Graphviz)
dot -Tpng output.dot -o graph.png
```

**Example:**
```bash
bpe2dot compressed.bpe graph.dot
dot -Tpng graph.dot -o visualization.png
```

## Files

- `txt2bpe.c` - Main compression program
- `bpe2dot.c` - Graph visualization generator
- `bpe.c` - Core BPE functions
- `bpe.h` - Header file with type definitions
- `nob.h` - Utility library
- `stb_ds.h` - Data structures library

## How It Works

1. **Initialize**: Start with all ASCII characters (0-254) as tokens
2. **Find Pairs**: Count frequency of all adjacent character pairs
3. **Replace**: Replace most frequent pair with a new token
4. **Repeat**: Continue until no pair occurs more than once
5. **Save**: Store the learned pairs for decompression

## Example Output

```
Starting BPE compression
Input text: "aaabbbcccaaabbbccc" (length: 18)
Initializing data structures...
Iteration 1: Most frequent pair: (97, 97) occurs 4 times
Iteration 2: Most frequent pair: (98, 98) occurs 4 times
...
Compression complete after 9 iterations!
Final compressed tokens: [262][262]
```

The DOT graph shows how tokens are built from simpler components, creating a tree-like structure of the compression hierarchy.
