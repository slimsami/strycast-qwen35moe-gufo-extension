# strycast-qwen35moe-gufo-extension

A [Gufo](https://github.com/gufo-org/gufo) extension that serves
**Qwen3.6-35B-A3B** and **Ornith 1.5 122B** (GGUF architecture `qwen35moe`) on
AMD Strix Halo (gfx1151), with MTP and DFlash2 speculative decoding and an
optional 8-bit KV cache. It is the 35B runner from
[Strycast](https://github.com/slimsami/strycast) packaged as an external
extension, so stock Gufo can load it without a fork.

**Status: community extension, not reviewed by Gufo maintainers.** This tree is
generated from Strycast `88ca363e` and built against Gufo main `0fb50ce4`
(`GUFO_REVISION`); greedy outputs are byte-identical to Strycast's. Linux and
ROCm only: the providers need HIP.

## Install

With a Gufo that has extension modules (main `0fb50ce4` or later, experimental,
gufo#1077), installed with its SDK:

```sh
gufo extension install https://github.com/slimsami/strycast-qwen35moe-gufo-extension@<commit>
gufo extension list
```

Pin a commit (or, after the next Strycast release, a tag). `install` builds the
module locally with your Gufo's toolchain and warns that the code is not
reviewed by Gufo; read it first.

You can also build it into Gufo:

```sh
cmake --preset gpu-test -DGUFO_EXTENSION_DIRS=/absolute/path/to/strycast-qwen35moe-gufo-extension
cmake --build --preset gpu-test --target gufo --parallel
build/gpu-test/gufo diagnose --section extensions --json
```

`diagnose` should list the extension `strycast_qwen35moe` with provider
`qwen35moe` available. It only claims `gguf:qwen35moe`, so Gufo's own providers
are unaffected.

## Serve

```sh
gufo serve --sessions 4 llm --model Qwen3.6-35B-A3B-UD-Q4_K_XL.gguf --context 262144 \
  --speculative mtp --mtp-model Qwen3.6-35B-A3B-UD-Q4_K_XL.gguf --draft-tokens 2
```

For DFlash2, use `--speculative dflash2 --dflash-model
Qwen3.6-35B-A3B-DFlash2-Q8_0.gguf --draft-tokens 7 --draft-policy adaptive`.
Add `--kv-cache q8_0` to store the attention KV cache in 8 bits: about half
the KV memory (2.34 GiB less per session at 262K), the same quality, and
slightly faster decode at long context.

The supported GGUFs, memory use and benchmarks are in Strycast's
[35B guide](https://github.com/slimsami/strycast/tree/main/docs/models/qwen3.6-35b-a3b)
and [Ornith guide](https://github.com/slimsami/strycast/tree/main/docs/models/ornith-1.5-122b).

## Checked

On Strix Halo (ROCm 10.1), against Strycast `88ca363e` built with the same
toolchain, greedy outputs are byte-identical, and speed is within noise:

- 35B Q8 and Q4_K_M: plain, DFlash2 and MTP at 0, 8K and 32K context, and 1-4
  concurrent users with DFlash2;
- Ornith 1.5 122B Q4_K_M, plain and MTP;
- `--kv-cache q8_0` (Q4_K_M, plain and DFlash2), both built in and installed
  as a module with `gufo extension install`.

## How it is made

Strycast is the source. Its `tools/extension/qwen35moe/generate.sh` copies the
Qwen code, moves it to namespace `strycast`, and applies a few patches that
adapt Strycast's core API additions to Gufo's public extension API; see its
[README](https://github.com/slimsami/strycast/tree/staging/tools/extension/qwen35moe)
(added in slimsami/strycast#51). Strycast's CI builds the result as a module
against `GUFO_REVISION` and publishes it here after each Strycast release,
tagged with the release's tag. Changes belong in Strycast, not here.

## License

MIT, see `LICENSE`. Third-party notices are in `NOTICE` and `licenses/`.
