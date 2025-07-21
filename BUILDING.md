# Building NotDeafBeef - Detailed Instructions

## Prerequisites

### macOS ARM64 (Required)
- Apple Silicon Mac (M1/M2/M3 series)
- macOS 13.0+ recommended
- Xcode Command Line Tools

```bash
# Install Xcode CLI tools if not present
xcode-select --install

# Verify installation
clang --version
# Should show: Apple clang version 15.0.0+ with arm64 support
```

## Build Process

### 1. Environment Setup

```bash
# Navigate to audio engine
cd src/c

# Verify prerequisites
ls -la include/  # Should contain header files
ls -la ../asm/active/  # Should contain .s assembly files
```

### 2. Clean Build (Recommended)

```bash
# Remove any previous build artifacts
make clean

# This removes: src/*.o, bin/, asm objects
```

### 3. Build Dependencies

**Critical:** Build euclid dependency first (common failure point):

```bash
# Build euclidean rhythm generator
make src/euclid.o

# Verify it was created
ls -la src/euclid.o
```

### 4. Choose Build Configuration

#### A. 100% Assembly Build (Main Achievement)
```bash
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM SNARE_ASM HAT_ASM MELODY_ASM FM_VOICE_ASM LIMITER_ASM DELAY_ASM"

# Should output: "Generated segment.wav"
```

#### B. C Reference Build  
```bash
make segment USE_ASM=0

# May require manual linking if euclid.o not found:
clang -std=c11 -Wall -Wextra -O2 -Iinclude \
  -o bin/segment src/*.o
```

#### C. Incremental Assembly Build (For Testing)
```bash
# Start with just generator
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM"

# Add components progressively
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM"
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM SNARE_ASM"
# ... continue adding HAT_ASM, MELODY_ASM, FM_VOICE_ASM, LIMITER_ASM, DELAY_ASM
```

### 5. Test Execution

```bash
# Run the audio generator
./bin/segment

# Expected output:
# DEBUG: EVT_MID events scheduled = 9
# DEBUG: After delay_init - buf=0x... size=50851 idx=0
# ... [generation process] ...
# Wrote seed_0xcafebabe.wav (406815 frames, 52.03 bpm, root 233.08 Hz)
# Generated segment.wav
```

### 6. Verify Output

```bash
# Check generated audio file
ls -la seed_0xcafebabe.wav
# Should be ~1.6MB

# Play audio (macOS)
afplay seed_0xcafebabe.wav

# Get file info
file seed_0xcafebabe.wav
# Should show: RIFF (little-endian) data, WAVE audio
```

## Common Build Issues & Solutions

### Issue 1: Undefined symbol `_euclid_pattern`

**Symptoms:**
```
Undefined symbols for architecture arm64:
  "_euclid_pattern", referenced from:
      _generator_init in generator.o
```

**Solution:**
```bash
make clean
make src/euclid.o  # Build dependency first
make segment USE_ASM=1 VOICE_ASM="..."  # Then main build
```

### Issue 2: Segmentation fault during execution

**Symptoms:**
```
bin/segment
make: *** [segment] Segmentation fault: 11
```

**Debugging steps:**
1. Try C version first: `make segment USE_ASM=0`
2. If C works, try incremental assembly build
3. Check register aliasing in assembly code
4. Verify stack alignment in assembly functions

**Most common cause:** Register aliasing in assembly code (x17-x20 range)

### Issue 3: clang not found

**Solution:**
```bash
# Install Xcode Command Line Tools
xcode-select --install

# Or install full Xcode from App Store
# Verify: which clang
```

### Issue 4: Wrong architecture

**Symptoms:**
Assembly code fails to link or crashes immediately

**Verification:**
```bash
uname -m  # Must show: arm64
file bin/segment  # Should show: Mach-O 64-bit executable arm64
```

### Issue 5: Build artifacts interfering

**Solution:**
```bash
# Nuclear option - clean everything
make clean
find . -name "*.o" -delete
find . -name "*.elf" -delete
rm -rf bin/

# Rebuild from scratch
make src/euclid.o
make segment USE_ASM=1 VOICE_ASM="..."
```

## Build System Details

### Makefile Variables

- `USE_ASM=1` - Enable assembly compilation
- `VOICE_ASM="..."` - Specify which components to build as assembly
- `DEBUG=1` - Add debug symbols (optional)

### Component Flags

Each assembly component adds preprocessor flags:
- `GENERATOR_ASM` → `-DGENERATOR_ASM`
- `FM_VOICE_ASM` → `-DFM_VOICE_ASM`
- etc.

### Assembly Compilation

Assembly files are compiled with clang:
```bash
clang -c ../asm/active/fm_voice.s -o ../asm/active/fm_voice.o
```

No special assembler needed - clang handles ARM64 assembly directly.

### Linking

Final link includes both C objects and assembly objects:
```bash
clang -o bin/segment \
  src/*.o \              # C objects
  ../asm/active/*.o \    # Assembly objects  
  -I include
```

## Performance Notes

### Assembly vs C Performance

- Assembly version: Hand-optimized NEON vectorization
- C version: Compiler-optimized, reference implementation
- Both should produce identical audio output
- Assembly version demonstrates low-level optimization techniques

### Build Times

- C version: ~2-3 seconds
- Assembly version: ~5-8 seconds (more object files)
- Clean build: +1-2 seconds

## Debugging Assembly Code

### LLDB Integration

```bash
# Build with debug symbols
make segment USE_ASM=1 VOICE_ASM="..." DEBUG=1

# Debug with LLDB
lldb bin/segment
(lldb) run
# If crash occurs:
(lldb) bt  # Show stack trace
(lldb) register read  # Show register state
```

### Register Inspection

Assembly code uses specific register conventions:
- x19, x20: Base pointers (preserved)
- x17, x18: Function pointers (preserved)  
- v0-v31: NEON vector registers
- Avoid aliasing (s17 aliases x17, etc.)

## Advanced Builds

### Custom Component Selection

```bash
# Just FM synthesis
make segment USE_ASM=1 VOICE_ASM="FM_VOICE_ASM"

# Drums only
make segment USE_ASM=1 VOICE_ASM="KICK_ASM SNARE_ASM HAT_ASM"

# Everything except delay
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM SNARE_ASM HAT_ASM MELODY_ASM FM_VOICE_ASM LIMITER_ASM"
```

### Multiple Builds

```bash
# Build both versions
make segment USE_ASM=0 && cp bin/segment bin/segment-c
make segment USE_ASM=1 VOICE_ASM="..." && cp bin/segment bin/segment-asm

# Compare outputs
./bin/segment-c && mv seed_0xcafebabe.wav seed-c.wav
./bin/segment-asm && mv seed_0xcafebabe.wav seed-asm.wav
# Should be identical or very close
```

---

**Note:** This build system was developed and tested specifically for macOS ARM64. Porting to other platforms would require assembly code changes and Makefile modifications.
