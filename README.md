# patchshebangs

I'm skeptical of the design of nixpkgs.mkStdDerivation, and the main impediment to avoiding it often seems to be the lack of a replacement for its patchShebangs function. I present a minimalist alternative.

## Use

### Outputs

```bash
$out # the default-wrapped patchshebangs executable
$default/bin/patchshebangs # the default-wrapped patchshebangs executable
$custom/bin/patchshebangs # the unwrapped executable
```

Import this repository, which defines a function from nixpkgs to a derivation with the three outputs listed above. The "default" output is a program that makes default substitutions for bash and env. The "custom" output is a program that allows interpreter mappings to be input via stdin, one to a line, simply of the format "old-interpreter new-interpreter". The primary output "out" links to the "default" output executable for ease of use in builder scripts. For all flavors of output, files listed on the command line are processed and replaced.

```nix
patchshebangs = import (builtins.fetchGit {
    url = "git@github.com:alrunner4/patchshebangs";
    ref = "main";
    # you'll probably want to pick a rev, too
}) { inherit pkgs; }
```

You can then subsequently use it in builder scripts like
```nix
${patchshebangs} my-script.sh
```
or with custom interpreter rewrites
```nix
${patchshebangs.custom}/bin/patchshebangs my-script.sh <<REWRITES
/bin/bash ${my-customized-bash}
/bin/env ${my-customized-env}
REWRITES
```
