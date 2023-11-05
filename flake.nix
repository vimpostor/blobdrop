{
	description = "Drag and drop your files directly out of the terminal";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
			stdenvs = [ { name = "gcc"; pkg = pkgs.gcc13Stdenv; } { name = "clang"; pkg = pkgs.llvmPackages_16.stdenv; } ];
			defaultStdenv = (builtins.head stdenvs).name;
			makeStdenvPkg = env: env.mkDerivation {
				pname = "blobdrop";
				version = "2.0";

				src = ./.;

				nativeBuildInputs = with pkgs; [
					cmake
					pkg-config
					qt6.wrapQtAppsHook
				];

				buildInputs = with pkgs; [
					qt6.qtbase
					qt6.qtdeclarative
					qt6.qtsvg
					xorg.libxcb
					xorg.xcbutilwm
				];
			};
		in {
			packages = rec {
				default = self.outputs.packages.${system}.${defaultStdenv};
			} // builtins.listToAttrs (map (x: { name = x.name; value = makeStdenvPkg x.pkg; }) stdenvs);
		}
	);
}
