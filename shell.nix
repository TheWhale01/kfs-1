{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
	nativeBuildInputs = with pkgs; [
		gnumake
		qemu
		clang
		bear
		grub2
	];
}
