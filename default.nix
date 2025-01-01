{ nixpkgs ? import <nixpkgs> {} }:

let

default-wrapper = ''
#!${nixpkgs.bash}/bin/bash
$custom/bin/patchshebangs \"\$@\" <<REWRITES
/bin/bash ${nixpkgs.bash}/bin/bash
/usr/bin/bash ${nixpkgs.bash}/bin/bash
/bin/env ${nixpkgs.coreutils}/bin/env
/usr/bin/env ${nixpkgs.coreutils}/bin/env
REWRITES
'';

in
derivation {
	name = "patchshebangs";
	system = builtins.currentSystem;
	outputs = [ "out" "custom" "default" ];
	builder = nixpkgs.writeShellScript "patchshebangs-builder" ''
		${nixpkgs.coreutils}/bin/mkdir -p $custom/bin
		${nixpkgs.gcc}/bin/g++ -Wall -Werror -Wextra ${./main.cpp} -o $custom/bin/patchshebangs
		${nixpkgs.coreutils}/bin/mkdir -p $default/bin
		echo "${default-wrapper}" >$default/bin/patchshebangs
		${nixpkgs.coreutils}/bin/chmod +x $default/bin/patchshebangs
		${nixpkgs.coreutils}/bin/ln -s $default/bin/patchshebangs $out
		'';
}
