{ pkgs ? import <nixpkgs> {} }:

let e =
  pkgs.buildFHSEnv {
    name = "gcc-git-build-env";
    targetPkgs = ps: with ps; [
      gmp gmp.dev
      isl
      libffi libffi.dev
      libmpc
      libxcrypt
      mpfr mpfr.dev
      xz xz.dev
      zlib zlib.dev
      m4
      bison
      flex
      texinfo
      dejagnu
      autogen
      valgrind valgrind.dev
      gcc
      stdenv.cc
      stdenv.cc.libc stdenv.cc.libc_dev
      nasm
      qemu
      bear
      grub2
      xorriso
      gdb
    ];
  };
in e.env
