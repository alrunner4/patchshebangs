{ nixpkgs ? import <nixpkgs> {} }:

let

custom = derivation {
	name = "patchshebangs";
	system = builtins.currentSystem;
	builder = nixpkgs.writeShellScript "patchshebangs-builder" ''
		${nixpkgs.coreutils}/bin/mkdir -p $out/bin
		${nixpkgs.gcc}/bin/g++ -Wall -Werror -Wextra ${./main.cpp} -o $out/bin/patchshebangs
	'';
};

default-wrapper = ''
	${custom}/bin/patchshebangs \"\$@\" <<REWRITES
	/bin/bash ${nixpkgs.bash}/bin/bash
	/usr/bin/bash ${nixpkgs.bash}/bin/bash
	/bin/env ${nixpkgs.coreutils}/bin/env
	/usr/bin/env ${nixpkgs.coreutils}/bin/env
	REWRITES
'';

in
{

	inherit custom;

	default = derivation {
		name = "patchshebangs-custom";
		system = builtins.currentSystem;
		builder = nixpkgs.writeShellScript "patchshebangs-custom-builder" ''
			${nixpkgs.coreutils}/bin/mkdir -p $out/bin
			echo "${default-wrapper}" >$out/bin/patchshebangs
			${nixpkgs.coreutils}/bin/chmod +x $out/bin/patchshebangs
		'';
	};
}
