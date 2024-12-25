# patchshebangs

I'm skeptical of the design of nixpkgs.mkStdDerivation, and the main impediment to avoiding it often seems to be the lack of a replacement for its patchShebangs function. I present a minimalist alternative.

## Use

Import this repository, which defines a function from nixpkgs to an attrset with two properties, each of which evaluates to a derivation producing $out/bin/patchshebangs. The attribute "default" is a program that makes default substitutions for bash and env. The attribute "custom" is a program that allows interpreter mappings to be input via stdin, one to a line, simply of the format "old-interpreter new-interpreter". Files listed on the command line are processed and replaced.

```nix
patchshebangs = import (builtins.fetchGit {
    url = "git@github.com:alrunner4/patchshebangs";
    ref = "main";
    # you'll probably want to pick a rev, too
}) { inherit nixpkgs; }
```

You can then subsequently use it in builder scripts like
```nix
${patchshebangs.default}/bin/patchshebangs my-script.sh
```
