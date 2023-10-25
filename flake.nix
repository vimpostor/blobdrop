{
	description = "Drag and drop your files directly out of the terminal";
	inputs = {
		nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
		flake-utils.url = "github:numtide/flake-utils";
	};

	outputs = { self, nixpkgs, flake-utils }: flake-utils.lib.eachDefaultSystem (system:
		let
			pkgs = nixpkgs.legacyPackages.${system};
		in {
			packages = rec {
				default = blobdrop;

				blobdrop = pkgs.gcc13Stdenv.mkDerivation {
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
			};
		}
	);
}
