with import <nixpkgs> {};
  stdenv.mkDerivation {
    name = "cpp";
    hardeningDisable = [ "all" ];
    nativeBuildInputs = with pkgs; [
      pkg-config
      gcc
      gnumake
      cmake
      ninja
      clang-tools
      valgrind
    ];
    buildInputs = with pkgs.buildPackages; [
      raylib
      fmt
    ];
  }
