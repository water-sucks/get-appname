{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";

    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs = {
    self,
    flake-parts,
    ...
  }:
    flake-parts.lib.mkFlake {inherit self;} {
      imports = [];

      systems = ["x86_64-linux" "aarch64-linux"];

      perSystem = {pkgs, ...}: {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "get-appname";
          version = "0.1.0";
          src = ./.;

          nativeBuildInputs = with pkgs; [pkg-config meson ninja];
          buildInputs = with pkgs; [gtkmm3];
        };

        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            gcc
            gdb
            valgrind
            meson
            ninja
            pkg-config
            gtkmm3
          ];
        };
      };

      flake = {
        overlays.default = final: prev: {
        };
      };
    };
}
