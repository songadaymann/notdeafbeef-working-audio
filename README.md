# NotDeafBeef - 100% Assembly Audio Synthesis Engine

A real-time audio synthesis engine with **100% assembly implementation** of the entire audio pipeline, plus complete visual system for audio-reactive graphics.

## 🎯 Achievement: 100% Assembly Audio Pipeline

This project successfully implements a complete audio synthesis system entirely in ARM64 assembly:

- **Generator** (ASM) - Main audio generation engine
- **FM Voice** (ASM) - Frequency modulation synthesis 
- **Drum Voices** (ASM) - Kick, snare, hi-hat synthesis
- **Melody Voice** (ASM) - Melodic line generation
- **Limiter** (ASM) - Audio dynamics processing
- **Delay** (ASM) - Time-based audio effects
- **Oscillators** (ASM) - Sine waves, noise, wave shaping
- **Math Functions** (ASM) - SIMD sine, exponential functions

All critical audio processing runs in hand-optimized ARM64 assembly with NEON vectorization.

## 🛠 Build Requirements

**macOS (ARM64)**
- Xcode Command Line Tools (`xcode-select --install`)
- clang compiler with ARM64 support

**Tested Configuration:**
- macOS 15.5 on Apple Silicon (arm64)
- clang version 15.0.0+

## 🚀 Quick Start

### Audio Engine (100% Assembly)

```bash
# Clone and enter directory
cd src/c

# Build 100% assembly version
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM SNARE_ASM HAT_ASM MELODY_ASM FM_VOICE_ASM LIMITER_ASM DELAY_ASM"

# Run audio generation
./bin/segment
```

This generates `seed_0xcafebabe.wav` - a complete musical composition synthesized entirely in assembly.

### Alternative: C Reference Version

```bash
# Build C version (for comparison/reference)
make clean
make src/euclid.o  # Important: build this dependency first
make segment USE_ASM=0

# Manual link if needed
clang -o bin/segment src/*.o -I include
```

## 📁 Project Structure

```
src/
├── c/                          # Main audio engine
│   ├── src/                    # C source (reference implementation)
│   ├── include/                # Headers and API definitions  
│   ├── Makefile               # Build system
│   └── bin/                   # Build outputs
├── asm/active/                # Assembly implementations
│   ├── generator.s            # Main generator (ASM)
│   ├── fm_voice.s            # FM synthesis (ASM)
│   ├── kick.s, snare.s, hat.s # Drum synthesis (ASM)
│   ├── melody.s              # Melody voice (ASM)
│   ├── limiter.s, delay.s    # Effects (ASM)
│   └── osc_*.s               # Oscillators & math (ASM)
├── visual_core.c             # Visual system
├── ascii_renderer.c          # ASCII art rendering
├── particles.c               # Particle effects
└── [other visual components] # Complete visual engine
```

## ⚙️ Build Configurations

### Assembly Components Available:
- `GENERATOR_ASM` - Core audio generation
- `FM_VOICE_ASM` - FM synthesis voice
- `KICK_ASM` - Kick drum synthesis  
- `SNARE_ASM` - Snare drum synthesis
- `HAT_ASM` - Hi-hat synthesis
- `MELODY_ASM` - Melody line generation
- `LIMITER_ASM` - Audio limiting/compression
- `DELAY_ASM` - Delay/echo effects

### Example Partial Assembly Builds:

```bash
# 90% Assembly (without delay)
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM SNARE_ASM HAT_ASM MELODY_ASM FM_VOICE_ASM LIMITER_ASM"

# Just generator + drums
make segment USE_ASM=1 VOICE_ASM="GENERATOR_ASM KICK_ASM SNARE_ASM HAT_ASM"

# C reference version
make segment USE_ASM=0
```

## 🐛 Troubleshooting

### Common Issues:

**1. Undefined symbol `_euclid_pattern`**
```bash
# Solution: Build euclid dependency first
make src/euclid.o
# Then retry main build
```

**2. Segmentation fault on assembly build**
- Verify ARM64 architecture: `uname -m` should show `arm64`
- Try incremental assembly build (add one ASM component at a time)
- Check for register aliasing issues in assembly code

**3. Clang not found**
```bash
# Install Xcode Command Line Tools
xcode-select --install
```

**4. Build artifacts not cleaned**
```bash
make clean
# Remove any .o files manually if needed
find . -name "*.o" -delete
```

## 🎵 Audio Output

The engine generates complete musical compositions with:
- **Euclidean rhythm patterns** for drums
- **FM synthesis** for harmonic content  
- **Procedural melodies** with voice leading
- **Dynamic limiting** for clean output
- **Delay effects** for spatial depth

**Typical output:** `seed_0xcafebabe.wav` (1.6MB, 44.1kHz, ~9 seconds)

## 🎨 Visual System

Complete audio-reactive visual engine with:
- ASCII art rendering
- Particle systems  
- Terrain generation
- Glitch effects
- Bass-reactive animations
- Real-time audio analysis

## 🏗 Architecture Notes

### Assembly Implementation Highlights:
- **NEON vectorization** for 4x parallel processing
- **Register preservation** following ARM64 ABI
- **Function pointer tables** for modular components  
- **Stack frame management** with debugging hooks
- **Hand-optimized math** (sine, exponential, noise generation)

### Key Technical Achievements:
- **54 rounds of debugging** to achieve stable FM voice assembly
- **Register aliasing fixes** (function pointers vs. data registers)
- **Cross-component integration** with mixed C/ASM builds
- **Deterministic audio output** matching C reference

## 📊 Performance

Assembly implementation provides:
- **Optimized execution** through hand-tuned NEON code
- **Reduced overhead** vs. compiler-generated assembly  
- **Precise timing control** for real-time audio synthesis
- **Educational value** for low-level audio programming

## 🔗 Development History

This represents the culmination of extensive assembly optimization work:
- Complete C reference implementation
- Incremental assembly conversion
- Extensive debugging and register management  
- Integration testing across all components
- Clean repository migration preserving all functionality

## 📝 License

[Add your preferred license]

---

**Status:** ✅ 100% Assembly Audio Pipeline Complete  
**Last Updated:** July 2024  
**Platform:** macOS ARM64 (Apple Silicon)
