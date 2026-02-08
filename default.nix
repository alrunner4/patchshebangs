{ pkgs ? import <nixpkgs> {} }:

let

default-wrapper = ''
#!${pkgs.bash}/bin/bash
$custom/bin/patchshebangs \"\$@\" <<REWRITES
/bin/bash ${pkgs.bash}/bin/bash
/usr/bin/bash ${pkgs.bash}/bin/bash
/bin/env ${pkgs.coreutils}/bin/env
/usr/bin/env ${pkgs.coreutils}/bin/env
REWRITES
'';

batch-wrapper = ''
#!${pkgs.bash}/bin/bash
while [ -n \"\$1\" ]
do
	if [ ! -d \"\$1\" ]; then
		echo \"error: not a directory: \$1\" >&2
		exit 1
	fi
	exec ${pkgs.findutils}/bin/find \"\$1\" -type f -executable | ${pkgs.findutils}/bin/xargs -t $custom/bin/patchshebangs
	shift
done
'';

in
derivation {
	name = "patchshebangs";
	system = builtins.currentSystem;
	outputs = [ "out" "custom" "default" "batch" ];
	builder = pkgs.writeShellScript "patchshebangs-builder" ''
		${pkgs.coreutils}/bin/mkdir -p $custom/bin
		${pkgs.gcc}/bin/g++ -Wall -Werror -Wextra ${./main.cpp} -o $custom/bin/patchshebangs
		${pkgs.coreutils}/bin/mkdir -p $default/bin
		echo "${default-wrapper}" >$default/bin/patchshebangs
		${pkgs.coreutils}/bin/chmod +x $default/bin/patchshebangs
		${pkgs.coreutils}/bin/ln -s $default/bin/patchshebangs $out
		${pkgs.coreutils}/bin/mkdir -p $batch/bin
		echo "${batch-wrapper}" >$batch/bin/patchshebangs
		${pkgs.coreutils}/bin/chmod +x $batch/bin/patchshebangs
		'';
}
